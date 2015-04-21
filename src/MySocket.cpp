/*#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
	#include <winsock2.h>
#else
	#include <sys/socket.h>
	#include <netinet/in.h>
	#include "linuxh.h"
	#include "poll.h"
#endif*/

#include "../include/MySocket.h"

#ifdef _WIN32
	#pragma comment(lib, "Ws2_32.lib")
	#pragma comment(lib, "mswsock.lib")
#endif
//---------------------------------------------------------------------------------------------
void VNetTS::PutString(char* Buf,char* Str, unsigned int Len)
{
	memset(Buf,0,Len);
	if(strlen(Str)>=Len) memcpy(Buf,Str,Len);
	else memcpy(Buf,Str,strlen(Str));
}

void VNetTS::GetString(char* Buf,char* Str,unsigned int Len)
{
	if(strlen(Buf)>Len){
		strncpy(Str,Buf,Len);
		strcat(Str,"\0");
	}
	else{
		strcpy(Str,Buf);
	}
}

unsigned long VNetTS::GetULong(unsigned char* Buf)
{
	return ntohl(*(unsigned long*)Buf);
}

unsigned short VNetTS::GetUShort(unsigned char* Buf)
{
	return ntohs(*(unsigned short*)Buf);
}

void VNetTS::SetULong(unsigned char* Buf,unsigned long Val)
{
	*(unsigned long*)Buf = htonl(Val);
}

void VNetTS::SetUShort(unsigned char* Buf,u_short Val)
{
	*(unsigned short*)Buf = htons(Val);
}

//---------------------------------------------------------------------------------------------

#ifdef _WIN32
int inet_aton(const char *address, struct in_addr *sock)
{
	unsigned int s;
	s = inet_addr(address);
	if ( s == INADDR_NONE || strcmp (address, "255.255.255.225") == 0)
		return 0;
	sock->s_addr = s;
	return 1;
}
#endif

#ifdef _WIN32
#define close(a) closesocket(a)
#define gethostbyname_r(b,c,d,e,a,f) *(a)=gethostbyname(b)
#define ioctl ioctlsocket
#define poll WSAPoll
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
MySocket* MySocket::NewSockInst()
{
    return new MySocket;
}
#ifdef _WIN32
int MySocket::GetSockAddrFromChar(sockaddr *addr, char *ip, int port)
{
	struct hostent *phe;
	//struct servent* pse;
	struct sockaddr_in sin;
	char  phebuf[1024];
	int   herr;

	memset(&sin,0,sizeof(sin));
	sin.sin_family = AF_INET;
	if ((sin.sin_port = htons((u_short)port))==0)
	{
		return FAIL;
	}
	gethostbyname_r(ip,&pphhee,phebuf,1024,&phe,&herr);
	if(phe){
		memcpy(&sin.sin_addr, phe->h_addr, phe->h_length);
	}
	else if ((sin.sin_addr.s_addr = inet_addr(ip))== INADDR_NONE )
	{
		return FAIL;
	}
	memcpy(addr,&sin,sizeof(sin));
	return OK;
}
#endif

MySocket::MySocket()
{
	m_socket = SockInvalid;
	m_nSocketType = SOCK_STREAM;
	Initialize();
}

MySocket::~MySocket()
{
	Close();
}

void MySocket::Initialize()
{
	memset(&m_sockDest,0,sizeof(m_sockDest));
	memset(&m_sockLocal,0,sizeof(m_sockLocal));
	memset(&m_sockRecvFrom,0,sizeof(m_sockRecvFrom));
	memset(m_MulticastIp,0,sizeof(m_MulticastIp));

#ifdef _WIN32

	static bool inited = false;

	if(!inited)
	{
		WSADATA wsa;

		if(WSAStartup(MAKEWORD(2,2),&wsa)!=0)
		{
			printf("init socket dll failed. \n");
		}

		inited = true;

	}

#endif

}

void MySocket::Close()
{
	CloseMulticast();
	if(m_socket != SockInvalid )
	{
		close(m_socket);
	}
	m_socket = SockInvalid;
	m_nSocketType = SOCK_STREAM;
	Initialize();
}

void MySocket::CloseMulticast()
{
	if(strlen(m_MulticastIp)>0){
		struct ip_mreq mreq;
		bzero(&mreq, sizeof(struct ip_mreq));
		inet_aton(m_MulticastIp, &mreq.imr_multiaddr);
		mreq.imr_interface.s_addr = htonl(INADDR_ANY);
    	SetSockOpt((int)IPPROTO_IP,IP_DROP_MEMBERSHIP,(char*)&mreq,sizeof(mreq));
    	memset(m_MulticastIp,0,20);
    }
}

int MySocket::Create(int nSocketType)
{
	if(m_socket != SockInvalid)
		return FAIL;
	m_nSocketType = nSocketType;
	m_socket = socket(PF_INET, m_nSocketType, 0 );
	if(m_socket==SockInvalid) return FAIL;
	else return OK;
}

int MySocket::CreateClient(char* Address , UINT nSocketPort, int nSocketType,int BufSize)
{
	char port[20];
	sprintf(port,"%d",nSocketPort);
	return CreateClient(Address,port,nSocketType,BufSize);
}

int MySocket::CreateClient(char* Address, char* Port, int nSocketType,int BufSize)
{
	m_nSocketType = nSocketType;
	m_socket = socket(PF_INET,m_nSocketType,0);
	if(m_socket!=SockInvalid){
		if(BufSize>0) SetRecvBufSize(BufSize);
		else if(BufSize<0) SetRecvBufSize(64*1024L);
		if(Connect(Address,Port)==OK){
			return OK;
		}
	}
	return FAIL;
}

