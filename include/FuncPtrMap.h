#ifndef FUNCPTRMAP_H
#define FUNCPTRMAP_H

#include "map"

using namespace std;

class FuncPtrMap
{
    public:
        /** Default constructor */
        FuncPtrMap();
        /** Default destructor */
        virtual ~FuncPtrMap();

        void *getFuncPtr(char *name, int length);
        bool setFuncPtr(char *name, int length, void *func);
    protected:
        map<string, void*> *_funcMap;
    private:
};

#endif // FUNCPTRMAP_H
