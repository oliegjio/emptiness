#ifndef RUNGUARD_H
#define RUNGUARD_H

#include <QSharedMemory>
#include <QSystemSemaphore>
#include <QCryptographicHash>
#include <QCoreApplication>
#include <QThread>
#include <QFileInfo>
#include <QDir>

class SingleApplication : public QThread
{
    Q_OBJECT

public:
    explicit SingleApplication(const QString& key, QObject* parent = nullptr);
    ~SingleApplication() override;

    bool isAnotherRunning();
    bool tryToRun();
    void release();

private:
    const QString key;

    const QString memoryLockForPath_Key;
    const QString sharedMemoryForPath_Key;

    const QString memoryLockForWorkingDirectory_Key;
    const QString sharedMemoryForWorkingDirectory_Key;

    QSharedMemory sharedMemoryForPath;
    QSystemSemaphore memoryLockForPath;

    QSharedMemory sharedMemoryForWorkingDirectory;
    QSystemSemaphore memoryLockForWorkingDirectory;

    char* dataStringForPath;
    char* dataStringForWorkingDirectory;

    const int dataSegmentsSize = sizeof(char) * 500;

    void writePathToDataString();
    void writeWorkingDirectoryToDataString();

    void run() override;

signals:
    void sharedMemoryForPathChanged(const QString& result);
    void sharedMemoryForWorkingDirectoryChanged(const QString& result);
};

#endif // RUNGUARD_H
