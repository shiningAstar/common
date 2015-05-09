#include "FuncPtrMap.h"
#include "base.h"
#include "string.h"

FuncPtrMap::FuncPtrMap() : _funcMap(NULL), index(0)
{
    //ctor
    _funcMap = new map<string, CmdFunc>;
    if(_funcMap == NULL)
    {
        return;
    }
    cur = _funcMap->begin();
    index = 0;
}

FuncPtrMap::~FuncPtrMap()
{
    //dtor
    if(_funcMap == NULL)
        return;
    _funcMap->clear();
    index = 0;
    delete _funcMap;
}

CmdFunc FuncPtrMap::getFuncPtr(char *name, int length)
{
    map<string, CmdFunc>::iterator iter;
    if(name == NULL || length <= 0 || !corr_string(name, length))
    {
        return NULL;
    }
    if(_funcMap == NULL)
    {
        return NULL;
    }
    iter = _funcMap->find(name);
    if(iter == _funcMap->end())
    {
        return NULL;
    }
    return iter->second;
}

bool FuncPtrMap::setFuncPtr(char *name, int length, CmdFunc func)
{
    map<string, CmdFunc>::iterator iter;
    if(name == NULL || length <= 0 || !corr_string(name, length) || func == NULL)
    {
        return false;
    }
    if(_funcMap == NULL)
    {
        return false;
    }
    iter = _funcMap->find(name);
    if(iter == _funcMap->end())
    {
        _funcMap->insert(pair<string, CmdFunc>(name, func));
        cur = _funcMap->begin();
        index = 0;
        return true;
    }
    iter->second = func;
    return true;
}

CmdFunc FuncPtrMap::getFuncPtr(int index, char *name, int length)
{
    int size, len;
    if(_funcMap == NULL)
    {
        return NULL;
    }
    size = numOfFunc();
    if(size <= 0 || index < 0 || index >= size || name == NULL || length <= 0)
    {
        return NULL;
    }
    len = index - this->index;
    if(len >= 0)
    {
        for(int i = 0; i < len; i++)
        {
            cur++;
        }
    }
    else
    {
        for(int i = 0; i < -len; i++)
        {
            cur--;
        }
    }
    if(cur == _funcMap->end())
    {
        cur = _funcMap->begin();
        this->index = 0;
        return NULL;
    }
    this->index = index;
    if(!corr_string(cur->first.c_str(), length))
    {
        return NULL;
    }
    strncpy(name, cur->first.c_str(), length);
    return cur->second;
}

int FuncPtrMap::numOfFunc()
{
    if(_funcMap == NULL)
    {
        return -1;
    }
    return _funcMap->size();
}

CmdIdMap::CmdIdMap() : _idMap(NULL), index(0)
{
    //ctor
    _idMap = new map<string, int>;
    if(_idMap == NULL)
    {
        return;
    }
    cur = _idMap->begin();
    index = 0;
}

CmdIdMap::~CmdIdMap()
{
    //dtor
    if(_idMap == NULL)
        return;
    _idMap->clear();
    index = 0;
    delete _idMap;
}

int CmdIdMap::getCmdId(char *name, int length)
{
    map<string, int>::iterator iter;
    if(name == NULL || length <= 0 || !corr_string(name, length))
    {
        return -1;
    }
    if(_idMap == NULL)
    {
        return -1;
    }
    iter = _idMap->find(name);
    if(iter == _idMap->end())
    {
        return -1;
    }
    return iter->second;
}

bool CmdIdMap::setCmdId(char *name, int length, int id)
{
    map<string, int>::iterator iter;
    if(name == NULL || length <= 0 || !corr_string(name, length) || id <= 0)
    {
        return false;
    }
    if(_idMap == NULL)
    {
        return false;
    }
    iter = _idMap->find(name);
    if(iter == _idMap->end())
    {
        _idMap->insert(pair<string, int>(name, id));
        cur = _idMap->begin();
        index = 0;
        return true;
    }
    iter->second = id;
    return true;
}

int CmdIdMap::getCmdId(int index, char *name, int length)
{
    int size, len;
    if(_idMap == NULL)
    {
        return -1;
    }
    size = numOfId();
    if(size <= 0 || index < 0 || index >= size || name == NULL || length <= 0)
    {
        return NULL;
    }
    len = index - this->index;
    if(len >= 0)
    {
        for(int i = 0; i < len; i++)
        {
            cur++;
        }
    }
    else
    {
        for(int i = 0; i < -len; i++)
        {
            cur--;
        }
    }
    if(cur == _idMap->end())
    {
        cur = _idMap->begin();
        this->index = 0;
        return -1;
    }
    this->index = index;
    if(!corr_string(cur->first.c_str(), length))
    {
        return -1;
    }
    strncpy(name, cur->first.c_str(), length);
    return cur->second;
}

int CmdIdMap::numOfId()
{
    if(_idMap == NULL)
    {
        return -1;
    }
    return _idMap->size();
}
