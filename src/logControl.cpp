#include "logControl.h"
#include "stdio.h"
#include "stdarg.h"

logControl::logControl()
{
    //ctor
    control = NULL;
}

logControl::~logControl()
{
    //dtor
}

int logControl::log(char *log_txt, ...)
{
    if(control == NULL)
    {
        return -1;
    }
    va_list arg_list;
    va_start(arg_list, log_txt);
    vsprintf(buffer, log_txt, arg_list);
    va_end(arg_list);
    if(!control->log_in_control(buffer, sizeof(buffer)))
    {
        return -1;
    }
    return 0;
}

int logControl::vlog(char *log_txt, va_list va)
{
    if(control == NULL)
    {
        return -1;
    }
    sprintf(buffer, log_txt, va);
    if(!control->log_in_control(buffer, sizeof(buffer)))
    {
        return -1;
    }
    return 0;
}

int logControl::flush()
{
    return 0;
}

bool logControl::init(ControlwithLog *control)
{
    if(control == NULL)
    {
        return false;
    }
    this->control = control;
    return true;
}
