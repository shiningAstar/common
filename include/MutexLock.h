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

//���������������ڽ����ڸ��̼߳�ͬ��
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

//���������������ڽ��̼�ͬ��
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

//������������
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
����������ͨ��MutexLockGuard����ʵ��
����ʹ�÷���Ϊ
������һ�����ڴ��ڵĻ�����������ȫ�ֻ������Ա��
MutexLock mutex;

Ȼ������Ҫ�������������ٽ�����������ٽ�����һ��{}�԰���
{
    //�˴�������Ϊȫ����Ҫ�����������ٽ�������
    MutexLockGuard lock(mutex);   //�������һ����䣬����һ����mutexΪ���������MutexLockGuard����

    ...
    //��Ϊȫ���ٽ����Ĵ���
}
*/

#endif // MUTEXLOCK_H