int MySocket::CreateMulticast(char* Address,int Port,int BufSize)
{
	int ret = FAIL;
	strcpy(m_MulticastIp,Address);
	if(Create(SOCK_DGRAM)==OK){
		SetReUseAddr(true);
		if(Bind(Address,Port)==OK){
			if(BufSize>0) SetRecvBufSize(BufSize);
			struct ip_mreq mreq;
			bzero(&mreq, sizeof(struct ip_mreq));
			inet_aton(Address, &mreq.imr_multiaddr);
			mreq.imr_interface.s_addr = htonl(INADDR_ANY);
			if(SetSockOpt ((int)IPPROTO_IP, IP_ADD_MEMBERSHIP, (char*)&mreq, sizeof(struct ip_mreq))==0){
				ret = OK;
			}
		}
	}
	return ret;
}

int MySocket::SetMulticastTtl(unsigned char Ttl)
{
	return SetSockOpt(IPPROTO_IP,IP_MULTICAST_TTL,(char*)&Ttl,sizeof(Ttl));
}

int MySocket::SetMulticastLoop(unsigned char Loop)
{
	return SetSockOpt(IPPROTO_IP,IP_MULTICAST_LOOP,(char*)&Loop,sizeof(Loop));
}

int MySocket::Connect(char* destAddr,int destPort)
{
	char Port[10];
	sprintf(Port,"%d",destPort);
	return Connect(destAddr,Port);
}

int MySocket::Connect(char* destAddr,char* destPort)
{
	struct hostent pphhee,*phe;
	char   phebuf[1024];
	int    herr;
    struct sockaddr_in sin;

	memset(&sin,0,sizeof(sin));
	sin.sin_family = AF_INET;
	if ((sin.sin_port = htons((u_short)atoi(destPort)))==0)
	{
		return FAIL;
	}

	//search hostname
//#ifdef _WIN32
//	phe=gethostbyname(destAddr);
//#else
	gethostbyname_r(destAddr,&pphhee,phebuf,1024,&phe,&herr);
//#endif
	// phe=gethostbyname(destAddr);
	if(phe){
		memcpy(&sin.sin_addr, phe->h_addr, phe->h_length);
	}
	else if((sin.sin_addr.s_addr = inet_addr(destAddr))== INADDR_NONE )
	{
		return FAIL;
	}

	memcpy(&m_sockDest,&sin,sizeof(sin));
	if(m_nSocketType==SOCK_DGRAM)
	{
		int share=1;
		sin.sin_family=AF_INET ;
		sin.sin_port=0 ;
		sin.sin_addr.s_addr=INADDR_ANY ;
		SetSockOpt((int)SOL_SOCKET,SO_REUSEADDR,(char*)&share,sizeof(share));
		if(bind(m_socket,(struct sockaddr*)&sin,sizeof(sin))==SockError)
		{
			close(m_socket);
			m_socket = SockInvalid;
		}
		else
		{
			socklen_t namelen=sizeof(m_sockLocal);
			getsockname(m_socket,&m_sockLocal,&namelen);
		}
	}
	if(connect(m_socket, (struct sockaddr*)&m_sockDest, sizeof(m_sockDest))==SockError)
	{
		close(m_socket);
		m_socket = SockInvalid;
	}

	if(m_socket==SockInvalid) return FAIL;
	else
	{
		ObtainLocalAddr();
		return OK;
	}
}

int MySocket::Connect(char* destAddr,int destPort,long MSec)
{
	struct hostent pphhee,*phe;
	char phebuf[1024];
	int  herr;
    struct sockaddr_in sin;

	memset(&sin,0,sizeof(sin));
	sin.sin_family = AF_INET;
	if ((sin.sin_port = htons((u_short)destPort))==0)
	{
		return FAIL;
	}

	//search hostname
//#ifdef _WIN32
	phe=gethostbyname(destAddr);
//#else
	gethostbyname_r(destAddr,&pphhee,phebuf,1024,&phe,&herr);
//#endif
	// phe = gethostbyname(destAddr);
	if(phe){
		memcpy(&sin.sin_addr, phe->h_addr, phe->h_length);
	}
	else if((sin.sin_addr.s_addr = inet_addr(destAddr))== INADDR_NONE )
	{
		return FAIL;
	}

	memcpy(&m_sockDest,&sin,sizeof(sin));
	if(m_nSocketType==SOCK_DGRAM)
	{
		int share=1;
		sin.sin_family=AF_INET ;
		sin.sin_port=0 ;
		sin.sin_addr.s_addr=INADDR_ANY ;
		SetSockOpt((int)SOL_SOCKET,SO_REUSEADDR,(char*)&share,sizeof(share));
		if(bind(m_socket,(struct sockaddr*)&sin,sizeof(sin))==SockError)
		{
			close(m_socket);
			m_socket = SockInvalid;
		}
		else
		{
			socklen_t namelen=sizeof(m_sockLocal);
			getsockname(m_socket,&m_sockLocal,&namelen);
		}
	}

	SetNonBlockMode(1);
	int ret = connect(m_socket, (struct sockaddr*)&m_sockDest, sizeof(m_sockDest));
	if(ret!=0){
		fd_set fdw,fdr;
		FD_ZERO( &fdw );
		FD_SET(m_socket, &fdw );
		fdr = fdw;
		ret = Select(&fdr,&fdw,NULL,MSec);
		if(ret<=0){
	    	SetNonBlockMode(0);
			close(m_socket);
			m_socket = SockInvalid;
			return TIMEOUT;
		}
		else{
			if(FD_ISSET(m_socket,&fdr) || !FD_ISSET(m_socket,&fdw)){
				SetNonBlockMode(0);
				close(m_socket);
				m_socket = SockInvalid;
				return FAIL;
			}
		}
	}
    SetNonBlockMode(0);

	if(m_socket==SockInvalid) return FAIL;
	else
	{
		ObtainLocalAddr();
		return OK;
	}

}

