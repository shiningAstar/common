#ifndef ERROR_H
#define ERROR_H

#include "errno.h"
#include "string.h"

namespace Error
{
    //���ش����errno
    inline int no()
    {
        return errno;
    }
    //���ش��������ַ�
    inline char* msg()
    {
        return strerror(errno);
    }
};

#endif // ERROR_H
