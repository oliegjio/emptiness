#ifndef RUNGUARD_H
#define RUNGUARD_H

#include <QSharedMemory>
#include <QSystemSemaphore>
#include <QCryptographicHash>
#include <QCoreApplication>
#include <QThread>

class RunGuard : public QThread
{
    Q_OBJECT

public:
    explicit RunGuard(const QString& key, QObject* parent = nullptr);
    ~RunGuard();

    bool isAnotherRunning();
    bool tryToRun();
    void release();

private:
    const QString key;
    const QString memoryLockKey;
    const QString sharedMemoryKey;

    QSharedMemory sharedMemory;
    QSystemSemaphore memoryLock;

    char* dataString;
    const int dataStringSize = sizeof(char) * 500;

    void run() override;

signals:
    void sharedMemoryChanged(const QString& result);
};

#endif // RUNGUARD_H
