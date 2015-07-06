#include "SemaphorePV.h"
#include "stdlib.h"
#include "string.h"
#include "fcntl.h"
#include "Error.h"
#include "stdio.h"
#include "time.h"
#ifdef _WIN32
int g_sema_use_global_name = 1;
#endif // _WIN32
#define _DEBUG


#ifdef _DEBUG
#include "stdio.h"
#endif // _DEBUG
SemaphoreBase::SemaphoreBase()
{

}

SemaphoreBase::~SemaphoreBase()
{

}

SemaphoreInProcessPV::SemaphoreInProcessPV()
{
    //ctor
    valueInit();
}

SemaphoreInProcessPV::SemaphoreInProcessPV(int value)
{
    valueInit();
    init(value);
}

SemaphoreInProcessPV::~SemaphoreInProcessPV()
{
    //dtor
    sem_destroy(&sem);
}
#ifdef _WIN32
#define SEM_FAILED NULL
#else
#ifndef SEM_VALUE_MAX
#define SEM_VALUE_MAX 32767
#endif // SEM_VALUE_MAX
#endif // _WIN32

void SemaphoreInProcessPV::valueInit()
{
#ifdef _WIN32
    sem = NULL;
#else
    memset(&sem, 0, sizeof(sem_t));
    avail = false;
#endif
}

