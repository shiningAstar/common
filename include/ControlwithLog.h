#ifndef CONTROLWITHLOG_H
#define CONTROLWITHLOG_H


class ControlwithLog
{
    public:
        /** Default constructor */
        ControlwithLog();
        /** Default destructor */
        virtual ~ControlwithLog();

        virtual bool log_in_control(char *log, int max_length) = 0;
    protected:
    private:
};

#endif // CONTROLWITHLOG_H