int MySocket::SelectWait(long SEvent,long MSec)
{
    fd_set fdr,fdw,*pfdr=NULL,*pfdw=NULL;
    if(SEvent & FD_READ){
		FD_ZERO(&fdr);
		pfdr = &fdr;
		FD_SET(m_socket, &fdr);
	}
	if(SEvent & FD_WRITE){
		FD_ZERO(&fdw);
		pfdw = &fdw;
		FD_SET(m_socket, &fdw);
	}
	int sn=Select(pfdr,pfdw, NULL,MSec);
	if (sn > 0){
		if(SEvent==FD_READWRITE){
			if(FD_ISSET(m_socket, pfdr) && FD_ISSET(m_socket,pfdw)) {
				return (sn = FD_READWRITE);
			}
			else if(FD_ISSET(m_socket, pfdr)){
				return (sn = FD_READ);
			}
			else if(FD_ISSET(m_socket, pfdw)){
				return (sn = FD_WRITE);
			}
		}
		else if(SEvent==FD_READ){
			if(FD_ISSET(m_socket, pfdr)){
				return (sn = FD_READ);
			}
		}
		else if(SEvent==FD_WRITE){
			if(FD_ISSET(m_socket, pfdw)){
				return (sn = FD_WRITE);
			}
		}
		sn = FD_NONE;
	}
	return sn;
}

int MySocket::Select(fd_set *readfds, fd_set *writefds,fd_set *exceptfds,long msec)
{
	struct timeval tv,*ptv=NULL;
	if(msec>0){
		tv.tv_sec = msec / 1000;
		tv.tv_usec = (msec % 1000)*1000;
		ptv = &tv;
	}
	return select(m_socket+1,readfds,writefds,exceptfds,ptv);
}

int MySocket::SetRecvBufSize(int bufsize)
{
	return SetSockOpt(SOL_SOCKET,SO_RCVBUF,(char*)(&bufsize),sizeof(bufsize));
}

int MySocket::SetSendBufSize(int bufsize)
{
	return SetSockOpt(SOL_SOCKET,SO_SNDBUF,(char*)(&bufsize),sizeof(bufsize));
}

int MySocket::SetLinger(int OnOff,int LSec)
{
    struct linger lg;
    lg.l_onoff= OnOff?1:0;
    lg.l_linger= LSec;
    return SetSockOpt(SOL_SOCKET,SO_LINGER,(const char*)&lg,sizeof(lg));
}

int MySocket::SetReUseAddr(bool bReUse)
{
	int Share= (bReUse)?1:0;
   	return SetSockOpt (SOL_SOCKET, SO_REUSEADDR, (const char*)&Share, sizeof (Share));
}

int MySocket::SetTcpNoDelay(bool bNoDelay)
{
	//int One = (bNoDelay?1:0);
	return 0; //SetSockOpt(IPPROTO_TCP, TCP_NODELAY, (const char * ) &One, sizeof (One));
}

int MySocket::SetSockOpt(int level,int optname,const char *optval,int optlen)
{
   return setsockopt(m_socket,level,optname,optval,optlen);
}

int MySocket::GetSockOpt(int level,int optname,char *optval,int *optlen)
{
   return getsockopt(m_socket,level,optname,optval,(socklen_t*)optlen);
}

int MySocket::Send(char* pData, int nLen)
{
#ifdef _WIN32
	return Send_base(pData, nLen, 0, NULL, NULL);
#else
	return Send_base(pData, nLen, MSG_NOSIGNAL, NULL, NULL);
#endif
}

int MySocket::Send_DontWait(char* pData, int nLen)
{
#ifdef _WIN32
	SetNonBlockMode(1);
	int rv = Send_base(pData, nLen, 0, NULL, NULL);
	SetNonBlockMode(0);
	return rv;
#else
	return Send_base(pData, nLen, MSG_NOSIGNAL | MSG_DONTWAIT, NULL, NULL);
#endif
}

int MySocket::Recv(char* pData, int nLen)
{
#ifdef _WIN32
	return Recv_base(pData, nLen, 0, NULL, NULL);
#else
	return Recv_base(pData, nLen, 0, NULL, NULL);
#endif
}

int MySocket::Recv_DontWait(char* pData, int nLen)
{
#ifdef _WIN32
	SetNonBlockMode(1);
	int rv = Recv_base(pData, nLen, 0, NULL, NULL);
	SetNonBlockMode(0);
	return rv;
#else
	return Recv_base(pData, nLen, MSG_DONTWAIT, NULL, NULL);
#endif
}

int MySocket::RecvPeek(char* pData, int nLen)
{
	if( m_socket == SockInvalid )
	{
		return FAIL;
	}
	int len = recv(m_socket,pData,nLen,MSG_PEEK);
	return len;
}

int MySocket::Writen(char* pData,int nLen)
{
	int sLen = 0,n;
	while(sLen<nLen){
		 n = Send(pData+sLen,nLen-sLen);
		 if(n==ERROR)
		 {
			 if(errno==EINTR || errno == EWOULDBLOCK || errno == EAGAIN)
				 n=0;
			 else
			 	 return WRITEERROR;
		 }
		 else if(n==DISCONNECT) return DISCONNECT;
		 sLen += n;
	}
	return sLen;
}

