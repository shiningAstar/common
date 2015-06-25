/*
 * File.cpp
 *
 *  Created on: 2012-5-25
 *      Author: root
 */

#include "File.h"
#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "errno.h"
#include "unistd.h"
#include "sys/stat.h"
#include "unistd.h"

//#include "linuxh.h"
File::File()
{
	// TODO Auto-generated constructor stub
	#ifdef _WIN32
	fd = (HANDLE)0xffffffff;
	#else
	fd = -1;
	#endif
	memset(name, 0, sizeof(name));
	memset(path, 0, sizeof(path));
}

File::~File()
{
	// TODO Auto-generated destructor stub
	close();
}

bool File::open_create(char* path, char* name)
{
    return open(path, name, O_RDWR | O_CREAT | O_EXCL, 0660);
}

bool File::open_append(char* path, char* name)
{
    return open(path, name, O_RDWR | O_CREAT | O_APPEND, 0660);
}

bool File::open(char* path, char* name, int flag, int right, int shared)
{
	char fullname[256];
	int pathLen;
#ifndef _WIN32
	if(fd >= 0)
	{
		printf("file already opened, close first befor open.");
		return false;
	}

	if(name == NULL || strlen(name) == 0 || strlen(name) >= sizeof(this->name))
	{
		printf("file name null. \n");
		return false;
	}

	if(path != NULL)
	{
		strcpy(this->path, path);
	}
	else
	{
		memset(this->path, 0, sizeof(this->path));
	}

	strcpy(this->name, name);

	pathLen = strlen(this->path);

	if(pathLen == 0 || this->path[pathLen - 1] == '/')
		sprintf(fullname, "%s%s", this->path, this->name);
	else
		sprintf(fullname, "%s/%s", this->path, this->name);

	fd = ::open(fullname, flag, right);

	if(fd < 0)
	{
	    error = errno;
		printf("file open failed. errno = %d %s. \n", errno, strerror(errno));
		return false;
	}
#else
    DWORD access = 0, create = 0;
    int flag_ret;
    if(fd != (HANDLE)0xffffffff)
	{
		printf("file already opened, close first befor open.");
		return false;
	}
	if(name == NULL || strlen(name) == 0 || strlen(name) >= sizeof(this->name))
	{
		printf("file name null. \n");
		return false;
	}
	if(path != NULL)
	{
		strcpy(this->path, path);
	}
	else
	{
		memset(this->path, 0, sizeof(this->path));
	}
	strcpy(this->name, name);

	pathLen = strlen(this->path);

	if(pathLen == 0 || this->path[pathLen - 1] == '/' || this->path[pathLen - 1] == '\\')
		sprintf(fullname, "%s%s", this->path, this->name);
	else
		sprintf(fullname, "%s/%s", this->path, this->name);
    flag_ret = flag & 0x00000003;
    if(flag_ret == O_RDONLY)
    {
        access |= GENERIC_READ;
    }
    else if(flag_ret == O_WRONLY)
    {
        access |= GENERIC_WRITE;
    }
    else if(flag_ret == O_RDWR)
    {
        access |= (GENERIC_READ | GENERIC_WRITE);
    }
    else
    {
        printf("file flag error. \n");
        return false;
    }
    if(flag & (O_CREAT))
    {
        if(flag & (O_EXCL))
        {
            create = CREATE_NEW;
        }
        else
        {
            create = OPEN_ALWAYS;
        }
    }
    else
    {
        create = OPEN_EXISTING;
    }
    if(shared != 0)
    {
        shared = 0x07;
    }
    fd = CreateFileA(
        fullname,    // 指向文件名的指针
        access,    // 访问模式（写 / 读）
        (DWORD)shared,    // 共享模式
        NULL, // 指向安全属性的指针
        create,   // 如何创建
        0,   // 文件属性
        0    // 用于复制文件句柄
        );
    if(fd == INVALID_HANDLE_VALUE)
    {
        error = GetLastError();
        printf("create file error:%d.\n", (int)error);
        return false;
    }
    if((flag & O_APPEND) == 0)
    {
        return true;
    }

    if(SetFilePointer(
                fd,
                0,
                NULL,
                FILE_END
            ) == INVALID_SET_FILE_POINTER)
    {
        error = GetLastError();
        printf("create file error:%d.\n", (int)error);
        return false;
    }
#endif
	return true;

}

int File::writenBytes(const char* buffer, int n)
{
	int writeBytes, returnValue;
	char *p;
#ifndef _WIN32
	if(fd < 0  || n < 0)
#else
    bool ret;
    if(fd == (HANDLE)0xffffffff  || n < 0)
#endif
	{
		printf("file did not open. \n");
		return -1;
	}

	p = (char*)buffer;
	returnValue = 0;

	while(true)
	{
#ifndef _WIN32
		writeBytes = write(fd, p, n);
		if(writeBytes < 0 && errno != EINTR)
		{
			return returnValue;
		}
#else
        ret = WriteFile(
            fd,//文件句柄
            p,//数据缓存区指针
            n,//你要写的字节数
            (PDWORD)&writeBytes,//用于保存实际写入字节数的存储区域的指针
            NULL //OVERLAPPED结构体指针
        );
        if(writeBytes < 0 || ret == false)
		{
			return returnValue;
		}
#endif
			returnValue += writeBytes;
			n -= writeBytes;
			p += writeBytes;

        if(writeBytes < n)
        {
            return returnValue;
        }

		if(n == 0)
			break;
	}
	return returnValue;
}

