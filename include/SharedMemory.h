#ifndef SHAREDMEMORY_H
#define SHAREDMEMORY_H

#ifdef _WIN32
#include "windows.h"
#endif // _WIN32

#include "stdlib.h"

class SharedMemory
{
    public:
        /** Default constructor */
        SharedMemory();

        SharedMemory(char *name, size_t size);
        /** Default destructor */
        virtual ~SharedMemory();

        bool init(char *name, size_t size);

    protected:
        char name[128];
        size_t size;
        bool create;
        #ifdef _WIN32
        HANDLE hfile;
        #endif // _WIN32
        void *mem_ptr;
    private:
};

#endif // SHAREDMEMORY_H
