#ifndef NORMAP_H_INCLUDED
#define NORMAP_H_INCLUDED

#include "map"
#include "string"
#include "base.h"
#include "string.h"
/**
    注意：delvalue()函数使用时，未找到与要删除的键对匹配的键值时，返回true，若键值正确，value错误，返回false
*/
using namespace std;

template <typename keytype, typename valuetype>
class NorMap
{
public:
    NorMap()
    {
        _map = new map<keytype, valuetype>;
        if(_map == NULL)
        {
            return;
        }
        cur = _map->begin();

        index = 0;
    }
    virtual ~NorMap()
    {
        if(_map == NULL)
            return;
        _map->clear();
        index = 0;
        delete _map;
    }

    bool getValue(keytype key, valuetype *value)
    {
        typename map<keytype, valuetype>::iterator iter;
        if(value == NULL || _map == NULL)
        {
            return false;
        }
        iter = _map->find(key);
        if(iter == _map->end())
        {
            return false;
        }
        *value = iter->second;
        return true;
    }
    bool setValue(keytype key, valuetype value)
    {
        typename map<keytype, valuetype>::iterator iter;
        if(_map == NULL)
        {
            return false;
        }
        iter = _map->find(key);
        if(iter == _map->end())
        {
            _map->insert(pair<keytype, valuetype>(key, value));
            cur = _map->begin();
            index = 0;
            return true;
        }
        iter->second = value;
        return true;
    }
    bool addValue(keytype key, valuetype value)
    {
        typename map<keytype, valuetype>::iterator iter;
        if(_map == NULL)
        {
            return false;
        }
        iter = _map->find(key);
        if(iter != _map->end())
        {
            return false;
        }

        _map->insert(pair<keytype, valuetype>(key, value));
        cur = _map->begin();
        index = 0;
        return true;
    }
    bool delValue(keytype key)
    {
        typename map<keytype, valuetype>::iterator iter;
        if(_map == NULL)
        {
            return false;
        }
        iter = _map->find(key);
        if(iter == _map->end())
        {
            return true;
        }
        _map->erase(iter);
        return true;
    }
    bool delValue(keytype key, valuetype value)
    {
        typename map<keytype, valuetype>::iterator iter;
        if(_map == NULL)
        {
            return false;
        }
        iter = _map->find(key);
        if(iter == _map->end())
        {
            return true;
        }
        if(iter->second != value)
        {
            return false;
        }
        _map->erase(iter);
        return true;
    }
    bool getValue(int index, keytype *key, valuetype *value)
    {
        int size, len;
        if(key == NULL || value == NULL || _map == NULL)
        {
            return false;
        }
        size = numOfPair();
        if(size <= 0 || index < 0 || index >= size)
        {
            return false;
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
        if(cur == _map->end())
        {
            cur = _map->begin();
            this->index = 0;
            return false;
        }
        this->index = index;
        *key = cur->first;
        *value = cur->second;
        return true;
    }

    int numOfPair()
    {
        if(_map == NULL)
        {
            return -1;
        }
        return _map->size();
    }
protected:
    map<keytype, valuetype> *_map;
    typename map<keytype, valuetype>::iterator cur;
    int index;
};

template <typename valuetype>
class NorMap <string, valuetype>
{
public:
    NorMap()
    {
        _map = new map<string, valuetype>;
        if(_map == NULL)
        {
            return;
        }
        cur = _map->begin();

        index = 0;
    }
    virtual ~NorMap()
    {
        if(_map == NULL)
            return;
        _map->clear();
        index = 0;
        delete _map;
    }

