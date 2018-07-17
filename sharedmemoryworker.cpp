#include "sharedmemoryworker.h"

SharedMemoryWorker::SharedMemoryWorker(QObject* parent, const QString& sharedMemoryKey,
                                       const QString& memoryLockKey)
    : QObject(parent)
    , sharedMemoryKey(sharedMemoryKey)
    , memoryLockKey(memoryLockKey)
{}

void SharedMemoryWorker::run()
{
}
