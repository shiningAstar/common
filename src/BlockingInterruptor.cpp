#include "BlockingInterruptor.h"
#include "Thread.h"
#include "time.h"

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
    unsigned long doWork()
    {
        time_t s, t;
        if(client == NULL)
        {
            return 0;
        }
        s = time(NULL);
        while(client->Connect("127.0.0.1", 11111) != OK)
        {
            t = time(NULL);
            if(t - s > 5)
                return 0;
        }
        return 1;
    }
};

bool BlockingInterruptor::init()
{
    ClientThread thread;
    if(server.Create() != OK)
    {
        return false;
    }
    if(sock_in.Create() != OK)
    {
        return false;
    }

    thread.client = &sock_in;
    if(!thread.start())
    {
        return false;
    }

    if(server.Bind("127.0.0.1", 11111) != OK)
    {
        return false;
    }
    if(server.Listen(1) != OK)
    {
        return false;
    }




    //thread.waittoStop();



    if(server.SelectWait(FD_READ, 5000) != FD_READ)
    {
        return false;
    }
    if(server.Accept_s(&sock_out) != OK)
    {
        return false;
    }
    server.Close();
    avail = true;
    thread.waittoStop();
    return true;

}

MySocket *BlockingInterruptor::getSockIn()
{
    return &sock_in;
}

MySocket *BlockingInterruptor::getSockOut()
{
    return &sock_out;
}

bool BlockingInterruptor::available()
{
    return avail;
}

bool BlockingInterruptor::interrupt()
{
    char c = 0;
    if(!avail)
    {
        return false;
    }
    if(sock_in.Writen(&c, 1, 1000) != 1)
    {
        return false;
    }
    return true;
}

bool BlockingInterruptor::restore()
{
    char c;
    if(!avail)
    {
        return false;
    }
    if(sock_out.Readn(&c, 1, 1000) != 1)
    {
        return false;
    }
    if(c != 0)
    {
        return false;
    }
    return true;
}

