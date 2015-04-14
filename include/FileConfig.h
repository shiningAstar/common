#ifndef FILECONFIG_H
#define FILECONFIG_H

#include "ConfigBase.h"
#include "File.h"


class FileConfig : public ConfigBase
{
    public:
        //����FileConfig����ӿڣ�����ģʽ
        //static FileConfig *get_inst(char *file);

        /** Default constructor */
        FileConfig(char *file);
        /** Default destructor */
        virtual ~FileConfig();

        //��Դ����������
        virtual bool loadAll();
        //�������õ�Դ��
        virtual bool saveAll();

    protected:
    private:
        char file[256];

};

#endif // FILECONFIG_H
