//#include "stdio.h"
#include "stdarg.h"
#ifndef LOGBASE_H
#define LOGBASE_H


class logBase
{
    public:
        /** Default constructor */
        logBase();
        /** Default destructor */
        virtual ~logBase();
        virtual int log(char *log_txt, ...) = 0;
        virtual int vlog(char *log_txt, va_list va) = 0;
        virtual int flush() = 0;

        int level;
        char status;
    protected:
    private:
};

#endif // LOGBASE_H
