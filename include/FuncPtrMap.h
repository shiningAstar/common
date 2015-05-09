#ifndef FUNCPTRMAP_H
#define FUNCPTRMAP_H

#include "map"
#include "string"
#include "base.h"
#include "string.h"

using namespace std;

typedef unsigned long (*CmdFunc)(void *arg);

class FuncPtrMap
{
    public:
        /** Default constructor */
        FuncPtrMap();
        /** Default destructor */
        virtual ~FuncPtrMap();

        CmdFunc getFuncPtr(char *name, int length);
        bool setFuncPtr(char *name, int length, CmdFunc func);

        CmdFunc getFuncPtr(int index, char *name, int length);

        int numOfFunc();
    protected:
        map<string, CmdFunc> *_funcMap;
        map<string, CmdFunc>::iterator cur;
        int index;
    private:
};

class CmdIdMap
{
public:
    CmdIdMap();
    virtual ~CmdIdMap();

    int getCmdId(char *name, int length);
    bool setCmdId(char *name, int length, int id);

    int getCmdId(int index, char *name, int length);

    int numOfId();
protected:
    map<string, int> *_idMap;
    map<string, int>::iterator cur;
    int index;
};

#endif // FUNCPTRMAP_H
