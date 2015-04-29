#include "FileConfig.h"

#include "stdio.h"

#define BUF_SIZE 256

FileConfig::FileConfig(char *file)
{
    //ctor
    if(file == NULL)
    {
        return;
    }
    strncpy(this->file, file, 256);
}

FileConfig::~FileConfig()
{
    //dtor
}

//FileConfig *FileConfig::get_inst(char *file)
//{
//    static FileConfig config(file);
//    return &config;
//}

//从文件file中读取一行，行尾以\n或\r\n标识，返回行的长度>=0,或错误<0
int loadLine(File *file, char *buffer, int buf_len)
{
    int pos = -1, ret;
    char c = 0;
    if(file == NULL || buffer == NULL || buf_len <= 0)
    {
        return -1;
    }
    if(!file->available())
    {
        return -1;
    }
    while(c != '\n')
    {
        pos++;
        if(pos > buf_len - 2)
        {
            return -1;
        }
        if((ret = file->readnBytes(&c, 1)) == READ_EOF)
        {
            break;
        }
        if(ret != 1)
        {
            pos = -1;
            break;
        }
        if(c == '\r')
        {
            if((ret = file->readnBytes(&c, 1)) == READ_EOF)
            {
                break;
            }
            if(ret != 1)
            {
                pos = -1;
                break;
            }
            if(c != '\n')
            {
                break;
            }
            break;
        }
        buffer[pos] = c;
    }
    if(pos < 0)
        return -1;
    buffer[pos] = 0;
    return pos;
}

//从源处载入配置
bool FileConfig::loadAll()
{
    char buf[BUF_SIZE], key[BUF_SIZE], value[BUF_SIZE];
    int ret;
    File f;
    if(_configs == NULL)
    {
        return false;
    }
    if(!f.open_append(NULL, file))
    {
        return false;
    }
    if(!f.setReadWritePos(0))
    {
        return false;
    }
    while((ret = loadLine(&f, buf, BUF_SIZE)) > 0)
    {
        int i_key = 0, i_value = 0, c = 0;// c 五状态 0开始 1key内容 2中间 3value内容 4结束
        //bool ill = false;
        for(int i = 0; i < ret; i++)
        {
#define ill(x) (x < '0' && x != '-') || (x > '9' && x < 'A') || (x > 'Z' && x < 'a' && x != '_') || x > 'z'
#define unprintable(x) x < ' ' || x > '~'
#define trim(x,n) for(int i = n-1; i >= 0; i--) if(x[i] == ' ')x[i] = 0; else break;
            char ch = buf[i];
            switch(c)
            {
                case 0 : //开始
                if(ill(ch))
                {
                    //非法字符，去掉空格
                    if(ch == ' ')
                    {
                        continue;
                    }
                    //ill = true;
                    goto ill_line;
                }
                c = 1;
                key[i_key] = ch;
                i_key++;
                break;
                case 1 : //key内容
                if(ill(ch))
                {
                    //非法字符，包括空格
                    if(ch == ' ')
                    {
                        if(i < ret && (buf[i + 1] == ' ' || buf[i + 1] == '='))
                        {
                            continue;
                        }
                    }
                    if(ch == '=')
                    {
                        key[i_key] = 0;
                        c = 2;
                        continue;
                    }
                    goto ill_line;
                }
                key[i_key] = ch;
                i_key++;
                break;
                case 2:  //中间
                if(ill(ch))
                {
                    if(ch == ' ')
                    {
                        continue;
                    }
                    goto ill_line;
                }
                c = 3;
                value[i_value] = ch;
                i_value++;
                break;
                case 3: //value内容
                if(unprintable(ch))
                {
                    goto ill_line;
                }
                value[i_value] = ch;
                i_value++;
            }
        }
        if(c != 3)
        {
            continue;
        }
        value[i_value] = 0;
        trim(value, i_value);
        printf("key : %s, value : %s\n", key, value);
        _configs->insert(pair<string,string>(key, value));
ill_line:
        ;
    }
    if(!f.close())
    {
        return false;
    }
    return true;
}
//保存配置到源处
bool FileConfig::saveAll()
{
    File f;
    map<string, string>::iterator iter;
    if(_configs == NULL)
    {
        return false;
    }
    if(!f.open_append(NULL, file))
    {
        return false;
    }
    if(!f.setSize(0))
    {
        return false;
    }
    if(!f.setReadWritePos(0))
    {
        return false;
    }
    for(iter = _configs->begin(); iter != _configs->end(); iter++)
    {
        f.writenBytes(iter->first.c_str(), iter->first.size());
        f.writenBytes("=", 1);
        f.writenBytes(iter->second.c_str(), iter->second.size());
        f.writenBytes("\n", 1);
    }
    if(!f.close())
    {
        return false;
    }
    return true;
}
