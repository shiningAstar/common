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

//�����ź������ڽ����ڸ��߳�֮��ͬ����
class SemaphoreInProcessPV : SemaphoreBase
{
    //���ֹ��췽ʽ��ֱ�ӹ��죬���ô��ι��캯��������ʽ��ӹ��죬����Ĭ�Ϲ��캯����Ȼ�����init()
    public:
        /** Default constructor */
        SemaphoreInProcessPV();

        //value ��ʼֵ
        SemaphoreInProcessPV(int value);
        /** Default destructor */
        virtual ~SemaphoreInProcessPV();
        bool init(int value);
        //P���������ٹ�����Դ������û����Դʱ�ȴ�
        bool P();
        bool P(long wait_sec, long wait_nsec);
        bool tryP();
        //V�������ͷŹ�����Դ�����ܻ����ڹ�����Դ�ϵȴ��Ľ��̣��̣߳�
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

//�����ź������ڽ���֮��ͬ����
class SemaphoreOutProcessPV : SemaphoreBase
{
    //���ֹ��췽ʽ��ֱ�ӹ��죬���ô��ι��캯��������ʽ��ӹ��죬����Ĭ�Ϲ��캯����Ȼ�����init()
    public:
        /** Default constructor */
        SemaphoreOutProcessPV();
        enum sema_openflag{OPEN_EXIST, OPEN_OR_CREATE, CREATE_ONLY};
        // name �ź�������
        //open_flag �ź����򿪷�ʽ���򿪴��ڣ��򿪴��ڻ򴴽���������
        //value ��ʼֵ�����������ź�������ʱ, ����ʱ����
        SemaphoreOutProcessPV(char *name, int open_flag, int value);
        /** Default destructor */
        virtual ~SemaphoreOutProcessPV();
        bool init(char *name, int open_flag, int value);
        //P���������ٹ�����Դ������û����Դʱ�ȴ�
        bool P();

        bool P(long wait_sec, long wait_nsec);
        #ifndef _WIN32
        bool tryP();
        #endif // _WIN32

        //V�������ͷŹ�����Դ�����ܻ����ڹ�����Դ�ϵȴ��Ľ��̣��̣߳�
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