    bool getValue(char *key, int key_len, valuetype *value)
    {
        typename map<string, valuetype>::iterator iter;
        if(!corr_string(key, key_len))
        {
            return false;
        }
        if(value == NULL || _map == NULL)
        {
            return false;
        }
        iter = _map->find(key);
        if(iter == _map->end())
        {
            return false;
        }
        *value = iter->second;
        return true;
    }
    bool setValue(char *key, int key_len, valuetype value)
    {
        typename map<string, valuetype>::iterator iter;
        if(!corr_string(key, key_len) || _map == NULL)
        {
            return false;
        }
        iter = _map->find(key);
        if(iter == _map->end())
        {
            _map->insert(pair<string, valuetype>(key, value));
            cur = _map->begin();
            index = 0;
            return true;
        }
        iter->second = value;
        return true;
    }
    bool addValue(char *key, int key_len, valuetype value)
    {
        typename map<string, valuetype>::iterator iter;
        if(!corr_string(key, key_len) || _map == NULL)
        {
            return false;
        }
        iter = _map->find(key);
        if(iter != _map->end())
        {
            return false;
        }

        _map->insert(pair<string, valuetype>(key, value));
        cur = _map->begin();
        index = 0;
        return true;
    }
    bool delValue(char *key, int key_len)
    {
        typename map<string, valuetype>::iterator iter;
        if(!corr_string(key, key_len) || _map == NULL)
        {
            return false;
        }
        iter = _map->find(key);
        if(iter == _map->end())
        {
            return true;
        }
        _map->erase(iter);
        return true;
    }
    bool delValue(char *key, int key_len, valuetype value)
    {
        typename map<string, valuetype>::iterator iter;
        if(!corr_string(key, key_len) || _map == NULL)
        {
            return false;
        }
        iter = _map->find(key);
        if(iter == _map->end())
        {
            return true;
        }
        if(iter->second != value)
        {
            return false;
        }
        _map->erase(iter);
        return true;
    }
    bool getValue(int index, char *key, int key_len, valuetype *value)
    {
        int size, len;
        if(key == NULL || key_len <= 1 || value == NULL || _map == NULL)
        {
            return false;
        }
        size = numOfPair();
        if(size <= 0 || index < 0 || index >= size)
        {
            return false;
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
        if(cur == _map->end())
        {
            cur = _map->begin();
            this->index = 0;
            return false;
        }
        this->index = index;
        if(cur->first.size() >= key_len)
        {
            return false;
        }
        strncpy(key, cur->first.c_str(), key_len);
        *value = cur->second;
        return true;
    }

    int numOfPair()
    {
        if(_map == NULL)
        {
            return -1;
        }
        return _map->size();
    }
protected:
    map<string, valuetype> *_map;
    typename map<string, valuetype>::iterator cur;
    int index;
};

template <typename keytype>
class NorMap <keytype, string>
{
public:
    NorMap()
    {
        _map = new map<keytype, string>;
        if(_map == NULL)
        {
            return;
        }
        cur = _map->begin();

        index = 0;
    }
    virtual ~NorMap()
    {
        if(_map == NULL)
            return;
        _map->clear();
        index = 0;
        delete _map;
    }

    bool getValue(keytype key, char *value, int value_len)
    {
        typename map<keytype, string>::iterator iter;
        if(value == NULL || value_len <= 1 || _map == NULL)
        {
            return false;
        }
        iter = _map->find(key);
        if(iter == _map->end())
        {
            return false;
        }
        if(iter->second.size() >= value_len)
        {
            return false;
        }
        strncpy(value, iter->second.c_str(), value_len);
        return true;
    }
    bool setValue(keytype key, char *value, int value_len)
    {
        typename map<keytype, string>::iterator iter;
        if(!corr_string(value, value_len) || _map == NULL)
        {
            return false;
        }
        iter = _map->find(key);
        if(iter == _map->end())
        {
            _map->insert(pair<keytype, string>(key, value));
            cur = _map->begin();
            index = 0;
            return true;
        }
        iter->second = value;
        return true;
    }
    bool addValue(keytype key, char *value, int value_len)
    {
        typename map<keytype, string>::iterator iter;
        if(!corr_string(value, value_len) || _map == NULL)
        {
            return false;
        }
        iter = _map->find(key);
        if(iter != _map->end())
        {
            return false;
        }

        _map->insert(pair<keytype, string>(key, value));
        cur = _map->begin();
        index = 0;
        return true;
    }
    bool delValue(keytype key)
    {
        typename map<keytype, string>::iterator iter;
        if(_map == NULL)
        {
            return false;
        }
        iter = _map->find(key);
        if(iter == _map->end())
        {
            return true;
        }
        _map->erase(iter);
        return true;
    }
    bool delValue(keytype key, char *value, int value_len)
    {
        typename map<keytype, string>::iterator iter;
        if(!corr_string(value, value_len) || _map == NULL)
        {
            return false;
        }
        iter = _map->find(key);
        if(iter == _map->end())
        {
            return true;
        }
        if(strcmp(iter->second.c_str(), value) != 0)
        {
            return false;
        }
        _map->erase(iter);
        return true;
    }
    bool getValue(int index, keytype *key, char *value, int value_len)
    {
        int size, len;
        if(key == NULL || value == NULL || value_len <= 1 || _map == NULL)
        {
            return false;
        }
        size = numOfPair();
        if(size <= 0 || index < 0 || index >= size)
        {
            return false;
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
        if(cur == _map->end())
        {
            cur = _map->begin();
            this->index = 0;
            return false;
        }
        this->index = index;
        if(cur->second.size() >= value_len)
        {
            return false;
        }
        *key = cur->first;
        strncpy(value, cur->second.c_str(), value_len);
        return true;
    }

    int numOfPair()
    {
        if(_map == NULL)
        {
            return -1;
        }
        return _map->size();
    }
protected:
    map<keytype, string> *_map;
    typename map<keytype, string>::iterator cur;
    int index;
};

template <>
class NorMap <string, string>
{
public:
    NorMap()
    {
        _map = new map<string, string>;
        if(_map == NULL)
        {
            return;
        }
        cur = _map->begin();

        index = 0;
    }
    virtual ~NorMap()
    {
        if(_map == NULL)
            return;
        _map->clear();
        index = 0;
        delete _map;
    }

