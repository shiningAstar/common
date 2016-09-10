#include "SharedMemory.h"
#include "string.h"
#include "stdio.h"

#define prefix "sharememory."

#if defined(WIN32) || defined(_WINDOWS) || defined(_WIN32)
int sharememory_use_global_name = 0;
void sm_use_global_name(int is_use)
{
	if(is_use)
		sharememory_use_global_name = 1;
	else
		sharememory_use_global_name = 0;
}

#else

#include "sys/mman.h"
#include "fcntl.h"
#include "errno.h"
#include "sys/stat.h"
#include "unistd.h"

#endif

SharedMemory::SharedMemory() : size(0), mem_ptr(NULL)
{
    //ctor
    valueInit();
}

SharedMemory::~SharedMemory()
{
    //dtor

	if(mem_ptr == NULL)
	{
		return;
	}

#if defined(WIN32) || defined(_WINDOWS) || defined(_WIN32)
	UnmapViewOfFile(mem_ptr);
	CloseHandle(hfile);
#else

	munmap(mem_ptr, size);

#endif
}

SharedMemory::SharedMemory(char *name, size_t size) : size(size), mem_ptr(NULL)
{
    valueInit();
}

void SharedMemory::valueInit()
{
    memset(name, 0, sizeof(name));
#ifdef _WIN32
    hfile = INVALID_HANDLE_VALUE;
#endif // _WIN32
}

void *SharedMemory::getAddr()
{
    return mem_ptr;
}

unsigned long SharedMemory::getSize()
{
    return size;
}

bool SharedMemory::init(char *name, size_t size)
{
    char name_share[256];
    if(name == NULL || size < 0)
    {
        return false;
    }
#if defined(WIN32) || defined(_WINDOWS) || defined(_WIN32)
	MEMORY_BASIC_INFORMATION  mm_info;
	SECURITY_ATTRIBUTES sa ={0};
	SECURITY_DESCRIPTOR sd ={0};
#if defined(_WIN64)
	DWORD size_high = size >>32;
	DWORD size_low = size;
#else
	DWORD size_high = 0;
	DWORD size_low = size;
#endif

	InitializeSecurityDescriptor( &sd, SECURITY_DESCRIPTOR_REVISION );
	SetSecurityDescriptorDacl(&sd,TRUE,NULL,FALSE);  //pDACL参数传NULL表示建立NULL DACL，允许所有的访问

	sa.nLength = sizeof(sa);
    sa.lpSecurityDescriptor = &sd;
    sa.bInheritHandle = FALSE;

	if(sharememory_use_global_name)
		strncpy(this->name,"Global\\",64);

#endif
    strncpy(this->name, name, 128);
	strncat(name_share, prefix, 64);
	strncat(name_share, name,64);

	//strncpy(p_shm->name,shm_name,64 );
	//this->size = size;
	//p_shm->is_new=1;

	create = true;

	//判断是否存在
#if defined(WIN32) || defined(_WINDOWS) || defined(_WIN32)

	hfile = CreateFileMappingA(
		INVALID_HANDLE_VALUE,    // use paging file
		&sa,                    // default security
		PAGE_READWRITE,          // read/write access
		size_high,                       // maximum object size (high-order DWORD)
		size_low,                // maximum object size (low-order DWORD)
		this->name);                 // name of mapping object

	if (hfile == NULL)
	{
		printf("CreateFileMappingA %s error: (%d).\n" , this->name, GetLastError());
		return false;
	}

	//已经存在
	if(GetLastError() == ERROR_ALREADY_EXISTS)
	{
		create = false;
	}

	mem_ptr = MapViewOfFile(hfile,   // handle to map object
		FILE_MAP_ALL_ACCESS, // read/write permission
		0,
		0,
		0);

	if (mem_ptr == NULL)
	{
		printf("MapViewOfFile %s error: (%d).\n" , this->name, GetLastError());
		CloseHandle(hfile);
		return false;
	}

	//获取大小
	if(VirtualQuery(mem_ptr,&mm_info,sizeof(mm_info)) == 0)
	{
		printf("mm_init VirtualQuery %s error: (%d).\n" , this->name, GetLastError());
		CloseHandle(hfile);
		return false;
	}

	this->size= mm_info.RegionSize;
#else

    int fd;
    struct stat shm_stat;
    fd = shm_open(name_share, O_RDWR | O_CREAT | O_EXCL, 666);
    if(fd < 0)
    {
        if(errno != EEXIST)
        {
            printf("share memory shm_open %s failed. errno=%d \n", name_share, errno);
            return false;
        }
        printf("share memory shm_open EEXIST\n");
    	create = false;
        fd = shm_open(name_share, O_RDWR, 666);
        if (fd < 0)
        {
            printf("share memory shm_open %s failed. errno=%d \n", name_share, errno);
            return false;
        }
        if(fstat(fd,&shm_stat) < 0)
        {
            printf("share memory stat %s failed. errno=%d\n", name_share, errno);
            close(fd);
            return false;
        }
        this->size = shm_stat.st_size;
    }



    //创建设置大小
	if (create && ftruncate(fd, size) < 0)
	{
		printf("share memory ftruncate %s failed. errno=%d\n", name_share, errno);
		close(fd);
		return false;
    }
    if(create)
    {
        this->size = size;
    }
filemap:
    /* map memory to process */
    mem_ptr = mmap(NULL, size, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);

    close(fd);

    if(mem_ptr == MAP_FAILED)
    {
        printf("share memory mmap %s failed. errno=%d\n", name_share, errno);
        this->size = 0;
        return false;
    }

#endif
    //printf("shm_init:  %x---%x\n",p_shm,p_shm->shm);
    return true;
}

bool SharedMemory::finish()
{
    if (shm_unlink(name) < 0)
    {
        printf("share memory shm_unlink.\n");
        return false;
    }
    return true;
}