int MySocket::Writen_DontWait(char* pData,int nLen)
{
	int sLen = 0,n;
	while(sLen<nLen){
		 n = Send_DontWait(pData+sLen,nLen-sLen);
		 if(n==ERROR)
		 {
			 if(errno==EINTR || errno == EAGAIN)
				 n=0;
			 else
			 	 return WRITEERROR;
		 }
		 else if(n==DISCONNECT) return DISCONNECT;
		 else if(n==WOULDBLOCK) break;
		 sLen += n;
	}
	return sLen;
}

int MySocket::Writen(char* pData,int nLen,int MSec)
{
	if(MSec==0) return Writen(pData,nLen);
	int sLen = 0,n,sn;
	fd_set fdw;
	while(sLen<nLen){
		 FD_ZERO(&fdw);
		 FD_SET(m_socket,&fdw);
		 sn = Select(NULL,&fdw,NULL,MSec);
		 if(sn>0){
		 	n = Send(pData+sLen,nLen-sLen);
		 	if(n==-1)
		 	{
		 		if(errno==EINTR || errno == EWOULDBLOCK || errno == EAGAIN)
		 			n=0;
		 		else
		 			return WRITEERROR;
		 	}
		 	else if(n==0) return DISCONNECT;
		 	sLen += n;
		 }
			else if(sn == 0)
				return TIMEOUT;
			else
			{
				return ERROR;
			}
	}
	return sLen;
}

int MySocket::Readn(char* pData,int nLen)
{
	int rLen = 0,n;
	while(rLen<nLen){
		 n = Recv(pData+rLen,nLen-rLen);
		 if(n == ERROR){
		      if(errno==EINTR || errno == EWOULDBLOCK || errno == EAGAIN) n=0;
		      else  return READERROR;
		 }
		 else if(n==DISCONNECT){
		    return DISCONNECT;
		 }
		 rLen += n;
	}
	return rLen;
}

int MySocket::Readn_DontWait(char* pData,int nLen)
{
	int rLen = 0,n;
	while(rLen<nLen){
		 n = Recv_DontWait(pData+rLen,nLen-rLen);
		 if(n == ERROR){
		      if(errno==EINTR || errno == EAGAIN) n=0;
			  else return READERROR;
		 }
		 else if(n==DISCONNECT){
		    return DISCONNECT;
		 }
		 else if(n == WOULDBLOCK)
			 break;
		 rLen += n;
	}
	return rLen;
}

int MySocket::Readn(char* pData,int nLen,int MSec)
{
	if(MSec==0) return Readn(pData,nLen);
	int rLen = 0,n,sn;
	fd_set  fdr;
	while(rLen<nLen){
		FD_ZERO(&fdr);
		FD_SET(m_socket,&fdr);
		sn = Select(&fdr,NULL,NULL,MSec);
		if(sn>0){
			n = Recv(pData+rLen,nLen-rLen);
			if(n<0){
			     if(errno==EINTR || errno == EWOULDBLOCK || errno == EAGAIN) n=0;
			     else return READERROR;
			}
			else if(n==0){
			    return DISCONNECT;
			}
			rLen += n;
		}
		else if(sn == 0)
			return TIMEOUT;
		else
		{
			return ERROR;
		}
	}
	return rLen;
}

int MySocket::ReadLine(char* pData,int MaxLen)
{
	int  n,rLen=0;
	char c, *ptr;

	ptr = pData;
	while(rLen<MaxLen - 1){
		n = Recv(&c,1);
		if(n<0){
			if(!(errno == EINTR || errno == EWOULDBLOCK || errno == EAGAIN))
				return READERROR;
		}
		else if(n==0){
			rLen = 0;
			break;
		}
		else{
			*ptr++ = c;
			rLen += n;
			if(c == '\n') break;	//readover
		}
	}
	*ptr = 0;
	return rLen;
}

int MySocket::ReadLine(char* pData,int MaxLen,int MSec)
{
	int  n, sn,rLen=0;
	char c, *ptr;
	fd_set fdr;

    if(MSec<=0){
    	return ReadLine(pData,MaxLen);
    }
	ptr = pData;
	while(rLen<MaxLen - 1)
	{
		FD_ZERO(&fdr);
		FD_SET(m_socket,&fdr);
		sn = Select(&fdr,NULL,NULL,MSec);
		if(sn>0)
		{
			n = Recv(&c,1);
			if(n<0)
			{
			     if(!(errno == EINTR || errno == EWOULDBLOCK || errno == EAGAIN))
			         return READERROR;
			}
			else if(n==0)
			{
			    rLen = 0;
			    break;
			}
			else
			{
			    *ptr++ = c;
			    rLen += n;
			    if(c == '\n') break;	//readover
			}
		}
		else if(sn == 0)
			return TIMEOUT;
		else
		{
			return ERROR;
		}
	}
	*ptr = 0;
	return rLen;
}

int MySocket::RecvFrom(char *pData, int nLen)
{
	if( m_socket ==SockInvalid)
	{
		return FAIL;
	}
	socklen_t slen = sizeof(sockaddr);
	int len = recvfrom(m_socket,pData,nLen,0,&m_sockRecvFrom,&slen);
	if(len==SockError) return FAIL;
	else return len;
}

