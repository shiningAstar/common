#include "Thread.h"
#include "string.h"



#ifdef _WIN32
    #include "windows.h"
#else
//    #define _GNU_SOURCE
    #include "unistd.h"
    #include "sys/syscall.h"
    #include "sys/types.h"
#endif // _WIN32

Thread::Thread() :  status(STOPPING), detached(true)
{
    //ctor
}

Thread::~Thread()
{
    //dtor
}

bool Thread::start(bool detached, pthread_attr_t *attr)
{
    if(attr != NULL)
    {
        memcpy(&(this->attr), attr, sizeof(pthread_attr_t));
    }
    if(!detached)
    {
        this->detached = false;
    }
    if(pthread_create(&tid, attr, Thread::threadProc, this) < 0)
    {
        return false;
    }
    return true;
}

bool Thread::waittoStop(void **ret)
{
    void *pr;
    //if(status == STOPPING)
        //return true;
    if(pthread_join(tid, &pr))
    {
        return false;
    }
    if(ret == NULL)
    {
        return true;
    }
    *ret = pr;
    return true;
}

void *Thread::threadProc(void *arg)
{
    Thread *thread;
    void *ret;
    if(arg == NULL)
    {
        return NULL;
    }
    thread = (Thread*)arg;
    if(thread->detached)
    {
        pthread_detach(pthread_self());
    }
    thread->status = RUNNING;
    ret = (void*)thread->doWork();
    thread->status = STOPPING;
    return ret;
}

THREAD_STATUS Thread::getStatus()
{
    return status;
}

namespace CurrentThread
{
    __thread pid_t thread_id = 0;
    pid_t tid()
    {
        if(thread_id != 0)
        {
            return thread_id;
        }
        #ifndef _WIN32
            thread_id = static_cast<pid_t>(::syscall(SYS_gettid));
        #else
            thread_id = (pid_t)GetCurrentThreadId();
        #endif // _WIN32
        return thread_id;
    }
}
