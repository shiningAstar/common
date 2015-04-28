#ifndef COMMON_SOCKET_H
#define COMMON_SOCKET_H

#include <stdio.h>
#include <string>
#include <stdlib.h>

#ifdef _WIN32
#include <WinSock2.h>
#include <Ws2tcpip.h>
#include <MSWSock.h>
#include <windows.h>
#define EWOULDBLOCK EAGAIN
#else
#include	<sys/types.h>	/* basic system data types */
#include	<sys/socket.h>	/* basic socket definitions */
#include	<sys/time.h>	/* timeval{} for select() */
#include    <sys/poll.h>
#include	<time.h>	/* timespec{} for pselect() */
#include	<netinet/in.h>	/* sockaddr_in{} and other Internet defns */
#include	<arpa/inet.h>	/* inet(3) functions */
#include	<errno.h>
#include	<fcntl.h>	/* for nonblocking */
#include    <sys/ioctl.h>
#include	<netdb.h>
#include	<signal.h>
#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>
#include	<sys/stat.h>	/* for S_xxx file mode constants */
#include	<sys/uio.h>	/* for iovec{} and readv/writev */
#include	<unistd.h>
#include	<sys/wait.h>
#include	<sys/un.h>	/* for Unix domain sockets */
#include    <linux/limits.h>
#include	<math.h>
#include 	<stdarg.h>
#include    <openssl/md5.h>
#include    <sys/vfs.h>
#include    <net/if.h>

#include    <sys/timeb.h>
#include    <vector>
#include    <string>
#include 	<list>
#include   <deque>
#include    <map>
#endif

#define LIB_EXPORT

using namespace std;

class LIB_EXPORT VNetTS{
public:
	static void  PutString(char* Buf,char* Str,unsigned int Len);
	static void  GetString(char* Buf,char* Str,unsigned int Len);
	static unsigned long GetULong(unsigned char* Buf);
	static unsigned short GetUShort(unsigned char* Buf);
	static void SetULong(unsigned char* Buf,unsigned long Val);
	static void SetUShort(unsigned char* Buf,unsigned short Val);
};

#define  INVALID_SOCKET  -1
#define  SOCKET_ERROR    -1
#define  FD_NONE      0x00
#define  FD_READ  	   0x01
#define  FD_WRITE 	   0x02
#define  FD_READWRITE  0x03
#define  FD_DISCONNECT       0x04
#define  FD_INTERRUPT  0x08

#ifdef _WIN32
	typedef int socklen_t;
#else
	typedef  int  SOCKET;
	typedef  unsigned long  DWORD;
	typedef  unsigned int   UINT;
#endif

#ifdef _WIN32
	#define bzero(a,b) memset(a,0,b)
	#define ERROR S_ERROR
	#define IGNORE S_IGNORE
#endif

//windows平台下重叠IO的结构定义
#ifdef _WIN32
	#define ACCEPTEX_CONNECTEX_BUFFER_SIZE 8192
	class LIB_EXPORT MySocket;
	typedef enum
	{
		OP_READ = 0x01,
		OP_WRITE = 0x02,
		OP_ACCEPT = 0x04
	}op;
	typedef struct _Accept_Connect_Per_IO
	{
		OVERLAPPED overlapped;
		int operation;
		MySocket *socket_accepted;
		char buffer[ACCEPTEX_CONNECTEX_BUFFER_SIZE];
		int bytes_inited;
		~_Accept_Connect_Per_IO()
		{
			if(socket_accepted != NULL)
				delete socket_accepted;
		}
	}Accept_Connect_Per_IO, *pAccept_Connect_Per_IO;
#endif


typedef enum
{
	OK=0,
	FAIL=-1,
	TIMEOUT=-2,
	DISCONNECT=0,
	READERROR=-1,
	WRITEERROR=-1,
	ERROR=-1,
	WOULDBLOCK=-4,
	IGNORE=-3
} ReturnValue;

class BlockingInterruptor;

class LIB_EXPORT MySocket
{
public:
// SockRetΪsocket
    typedef enum{
	    SockSuccess=0,     //
	    SockClose = 0,     // socket(recv,recvfrom)
		SockTimeOut = 0,   // select
		SockInvalid=INVALID_SOCKET,  //Чsocket
		SockError=SOCKET_ERROR  //socket
	} SockRetCode;
protected:
	SOCKET		m_socket;
	int			m_nSocketType;
	struct sockaddr	   m_sockLocal;
	struct sockaddr	   m_sockDest;
	struct sockaddr    m_sockRecvFrom;
	char        m_MulticastIp[20];
public:
	MySocket();
	virtual ~MySocket();
public:
	void  Initialize();
	//库中生成socket实体
	static MySocket* NewSockInst();
	//根据字符串形式ip和整形port返回网络地址结构sockaddr
	static int GetSockAddrFromChar(sockaddr *addr, char *ip, int port);
#ifdef _WIN32
	//windows 下重叠IO函数
	int CreateOverlaped();
	//重叠IOaccept函数，第二个参数直接传buffer长度，第三个参数返回收到的字节数
	int AcceptEx(pAccept_Connect_Per_IO per_io);
	//调用AcceptEx之后，在工作线程返回了accept的socket后，由accept的socket调用，得到连接的本地和远程地址,
	int GetAcceptExSockaddrs(pAccept_Connect_Per_IO per_io);
	//重叠IOconnect函数，后三个数据是连接上同时发送的数据(可选)
	int ConnectEx(char *destIP, int destPort, pAccept_Connect_Per_IO per_io);

