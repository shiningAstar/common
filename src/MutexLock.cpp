#include "MutexLock.h"
#include "log.h"
#include "stdio.h"
#include "string.h"
#if defined(WIN32) || defined(_WINDOWS) || defined(_WIN32)
#include "windows.h"
int g_mutex_use_global_name = 1;
#define mutex_name_prefix "mutex."
#endif // defined

//#define use_log //use or comment this to use/unuse the log component in this library
MutexLockBase::MutexLockBase()
{

}

MutexLockBase::~MutexLockBase()
{

}

MutexLockInProcess::MutexLockInProcess() : _holder(0)
{
    //ctor
#ifdef _WIN32
    _mutex = NULL;
#else
    memset(&_mutex, 0, sizeof(_mutex));
#endif
    if(pthread_mutex_init(&_mutex, NULL) < 0)
    {
#ifdef _WIN32
    _mutex = NULL;
#else
    memset(&_mutex, 0, sizeof(_mutex));
#endif
        //err = errno;
    }
}

MutexLockInProcess::~MutexLockInProcess()
{
    //dtor
    if(_holder != 0)
    {
//#ifdef use_log
//        log_warn_time_line("mutex destructed while locking. locking thread id：%d.", _holder);
//#else
        printf("mutex destructed while locking. locking thread id：%d.\n", _holder);
//#endif // use_log
    }
    pthread_mutex_destroy(&_mutex);
}

void MutexLockInProcess::lock()
{
    pthread_mutex_lock(&_mutex);
    _holder = CurrentThread::tid();
}

void MutexLockInProcess::unlock()
{
    if(_holder != CurrentThread::tid())
    {
        return;
    }
    _holder = 0;
    pthread_mutex_unlock(&_mutex);
}

bool MutexLockInProcess::lockedByThisThread()
{
    return _holder == CurrentThread::tid();
}

MutexLockOutProcess::MutexLockOutProcess() : _holder(0)
{
    //ctor
    //_holder = 0;
}

MutexLockOutProcess::MutexLockOutProcess(char *name, int open_flag):_holder(0)
{

    init(name, open_flag);
}

MutexLockOutProcess::~MutexLockOutProcess()
{
    //dtor
    #ifndef _WIN32
    if(_holder != 0)
    {
//#ifdef use_log
//        log_warn_time_line("mutex destructed while locking. locking thread id：%d.", _holder);
//#else
        printf("mutex destructed while locking. locking thread id：%d.\n", _holder);;
//#endif // use_log
    }
    #else
    CloseHandle(h_mutex);
    #endif // _WIN32

}

bool MutexLockOutProcess::init(char *name, int open_flag)
{
    if(name == NULL)
	{
		return false;
	}
    #ifndef _WIN32
    return sem.init(name, open_flag, 1);
    #else
    char filename[128]={0};
	MEMORY_BASIC_INFORMATION  mm_info;
	SECURITY_ATTRIBUTES sa ={0};
	SECURITY_DESCRIPTOR sd ={0};

	InitializeSecurityDescriptor( &sd, SECURITY_DESCRIPTOR_REVISION );
	SetSecurityDescriptorDacl(&sd,TRUE,NULL,FALSE);  //pDACL参数传NULL表示建立NULL DACL，允许所有的访问

	sa.nLength = sizeof(sa);
    sa.lpSecurityDescriptor = &sd;
    sa.bInheritHandle = FALSE;

	if(g_mutex_use_global_name)
		strncpy(filename,"Global\\",64);

	strncat(filename,mutex_name_prefix,64);
	strncat(filename,name,64);
    switch( open_flag )
    {
    case OPEN_EXIST:
        if( ( h_mutex = OpenMutexA( MUTEX_ALL_ACCESS, FALSE, name ) ) == NULL )
        {
            return false;
        }

        break;

    case OPEN_OR_CREATE:
        if( ( h_mutex = CreateMutexA( &sa, FALSE, name ) ) == NULL )
        {
            return false;
        }

        break;

    case CREATE_ONLY:
        SetLastError( 0 );

        if( ( h_mutex = CreateMutexA( &sa, FALSE, name ) ) == NULL )
        {
            return false;
        }

        if( GetLastError() == ERROR_ALREADY_EXISTS )
        {
            CloseHandle(h_mutex);
            return false;
        }

        break;
    }

    return true;
    #endif // _WIN32

}

void MutexLockOutProcess::lock()
{
    #ifndef _WIN32
    sem.P();
    _holder = CurrentThread::tid();
    #else
        if(WaitForSingleObject(h_mutex,INFINITE) == WAIT_FAILED)
        {
            return;
        }
    #endif // _WIN32

//#ifndef _WIN32

/* #else
 *     _holder = (unsigned long)GetCurrentThread();
 * #endif
 */
}

void MutexLockOutProcess::unlock()
{
    #ifndef _WIN32
    if(_holder != CurrentThread::tid())
    {
        return;
    }
    _holder = 0;

    sem.V();
    #else
    if(!ReleaseMutex(h_mutex))
    {
        return;
    }
    #endif

}

//bool MutexLockOutProcess::lockedByThisThread()
//{
//    return _holder == (unsigned long)GetCurrentThread();
//}

