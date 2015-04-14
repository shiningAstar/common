#ifndef LOGTERMINAL_H
#define LOGTERMINAL_H

#include <logBase.h>


class logTerminal : public logBase
{
    public:
        /** Default constructor */
        logTerminal();
        /** Default destructor */
        virtual ~logTerminal();

        virtual int log(char *log_txt, ...);
        virtual int vlog(char *log_txt, va_list va);
        virtual int flush();
    protected:
    private:
};

#endif // LOGTERMINAL_H
