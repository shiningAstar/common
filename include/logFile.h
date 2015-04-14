#ifndef LOGFILE_H
#define LOGFILE_H

#include <logBase.h>
#include "File.h"


class logFile : public logBase
{
    public:
        /** Default constructor */
        logFile();
        /** Default destructor */
        virtual ~logFile();

        virtual int log(char *log_txt, ...);
        virtual int vlog(char *log_txt, va_list va);
        virtual int flush();

        bool init(char *log_file_path, char *log_file_name);
        //member variable
        File file;
        char buffer[32768];
    protected:
    private:
};

#endif // LOGFILE_H
