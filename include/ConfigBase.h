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

        //从源处载入配置
        virtual bool loadAll() = 0;
        //保存配置到源处
        virtual bool saveAll() = 0;

        bool getConf(const char *key, int key_buflen, char *cont, int cont_buflen);
        bool setConf(const char *key, int key_buflen, const char *cont, int cont_buflen);

        bool getConf(const int index, char *key, int key_buflen, char *value, int value_buflen);

        int numOfConf();

//        bool confBegin();
//        bool confEnd();
//        bool confNext();
//        bool getCurrentConf(char *key, int key_length, char *value, int value_length);


    protected:
        map<string, string> *_configs;
        map<string, string>::iterator cur;
        int index;
    private:

};

#define foreachconf(config,key,keylength,value,valuelength) \
            for(config.confBegin();!config.confEnd();if(config.confNext())config.getCurrentConf(key,keylength,value,valuelength))

#endif // CONFIG_H
