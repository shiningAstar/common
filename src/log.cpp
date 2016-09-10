#include "log.h"
#include "stdlib.h"
#include "string.h"
#include "stdarg.h"
#include "time.h"

log::log()
{
    //ctor
    memset(log_func_list, 0, sizeof(log_func_list));
    num_of_log = 0;
    log_level = LOG_LEVEL_INFO;
}

log::~log()
{
    //dtor
}

log* log::getInstance()
{
    static log in_log;
    return &in_log;
}

bool log::init(logBase **log_first, int num, int level)
{
    if(log_first == NULL || num <= 0 || level < LOG_LEVEL_DEBUG || level > LOG_LEVEL_NONE)
    {
        return false;
    }
    for(int i = 0; i < num; i++)
    {
        log_func_list[i] = log_first[i];
    }
    num_of_log = num;
    log_level = level;
    return true;
}

char * get_asc_time_without_reline()
{
    time_t t;
    time(&t);
    char *time_char = asctime(localtime(&t));
    if(time_char == NULL)
    {
        return NULL;
    }
    time_char[strlen(time_char) - 1] = 0;
    return time_char;
}

bool log::output_log(char *log_txt, ...)
{
    bool ret = true;
    va_list arg_list;
    if(num_of_log <= 0)
    {
        return false;
    }
    va_start(arg_list, log_txt);
    for(int i = 0; i < num_of_log; i++)
    {
        if(log_func_list[i]->vlog(log_txt, arg_list) < 0)
        {
            ret = false;
        }
    }
    va_end(arg_list);
    return ret;
}

bool log::voutput_log(char *log_txt, va_list va)
{
    bool ret = true;
    if(num_of_log <= 0)
    {
        return false;
    }
    for(int i = 0; i < num_of_log; i++)
    {
        if(log_func_list[i]->vlog(log_txt, va) < 0)
        {
            ret = false;
        }
    }
    return ret;
}

char *level_str[4] = {"[debug]", "[info]", "[warn]", "[error]"};

bool log::output_log_with_level(int level, char *log_txt, ...)
{
    bool ret = true;
    va_list arg_list;
    //if(level < log_level || level < 0 || level > 3)
    if(level < 0 || level > 3)
    {
        return false;
    }
    if(num_of_log <= 0)
    {
        return false;
    }
    va_start(arg_list, log_txt);
    for(int i = 0; i < num_of_log; i++)
    {
        if(level < log_func_list[i]->level)
            continue;
        if(log_func_list[i]->log(level_str[level]) < 0)
        {
            ret = false;
        }
        if(log_func_list[i]->vlog(log_txt, arg_list) < 0)
        {
            ret = false;
        }
    }
    va_end(arg_list);
    return ret;
}

bool log::voutput_log_with_level(int level, char *log_txt, va_list va)
{
    bool ret = true;
    //if(level < log_level || level < 0 || level > 3)
    if(level < 0 || level > 3)
    {
        return false;
    }
    if(num_of_log <= 0)
    {
        return false;
    }
    for(int i = 0; i < num_of_log; i++)
    {
        if(level < log_func_list[i]->level)
            continue;
        if(log_func_list[i]->log(level_str[level]) < 0)
        {
            ret = false;
        }
        if(log_func_list[i]->vlog(log_txt, va) < 0)
        {
            ret = false;
        }
    }
    return ret;
}

bool log::output_log_with_level_line(int level, char *log_txt, ...)
{
    bool ret = true;
    va_list arg_list;
    //if(level < log_level || level < 0 || level > 3)
    if(level < 0 || level > 3)
    {
        return false;
    }
    if(num_of_log <= 0)
    {
        return false;
    }
    va_start(arg_list, log_txt);
    for(int i = 0; i < num_of_log; i++)
    {
        if(level < log_func_list[i]->level)
            continue;
        if(log_func_list[i]->log(level_str[level]) < 0)
        {
            ret = false;
        }
        if(log_func_list[i]->vlog(log_txt, arg_list) < 0)
        {
            ret = false;
        }
        if(log_func_list[i]->log("\n") < 0)
        {
            ret = false;
        }
    }
    va_end(arg_list);
    return ret;
}

bool log::voutput_log_with_level_line(int level, char *log_txt, va_list va)
{
    bool ret = true;
    //if(level < log_level || level < 0 || level > 3)
    if(level < 0 || level > 3)
    {
        return false;
    }
    if(num_of_log <= 0)
    {
        return false;
    }
    for(int i = 0; i < num_of_log; i++)
    {
        if(level < log_func_list[i]->level)
            continue;
        if(log_func_list[i]->log(level_str[level]) < 0)
        {
            ret = false;
        }
        if(log_func_list[i]->vlog(log_txt, va) < 0)
        {
            ret = false;
        }
        if(log_func_list[i]->log("\n") < 0)
        {
            ret = false;
        }
    }
    return ret;
}

bool log::output_log_with_level_time_line(int level, char *log_txt, ...)
{
    bool ret = true;
    va_list arg_list;
    char * time_char;
    //if(level < log_level || level < 0 || level > 3)
    if(level < 0 || level > 3)
    {
        return false;
    }
    if(num_of_log <= 0)
    {
        return false;
    }
    if((time_char = get_asc_time_without_reline()) == NULL)
    {
        return false;
    }
    va_start(arg_list, log_txt);
    for(int i = 0; i < num_of_log; i++)
    {
        if(level < log_func_list[i]->level)
            continue;
        if(log_func_list[i]->log(level_str[level]) < 0)
        {
            ret = false;
        }
        if(log_func_list[i]->vlog(log_txt, arg_list) < 0)
        {
            ret = false;
        }
        if(log_func_list[i]->log("(%s)\n", time_char) < 0)
        {
            ret = false;
        }
    }
    va_end(arg_list);
    return ret;
}

