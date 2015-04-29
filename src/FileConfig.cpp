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

//���ļ�file�ж�ȡһ�У���β��\n��\r\n��ʶ�������еĳ���>=0,�����<0
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

//��Դ����������
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
        int i_key = 0, i_value = 0, c = 0;// c ��״̬ 0��ʼ 1key���� 2�м� 3value���� 4����
        //bool ill = false;
        for(int i = 0; i < ret; i++)
        {
#define ill(x) (x < '0' && x != '-') || (x > '9' && x < 'A') || (x > 'Z' && x < 'a' && x != '_') || x > 'z'
#define unprintable(x) x < ' ' || x > '~'
#define trim(x,n) for(int i = n-1; i >= 0; i--) if(x[i] == ' ')x[i] = 0; else break;
            char ch = buf[i];
            switch(c)
            {
                case 0 : //��ʼ
                if(ill(ch))
                {
                    //�Ƿ��ַ���ȥ���ո�
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
                case 1 : //key����
                if(ill(ch))
                {
                    //�Ƿ��ַ��������ո�
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
                case 2:  //�м�
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
                case 3: //value����
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
//�������õ�Դ��
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
