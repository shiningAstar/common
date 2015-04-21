#include "SemaphorePV.h"
#include "stdlib.h"
#include "string.h"
#include "fcntl.h"
#include "Error.h"

SemaphoreBase::SemaphoreBase()
{

}

SemaphoreBase::~SemaphoreBase()
{

}

SemaphoreInProcessPV::SemaphoreInProcessPV()
{
    //ctor
#ifdef _WIN32
    sem = NULL;
#else
    memset(&sem, 0, sizeof(sem_t));
#endif
}

SemaphoreInProcessPV::SemaphoreInProcessPV(int value)
{
    SemaphoreInProcessPV();
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

bool SemaphoreInProcessPV::init(int value)
{

    if(value < 0 || value > SEM_VALUE_MAX)
    {
        return false;
    }
    if(sem_init(&sem, 1, value) < 0)
    {
#ifdef _WIN32
        sem = NULL;
#else
        memset(&sem, 0, sizeof(sem_t));
#endif
        return false;
    }
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
    sem_t s;
    memset(&s, 0, sizeof(sem_t));
    if(memcmp(&sem, &s, sizeof(sem_t)) == 0)
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
    time.tv_sec = wait_sec;
    time.tv_nsec = wait_nsec;

#ifdef _WIN32
    if(sem == NULL)
    {
        return false;
    }
#else
    sem_t s;
    memset(&s, 0, sizeof(sem_t));
    if(memcmp(&sem, &s, sizeof(sem_t)) == 0)
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
    sem_t s;
    memset(&s, 0, sizeof(sem_t));
    if(memcmp(&sem, &s, sizeof(sem_t)) == 0)
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
    sem_t s;
    memset(&s, 0, sizeof(sem_t));
    if(memcmp(&sem, &s, sizeof(sem_t)) == 0)
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

#ifdef _WIN32
    if(sem == NULL)
    {
        return false;
    }
#else
    sem_t s;
    memset(&s, 0, sizeof(sem_t));
    if(memcmp(&sem, &s, sizeof(sem_t)) == 0)
    {
        return false;
    }
#endif
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
    sem_t s;
    memset(&s, 0, sizeof(sem_t));
    if(memcmp(&sem, &s, sizeof(sem_t)) == 0)
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
    sem_t s;
    memset(&s, 0, sizeof(sem_t));
    return memcmp(&sem, &s, sizeof(sem_t)) != 0;
#endif
}

int SemaphoreInProcessPV::getErrno()
{
    return Error::no();
}

SemaphoreOutProcessPV::SemaphoreOutProcessPV()
{
    //ctor
    memset(this->name, 0, sizeof(name));
    psem = NULL;
}

SemaphoreOutProcessPV::SemaphoreOutProcessPV(char *name, int open_flag, int value)
{
    SemaphoreOutProcessPV();
    init(name, open_flag, value);
}

SemaphoreOutProcessPV::~SemaphoreOutProcessPV()
{
    //dtor
    sem_close(psem);
    sem_unlink(name);

}

bool SemaphoreOutProcessPV::init(char *name, int open_flag, int value)
{
    int oflag;
    if(name == NULL || value < 0 || value > SEM_VALUE_MAX || open_flag < OPEN_EXIST || open_flag > CREATE_ONLY)
    {
        return false;
    }
    switch(open_flag)
    {
        case OPEN_EXIST: oflag = 0; break;
        case OPEN_OR_CREATE: oflag = O_CREAT; break;
        case CREATE_ONLY: oflag = O_CREAT | O_EXCL; break;
    }
    if((psem = (sem_t*)sem_open(name, oflag, 666, value)) == SEM_FAILED)
    {
        psem = NULL;
        return false;
    }
    strncpy(this->name, name, sizeof(this->name));
    return true;
}

bool SemaphoreOutProcessPV::P()
{
    if(psem == NULL)
    {
        return false;
    }
    if(sem_wait(psem) < 0)
    {
        return false;
    }
    return true;
}
bool SemaphoreOutProcessPV::P(long wait_sec, long wait_nsec)
{
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
    if(sem_timedwait(psem, &time) < 0)
    {
        return false;
    }
    return true;
}
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

bool SemaphoreOutProcessPV::V()
{
    if(psem == NULL)
    {
        return false;
    }
    if(sem_post(psem) < 0)
    {
        return false;
    }
    return true;
}

#ifdef _WIN32

bool SemaphoreOutProcessPV::V(int res_count)
{
    if(res_count <= 0 || res_count > SEM_VALUE_MAX)
    {
        return false;
    }
    if(psem == NULL)
    {
        return false;
    }
    if(sem_post_multiple(psem, res_count) < 0)
    {
        return false;
    }
    return true;
}

#endif

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
bool SemaphoreOutProcessPV::available()
{
    return psem != NULL;
}

int SemaphoreOutProcessPV::getErrno()
{
    return Error::no();
}
