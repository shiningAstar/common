#ifndef THREAD_H
#define THREAD_H

#include "sys/types.h"
#include "pthread.h"

//#ifdef _WIN32
//    typedef unsigned long int pid_t;
//#endif

typedef enum {STOPPING, RUNNING}THREAD_STATUS;

class Thread
{
    public:
        /** Default constructor */
        Thread();
        /** Default destructor */
        virtual ~Thread();

        //#ifdef _WIN32
        //static DWORD WINAPI threadProc(DWORD arg);
        //#else
        static void *threadProc(void *arg);
        //#endif // _WIN32
        virtual unsigned long doWork() = 0;
        bool start(bool detached = true, pthread_attr_t *attr = NULL);
        THREAD_STATUS getStatus();
        bool waittoStop(void **ret=NULL);
    protected:
        pthread_t tid;
        pthread_attr_t attr;
        THREAD_STATUS status;
        bool detached;
    private:
};

namespace CurrentThread
{
    extern pid_t tid();
}

#endif // THREAD_H
