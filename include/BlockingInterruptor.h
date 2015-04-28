#ifndef BLOCKINGINTERRUPTOR_H
#define BLOCKINGINTERRUPTOR_H

#include "MySocket.h"


class BlockingInterruptor
{
    public:
        /** Default constructor */
        BlockingInterruptor();
        /** Default destructor */
        virtual ~BlockingInterruptor();

        bool init();
        MySocket *getSockIn();
        MySocket *getSockOut();
        bool available();
        bool interrupt();
        bool restore();
    protected:
        #ifdef _WIN32
        MySocket server;
        MySocket sock_in;
        MySocket sock_out;
        #else

        #endif // _WIN32
        bool avail;
    private:
};

#endif // BLOCKINGINTERRUPTOR_H