int MySocket::RecvFrom(char* pData, int nLen,int MSec)
{
	if( m_socket ==SockInvalid)
	{
		return FAIL;
	}
	if(MSec<=0) return RecvFrom(pData,nLen);
	int sn;
	fd_set  fdr;
	FD_ZERO(&fdr);
	FD_SET(m_socket,&fdr);
	sn = Select(&fdr,NULL,NULL,MSec);
	if(sn>0){
		return RecvFrom(pData,nLen);
	}
	else if(sn==0){
		return TIMEOUT;
	}
	else return FAIL;
}

int MySocket::SendTo(char *pData, int nLen)
{
	if( m_socket == SockInvalid )
	{
		return FAIL;
	}
	int len = sendto(m_socket,pData,nLen,0,&m_sockDest,sizeof(sockaddr));
	if(len==SockError) return FAIL;
	else return len;
}

int MySocket::Bind(char *localAddr, int localPort)
{
	char Port[10];
	sprintf(Port,"%d",localPort);
	return Bind(localAddr,Port);
}

int MySocket::Bind(char *localAddr, char *localPort)
{
	if(m_socket == SockInvalid){
		return FAIL;
	}
    struct hostent  pphhee,*phe;
    char  phebuf[1024];
    int   herr;
    struct sockaddr_in sin;
	memset(&sin,0,sizeof(sin));
	sin.sin_family = AF_INET;
	sin.sin_port = htons((u_short)atoi(localPort));
	gethostbyname_r(localAddr,&pphhee,phebuf,sizeof(phebuf),&phe,&herr);
	// phe = gethostbyname(localAddr);
	if(phe){
		memcpy(&sin.sin_addr, phe->h_addr, phe->h_length);
	}
	else if ((sin.sin_addr.s_addr = inet_addr(localAddr))== INADDR_NONE )
	{
		sin.sin_addr.s_addr = INADDR_ANY;
	}

	if(bind(m_socket,(struct sockaddr*)&sin,sizeof(sin))==SockError)
	{
		return FAIL;
	}
	socklen_t namelen=sizeof(m_sockLocal);
	getsockname(m_socket,&m_sockLocal,&namelen);

	return OK;
}

int MySocket::Listen(int BackLog)
{
	if(listen(m_socket,BackLog)==0){
		return OK;
	}
	else{
		return FAIL;
	}
}

SOCKET MySocket::Accept(struct sockaddr *cliaddr,socklen_t *addrlen)
{
	SOCKET newsocket;
#ifdef _WIN32
	newsocket = WSAAccept(m_socket, cliaddr, addrlen, NULL, 0);
#else
	newsocket = accept(m_socket,cliaddr,addrlen);
#endif
	return newsocket;
}

int  MySocket::Accept(MySocket *&newrs)
{
	socklen_t addrlen = sizeof(struct sockaddr);
	SOCKET newsocket;
	while((newsocket = Accept(&m_sockDest,&addrlen))<0){
		if(errno!=EINTR) break;
	}
	if(newsocket>=0){
		newrs = new MySocket;
		newrs->SetSocket(newsocket);
		newrs->SetDestAddr(&m_sockDest);
		newrs->ObtainLocalAddr();
		return OK;
	}
	else{
		return FAIL;
	}
}

int  MySocket::Accept_s(MySocket *newrs)
{
	socklen_t addrlen = sizeof(struct sockaddr);
	SOCKET newsocket;
	if(newrs == NULL)
    {
        return FAIL;
    }
	while((newsocket = Accept(&m_sockDest,&addrlen))<0){
		if(errno!=EINTR) break;
	}
	if(newsocket>=0){
		newrs->SetSocket(newsocket);
		newrs->SetDestAddr(&m_sockDest);
		newrs->ObtainLocalAddr();
		return OK;
	}
	else{
		return FAIL;
	}
}

void MySocket::SetSocket(SOCKET s)
{
	this->m_socket = s;
}

int MySocket::SetDestAddr(char *destIP, char *destPort)
{
	char transport[10];
	if(m_nSocketType == SOCK_DGRAM)	strcpy(transport,"udp");
	else if(m_nSocketType == SOCK_STREAM) strcpy(transport,"tcp");
	else return FAIL;

	struct hostent pphhee,*phe;
	struct servent* pse;
	struct sockaddr_in sin;
	char  phebuf[1024];
	int   herr;

	memset(&sin,0,sizeof(sin));
	sin.sin_family = AF_INET;
	if((pse=getservbyname(destPort,transport))){
		sin.sin_port = pse->s_port;
	}
	else if ((sin.sin_port = htons((u_short)atoi(destPort)))==0)
	{
		return FAIL;
	}
	gethostbyname_r(destIP,&pphhee,phebuf,1024,&phe,&herr);
//	phe = gethostbyname(destIP);
	if(phe){
		memcpy(&sin.sin_addr, phe->h_addr, phe->h_length);
	}
	else if ((sin.sin_addr.s_addr = inet_addr(destIP))== INADDR_NONE )
	{
		return FAIL;
	}
	memcpy(&m_sockDest,&sin,sizeof(sin));
	return OK;
}

int MySocket::SetDestAddr(char *destIP, short destPort)
{
	struct hostent pphhee,*phe;
	//struct servent* pse;
	struct sockaddr_in sin;
	char  phebuf[1024];
	int   herr;

	memset(&sin,0,sizeof(sin));
	sin.sin_family = AF_INET;
	if ((sin.sin_port = htons((u_short)destPort))==0)
	{
		return FAIL;
	}
	gethostbyname_r(destIP,&pphhee,phebuf,1024,&phe,&herr);
	if(phe){
		memcpy(&sin.sin_addr, phe->h_addr, phe->h_length);
	}
	else if ((sin.sin_addr.s_addr = inet_addr(destIP))== INADDR_NONE )
	{
		return FAIL;
	}
	memcpy(&m_sockDest,&sin,sizeof(sin));
	return OK;
}

