/*
 * File.h
 *
 *  Created on: 2012-5-25
 *      Author: root
 */

#ifndef FILE_H_
#define FILE_H_

#include "fcntl.h"
#ifdef _WIN32
#include "windows.h"
#endif


class File {
public:
	File();
	virtual ~File();

#ifdef _WIN32
    HANDLE fd;
    DWORD error;
#else
	int fd;
	int error;
#endif // _WIN32
#define READ_EOF -2
	char name[128];
	char path[128];

    /** \创建一个文件，如文件存在则报错 （读写）
     *
     * \param path 路径
     * \param name 文件名
     * \return 是否执行正确，如出错错误代码在error变量中
     *
     */
	bool open_create(char *path, char* name);
	/** \打开或创建一个文件，并在文件尾追加（读写）
     *
     * \param path 路径
     * \param name 文件名
     * \return 是否执行正确，如出错错误代码在error变量中
     *
     */
	bool open_append(char *path, char* name);
	/** \打开或创建一个文件
     *
     * \param path 路径
     * \param name 文件名
     * \param right 文件权限，如linux 10位权限
     * \param shared 是否可共享打开（同时有几个打开同一文件的调用），仅限windows
     * \return 是否执行正确，如出错错误代码在error变量中
     *
     */
	bool open(char *path, char* name, int flag = O_RDWR | O_CREAT, int right = 0660, int shared = 0);

    /** \向文件写入n字节
     *
     * \param buffer 写入数据缓冲
     * \param n 写入长度
     * \return 实际写入的字节数，如果小于n则表示写入错误
     *
     */
	int writenBytes(const char *buffer, int n);
	/** \从文件读取n字节
     *
     * \param buffer 读取数据缓冲
     * \param n 读取长度
     * \return 实际读取的字节数，如果小于n则表示读取错误, READ_EOF表示读取到文件尾
     *
     */
	int readnBytes(char *buffer, int n);
    /** \在pos位置向文件写入n字节
     *
     * \param buffer 写入数据缓冲
     * \param n 写入长度
     * \param pos 写入位置
     * \return 实际写入的字节数，如果小于n则表示写入错误
     *
     */
	int writenBytestoPos(const char *buffer, int n, int pos);
	/** \在pos位置从文件读取n字节
     *
     * \param buffer 读取数据缓冲
     * \param n 读取长度
     * \param pos 读取位置
     * \return 实际读取的字节数，如果小于n则表示读取错误
     *
     */
	int readnBytesfromPos(char *buffer, int n, int pos);

    /** \设置文件长度，小于实际长度则截断，大于实际长度则填充0
     *
     * \param size 文件长度，windows下最大4字节长度
     * \return
     *
     */
	bool setSize(unsigned long size);

    /** \读取文件长度
     *
     * \return 文件长度，windows下最大4字节长度
     *
     */
	unsigned long getSize();

    /** \设置读取写入位置
     *
     * \param pos 位置
     * \return
     *
     */
	bool setReadWritePos(unsigned long pos);

    /** \当前文件是否可用
     *
     * \return
     *
     */
	bool available();

    /** \重新打开文件，使用open时的文件名
     *
     * \param flag 打开标志
     * \param right 文件权限
     * \param shared 是否共享
     * \return
     *
     */

	bool reopen(int flag = O_RDWR | O_CREAT, int right = 0660, int shared = 0);

    /** \把缓存刷进文件
     *
     * \return
     *
     */
	bool flush();

    /** \关闭文件
     *
     * \return
     *
     */
	bool close();

};

#endif /* FILE_H_ */