    /**
        key_len和value_len均为字符串长度，即 len = 字符个数+1 ;
        注：string类型的length()函数为字符个数，若要用作 len 需要加1;
    */
    bool getValue(char *key, int key_len, char *value, int value_len)
    {
        map<string, string>::iterator iter;
        if(!corr_string(key, key_len) || value == NULL || value_len <= 1 || _map == NULL)
        {
            return false;
        }
        iter = _map->find(key);
        if(iter == _map->end())
        {
            return false;
        }
        if(iter->second.size() >= value_len)
        {
            return false;
        }
        strncpy(value, iter->second.c_str(), value_len);
        return true;
    }
    /**
        key_len和value_len均为字符串长度，即 len = 字符个数+1 ;
        注：string类型的length()函数为字符个数，若要用作 len 需要加1;
    */
    bool setValue(char *key, int key_len, char *value, int value_len)
    {
        map<string, string>::iterator iter;
        if(!corr_string(key, key_len) || !corr_string(value, value_len) || _map == NULL)
        {
            return false;
        }
        iter = _map->find(key);
        if(iter == _map->end())
        {
            _map->insert(pair<string, string>(key, value));
            cur = _map->begin();
            index = 0;
            return true;
        }
        iter->second = value;
        return true;
    }
    bool addValue(char *key, int key_len, char *value, int value_len)
    {
        map<string, string>::iterator iter;
        if(!corr_string(key, key_len) || !corr_string(value, value_len) || _map == NULL)
        {
            return false;
        }
        iter = _map->find(key);
        if(iter != _map->end())
        {
            return false;
        }

        _map->insert(pair<string, string>(key, value));
        cur = _map->begin();
        index = 0;
        return true;
    }
    bool delValue(char *key, int key_len)
    {
        map<string, string>::iterator iter;
        if(!corr_string(key, key_len) || _map == NULL)
        {
            return false;
        }
        iter = _map->find(key);
        if(iter == _map->end())
        {
            return true;
        }
        _map->erase(iter);
        return true;
    }
    bool delValue(char *key, int key_len, char *value, int value_len)
    {
        map<string, string>::iterator iter;
        if(!corr_string(key, key_len) || !corr_string(value, value_len) || _map == NULL)
        {
            return false;
        }
        iter = _map->find(key);
        if(iter == _map->end())
        {
            return true;
        }
        if(strcmp(iter->second.c_str(), value) != 0)
        {
            return false;
        }
        _map->erase(iter);
        return true;
    }
    bool getValue(int index, char *key, int key_len, char *value, int value_len)
    {
        int size, len;
        if(key == NULL || key_len <= 1 || value == NULL || value_len <= 1 || _map == NULL)
        {
            return false;
        }
        size = numOfPair();
        if(size <= 0 || index < 0 || index >= size)
        {
            return false;
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
        if(cur == _map->end())
        {
            cur = _map->begin();
            this->index = 0;
            return false;
        }
        this->index = index;
        if(cur->first.size() >= key_len || cur->second.size() >= value_len)
        {
            return false;
        }
        strncpy(key, cur->first.c_str(), key_len);
        strncpy(value, cur->second.c_str(), value_len);
        return true;
    }

    int numOfPair()
    {
        if(_map == NULL)
        {
            return -1;
        }
        return _map->size();
    }
protected:
    map<string, string> *_map;
    map<string, string>::iterator cur;
    int index;
};


#endif // NORMAP_H_INCLUDED