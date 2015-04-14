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

    /** \����һ���ļ������ļ������򱨴� ����д��
     *
     * \param path ·��
     * \param name �ļ���
     * \return �Ƿ�ִ����ȷ���������������error������
     *
     */
	bool open_create(char *path, char* name);
	/** \�򿪻򴴽�һ���ļ��������ļ�β׷�ӣ���д��
     *
     * \param path ·��
     * \param name �ļ���
     * \return �Ƿ�ִ����ȷ���������������error������
     *
     */
	bool open_append(char *path, char* name);
	/** \�򿪻򴴽�һ���ļ�
     *
     * \param path ·��
     * \param name �ļ���
     * \param right �ļ�Ȩ�ޣ���linux 10λȨ��
     * \param shared �Ƿ�ɹ���򿪣�ͬʱ�м�����ͬһ�ļ��ĵ��ã�������windows
     * \return �Ƿ�ִ����ȷ���������������error������
     *
     */
	bool open(char *path, char* name, int flag = O_RDWR | O_CREAT, int right = 0660, int shared = 0);

    /** \���ļ�д��n�ֽ�
     *
     * \param buffer д�����ݻ���
     * \param n д�볤��
     * \return ʵ��д����ֽ��������С��n���ʾд�����
     *
     */
	int writenBytes(const char *buffer, int n);
	/** \���ļ���ȡn�ֽ�
     *
     * \param buffer ��ȡ���ݻ���
     * \param n ��ȡ����
     * \return ʵ�ʶ�ȡ���ֽ��������С��n���ʾ��ȡ����, READ_EOF��ʾ��ȡ���ļ�β
     *
     */
	int readnBytes(char *buffer, int n);
    /** \��posλ�����ļ�д��n�ֽ�
     *
     * \param buffer д�����ݻ���
     * \param n д�볤��
     * \param pos д��λ��
     * \return ʵ��д����ֽ��������С��n���ʾд�����
     *
     */
	int writenBytestoPos(const char *buffer, int n, int pos);
	/** \��posλ�ô��ļ���ȡn�ֽ�
     *
     * \param buffer ��ȡ���ݻ���
     * \param n ��ȡ����
     * \param pos ��ȡλ��
     * \return ʵ�ʶ�ȡ���ֽ��������С��n���ʾ��ȡ����
     *
     */
	int readnBytesfromPos(char *buffer, int n, int pos);

    /** \�����ļ����ȣ�С��ʵ�ʳ�����ضϣ�����ʵ�ʳ��������0
     *
     * \param size �ļ����ȣ�windows�����4�ֽڳ���
     * \return
     *
     */
	bool setSize(unsigned long size);

    /** \��ȡ�ļ�����
     *
     * \return �ļ����ȣ�windows�����4�ֽڳ���
     *
     */
	unsigned long getSize();

    /** \���ö�ȡд��λ��
     *
     * \param pos λ��
     * \return
     *
     */
	bool setReadWritePos(unsigned long pos);

    /** \��ǰ�ļ��Ƿ����
     *
     * \return
     *
     */
	bool available();

    /** \���´��ļ���ʹ��openʱ���ļ���
     *
     * \param flag �򿪱�־
     * \param right �ļ�Ȩ��
     * \param shared �Ƿ���
     * \return
     *
     */

	bool reopen(int flag = O_RDWR | O_CREAT, int right = 0660, int shared = 0);

    /** \�ѻ���ˢ���ļ�
     *
     * \return
     *
     */
	bool flush();

    /** \�ر��ļ�
     *
     * \return
     *
     */
	bool close();

};

#endif /* FILE_H_ */
