#ifndef LIST_H
#define LIST_H

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
    prev->setNext(n);
    n->setPrev(prev);
    next->setPrev(n);
    n->setNext(next);
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

template <typename type, int offset>
class ListT
{
    public:
        /** Default constructor */
        ListT();
        /** Default destructor */
        virtual ~ListT();

        //void init(){head.init()}

        ListHeadT<type, offset> *getHead(){return &head;}

        ListHeadT<type, offset> *getFirst()
        {ListHeadT<type, offset> *tmp = head.getNext();if(tmp == &head) return NULL;return tmp;}
        ListHeadT<type, offset> *getLast()
        {ListHeadT<type, offset> *tmp = head.getPrev();if(tmp == &head) return NULL;return tmp;}

        void add(ListHeadT<type, offset> *list){head.add(list);}
        void addTail(ListHeadT<type, offset> *list){head.add_tail(list);}

        void moveFirst(ListHeadT<type, offset> *list){head.move(list);}
        void moveLast(ListHeadT<type, offset> *list){head.move_tail(list);}

        bool isEmpty(){return head.getNext() == &head;}
        bool isFirst(ListHeadT<type, offset> *list){return list == head.getNext();}
        bool isLast(ListHeadT<type, offset> *list){return list == head.getPrev();}

    protected:
    private:
        ListHeadT<type, offset> head;
};
class List
{
    public:
        /** Default constructor */
        List();
        /** Default destructor */
        virtual ~List();

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
