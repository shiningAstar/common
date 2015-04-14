#ifndef ERROR_H
#define ERROR_H

#include "errno.h"
#include "string.h"

namespace Error
{
    //·µ»Ø´íÎóºÅerrno
    inline int no()
    {
        return errno;
    }
    //·µ»Ø´íÎóÃèÊö×Ö·û
    inline char* msg()
    {
        return strerror(errno);
    }
};

#endif // ERROR_H
