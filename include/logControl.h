#ifndef LOGCONTROL_H
#define LOGCONTROL_H

#include <logBase.h>
#include "ControlwithLog.h"


class logControl : public logBase
{
    public:
        /** Default constructor */
        logControl();
        /** Default destructor */
        virtual ~logControl();

        virtual int log(char *log_txt, ...);
        virtual int vlog(char *log_txt, va_list va);
        virtual int flush();

        ControlwithLog *control;

        bool init(ControlwithLog *control);

        char buffer[32768];

    protected:
    private:
};

#endif // LOGCONTROL_H
