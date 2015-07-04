#ifndef QUE_H
#define QUE_H

#include "List.h"
#include "SemaphorePV.h"
//#include "errno.h"

//队列基类
class Que
{
    public:
        /** Default constructor */
        Que(){}
        /** Default destructor */
        ~Que(){}

        bool push(const char *data, unsigned int length)
        {
            if(data == NULL)
            {
                return false;
            }
            return true;
        }
        bool pop(char *data, unsigned int &length)
        {
            if(data == NULL)
            {
                return false;
            }
            return true;
        }
        virtual unsigned int numOfQueItem() = 0;
        bool getTail(char *data, unsigned int &length)
        {
            if(data == NULL)
            {
                return false;
            }
            return true;
        }
        bool getHead(char *data, unsigned int &length)
        {
            if(data == NULL)
            {
                return false;
            }
            return true;
        }
        bool getQueItem(int index, char *data, unsigned int &length)
        {
            if(index < 0 || data == NULL)
            {
                return false;
            }
            return true;
        }
        virtual bool isEmpty() = 0;
        virtual bool isFull() = 0;
    protected:
    private:
};
//基于链表的队列
template<typename type, int offset>
class ListQue : Que
{
public:
    ListQue() : num(0)
    {

    }
    ~ListQue()
    {

    }

    bool push(const char *data, unsigned int length)
    {
        if(!Que::push(data, length))
        {
            return false;
        }
        list.addTail((ListHeadT<type, offset>)(data + offset));
        num++;
        return true;
    }
    bool pop(char *&data, unsigned int &length)
    {
        ListHeadT<type, offset> *listhead;
        unsigned int len = sizeof(type);
        if(!Que::pop(data, length))
        {
            return false;
        }
        if(length < len)
        {
            return false;
        }
        if((listhead = list.getFirst()) == NULL)
        {
            return false;
        }
        memcpy(data, (char *)listhead->getEntry(), len);
        //data = (char *)listhead->getEntry();
        listhead->del();
        num--;
        return true;
    }
    unsigned int numOfQueItem()
    {
        return num;
    }
    bool getTail(char *data, unsigned int &length)
    {
        ListHeadT<type, offset> *listhead;
        unsigned int len = sizeof(type);
        if(!Que::getTail(data, length))
        {
            return false;
        }
        if(length < len)
        {
            return false;
        }
        if((listhead = list.getLast()) == NULL)
        {
            return false;
        }
        //data = (char *)listhead->getEntry();
        memcpy(data, (char *)listhead->getEntry(), len);
        return true;
    }
    bool getHead(char *data, unsigned int &length)
    {
        ListHeadT<type, offset> *listhead;
        unsigned int len = sizeof(type);
        if(!Que::getHead(data, length))
        {
            return false;
        }
        if(length < len)
        {
            return false;
        }
        if((listhead = list.getFirst()) == NULL)
        {
            return false;
        }
        //data = (char *)listhead->getEntry();
        memcpy(data, (char *)listhead->getEntry(), len);
        return true;
    }
    bool getQueItem(int index, char *&data, unsigned int &length)
    {
        ListHeadT<type, offset> *listhead;
        unsigned int len = sizeof(type), i = 0;
        if(!Que::getQueItem(index, data, length))
        {
            return false;
        }
        if(length < len)
        {
            return false;
        }
        if(index >= numOfQueItem())
        {
            return false;
        }
        for(listhead = list.getHead()->getNext(); listhead != list.getHead() &&
            i < index; listhead = listhead->getNext())
        {
            i++;
        }
        if(i != index || listhead == list.getHead())
        {
            return false;
        }
        //data = (char *)listhead->getEntry();
        memcpy(data, (char *)listhead->getEntry(), len);
        return true;
    }
    bool isEmpty()
    {
        return list.isEmpty();
    }
    bool isFull()
    {
        return false;
    }

protected:
    ListT<type, offset> list;
    int num;

};
//基于顺序表的队列
template<typename type>
class SequentListQue : Que
{
    public:
        /** Default constructor */
        SequentListQue() : sequentTable(NULL)
        {
            capacity = 1024;
            sequentTable = new type[capacity];
            head = tail = 0;
        }
        SequentListQue(int capacity) : capacity(capacity), sequentTable(NULL)
        {
            if(capacity < 2)
            {
                return;
            }
            sequentTable = new type[capacity];
            head = tail = 0;
        }
        /** Default destructor */
        ~SequentListQue()
        {
            if(sequentTable != NULL)
            {
                delete []sequentTable;
            }
        }