int MySocket::SetDestAddr(long destIP, short destPort)
{
	struct sockaddr_in sin;
	memset(&sin,0,sizeof(sin));
	sin.sin_family = AF_INET;
	if((sin.sin_port=htons((u_short)destPort))==0)
	{
		return FAIL;
	}
	if ((sin.sin_addr.s_addr = htonl((u_long)destIP))== INADDR_NONE )
	{
		return FAIL;
	}
	memcpy(&m_sockDest,&sin,sizeof(sin));
	return OK;
}

int MySocket::SetDestNetAddr(long destIP, short destPort)
{
	struct sockaddr_in sin;

	memset(&sin,0,sizeof(sin));
	sin.sin_family = AF_INET;
	if((sin.sin_port=((u_short)destPort))==0)
	{
		return FAIL;
	}
	if ((sin.sin_addr.s_addr = ((u_long)destIP))== INADDR_NONE )
	{
		return FAIL;
	}
	memcpy(&m_sockDest,&sin,sizeof(sin));
	return OK;
}

int MySocket::SetDestAddr(struct sockaddr* destAddr)
{
	//m_sockDest = *destAddr;
	memcpy(&m_sockDest,destAddr,sizeof(m_sockDest));
	return OK;
}

int MySocket::ObtainLocalAddr()
{
	socklen_t namelen=sizeof(m_sockLocal);
	if(getsockname(m_socket,&m_sockLocal,&namelen) < 0)
		return ERROR;
	else
		return OK;
}

int MySocket::SetLocalAddr(struct sockaddr* localAddr)
{
	//m_sockLocal = *localAddr;
	memcpy(&m_sockLocal,localAddr,sizeof(m_sockLocal));
	return OK;
}

SOCKET MySocket::GetSocket()
{
	return m_socket;
}

SOCKET* MySocket::GetSocketPtr()
{
	return &m_socket;
}

int MySocket::SetNonBlockMode(DWORD bBlock)
{
	if(ioctl(m_socket,FIONBIO,&bBlock)==0) return OK;
	else return FAIL;
}

int MySocket::GetUdpSize()
{
	socklen_t slen = sizeof(sockaddr);
	int len = recvfrom(m_socket,0,0,MSG_PEEK,&m_sockRecvFrom,&slen);
	if(len==SockError) return FAIL;
	else return len;
}


int MySocket::GetNetAddr(sockaddr *sa, char *addr,int *port)
{
	sockaddr_in sin;
	memcpy(&sin,sa,sizeof(sin));
	sprintf(addr,"%s",inet_ntoa(sin.sin_addr));
	*port = ntohs(sin.sin_port);
	return OK;
}

int MySocket::GetLocalAddr(char *addr,int *port)
{
	return GetNetAddr(&m_sockLocal,addr,port);
}

int MySocket::GetDestAddr(char *addr,int *port)
{
	return GetNetAddr(&m_sockDest,addr,port);
}

int MySocket::GetRecvFromAddr(char *addr,int *port)
{
	return GetNetAddr(&m_sockRecvFrom,addr,port);
}

int MySocket::GetLastSocketError()
{
	return  0;
}

int MySocket::Poll(int SEvent, int MSec)
{
#ifdef _WIN32
	return SelectWait(SEvent, MSec);
#else
	struct pollfd fd;

	int n, data;

	int status = 0;

	fd.fd = m_socket;

	fd.events = 0;

	if(SEvent & FD_READ)
		fd.events |= POLLIN;
	if(SEvent & FD_WRITE)
		fd.events |= POLLOUT;

	n = poll(&fd, 1, MSec);

	if(n > 0)
	{
		if(fd.revents & POLLIN)
		{
			status |= FD_READ;

			if(RecvPeek((char*)&data, sizeof(data)) <= 0)
			{
				status |= FD_DISCONNECT;
			}
		}
		if(fd.revents & POLLOUT)
		{
			status |= FD_WRITE;
		}
		return status;
	}

	return FD_NONE;
#endif

}

