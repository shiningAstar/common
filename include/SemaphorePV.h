#ifndef SEMAPORE_H
#define SEMAPORE_H

#include "sys/types.h"
#include "semaphore.h"
#include "pthread.h"
#ifdef _WIN32
#include "windows.h"
#define sema_name_prefix "sm."
#endif // _WIN
class SemaphoreBase
{
public:
    SemaphoreBase();
    virtual ~SemaphoreBase();

    virtual bool P() = 0;
    virtual bool P(long wait_sec, long wait_nsec) = 0;
    #ifndef _WIN32
    virtual bool tryP() = 0;
    virtual int getValue() = 0;
    #endif // _WIN32
    virtual bool V() = 0;
     #ifdef _WIN32
    virtual bool V(int res_count) = 0;
	#endif
    virtual bool available() = 0;
    virtual int getErrno() = 0;
};

//无名信号量，在进程内各线程之间同步用
class SemaphoreInProcessPV : SemaphoreBase
{
    //两种构造方式，直接构造，调用带参构造函数，二段式间接构造，调用默认构造函数，然后调用init()
    public:
        /** Default constructor */
        SemaphoreInProcessPV();

        //value 初始值
        SemaphoreInProcessPV(int value);
        /** Default destructor */
        virtual ~SemaphoreInProcessPV();
        bool init(int value);
        //P操作，减少共享资源，且在没有资源时等待
        bool P();
        bool P(long wait_sec, long wait_nsec);
        bool tryP();
        //V操作，释放共享资源，可能唤起在共享资源上等待的进程（线程）
        bool V();
        #ifdef _WIN32
        bool V(int res_count);
        #endif // _WIN32
        int getValue();
        bool available();
        int getErrno();
    protected:
        sem_t sem;
        #ifndef _WIN32
        bool avail;
        #endif // _WIN32
    private:
};

//有名信号量，在进程之间同步用
class SemaphoreOutProcessPV : SemaphoreBase
{
    //两种构造方式，直接构造，调用带参构造函数，二段式间接构造，调用默认构造函数，然后调用init()
    public:
        /** Default constructor */
        SemaphoreOutProcessPV();
        enum sema_openflag{OPEN_EXIST, OPEN_OR_CREATE, CREATE_ONLY};
        // name 信号量名称
        //open_flag 信号量打开方式，打开存在，打开存在或创建，仅创建
        //value 初始值，仅当有名信号量创建时, 仅打开时忽略
        SemaphoreOutProcessPV(char *name, int open_flag, int value);
        /** Default destructor */
        virtual ~SemaphoreOutProcessPV();
        bool init(char *name, int open_flag, int value);
        //P操作，减少共享资源，且在没有资源时等待
        bool P();

        bool P(long wait_sec, long wait_nsec);
        #ifndef _WIN32
        bool tryP();
        #endif // _WIN32

        //V操作，释放共享资源，可能唤起在共享资源上等待的进程（线程）
        bool V();
        #ifdef _WIN32
        bool V(int res_count);
        #endif // _WIN32
        #ifndef _WIN32
        int getValue();
        #endif // _WIN32
        bool available();
        int getErrno();
    protected:
        char name[128];
        #ifndef _WIN32
        sem_t *psem;
        #else
        HANDLE h_sema;
        #endif // _WIN32
    private:
};

#endif // SEMAPORE_H
