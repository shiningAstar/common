#ifndef CONFIG_H
#define CONFIG_H

#include "map"
#include "string"
#include "string.h"

using namespace std;

class ConfigBase
{
    public:

        /** Default constructor */
        ConfigBase();
        /** Default destructor */
        virtual ~ConfigBase();

        //��Դ����������
        virtual bool loadAll() = 0;
        //�������õ�Դ��
        virtual bool saveAll() = 0;

        bool getConf(const char *key, char *cont, int length);
        bool setConf(const char *key, const char *cont, int length);

        bool getConf(const int index, char *key, int key_length, char *value, int value_length);

        int numOfConf();


    protected:
        map<string, string> *_configs;
    private:

};

#endif // CONFIG_H
