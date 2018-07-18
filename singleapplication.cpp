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

    , memoryLockForPath_Key(generateKeyHash(key, "_memoryLockForPathKey"))
    , sharedMemoryForPath_Key(generateKeyHash(key, "_sharedMemoryForPathKey"))

    , memoryLockForWorkingDirectory_Key(generateKeyHash(key, "_memoryLockForWorkingDirectory_Key"))
    , sharedMemoryForWorkingDirectory_Key(generateKeyHash(key, "_sharedMemoryForWorkingDirectory_Key"))

    , sharedMemoryForPath(sharedMemoryForPath_Key)
    , memoryLockForPath(memoryLockForPath_Key, 1)

    , sharedMemoryForWorkingDirectory(sharedMemoryForWorkingDirectory_Key)
    , memoryLockForWorkingDirectory(memoryLockForWorkingDirectory_Key, 1)
{
    memoryLockForPath.acquire();
    {
        QSharedMemory unixFix1(sharedMemoryForPath_Key);
        unixFix1.attach();
    }
    memoryLockForPath.release();
    memoryLockForWorkingDirectory.acquire();
    {
        QSharedMemory unixFix2(sharedMemoryForWorkingDirectory_Key);
        unixFix2.attach();
    }
    memoryLockForWorkingDirectory.release();
}

SingleApplication::~SingleApplication()
{
    release();
}

bool SingleApplication::isAnotherRunning()
{
    if (sharedMemoryForPath.isAttached()) return false;
    if (sharedMemoryForWorkingDirectory.isAttached()) return false;

    memoryLockForPath.acquire();
    memoryLockForWorkingDirectory.acquire();

    const bool isRunning1 = sharedMemoryForPath.attach();
    if (isRunning1) sharedMemoryForPath.detach();

    const bool isRunning2 = sharedMemoryForWorkingDirectory.attach();
    if (isRunning2) sharedMemoryForWorkingDirectory.detach();

    memoryLockForWorkingDirectory.release();
    memoryLockForPath.release();

    return (isRunning1 || isRunning2);
}

void SingleApplication::writePathToDataString()
{
    memoryLockForPath.acquire();
    sharedMemoryForPath.attach();

    char* data = (char*) sharedMemoryForPath.data();
    if (QCoreApplication::arguments().length() != 1)
    {
        QString pathString = QCoreApplication::arguments().at(1);
        char* path = pathString.toLocal8Bit().data();

        memset(data, 0, dataSegmentsSize);
        memcpy(data, path, sizeof(char) * pathString.length());
    }

    sharedMemoryForPath.detach();
    memoryLockForPath.release();
}

void SingleApplication::writeWorkingDirectoryToDataString()
{
    memoryLockForWorkingDirectory.acquire();
    sharedMemoryForWorkingDirectory.attach();

    char* data = (char*) sharedMemoryForWorkingDirectory.data();

    QString workingDirectoryString = QDir::currentPath();
    char* workingDirectory = workingDirectoryString.toLocal8Bit().data();

    memset(data, 0, dataSegmentsSize);
    memcpy(data, workingDirectory, sizeof(char) * workingDirectoryString.length());

    sharedMemoryForWorkingDirectory.detach();
    memoryLockForWorkingDirectory.release();
}

bool SingleApplication::tryToRun()
{
    if (isAnotherRunning())
    {
        writePathToDataString();
        writeWorkingDirectoryToDataString();
        return false;
    }

    memoryLockForPath.acquire();
    const bool result1 = sharedMemoryForPath.create(dataSegmentsSize);
    memoryLockForPath.release();

    memoryLockForWorkingDirectory.acquire();
    const bool result2 = sharedMemoryForWorkingDirectory.create(dataSegmentsSize);
    memoryLockForWorkingDirectory.release();

    if (!result1 || !result2)
    {
        release();
        return false;
    }

    start();
    dataStringForPath = (char*) sharedMemoryForPath.data();
    dataStringForWorkingDirectory = (char*) sharedMemoryForWorkingDirectory.data();

    return true;
}

void SingleApplication::run()
{
    char* oldDataStringForPath = (char*) malloc(dataSegmentsSize);
    char* oldDataStringForWorkingDirectory = (char*) malloc(dataSegmentsSize);

    while (true)
    {
        memoryLockForWorkingDirectory.acquire();
        if (QString(dataStringForWorkingDirectory) != QString(oldDataStringForWorkingDirectory))
        {
            emit sharedMemoryForWorkingDirectoryChanged(QString(dataStringForWorkingDirectory));
        }
        memset(oldDataStringForWorkingDirectory, 0, dataSegmentsSize);
        memcpy(oldDataStringForWorkingDirectory, dataStringForWorkingDirectory, dataSegmentsSize);
        memoryLockForWorkingDirectory.release();

        memoryLockForPath.acquire();
        if (QString(dataStringForPath) != QString(oldDataStringForPath))
        {
            emit sharedMemoryForPathChanged(QString(dataStringForPath));
        }
        memset(oldDataStringForPath, 0, dataSegmentsSize);
        memcpy(oldDataStringForPath, dataStringForPath, dataSegmentsSize);
        memoryLockForPath.release();

        msleep(100);
    }
}

void SingleApplication::release()
{
    memoryLockForPath.acquire();
    if (sharedMemoryForPath.isAttached()) sharedMemoryForPath.detach();
    memoryLockForPath.release();

    memoryLockForWorkingDirectory.acquire();
    if (sharedMemoryForWorkingDirectory.isAttached()) sharedMemoryForWorkingDirectory.detach();
    memoryLockForWorkingDirectory.release();
}
