#include "ConfigBase.h"

ConfigBase::ConfigBase() : _configs(NULL)
{
    //ctor
    _configs = new map<string, string>;
}

ConfigBase::~ConfigBase()
{
    //dtor
    if(_configs == NULL)
    {
        return;
    }
    _configs->clear();
    delete _configs;
}

bool ConfigBase::getConf(const char *key, char *cont, int length)
{
    map<string, string>::iterator iter;
    if(key == NULL || cont == NULL || length <= 0 || _configs == NULL)
        return false;
    iter =  _configs->find(key);
    if(iter == _configs->end())
    {
        return false;
    }
    strncpy(cont, iter->second.c_str(), length);
    return true;
}

bool ConfigBase::setConf(const char *key, const char *cont, int length)
{
    map<string, string>::iterator iter;
    if(key == NULL || cont == NULL || length <= 0 || _configs == NULL)
        return false;
    iter =  _configs->find(key);
    if(iter == _configs->end())
    {
        _configs->insert(pair<string, string>(key, cont));
        return true;
    }
    iter->second = cont;
    return true;
}

bool ConfigBase::getConf(const int index, char *key, int key_length, char *value, int value_length)
{
    int size, length;
    map<string, string>::iterator iter;
    if(_configs == NULL)
    {
        return false;
    }
    size = numOfConf();
    if(size <= 0 || index < 0 || index >= size || key == NULL || key_length <= 0 || value == NULL || value_length <=0)
    {
        return false;
    }
    iter = _configs->begin();
    iter += index;
    if(iter == _configs->end())
    {
        return false;
    }
    if(strlen(iter->first) >= key_length || strlen(iter->second) >= value_length)
    {
        return false;
    }
    strncpy(key, iter-first, key_length);
    strncpy(value, iter->second, value_lenth);
    return true;
}

int ConfigBase::numOfConf()
{
    return _configs->size();
}


