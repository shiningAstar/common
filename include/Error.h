#ifndef ERROR_H
#define ERROR_H

#include "errno.h"
#include "string.h"

namespace Error
{
    //���ش����errno
    inline int no()
    {
        #ifndef _WIN32
        return errno;
        #else
        return GetLastError();
        #endif // _WIN32
    }
    //���ش��������ַ�
    inline char* msg()
    {

        return strerror(errno);
    }
};

#endif // ERROR_H