int MySocket::Recv_base(char* pData, int nLen, unsigned long flag, void *overlap_struct, void *overlap_routine_proc)
{
	if( m_socket == SockInvalid )
	{
		return ERROR;
	}
#ifdef _WIN32
	WSABUF buffer;
	buffer.len = nLen;
	buffer.buf = pData;
	DWORD len = 0;
	int rv;
	int error;

	/*WSABUF *pbuffer = (WSABUF*)(new char[sizeof(WSABUF) + 8] + 8);
	pbuffer->len = nLen;
	pbuffer->buf = pData;*/

	rv = WSARecv(m_socket, &buffer, 1, &len, &flag,
		(LPWSAOVERLAPPED)overlap_struct, (LPWSAOVERLAPPED_COMPLETION_ROUTINE)overlap_routine_proc);


	error = WSAGetLastError();

	if(overlap_struct != NULL) //重叠IO
	{
		if(rv == SOCKET_ERROR && error == WSA_IO_PENDING)
		{
			return 0;
		}
		if(rv >= 0)
		{
			return rv;
		}
		return -1;
	}

	//非重叠IO
	if(rv == SOCKET_ERROR)
	{
		if(error == WSAECONNABORTED || error == WSAECONNRESET)
		{
			return DISCONNECT;
		}
		if(error == WSAEWOULDBLOCK)
		{
			return WOULDBLOCK;
		}
		return ERROR;
	}

	if(len == 0)
	{
		return DISCONNECT;
	}

	return len;
#else
	int len = recv(m_socket,pData,nLen,flag);
	if(len == SockError)
	{
		return ERROR;
	}
	if(len == 0) return DISCONNECT;
	else return len;
#endif
}
int MySocket::Send_base(char* pData, int nLen, unsigned long flag, void *overlap_struct, void *overlap_routine_proc)
{
	if( m_socket == SockInvalid )
	{
		return ERROR;
	}
#ifdef _WIN32
	WSABUF buffer;
	buffer.len = nLen;
	buffer.buf = pData;
	DWORD len;
	int rv;
	int error;

	rv = WSASend(m_socket, &buffer, 1, &len, flag,
		(LPWSAOVERLAPPED)overlap_struct, (LPWSAOVERLAPPED_COMPLETION_ROUTINE)overlap_routine_proc);

	error = WSAGetLastError();

	if(overlap_struct != NULL)//重叠IO
	{
		if(rv == SOCKET_ERROR && error == WSA_IO_PENDING)
		{
			return 0;
		}
		if(rv == 0)
		{
			return 0;
		}
		return -1;
	}

	//非重叠IO
	if(rv == SOCKET_ERROR)
	{

		if(error == WSAECONNABORTED || error == WSAECONNRESET)
		{
			return DISCONNECT;
		}
		if(error == WSAEWOULDBLOCK)
		{
			return WOULDBLOCK;
		}
		return ERROR;
	}

	if(len == 0)
	{
		return DISCONNECT;
	}

	return len;

#else
	int len = send(m_socket,pData,nLen,flag);
	if(len == SockError)
	{
		return ERROR;
	}
	if(len == 0) return DISCONNECT;
	else return len;
#endif
}

#ifdef _WIN32

//windows 下重叠IO函数
int MySocket::CreateOverlaped()
{
	int         nRet;
	int         nZero = 0;
	LINGER      lingerStruct;


	m_socket = INVALID_SOCKET;

	m_socket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_IP, NULL, 0, WSA_FLAG_OVERLAPPED);
	if (INVALID_SOCKET == m_socket)
	{
		//printf("WSASocket(sdSocket): %d\n", WSAGetLastError());
		return(FAIL);
	}

	// Disable send buffering on the socket.  Setting SO_SNDBUF
	// to 0 causes winsock to stop bufferring sends and perform
	// sends directly from our buffers, thereby save one memory copy.
	nZero = 0;
	nRet = setsockopt(m_socket, SOL_SOCKET, SO_SNDBUF, (char *)&nZero, sizeof(nZero));
	if (SOCKET_ERROR == nRet)
	{
		//printf("setsockopt(SNDBUF): %d\n", WSAGetLastError());
		return(FAIL);
	}

	// Disable receive buffering on the socket.  Setting SO_RCVBUF
	// to 0 causes winsock to stop bufferring receive and perform
	// receives directly from our buffers, thereby save one memory copy.
	nZero = 0;
	nRet = setsockopt(m_socket, SOL_SOCKET, SO_RCVBUF, (char *)&nZero, sizeof(nZero));
	if (SOCKET_ERROR == nRet)
	{
		printf("setsockopt(SO_RCVBUF): %d\n", WSAGetLastError());
		return(FAIL);
	}

	lingerStruct.l_onoff = 1;
	lingerStruct.l_linger = 0;
	nRet = setsockopt(m_socket, SOL_SOCKET, SO_LINGER,
		(char *)&lingerStruct, sizeof(lingerStruct));
	if (SOCKET_ERROR == nRet)
	{
		printf("setsockopt(SO_LINGER): %d\n", WSAGetLastError());
		return(FAIL);
	}

	return OK;
}

/*重叠IOaccept函数，第二个参数传NULL新建一个Accept_Per_IO结构，如果非NULL使用传入的指针
*/
int MySocket::AcceptEx(pAccept_Connect_Per_IO per_io)
{
	int err;
	if(per_io == NULL)
	{
		per_io = new Accept_Connect_Per_IO;
		if(per_io == NULL)
			return FAIL;
	}
	per_io->socket_accepted = new MySocket;
	if(per_io->socket_accepted == NULL)
	{
		delete per_io;
		return FAIL;
	}
	memset(&per_io->overlapped, 0, sizeof(OVERLAPPED));
	if(per_io->socket_accepted->CreateOverlaped() != OK)
	{
		delete per_io;
		return FAIL;
	}

	per_io->operation = OP_ACCEPT;

	if(!::AcceptEx(m_socket, per_io->socket_accepted->GetSocket(),
		per_io->buffer, ACCEPTEX_CONNECTEX_BUFFER_SIZE - 2 * (sizeof(sockaddr_in) + 16),
		sizeof(sockaddr_in) + 16, sizeof(sockaddr_in) + 16, (LPDWORD)&per_io->bytes_inited,
		(LPOVERLAPPED)&per_io->overlapped))
	{
		if(GetLastError() != WSA_IO_PENDING)
		{
			delete per_io;
			return FAIL;
		}
	}

	return OK;
}

