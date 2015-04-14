#include "ConfigBase.h"

ConfigBase::ConfigBase()
{
    //ctor
}

ConfigBase::~ConfigBase()
{
    //dtor
}

bool ConfigBase::getConf(const char *key, char *cont, int length)
{
    map<string, string>::iterator iter;
    if(key == NULL || cont == NULL || length <= 0)
        return false;
    iter =  _configs.find(key);
    if(iter == _configs.end())
    {
        return false;
    }
    strncpy(cont, iter->second.c_str(), length);
    return true;
}

bool ConfigBase::setConf(const char *key, const char *cont, int length)
{
    map<string, string>::iterator iter;
    if(key == NULL || cont == NULL || length <= 0)
        return false;
    iter =  _configs.find(key);
    if(iter == _configs.end())
    {
        _configs.insert(pair<string, string>(key, cont));
        return true;
    }
    iter->second = cont;
    return true;
}

int ConfigBase::numOfConf()
{
    return _configs.size();
}
