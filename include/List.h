#ifndef LIST_H
#define LIST_H
/**
    使用说明：
    ListT:
        * ListT为拥有头结点的双向链表，链表中的节点类型为ListHeadT
        *假设有结构体 typedef struct data{
                                            int int_data;
                                            char ch_data[101];
                                            ListHeadT<data,105> listheadt;
                                                    }
        有链表 ListT<data,104>，其中，此模板中的data并非其节点类型，而是用来生成其节点类型，105为listheadt的相对偏移，也即
            该数据前的数据长度，记得内存对齐
        * 而ListT类型中的每个节点都是data类型的变量中的成员，所以，该节点的地址也是data类型的变量中的listHeader成员的地址
        * 所以，data类型的数据中，listheadt的相对首地址的地址偏移为104,也即listheadt的首个字节的地址减去105即得到data类型结构体的地址，
            即可得到data类型数据，这就是getEntry的功能，获取data类型数据。
    List:

        * list数据的插入操作与ListT类似，获取数据略有不同，通过list_entry()宏来实现数据的获取
        * list_entry(list_node, entry_type, member_name) 宏用来获取list节点的数据，下面介绍参数含义
            假设有结构体 typedef struct data{
                                            int int_data;
                                            char ch_data[101];
                                            ListHead listhead;
                                                    }
        * list_node：要获取数据的节点的指针
        * entry_type： 结构体的类型，这里为data
        * member_name，结构体中链表节点的名称，这里为listhead
*/
#include "stdlib.h"
template <typename type, int offset>
class ListHeadT;

class ListHead;

inline void __list_add(ListHead *n, ListHead *prev, ListHead *next);
inline void __list_del(ListHead *prev, ListHead *next);
template <typename type, int offset>
inline void __list_add(ListHeadT<type,offset> *n, ListHeadT<type,offset> *prev, ListHeadT<type,offset> *next);
template <typename type, int offset>
inline void __list_del(ListHeadT<type,offset> *prev, ListHeadT<type,offset> *next);

template <typename type, int offset>
class ListHeadT
{
public:
    ListHeadT()
    {
        init();
    }

    ~ListHeadT()
    {

    }

    void init()
    {
        prev = this;
        next = this;
    }

    ListHeadT<type,offset> *getPrev(){return prev;}
    ListHeadT<type,offset> *getNext(){return next;}
    void setPrev(ListHeadT<type,offset> *prev){this->prev = prev;}
    void setNext(ListHeadT<type,offset> *next){this->next = next;}

    void add(ListHeadT<type,offset> *n)
    {
        __list_add(n, this, next);
    }

    void add_tail(ListHeadT<type,offset> *n)
    {
        __list_add(n, prev, this);
    }

    void move(ListHeadT<type, offset> *n)
    {
        n->del();
        add(n);
    }

    void move_tail(ListHeadT<type, offset> *n)
    {
        n->del();
        add_tail(n);
    }

    void del()
    {
        __list_del(prev, next);
        init();
    }

    void replace_with(ListHeadT<type,offset> *n)
    {
        n->setNext(next);
        n->getNext()->setPrev(n);
        n->setPrev(prev);
        n->getPrev()->setNext(n);
        init();
    }

    /**
        *获取type型的数据
        *this的地址减去偏移offset即得到type类型数据的地址
        *其中，this为type中的ListHeadT类型的成员,offset为this所在地址距离type首地址的偏移，也即this前的数据的长度
    */
    type *getEntry()
    {
        return (type*)((char*)this - offset);
    }

    //type *getEntry();
    //{
        //return  (type*)(this - (&(((type*)0)->member)));
    //    return entry;
    //}

private:
    ListHeadT<type,offset> *prev,*next;
};


/** list_entry(list_node, entry_type, member_name) 宏用来获取list节点的数据，下面介绍参数含义
            假设有结构体 typedef struct data{
                                            int int_data;
                                            char ch_data[101];
                                            ListHead listhead;
                                                    }
        * list_node：要获取数据的节点的指针
        * entry_type： 结构体的类型，这里为data
        * member_name，结构体中链表节点的名称，这里为listhead
*/
#define list_entry(list_node, entry_type, member_name) ((entry_type*)((char*)(list_node) - (char*)(&(((entry_type*)0)->member_name))))

