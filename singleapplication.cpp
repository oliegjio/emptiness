#include "singleapplication.h"

namespace {
    QString generateKeyHash(const QString& key, const QString& salt)
    {
        QByteArray data;

        data.append(key.toUtf8());
        data.append(salt.toUtf8());
        data = QCryptographicHash::hash(data, QCryptographicHash::Sha1).toHex();

        return data;
    }
}

SingleApplication::SingleApplication(const QString& key, QObject* parent)
    : QThread(parent)
    , key(key)
    , memoryLockKey(generateKeyHash(key, "_memoryLockKey"))
    , sharedMemoryKey(generateKeyHash(key, "_sharedMemoryKey"))
    , sharedMemory(sharedMemoryKey)
    , memoryLock(memoryLockKey, 1)
{
    memoryLock.acquire();
    {
        QSharedMemory unixFix(sharedMemoryKey);
        unixFix.attach();
    }
    memoryLock.release();
}

SingleApplication::~SingleApplication()
{
    release();
}

bool SingleApplication::isAnotherRunning()
{
    if (sharedMemory.isAttached()) return false;

    memoryLock.acquire();
    const bool isRunning = sharedMemory.attach();
    if (isRunning) sharedMemory.detach();
    memoryLock.release();

    return isRunning;
}

void SingleApplication::writePathToDataString()
{
    memoryLock.acquire();
    sharedMemory.attach();

    char* data = (char*) sharedMemory.data();
    if (QCoreApplication::arguments().length() != 1)
    {
        QString pathString = QCoreApplication::arguments().at(1);
        char* path = pathString.toLocal8Bit().data();

        memset(data, 0, dataStringSize);
        memcpy(data, path, sizeof(char) * pathString.length());
    }

    sharedMemory.detach();
    memoryLock.release();
}

bool SingleApplication::tryToRun()
{
    if (isAnotherRunning())
    {
        writePathToDataString();
        return false;
    }

    memoryLock.acquire();
    const bool result = sharedMemory.create(dataStringSize);
    memoryLock.release();

    if (!result)
    {
        release();
        return false;
    }

    start();
    dataString = (char*) sharedMemory.data();

    return true;
}

void SingleApplication::run()
{
    char* oldDataString = (char*) malloc(dataStringSize);

    while (true)
    {
        memoryLock.acquire();

        if (QString(dataString) != QString(oldDataString))
        {
            emit sharedMemoryChanged(QString(dataString));
        }
        memset(oldDataString, 0, dataStringSize);
        memcpy(oldDataString, dataString, dataStringSize);

        memoryLock.release();

        msleep(100);
    }
}

void SingleApplication::release()
{
    memoryLock.acquire();
    if (sharedMemory.isAttached()) sharedMemory.detach();
    memoryLock.release();
}