/*调用AcceptEx之后，在工作线程返回了accept的socket后，由accept的socket调用，得到连接的本地和远程地址,
*/
#ifndef _MSC_VER
typedef VOID PASCAL (*LPFN_GETACCEPTEXSOCKADDRS)(PVOID,DWORD,DWORD,DWORD,struct sockaddr**, LPINT, struct sockaddr**, LPINT);
#endif
#ifndef WSAID_ACCEPTEX
# define WSAID_ACCEPTEX \
{0xb5367df1, 0xcbac, 0x11cf, \
{0x95, 0xca, 0x00, 0x80, 0x5f, 0x48, 0xa1, 0x92}}

# define WSAID_CONNECTEX \
{0x25a207b9, 0xddf3, 0x4660, \
{0x8e, 0xe9, 0x76, 0xe5, 0x8c, 0x74, 0x06, 0x3e}}

# define WSAID_GETACCEPTEXSOCKADDRS \
{0xb5367df2, 0xcbac, 0x11cf, \
{0x95, 0xca, 0x00, 0x80, 0x5f, 0x48, 0xa1, 0x92}}

# define WSAID_DISCONNECTEX \
{0x7fda2e11, 0x8630, 0x436f, \
{0xa0, 0x31, 0xf5, 0x36, 0xa6, 0xee, 0xc1, 0x57}}

# define WSAID_TRANSMITFILE \
{0xb5367df0, 0xcbac, 0x11cf, \
{0x95, 0xca, 0x00, 0x80, 0x5f, 0x48, 0xa1, 0x92}}

typedef BOOL PASCAL (*LPFN_ACCEPTEX)
(SOCKET sListenSocket,
SOCKET sAcceptSocket,
PVOID lpOutputBuffer,
DWORD dwReceiveDataLength,
DWORD dwLocalAddressLength,
DWORD dwRemoteAddressLength,
LPDWORD lpdwBytesReceived,
LPOVERLAPPED lpOverlapped);

typedef BOOL PASCAL (*LPFN_CONNECTEX)
(SOCKET s,
const struct sockaddr* name,
int namelen,
PVOID lpSendBuffer,
DWORD dwSendDataLength,
LPDWORD lpdwBytesSent,
LPOVERLAPPED lpOverlapped);

typedef void PASCAL (*LPFN_GETACCEPTEXSOCKADDRS)
(PVOID lpOutputBuffer,
DWORD dwReceiveDataLength,
DWORD dwLocalAddressLength,
DWORD dwRemoteAddressLength,
LPSOCKADDR* LocalSockaddr,
LPINT LocalSockaddrLength,
LPSOCKADDR* RemoteSockaddr,
LPINT RemoteSockaddrLength);

typedef BOOL PASCAL (*LPFN_DISCONNECTEX)
(SOCKET hSocket,
LPOVERLAPPED lpOverlapped,
DWORD dwFlags,
DWORD reserved);

typedef BOOL PASCAL (*LPFN_TRANSMITFILE)
(SOCKET hSocket,
HANDLE hFile,
DWORD nNumberOfBytesToWrite,
DWORD nNumberOfBytesPerSend,
LPOVERLAPPED lpOverlapped,
LPTRANSMIT_FILE_BUFFERS lpTransmitBuffers,
DWORD dwFlags);

typedef PVOID RTL_SRWLOCK;
typedef RTL_SRWLOCK SRWLOCK, *PSRWLOCK;
#endif
int MySocket::GetAcceptExSockaddrs(pAccept_Connect_Per_IO per_io)
{
	int localLen, remoteLen;
	sockaddr *localAddr, *remoteAddr;
	if(per_io == NULL)
	{
		return FAIL;
	}
	LPFN_GETACCEPTEXSOCKADDRS lpfn_getacceptexsockaddrs = ::GetAcceptExSockaddrs;
	lpfn_getacceptexsockaddrs(per_io->buffer, ACCEPTEX_CONNECTEX_BUFFER_SIZE - 2 * (sizeof(sockaddr_in) + 16),
		sizeof(sockaddr_in) + 16, sizeof(sockaddr_in) + 16, &localAddr, &localLen,
		&remoteAddr, &remoteLen);

	memcpy(&m_sockLocal, localAddr, sizeof(sockaddr));
	memcpy(&m_sockDest, remoteAddr, sizeof(sockaddr));

	return OK;
}

//重叠IOconnect函数，后三个数据是连接上同时发送的数据(可选)
int MySocket::ConnectEx(char *destIP, int destPort, pAccept_Connect_Per_IO per_io)
{
	sockaddr remote;

	if(destIP == NULL || destPort < 0 || destPort > 65535)
	{
		return FAIL;
	}

	if(MySocket::GetSockAddrFromChar(&remote, destIP, destPort) != OK)
	{
		return FAIL;
	}

	if(per_io == NULL)
	{
		per_io = new Accept_Connect_Per_IO;
		if(per_io == NULL)
		{
			return FAIL;
		}
	}

	per_io->socket_accepted = this;
	LPFN_CONNECTEX lpfn_connectex;
	if(!lpfn_connectex(m_socket, &remote, sizeof(sockaddr), NULL, 0, NULL, (LPOVERLAPPED)per_io))
	{
		delete per_io;
		return FAIL;
	}

	return OK;
}

int MySocket::WSAIoctl(DWORD dwIoControlCode, void* lpvInBuffer, DWORD cbInBuffer, void* lpvOutBuffer, DWORD cbOutBuffer,
     LPDWORD lpcbBytesReturned,
     LPWSAOVERLAPPED lpOverlapped,
     LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine)
{
	return ::WSAIoctl(m_socket, dwIoControlCode, lpvInBuffer, cbInBuffer, lpvOutBuffer, cbOutBuffer, lpcbBytesReturned, lpOverlapped,
		lpCompletionRoutine);
}

#endif