class ListHead
{
public:
    ListHead()
    {
        init();
    }

    ~ListHead()
    {

    }

    void init()
    {
        prev = this;
        next = this;
    }

    ListHead *getPrev(){return prev;}
    ListHead *getNext(){return next;}
    void setPrev(ListHead *prev){this->prev = prev;}
    void setNext(ListHead *next){this->next = next;}

    void add(ListHead *n)
    {
        __list_add(n, this, next);
    }

    void add_tail(ListHead *n)
    {
        __list_add(n, prev, this);
    }

    void move(ListHead *n)
    {
        n->del();
        add(n);
    }

    void move_tail(ListHead *n)
    {
        n->del();
        add_tail(n);
    }

    void del()
    {
        __list_del(prev, next);
        init();
    }

    void replace_with(ListHead *n)
    {
        n->setNext(next);
        n->getNext()->setPrev(n);
        n->setPrev(prev);
        n->getPrev()->setNext(n);
        init();
    }

    //type *getEntry();
    //{
        //return  (type*)(this - (&(((type*)0)->member)));
    //    return entry;
    //}

private:
    ListHead *prev,*next;
};



inline void __list_add(ListHead *n, ListHead *prev, ListHead *next)
{
    n->setNext(next);
    next->setPrev(n);
    n->setPrev(prev);
    prev->setNext(n);
}
inline void __list_del(ListHead *prev, ListHead *next)
{
    prev->setNext(next);
    next->setPrev(prev);
}
template <typename type, int offset>
inline void __list_add(ListHeadT<type,offset> *n, ListHeadT<type,offset> *prev, ListHeadT<type,offset> *next)
{
    prev->setNext(n);
    n->setPrev(prev);
    next->setPrev(n);
    n->setNext(next);

}
template <typename type, int offset>
inline void __list_del(ListHeadT<type,offset> *prev, ListHeadT<type,offset> *next)
{
    prev->setNext(next);
    next->setPrev(prev);
}

/**
    *队列为拥有头结点的双向的链表，尾节点的前向指针指向头结点
    *初始化时，头结点前后指针都指向本身

*/
template <typename type, int offset>
class ListT
{
    public:
        /** Default constructor */
        ListT(){}
        /** Default destructor */
        virtual ~ListT(){}

        //void init(){head.init()}

        ListHeadT<type, offset> *getHead(){return &head;}

        ListHeadT<type, offset> *getFirst()
        {ListHeadT<type, offset> *tmp = head.getNext();if(tmp == &head) return NULL;return tmp;}

        ListHeadT<type, offset> *getLast()
        {ListHeadT<type, offset> *tmp = head.getPrev();if(tmp == &head) return NULL;return tmp;}

        /* 添加节点到头结点的下一个节点，即链表首*/
        void add(ListHeadT<type, offset> *list){head.add(list);}

        /*  添加节点到头结点的前一个节点，即链表尾*/
        void addTail(ListHeadT<type, offset> *list){head.add_tail(list);}

        void moveFirst(ListHeadT<type, offset> *list){head.move(list);}
        void moveLast(ListHeadT<type, offset> *list){head.move_tail(list);}

        bool isEmpty(){return head.getNext() == &head;}
        bool isFirst(ListHeadT<type, offset> *list){return list == head.getNext();}
        bool isLast(ListHeadT<type, offset> *list){return list == head.getPrev();}

    protected:
    private:
        /* 链表头节点，不赋值，只用于链表操作*/
        ListHeadT<type, offset> head;
};
class List
{
    public:
        /** Default constructor */
        List(){}
        /** Default destructor */
        virtual ~List(){}

        ListHead *getHead(){return &head;}

        void add(ListHead *list){head.add(list);}
        void add_tail(ListHead *list){head.add_tail(list);}

        void moveFirst(ListHead *list){head.move(list);}
        void moveLast(ListHead *list){head.move_tail(list);}

        bool isEmpty(){return head.getNext() == &head;}
        bool isFirst(ListHead *list){return list == head.getNext();}
        bool isLast(ListHead *list){return list == head.getPrev();}

    protected:
    private:
        ListHead head;
};

#endif // LIST_H
