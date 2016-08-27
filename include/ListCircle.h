#ifndef LISTCIRCLE_H_INCLUDED
#define LISTCIRCLE_H_INCLUDED

#include "List.h"

class ListCircle
{
    public:
        /** Default constructor */
        ListCircle(){ head.add(&tail);}
        /** Default destructor */
        virtual ~ListCircle(){}

        ListHead *getHead(){return &head;}
        ListHead *getTail(){return &tail;}

        void add_use(ListHead *list){head.add(list);}
        void add_free(ListHead *list){tail.add(list);}

        void get_use(ListHead **list){*list = tail.getPrev();}
        void get_free(ListHead **list){*list = head.getPrev();}

        //void moveFirst(ListHead *list){head.move(list);}
        //void moveLast(ListHead *list){head.move_tail(list);}

        bool useEmpty(){return head.getNext() == &tail;}
        bool freeEmpty(){return tail.getNext() == &head;}
        //bool isFirst(ListHead *list){return list == head.getNext();}
        //bool isLast(ListHead *list){return list == head.getPrev();}

    protected:
    private:
        ListHead head, tail;
}

<typename type, typename member>
class ListCircle
{
    public:
        ListCircle()
        {
            head.add(&tail);
            for(int i = 0; i < 256; i++)
            {
                add_free(&data_node[i]->member);
            }
        }
        virtual ~ListCircle(){}

        ListHead *getHead(){return &head;}
        ListHead *getTail(){return &tail;}

        void add_use(ListHead *list){head.add(list);}
        void add_free(ListHead *list){tail.add(list);}

        void get_use(ListHead **list){*list = tail.getPrev();}
        void get_free(ListHead **list){*list = head.getPrev();}

        void free_all(){for(ListHead *list = head.getNext(), ListHead *tmp = list; list != &tail; list = tmp->getNext(), tmp = list) {list.del();add_free(list);}}

        bool useEmpty(){return head.getNext() == &tail;}
        bool freeEmpty(){return tail.getNext() == &head;}

        type *getEntry(ListHead *list){return list_entry(list, type, member);}

    private:
        ListHead head, tail;
        type data_node[256];
}

#endif // LISTCIRCLE_H_INCLUDED
