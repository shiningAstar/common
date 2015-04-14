#include "MutexLock.h"
#include "log.h"
#include "stdio.h"

//#define use_log //use or comment this to use/unuse the log component in this library
MutexLockBase::MutexLockBase()
{

}

MutexLockBase::~MutexLockBase()
{

}

MutexLockInProcess::MutexLockInProcess() : _holder(0)
{
    //ctor
    _mutex = NULL;
    if(pthread_mutex_init(&_mutex, NULL) < 0)
    {
        _mutex = NULL;
        //err = errno;
    }
}

MutexLockInProcess::~MutexLockInProcess()
{
    //dtor
    if(_holder != 0)
    {
//#ifdef use_log
//        log_warn_time_line("mutex destructed while locking. locking thread id£º%d.", _holder);
//#else
        printf("mutex destructed while locking. locking thread id£º%d.\n", _holder);
//#endif // use_log
    }
    pthread_mutex_destroy(&_mutex);
}

void MutexLockInProcess::lock()
{
    pthread_mutex_lock(&_mutex);
    _holder = CurrentThread::tid();
}

void MutexLockInProcess::unlock()
{
    if(_holder != CurrentThread::tid())
    {
        return;
    }
    _holder = 0;
    pthread_mutex_unlock(&_mutex);
}

bool MutexLockInProcess::lockedByThisThread()
{
    return _holder == CurrentThread::tid();
}

MutexLockOutProcess::MutexLockOutProcess() : _holder(0)
{
    //ctor
    _holder = 0;
}

MutexLockOutProcess::MutexLockOutProcess(char *name, int open_flag)
{
    MutexLockOutProcess();
    init(name, open_flag);
}

MutexLockOutProcess::~MutexLockOutProcess()
{
    //dtor
    if(_holder != 0)
    {
//#ifdef use_log
//        log_warn_time_line("mutex destructed while locking. locking thread id£º%d.", _holder);
//#else
        printf("mutex destructed while locking. locking thread id£º%d.\n", _holder);;
//#endif // use_log
    }
}

bool MutexLockOutProcess::init(char *name, int open_flag)
{
    return sem.init(name, open_flag, 1);
}

void MutexLockOutProcess::lock()
{
    sem.P();
#ifndef _WIN32
    _holder = gettid();
#else
    _holder = (unsigned long)GetCurrentThread();
#endif
}

void MutexLockOutProcess::unlock()
{
    if(_holder != CurrentThread::tid())
    {
        return;
    }
    _holder = 0;
    sem.V();
}

//bool MutexLockOutProcess::lockedByThisThread()
//{
//    return _holder == (unsigned long)GetCurrentThread();
//}

