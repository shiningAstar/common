#ifndef SEMAPORE_H
#define SEMAPORE_H

#include "sys/types.h"
#include "semaphore.h"
#include "pthread.h"

class SemaphoreBase
{
public:
    SemaphoreBase();
    virtual ~SemaphoreBase();

    virtual bool P() = 0;
    virtual bool P(long wait_sec, long wait_nsec) = 0;
    virtual bool tryP() = 0;
    virtual bool V() = 0;
    virtual bool V(int res_count) = 0;
    virtual int getValue() = 0;
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
        bool V(int res_count);
        int getValue();
        bool available();
        int getErrno();
    protected:
        sem_t sem;
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
        bool tryP();
        //V�������ͷŹ�����Դ�����ܻ����ڹ�����Դ�ϵȴ��Ľ��̣��̣߳�
        bool V();
        bool V(int res_count);
        int getValue();
        bool available();
        int getErrno();
    protected:
        char name[128];
        sem_t *psem;
    private:
};

#endif // SEMAPORE_H