bool SemaphoreInProcessPV::init(int value)
{
    if(value < 0 || value > SEM_VALUE_MAX)
    {
        return false;
    }

#ifdef _WIN32
    if(sem_init(&sem, 0, value) < 0)
    {

        sem = NULL;
#else
    if(sem_init(&sem, 1, value) < 0)
    {
        memset(&sem, 0, sizeof(sem_t));
#endif
        return false;
    }
#ifndef _WIN32
    avail = true;
#endif // _WIN32
    return true;
}

bool SemaphoreInProcessPV::P()
{
#ifdef _WIN32
    if(sem == NULL)
    {
        return false;
    }
#else
    if(!avail)
    {
        return false;
    }
#endif
    if(sem_wait(&sem) < 0)
    {
        return false;
    }
    return true;
}
bool SemaphoreInProcessPV::P(long wait_sec, long wait_nsec)
{
    timespec time;
    if((wait_sec == 0 && wait_nsec == 0) || wait_sec < 0 || wait_nsec < 0)
    {
        return false;
    }
    memset(&time, 0, sizeof(timespec));
    time.tv_sec = ::time(NULL) + wait_sec;
    time.tv_nsec = wait_nsec;

#ifdef _WIN32
    if(sem == NULL)
    {
        return false;
    }
#else
    if(!avail)
    {
        return false;
    }
#endif
    if(sem_timedwait(&sem, &time) < 0)
    {
        return false;
    }
        return true;
}

bool SemaphoreInProcessPV::tryP()
{

#ifdef _WIN32
    if(sem == NULL)
    {
        return false;
    }
#else
    if(!avail)
    {
        return false;
    }
#endif
    if(sem_trywait(&sem) < 0)
    {
        return false;
    }
    return true;

}

bool SemaphoreInProcessPV::V()
{

#ifdef _WIN32
    if(sem == NULL)
    {
        return false;
    }
#else
    if(!avail)
    {
        return false;
    }
#endif
    if(sem_post(&sem) < 0)
    {
        return false;
    }
    return true;

}

#ifdef _WIN32

bool SemaphoreInProcessPV::V(int res_count)
{
    if(res_count <= 0 || res_count > SEM_VALUE_MAX)
    {
        return false;
    }

    if(sem == NULL)
    {
        return false;
    }
    if(sem_post_multiple(&sem, res_count) < 0)
    {
        return false;
    }
    return true;
}

#endif

int SemaphoreInProcessPV::getValue()
{
    int value;

#ifdef _WIN32
    if(sem == NULL)
    {
        return false;
    }
#else
    if(!avail)
    {
        return false;
    }
#endif
    if(sem_getvalue(&sem, &value) < 0)
    {
        return -1;
    }
    return value;

}
bool SemaphoreInProcessPV::available()
{
#ifdef _WIN32
    return sem != NULL;
#else
    return avail;
#endif
}

int SemaphoreInProcessPV::getErrno()
{
    return Error::no();
}

SemaphoreOutProcessPV::SemaphoreOutProcessPV()
{
    //ctor
    valueInit();

}

SemaphoreOutProcessPV::SemaphoreOutProcessPV(char *name, int open_flag, int value)
{
    valueInit();

    init(name, open_flag, value);
}

SemaphoreOutProcessPV::~SemaphoreOutProcessPV()
{
    //dtor
    #ifndef _WIN32
    sem_close(psem);
    sem_unlink(name);
    #else
    CloseHandle(h_sema);
    #endif // _WIN32


}

void SemaphoreOutProcessPV::valueInit()
{
    memset(this->name, 0, sizeof(name));
    #ifndef _WIN32
    psem = NULL;
    #else
    h_sema = NULL;
    #endif // _WIN32
}

bool SemaphoreOutProcessPV::init(char *name, int open_flag, int value)
{
    int oflag;
    if(name == NULL || value < 0 || value > SEM_VALUE_MAX || open_flag < OPEN_EXIST || open_flag > CREATE_ONLY)
    {
        return false;
    }
    #ifdef _WIN32
	char filename[128]={0};
	MEMORY_BASIC_INFORMATION  mm_info;
	SECURITY_ATTRIBUTES sa ={0};
	SECURITY_DESCRIPTOR sd ={0};

	InitializeSecurityDescriptor( &sd, SECURITY_DESCRIPTOR_REVISION );
	SetSecurityDescriptorDacl(&sd,TRUE,NULL,FALSE);  //pDACL参数传NULL表示建立NULL DACL，允许所有的访问

	sa.nLength = sizeof(sa);
    sa.lpSecurityDescriptor = &sd;
    sa.bInheritHandle = FALSE;

	if(g_sema_use_global_name)
		strncpy(filename,"Global\\",64);

	strncat(filename,sema_name_prefix,64);
	strncat(filename,name,64);
    #endif // _WIN32

    #ifndef _WIN32
    switch(open_flag)
    {
        case OPEN_EXIST: oflag = 0; break;
        case OPEN_OR_CREATE: oflag = O_CREAT; break;
        case CREATE_ONLY: oflag = O_CREAT | O_EXCL; break;
    }

    if((psem = (sem_t*)sem_open(name, oflag, 666, value)) == SEM_FAILED || psem == (sem_t*)0xfffffffff )
    {
        psem = NULL;
        return false;
    }
    #else

     switch(open_flag)
    {
        case OPEN_EXIST:
            if( ( h_sema = OpenSemaphoreA( SEMAPHORE_ALL_ACCESS , TRUE, filename ) ) == NULL )
                return false;
            break;
        case OPEN_OR_CREATE:
            if( ( h_sema = CreateSemaphoreA( &sa, value, 666, filename ) ) == NULL )
                {
                    return false;
                }
            break;
        case CREATE_ONLY:
            SetLastError(0);
            if( ( h_sema = CreateSemaphoreA( &sa, value, 666, filename ) ) == NULL)
               {
                  return false;
               }

            if(GetLastError() == ERROR_ALREADY_EXISTS)
            {
                CloseHandle(h_sema);
                return false;
            }
         break;
    }
    #endif // _WIN32

    strncpy(this->name, name, sizeof(this->name));
    return true;
}

bool SemaphoreOutProcessPV::P()
{
#ifndef _WIN32

        if( psem == NULL )
        {
            return false;
        }

        if( sem_wait( psem ) < 0 )
        {
            return false;
        }

#else

        if( h_sema == NULL )
        {
            return false;
        }

        if( WaitForSingleObject( h_sema, INFINITE ) == WAIT_FAILED )
        {
            return false;
        }
#endif // _WIN32
    return true;
}

bool SemaphoreOutProcessPV::P(long wait_sec, long wait_nsec)
{
    #ifndef _WIN32
    timespec time;
    if((wait_sec == 0 && wait_nsec == 0) || wait_sec < 0 || wait_nsec < 0)
    {
        return false;
    }
    time.tv_sec = wait_sec;
    time.tv_nsec = wait_nsec;
    if(psem == NULL)
    {
        return false;
    }
    if((sem_timedwait(psem, &time)) < 0)
    {
        return false;
    }
    #else
    if(WaitForSingleObject(h_sema,wait_nsec+wait_sec * 1000) == WAIT_TIMEOUT)
    {
        return false;
    }
    #endif // _WIN32

    return true;
}
#ifndef _WIN32
bool SemaphoreOutProcessPV::tryP()
{
    if(psem == NULL)
    {
        return false;
    }
    if(sem_trywait(psem) < 0)
    {
        return false;
    }
    return true;
}
#endif // _WIN32

bool SemaphoreOutProcessPV::V()
{

#ifndef _WIN32

        if( psem == NULL )
        {
            return false;
        }

        if( sem_post( psem ) < 0 )
        {
            return false;
        }

#else
        LONG lpl = 0 ;
        if( h_sema == NULL )
        {
            return false;
        }

        if( ReleaseSemaphore( h_sema, 1, &lpl ) == false )
        {
            return false;
        }

        #ifdef _DEBUG
        printf("the value after v is %d\n",lpl + 1);
        #endif // _DEBUG
#endif // _WIN32

        return true;
}

#ifdef _WIN32

bool SemaphoreOutProcessPV::V(int res_count)
{
    LONG lpl = 0 ;
    if(res_count <= 0 || res_count > SEM_VALUE_MAX)
    {
        return false;
    }
    if(h_sema == NULL)
    {
        return false;
    }

    if(ReleaseSemaphore(h_sema,res_count,&lpl) == false)
    {
        return false;
    }
    #ifdef _DEBUG
    printf("the value after v is %d\n",lpl + res_count);
    #endif // _DEBUG
    return true;
}

#endif

#ifndef _WIN32
int SemaphoreOutProcessPV::getValue()
{
    int value;

    if(psem == NULL)
    {
        return -1;
    }

    if(sem_getvalue(psem, &value) < 0)
    {
        return -1;
    }
    return value;

}
#endif // _WIN32
bool SemaphoreOutProcessPV::available()
{
#ifndef _WIN32
    return psem != NULL;
#else
    return h_sema != NULL;
#endif // _WIN32

}

int SemaphoreOutProcessPV::getErrno()
{
#ifndef _WIN32
        return Error::no();
#else
        return GetLastError();
#endif // _WIN32

}
