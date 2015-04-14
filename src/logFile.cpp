#include "logFile.h"
#include "stdio.h"

logFile::logFile()
{
    //ctor
}

logFile::~logFile()
{
    //dtor
    if(!file.available())
    {
        return;
    }
    file.close();
}

bool logFile::init(char *log_file_path, char *log_file_name)
{
    if(!file.open(log_file_path, log_file_name, O_RDWR | O_CREAT | O_APPEND))
    {
        printf("log file open error : %d.\n", (int)file.error);
        return false;
    }
    if(!file.available())
    {
        printf("log file not available.\n");
        return false;
    }
    return true;
}

int logFile::log(char *log_txt, ...)
{
    int ret;
    static bool log_valid = true;
    //如果文件失效，重试打开一次，如果重打开错误，后面log函数不做动作
    if(!file.available())
    {
        if(!log_valid)
            return 0;
        if(!file.reopen())
        {
            log_valid = false;
            printf("log file not available. reopen error.\n");
            return 0;
        }
        printf("log file not available. reopen ok.\n");
    }
    va_list arg_list;
    va_start(arg_list, log_txt);
    vsprintf(buffer, log_txt, arg_list);
    va_end(arg_list);
    ret = file.writenBytes(buffer, strlen(buffer));
    return ret;
}

int logFile::vlog(char *log_txt, va_list va)
{
    int ret;
    static bool log_valid = true;
    //如果文件失效，重试打开一次，如果重打开错误，后面log函数不做动作
    if(!file.available())
    {
        if(!log_valid)
            return 0;
        if(!file.reopen())
        {
            log_valid = false;
            printf("log file not available. reopen error.\n");
            return 0;
        }
        printf("log file not available. reopen ok.\n");
    }
    vsprintf(buffer, log_txt, va);
    ret = file.writenBytes(buffer, strlen(buffer));
    return ret;
}

int logFile::flush()
{
    file.flush();
    return 0;
}
