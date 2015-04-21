#include "ConfigBase.h"
#include "base.h"

ConfigBase::ConfigBase() : _configs(NULL), index(0)
{
    //ctor
    _configs = new map<string, string>;
    if(_configs == NULL)
    {
        return;
    }
    cur = _configs->begin();
    index = 0;
}

ConfigBase::~ConfigBase()
{
    //dtor
    if(_configs == NULL)
    {
        return;
    }
    _configs->clear();
    index = 0;
    delete _configs;
}

bool ConfigBase::getConf(const char *key, int key_buflen, char *cont, int cont_buflen)
{
    map<string, string>::iterator iter;
    if(!corr_string(key, key_buflen) || cont == NULL || cont_buflen <= 0 || _configs == NULL)
        return false;
    iter =  _configs->find(key);
    if(iter == _configs->end())
    {
        return false;
    }
    strncpy(cont, iter->second.c_str(), cont_buflen);
    return true;
}

bool ConfigBase::setConf(const char *key, int key_buflen, const char *cont, int cont_buflen)
{
    map<string, string>::iterator iter;
    if(!corr_string(key, key_buflen) || !corr_string(cont, cont_buflen) || _configs == NULL)
        return false;
    iter =  _configs->find(key);
    if(iter == _configs->end())
    {
        _configs->insert(pair<string, string>(key, cont));
        cur = _configs->begin();
        index = 0;
        return true;
    }
    iter->second = cont;
    return true;
}

bool ConfigBase::getConf(const int index, char *key, int key_buflen, char *value, int value_buflen)
{
    int size, length;
    map<string, string>::iterator iter;
    if(_configs == NULL)
    {
        return false;
    }
    size = numOfConf();
    if(size <= 0 || index < 0 || index >= size || key == NULL || key_buflen <= 0 || value == NULL || value_buflen <= 0)
    {
        return false;
    }
    length = index - this->index;
    if(length >= 0)
    {
        for(int i = 0; i < length; i++)
        {
            cur++;
        }
    }
    else
    {
        for(int i = 0; i < -length; i++)
        {
            cur--;
        }
    }
    this->index = index;
    if(cur == _configs->end())
    {
        cur = _configs->begin();
        this->index = 0;
        return false;
    }
    if(!corr_string(cur->first.c_str(), key_buflen) || !corr_string(cur->second.c_str(), value_buflen))
    {
        return false;
    }
    strncpy(key, iter->first.c_str(), key_buflen);
    strncpy(value, iter->second.c_str(), value_buflen);
    return true;
}

//bool ConfigBase::confBegin()
//{
//    if(_configs == NULL)
//    {
//        return false;
//    }
//    cur = _configs->begin();
//    if(cur.pointer == NULL)
//    {
//        return false;
//    }
//    return true;
//}
//bool ConfigBase::confEnd()
//{
//    if(_configs == NULL)
//    {
//        return false;
//    }
//    if(cur != _configs->end())
//    {
//        return false;
//    }
//    return true;
//}
//bool ConfigBase::confNext()
//{
//    if(_configs == NULL)
//    {
//        return false;
//    }
//    if(cur == NULL)
//    {
//        return false;
//    }
//    cur++;
//    return true;
//}
//bool ConfigBase::getCurrentConf(char *key, int key_length, char *value, int value_length)
//{
//    if(_configs == NULL)
//    {
//        return false;
//    }
//    if(cur == NULL)
//    {
//        return false;
//    }
//    if(key == NULL || key_length <= 0 || value == NULL || value_length <= 0)
//    {
//        return false;
//    }
//    strncpy(key, (char*)cur->first, key_length);
//    strncpy(value, (char*)cur->second, value_length);
//    return ture;
//}

int ConfigBase::numOfConf()
{
    return _configs->size();
}