	int WSAIoctl(DWORD dwIoControlCode, void* lpvInBuffer, DWORD cbInBuffer, void* lpvOutBuffer, DWORD cbOutBuffer,
     LPDWORD lpcbBytesReturned,
     LPWSAOVERLAPPED lpOverlapped,
     LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine);

#endif
	int Create(int nSocketType = SOCK_STREAM/*SOCK_DGRAM*/);
	int CreateClient(char* Address,UINT Port,int nSocketType=SOCK_STREAM/*SOCK_DGRAM*/,int BufSize=-1);
	int CreateClient(char* Address,char* Port,int nSocketType=SOCK_STREAM/*SOCK_DGRAM*/,int BufSize=-1 );
	int CreateMulticast(char* Address,int Port,int BufSize=-1);
	int Select(fd_set *readfds, fd_set *writefds=NULL,fd_set *exceptfds=NULL,long msec=0);
	int SelectWait(long SEvent=FD_READ,long MSec=0);
	int SelectWaitInterruptable(BlockingInterruptor *interruptor=NULL, long SEvent=FD_READ, long MSec=0);

	int Poll(int SEvent = FD_READ | FD_WRITE, int MSec = 0);
	int SetNonBlockMode(DWORD bBlock=1);
	int GetUdpSize();
	int Bind(char* localAddr, char* localPort);
	int Bind(char* localAddr,int localPort);
	int Connect(char* destAddr,char* destPort);
	int Connect(char* destAddr,int destPort);
	int Connect(char* destAddr,int destPort,long MSec);
	int Listen(int BackLog);
	SOCKET Accept(struct sockaddr *cliaddr,socklen_t *addrlen);
	int    Accept(MySocket *&rs);
	//要求rs为一个存在的MySocket类型的地址，且未初始化Create
	int    Accept_s(MySocket *rs);
	void   SetSocket(SOCKET s);
	int SendTo(char* pData, int nLen);
	int RecvFrom(char* pData, int nLen);
	int RecvFrom(char* pData, int nLen,int MSec);
	int Recv(char* pData, int nLen);
	int Recv_DontWait(char *pData, int nLen);
	int RecvPeek(char* pData, int nLen);
	int Send(char* pData, int nLen);
	int Send_DontWait(char* pData, int nLen);
	int Writen(char* pData,int nLen);
	int Writen_DontWait(char* pData,int nLen);
	int Writen(char* pData,int nLen,int MSec);
	int Readn(char* pData,int nLen);
	int Readn_DontWait(char* pData,int nLen);
	int Readn(char* pData,int nLen,int MSec);
	int ReadLine(char* pData,int MaxLen);
	int ReadLine(char* pData,int MaxLen,int MSec);
	void Close();
	void CloseMulticast();

    int GetLastSocketError();
    int ObtainLocalAddr();
	int GetNetAddr(sockaddr *sa,char *addr,int *port);
	int GetLocalAddr(char *addr,int *port);
	int GetDestAddr(char *addr,int *port);
	int GetRecvFromAddr(char *addr,int *port);
	SOCKET GetSocket();
	SOCKET* GetSocketPtr();
    int GetSockOpt(int level,int optname,char *optval,int *optlen);
    int SetSockOpt(int level,int optname,const char *optval,int optlen);
    int SetRecvBufSize(int bufsize);
	int SetSendBufSize(int bufsize);
	int SetLinger(int OnOff,int LSec);
	int SetReUseAddr(bool bReUse);
	int SetMulticastTtl(unsigned char Ttl=1);
	int SetMulticastLoop(unsigned char Loop=1);

	int SetTcpNoDelay(bool bNoDelay);
	int SetDestAddr(char* destIP, char* destPort);
	int SetDestAddr(char* destIP, short destPort);
	int SetDestAddr(long destIP, short destPort);
	int SetDestNetAddr(long destIP, short destPort);
	int SetDestAddr(struct sockaddr* addr);
	int SetLocalAddr(struct sockaddr* addr);

public:

	int Recv_base(char* pData, int nLen, unsigned long flag, void *overlap_struct, void *overlap_routine_proc);
	int Send_base(char* pData, int nLen, unsigned long flag, void *overlap_struct, void *overlap_routine_proc);

};



#endif
