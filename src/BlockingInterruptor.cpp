#include "BlockingInterruptor.h"
#include "Thread.h"
#include "time.h"
#include "unistd.h"
BlockingInterruptor::BlockingInterruptor() : avail(false)
{
    //ctor
}

BlockingInterruptor::~BlockingInterruptor()
{
    //dtor
}

class ClientThread : public Thread
{
public:
    MySocket *client;
    unsigned short port;
    unsigned long doWork()
    {
        time_t s, t;
        if(client == NULL)
        {
            return 0;
        }
        s = time(NULL);
        while(client->Connect("127.0.0.1", port) != OK)
        {
            t = time(NULL);
            //printf("connect to port %d.s %ud t %ud.\n");
            if(t - s > 5)
                return 0;
        }
        return 1;
    }
};

bool BlockingInterruptor::init()
{
    #ifdef _WIN32
    ClientThread thread;
    //printf("blockinginterrupt init.\n");

    if(server.Create() != OK)
    {
        printf("server create failed.\n");
        return false;
    }
    if(sock_in.Create() != OK)
    {
        printf("sock in create failed.\n");
        return false;
    }

    thread.client = &sock_in;
    thread.port = rand() % 65536;
    if(!thread.start(false))
    {
        printf("thread start failed.\n");
        return false;
    }

    if(server.Bind("127.0.0.1", thread.port) != OK)
    {
        printf("server bind failed.\n");
        return false;
    }
    if(server.Listen(1) != OK)
    {
        printf("server listen failed.\n");
        return false;
    }

    if(server.SelectWait(FD_READ, 5000) != FD_READ)
    {
        printf("server select failed.\n");
        return false;
    }
    if(server.Accept_s(&sock_out) != OK)
    {
        printf("server accept failed.\n");
        return false;
    }
    server.Close();
    #else
    pipe(fdctr);
    #endif // _WIN32
    avail = true;
    #ifdef _WIN32
    thread.waittoStop();
    #endif // _WIN32

    return true;

}


int *BlockingInterruptor::getFdctr()
{
    return fdctr;
}
#ifdef _WIN32
MySocket *BlockingInterruptor::getSockIn()
{
    return &sock_in;
}
MySocket *BlockingInterruptor::getSockOut()
{
    return &sock_out;
}
#endif // _WIN32
bool BlockingInterruptor::available()
{
    return avail;
}

bool BlockingInterruptor::interrupt()
{

    if(!avail)
    {
        return false;
    }
    #ifdef _WIN32
    char c = 0;
    if(sock_in.Writen(&c, 1, 1000) != 1)
    {
        return false;
    }
    #else
    int length;
    if((length = write(fdctr[1],"interrupt",9))!=9)
    {
        printf("pipe write error!\n");
    }
    #endif // _WIN32

    return true;
}

bool BlockingInterruptor::restore()
{

    if(!avail)
    {
        return false;
    }
    #ifdef _WIN32
    char c;
    if(sock_out.Readn(&c, 1, 1000) != 1)
    {
        return false;
    }
    if(c != 0)
    {
        return false;
    }
    #else
    char ptr[10];
    int length;

    if(( length = read(fdctr[0],ptr,9)) !=9)
    {
        printf("pipe read error!\n");
    }
    #endif

    return true;
}

