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
    sem = NULL;
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

#define SEM_FAILED NULL

bool SemaphoreInProcessPV::init(int value)
{

    if(value < 0 || value > SEM_VALUE_MAX)
    {
        return false;
    }
    if(sem_init(&sem, 1, value) < 0)
    {
        sem = NULL;
        return false;
    }
    return true;
}

bool SemaphoreInProcessPV::P()
{
    if(sem == NULL)
    {
        return false;
    }
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

    if(sem == NULL)
    {
        return false;
    }
    if(sem_timedwait(&sem, &time) < 0)
    {
        return false;
    }
        return true;
}

bool SemaphoreInProcessPV::tryP()
{

    if(sem == NULL)
    {
        return false;
    }
    if(sem_trywait(&sem) < 0)
    {
        return false;
    }
    return true;

}

bool SemaphoreInProcessPV::V()
{

    if(sem == NULL)
    {
        return false;
    }
    if(sem_post(&sem) < 0)
    {
        return false;
    }
    return true;

}

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

int SemaphoreInProcessPV::getValue()
{
    int value;

    if(sem == NULL)
    {
        return -1;
    }
    if(sem_getvalue(&sem, &value) < 0)
    {
        return -1;
    }
    return value;

}
bool SemaphoreInProcessPV::available()
{

    return sem != NULL;

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

#define SEM_FAILED NULL

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