int File::readnBytes(char* buffer, int n)
{
	int readBytes, returnValue;
	char *p;

#ifndef _WIN32
	if(fd < 0 || n < 0)
#else
    bool ret;
    if(fd == (HANDLE)0xffffffff  || n < 0)
#endif
	{
		printf("file did not open. \n");
		return -1;
	}

	p = (char*)buffer;
	returnValue = 0;

	while(true)
	{
#ifndef _WIN32
		readBytes = read(fd, p, n);
		if(readBytes < 0 && errno != EINTR)
		{
			return returnValue;
		}
#else
        ret = ReadFile(
            fd,                                    //文件的句柄
            p,                                //用于保存读入数据的一个缓冲区
            n,    //要读入的字节数
            (PDWORD)&readBytes,    //指向实际读取字节数的指针
            NULL
                //如文件打开时指定了FILE_FLAG_OVERLAPPED，那么必须，用这个参数引用一个特殊的结构。
                //该结构定义了一次异步读取操作。否则，应将这个参数设为NULL
        );
        if(readBytes < 0 || ret == false)
        {
            return returnValue;
        }

#endif
        if(readBytes == 0)
        {
            return READ_EOF;
        }
		//if(readBytes > 0)
		//{
			returnValue += readBytes;
			n -= readBytes;
			p += readBytes;
		//}
		if(readBytes < n)
        {
            return returnValue;
        }
		if(n == 0)
			break;
	}
	return returnValue;
}

int File::writenBytestoPos(const char* buffer, int n, int pos)
{

	if(fd < 0)
	{
		printf("file did not open. \n");
		return -1;
	}

	if(!setReadWritePos(pos))
	{
		printf("file setReadWritePos failed. \n");
		return false;
	}

	return writenBytes(buffer, n);


}

int File::readnBytesfromPos(char* buffer, int n, int pos)
{

	if(fd < 0)
	{
		printf("file did not open. \n");
		return -1;
	}

	if(!setReadWritePos(pos))
	{
		printf("file setReadWritePos failed. \n");
		return false;
	}

	return readnBytes(buffer, n);
}

bool File::setSize(unsigned long size)
{

#ifndef _WIN32
	if(fd < 0)
#else
    bool ret;
    if(fd == (HANDLE)0xffffffff)
#endif
	{
		printf("file did not open. \n");
		return false;
	}
#ifndef _WIN32
	if(ftruncate(fd, size) < 0)
	{
		return false;
	}
#else
    if(!setReadWritePos(size))
    {
        return false;
    }
    ret = SetEndOfFile(fd);
    if(ret == false)
    {
        return false;
    }
#endif

	return true;

}

unsigned long File::getSize()
{

	struct stat buf;

#ifndef _WIN32
	if(fd < 0)
#else
    DWORD size;
    if(fd == (HANDLE)0xffffffff)
#endif
	{
		printf("file did not open. \n");
		return -1;
	}
#ifndef _WIN32
	if(fstat(fd, &buf) < 0)
	{
		printf("file fstat failed. \n");
		return -1;
	}
#else
    size = GetFileSize(
                fd,
                NULL
            );
    if(size ==  INVALID_FILE_SIZE)
    {
        printf("getfilesize error.\n");
        return -1;
    }
    return size;
#endif

	return buf.st_size;
}

bool File::setReadWritePos(unsigned long pos)
{

#ifndef _WIN32
	if(fd < 0)
#else
    DWORD ret;
    if(fd == (HANDLE)0xffffffff)
#endif
	{
		printf("file did not open. \n");
		return false;
	}
#ifndef _WIN32
	if(lseek(fd, pos, SEEK_SET) < 0)
	{
		printf("file lseek failed. \n");
		return false;
	}
#else
     ret = SetFilePointer(
                fd,
                (DWORD)pos,
                NULL,
                FILE_BEGIN
            );
    if(ret != pos)
    {
        return false;
    }
#endif

	return true;
}

bool File::close()
{

#ifndef _WIN32
	if(fd >= 0)
	{
		::close(fd);
		printf("close file :%d. \n", fd);
		fd = -1;
		return true;
	}
#else
    //DWORD ret;
    if(fd != (HANDLE)0xffffffff)
    {
        CloseHandle(fd);
        //printf("close file :%d. \n", (int)fd);
        fd = (HANDLE)0xffffffff;
        return true;
    }
#endif


	return false;
}

bool File::available()
{
    #ifdef _WIN32
    if(fd == (HANDLE)0xffffffff)
    #else
    if(fd == -1)
    #endif // _WIN32
    {
        return false;
    }
    return true;
}

bool File::reopen(int flag, int right, int shared)
{
    return open(path, name, flag, right, shared);
}

bool File::flush()
{
#ifndef _WIN32
    if(fsync(fd) != 0)
    {
        error = errno;
        printf("flush error : %d.", error);
        return false;
    }
#else
    if(!FlushFileBuffers(fd))
    {
        error = GetLastError();
        printf("flush error : %d.", (int)error);
        return false;
    }
#endif
    return true;
}


