#ifndef FILECONFIG_H
#define FILECONFIG_H

#include "ConfigBase.h"
#include "File.h"


class FileConfig : public ConfigBase
{
    public:
        //访问FileConfig对象接口，单例模式
        //static FileConfig *get_inst(char *file);

        /** Default constructor */
        FileConfig(char *file);
        /** Default destructor */
        virtual ~FileConfig();

        //从源处载入配置
        virtual bool loadAll();
        //保存配置到源处
        virtual bool saveAll();

    protected:
    private:
        char file[256];

};

#endif // FILECONFIG_H