        bool push(const char *data, unsigned int length)
        {
            unsigned int len = sizeof(type);
            if(!Que::push(data, length))
            {
                return false;
            }
            if(capacity < 2 || sequentTable == NULL)
            {
                return false;
            }
            if(head < 0 || head >= capacity || tail < 0 || tail >= capacity)
            {
                return false;
            }
            if(length < len)
            {
                return false;
            }
            if(isFull())
            {
                return false;
            }
            memcpy((char *)&sequentTable[tail], data, len);
            tail++;
            if(tail == capacity)
            {
                tail = 0;
            }
            return true;
        }
        bool push(type *data)
        {
            return push((char *)data, sizeof(type));
        }
        bool pop(char *data, unsigned int &length)
        {
            unsigned int len = sizeof(type);
            if(!Que::pop(data, length))
            {
                return false;
            }
            if(capacity < 2 || sequentTable == NULL)
            {
                return false;
            }
            if(head < 0 || head >= capacity || tail < 0 || tail >= capacity)
            {
                return false;
            }
            if(length < len)
            {
                return false;
            }
            if(isEmpty())
            {
                return false;
            }
            memcpy(data, (char *)&sequentTable[head], len);
            head++;
            if(head == capacity)
            {
                head = 0;
            }
            return true;
        }
        bool pop(type *data)
        {
            return pop((char *)data, sizeof(type));
        }
        unsigned int numOfQueItem()
        {
            return (tail + capacity - head) % capacity;
        }
        bool getTail(char *data, unsigned int &length)
        {
            unsigned int len = sizeof(type);
            int i;
            if(!Que::getTail(data, length))
            {
                return false;
            }
            if(capacity < 2 || sequentTable == NULL)
            {
                return false;
            }
            if(head < 0 || head >= capacity || tail < 0 || tail >= capacity)
            {
                return false;
            }
            if(length < len)
            {
                return false;
            }
            if(isEmpty())
            {
                return false;
            }
            i = (tail + capacity - 1) % capacity;
            memcpy(data, (char *)&sequentTable[i], len);
            return true;
        }
        bool getHead(char *data, unsigned int &length)
        {
            unsigned int len = sizeof(type);
            if(!Que::getHead(data, length))
            {
                return false;
            }
            if(capacity < 2 || sequentTable == NULL)
            {
                return false;
            }
            if(head < 0 || head >= capacity || tail < 0 || tail >= capacity)
            {
                return false;
            }
            if(length < len)
            {
                return false;
            }
            if(isEmpty())
            {
                return false;
            }
            memcpy(data, (char *)&sequentTable[head], len);
            return true;
        }
        bool getQueItem(int index, char *data, unsigned int &length)
        {
            unsigned int len = sizeof(type);
            int i;
            if(!Que::getQueItem(index, data, length))
            {
                return false;
            }
            if(capacity < 2 || sequentTable == NULL)
            {
                return false;
            }
            if(head < 0 || head >= capacity || tail < 0 || tail >= capacity)
            {
                return false;
            }
            if(length < len)
            {
                return false;
            }
            if(index >= numOfQueItem())
            {
                return false;
            }
            i = (head + index) % capacity;
            memcpy(data, (char *)&sequentTable[i], len);
            return true;
        }
        bool isEmpty()
        {
            //empty while tail == head
            if(tail != head)
            {
                return false;
            }
            return true;
        }
        bool isFull()
        {
            //full while tail + 1 == head
            if((tail + 1) % capacity != head)
            {
                return false;
            }
            return true;
        }
    protected:
        int capacity;
        type *sequentTable;
        int head, tail;
    private:
};

//基于顺序表的单入单出进程内阻塞队列
template<typename type>
class SiSoPiBlockingSeqQue : SequentListQue<type>
{
public:
    SiSoPiBlockingSeqQue()
    {
        init();
    }
    SiSoPiBlockingSeqQue(int capacity) : SequentListQue<type>(capacity)
    {
        init();
    }
    ~SiSoPiBlockingSeqQue()
    {

    }

    bool push(const char *data, unsigned int length)
    {
        if(!SequentListQue<type>::push(data, length))
        {
            return false;
        }
        if(!sema.V())
        {
            return false;
        }
        return true;
    }
    bool push(type *data)
    {
        return push((char *)data, sizeof(type));
    }
    bool pop(char *data, unsigned int length)
    {
        if(!sema.P())
        {
            return false;
        }
        if(!SequentListQue<type>::pop(data, length))
        {
            return false;
        }
        return true;
    }
    bool pop(type *data)
    {
        return pop((char *)data, sizeof(type));
    }
    bool pop(char *data, unsigned int length, long wait_time_millisec, bool &timedout)
    {
        if(!sema.P(wait_time_millisec / 1000, (wait_time_millisec % 1000)*1000000))
        {
            #ifdef _WIN32
            if(errno == 10060)
                timedout = true;
            #else
            if(errno == ETIMEDOUT)
                timedout = true;
            #endif
            else
                timedout = false;
            return false;
        }
        if(!SequentListQue<type>::pop(data, length))
        {
            return false;
        }
        return true;
    }
    bool pop(type *data, long wait_time_millisec, bool &timedout)
    {
        return pop((char *)data, sizeof(type), wait_time_millisec, timedout);
    }

protected:
    SemaphoreInProcessPV sema;

    bool init()
    {
        return sema.init(0);
    }

};

#endif // QUE_H
