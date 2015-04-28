#include "FuncPtrMap.h"

FuncPtrMap::FuncPtrMap() : _funcMap(NULL)
{
    //ctor
    _funcMap = new map<string, void*>;
}

FuncPtrMap::~FuncPtrMap()
{
    //dtor
    if(_funcMap == NULL)
        return;
    delete _funcMap;
}


