#ifndef MUTEXLOCK_H
#define MUTEXLOCK_H

#include "pthread.h"
#include "SemaphorePV.h"
#include "Thread.h"
#ifdef _WIN32
#include "windows.h"
#endif

class MutexLockBase
{
    public:
        /** Default constructor */
        MutexLockBase();
        /** Default destructor */
        virtual ~MutexLockBase();
        virtual void lock() = 0;
        virtual void unlock() = 0;
    protected:

    private:

};

//无名互斥锁，用于进程内各线程间同步
class MutexLockInProcess : MutexLockBase
{
    public:
        /** Default constructor */
        MutexLockInProcess();
        /** Default destructor */
        virtual ~MutexLockInProcess();
        void lock();
        void unlock();
        bool lockedByThisThread();
    protected:
        pthread_mutex_t _mutex; //!< Member variable "_mutex"
        pid_t _holder; //!< Member variable "_holder;"
        int err;
    private:

};

//有名互斥锁，用于进程间同步
class MutexLockOutProcess : MutexLockBase
{
    public:
        /** Default constructor */
        MutexLockOutProcess();
        MutexLockOutProcess(char *name, int open_flag);
        /** Default destructor */
        virtual ~MutexLockOutProcess();
        bool init(char *name, int open_flag);
        void lock();
        void unlock();
    protected:

        pid_t _holder; //!< Member variable "_holder;"

        SemaphoreOutProcessPV sem;
    private:

};

//互斥锁调用类
class MutexLockGuard
{
    public:
        explicit MutexLockGuard(MutexLockBase *mutex) : _mutex(mutex)
        {
            if(_mutex != NULL)
            {
                _mutex->lock();
            }
        }
        ~MutexLockGuard()
        {
            if(_mutex != NULL)
            {
                _mutex->unlock();
            }
        }
    private:
        MutexLockBase *_mutex;
};
/*
互斥锁调用通过MutexLockGuard类型实现
具体使用方法为
先声明一个长期存在的互斥锁变量（全局或者类成员）
MutexLock mutex;

然后在需要互斥锁保护的临界区里，把整个临界区用一个{}对包括
{
    //此大括号内为全部需要加锁保护的临界区代码
    MutexLockGuard lock(mutex);   //这里加入一条语句，声明一个以mutex为参数构造的MutexLockGuard对象

    ...
    //此为全部临界区的代码
}
*/

#endif // MUTEXLOCK_H