bool log::voutput_log_with_level_time_line(int level, char *log_txt, va_list va)
{
    bool ret = true;
    char * time_char;
    //if(level < log_level || level < 0 || level > 3)
    if(level < 0 || level > 3)
    {
        return false;
    }
    if(num_of_log <= 0)
    {
        return false;
    }
    if((time_char = get_asc_time_without_reline()) == NULL)
    {
        return false;
    }
    for(int i = 0; i < num_of_log; i++)
    {
        if(level < log_func_list[i]->level)
            continue;
        if(log_func_list[i]->log(level_str[level]) < 0)
        {
            ret = false;
        }
        if(log_func_list[i]->vlog(log_txt, va) < 0)
        {
            ret = false;
        }
        if(log_func_list[i]->log("(%s)\n", time_char) < 0)
        {
            ret = false;
        }
    }
    return ret;
}

bool log::output_log_with_time_line(char *log_txt, ...)
{
    bool ret = true;
    va_list arg_list;
    char * time_char = NULL;
    if(num_of_log <= 0)
    {
        return false;
    }
    if((time_char = get_asc_time_without_reline()) == NULL)
    {
        return false;
    }
    va_start(arg_list, log_txt);
    for(int i = 0; i < num_of_log; i++)
    {
        if(log_func_list[i]->vlog(log_txt, arg_list) < 0)
        {
            ret = false;
        }
        if(log_func_list[i]->log("(%s)\n", time_char) < 0)
        {
            ret = false;
        }
    }
    va_end(arg_list);
    return ret;
}

bool log::voutput_log_with_time_line(char *log_txt, va_list va)
{
    bool ret = true;
    char * time_char;
    if(num_of_log <= 0)
    {
        return false;
    }
    if((time_char = get_asc_time_without_reline()) == NULL)
    {
        return false;
    }
    for(int i = 0; i < num_of_log; i++)
    {
        if(log_func_list[i]->vlog(log_txt, va) < 0)
        {
            ret = false;
        }
        if(log_func_list[i]->log("(%s)\n", time_char) < 0)
        {
            ret = false;
        }
    }
    return ret;
}

bool log::debug(char *log_txt, ...)
{
    bool ret;
    va_list arg_list;
    va_start(arg_list, log_txt);
    ret = voutput_log_with_level(LOG_LEVEL_DEBUG, log_txt, arg_list);
    va_end(arg_list);
    return ret;
}

bool log::info(char *log_txt, ...)
{
    bool ret;
    va_list arg_list;
    va_start(arg_list, log_txt);
    ret = voutput_log_with_level(LOG_LEVEL_INFO, log_txt, arg_list);
    va_end(arg_list);
    return ret;
}

bool log::warn(char *log_txt, ...)
{
    bool ret;
    va_list arg_list;
    va_start(arg_list, log_txt);
    ret = voutput_log_with_level(LOG_LEVEL_WARN, log_txt, arg_list);
    va_end(arg_list);
    return ret;
}

bool log::error(char *log_txt, ...)
{
    bool ret;
    va_list arg_list;
    va_start(arg_list, log_txt);
    ret = voutput_log_with_level(LOG_LEVEL_ERROR, log_txt, arg_list);
    va_end(arg_list);
    return ret;
}

bool log::debug_line(char *log_txt, ...)
{
    bool ret;
    va_list arg_list;
    va_start(arg_list, log_txt);
    ret = voutput_log_with_level_line(LOG_LEVEL_DEBUG, log_txt, arg_list);
    va_end(arg_list);
    return ret;
}

bool log::info_line(char *log_txt, ...)
{
    bool ret;
    va_list arg_list;
    va_start(arg_list, log_txt);
    ret = voutput_log_with_level_line(LOG_LEVEL_INFO, log_txt, arg_list);
    va_end(arg_list);
    return ret;
}

bool log::warn_line(char *log_txt, ...)
{
    bool ret;
    va_list arg_list;
    va_start(arg_list, log_txt);
    ret = voutput_log_with_level_line(LOG_LEVEL_WARN, log_txt, arg_list);
    va_end(arg_list);
    return ret;
}

bool log::error_line(char *log_txt, ...)
{
    bool ret;
    va_list arg_list;
    va_start(arg_list, log_txt);
    ret = voutput_log_with_level_line(LOG_LEVEL_ERROR, log_txt, arg_list);
    va_end(arg_list);
    return ret;
}

bool log::debug_time_line(char *log_txt, ...)
{
    bool ret;
    va_list arg_list;
    va_start(arg_list, log_txt);
    ret = voutput_log_with_level_time_line(LOG_LEVEL_DEBUG, log_txt, arg_list);
    va_end(arg_list);
    return ret;
}

bool log::info_time_line(char *log_txt, ...)
{
    bool ret;
    va_list arg_list;
    va_start(arg_list, log_txt);
    ret = voutput_log_with_level_time_line(LOG_LEVEL_INFO, log_txt, arg_list);
    va_end(arg_list);
    return ret;
}

bool log::warn_time_line(char *log_txt, ...)
{
    bool ret;
    va_list arg_list;
    va_start(arg_list, log_txt);
    ret = voutput_log_with_level_time_line(LOG_LEVEL_WARN, log_txt, arg_list);
    va_end(arg_list);
    return ret;
}

bool log::error_time_line(char *log_txt, ...)
{
    bool ret;
    va_list arg_list;
    va_start(arg_list, log_txt);
    ret = voutput_log_with_level_time_line(LOG_LEVEL_ERROR, log_txt, arg_list);
    va_end(arg_list);
    return ret;
}
