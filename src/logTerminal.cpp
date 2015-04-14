#include "logTerminal.h"
#include "stdarg.h"
#include "stdio.h"

logTerminal::logTerminal()
{
    //ctor
}

logTerminal::~logTerminal()
{
    //dtor
}

int logTerminal::log(char *log_txt, ...)
{
    int ret;
    va_list arg_list;
    va_start(arg_list, log_txt);
    ret = vprintf(log_txt,arg_list);
    va_end(arg_list);
    return ret;
}

int logTerminal::vlog(char *log_txt, va_list va)
{
    return vprintf(log_txt, va);
}

int logTerminal::flush()
{
    fflush(stdout);
    return 0;
}
