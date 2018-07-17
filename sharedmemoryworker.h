#ifndef SHAREDMEMORYWORKER_H
#define SHAREDMEMORYWORKER_H

#include <QThread>

class SharedMemoryWorker : public QThread
{
    Q_OBJECT

public:
    SharedMemoryWorker(QObject* parent = nullptr, const QString& key);

    void run() override;

private:
    const QString sharedMemoryKey;
    const QString memoryLockKey;

signals:
    void memoryChanged(const QString& result);
};

#endif // SHAREDMEMORYWORKER_H
