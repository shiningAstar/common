#ifndef LOG_HH
#define LOG_HH

#include "logBase.h"
#include "logTerminal.h"
#include "logFile.h"
#include "logControl.h"

#define LOG_LEVEL_DEBUG 0
#define LOG_LEVEL_INFO 1
#define LOG_LEVEL_WARN 2
#define LOG_LEVEL_ERROR 3
#define LOG_LEVEL_NONE 4

class log
{
    public:
        static log *getInstance();
        /** Default destructor */
        virtual ~log();

        logBase *log_func_list[32];
        int num_of_log;

        int log_level;

        /** \��ʼ����־
         *
         * \param log_first ָ��ͬlog��ʽ�ĵ�һ����ʽ��ַ��ָ��
         * \param num log��ʽ�ĸ���
         * \param level ��־�ȼ� ȡֵLOG_LEVEL_DEBUG��LOG_LEVEL_NONE֮��
         * \return
         *
         */
        bool init(logBase **log_first, int num, int level);
        /** \ �����־������ÿ�ַ�ʽ�����жϵ�ǰ����־�ȼ�
         *
         * \param
         * \param
         * \return
         *
         */
        bool output_log(char *log_txt, ...);
        bool voutput_log(char *log_txt, va_list va);
        /** \������־�ȼ������־��ֻ������ĵȼ�level���ڵ���ϵͳ��־�ȼ�log_levelʱ���
         *
         * \param
         * \param
         * \return
         *
         */
        bool output_log_with_level(int level, char *log_txt, ...);
        bool voutput_log_with_level(int level, char *log_txt, va_list va);
        /** \������־�ȼ������־�������һ������\n����ֻ������ĵȼ�level���ڵ���ϵͳ��־�ȼ�log_levelʱ���
         *
         * \param
         * \param
         * \return
         *
         */
        bool output_log_with_level_line(int level, char *log_txt, ...);
        bool voutput_log_with_level_line(int level, char *log_txt, va_list va);
        /** \������־�ȼ������־�����ڽ�β���ʱ���һ������\n����ֻ������ĵȼ�level���ڵ���ϵͳ��־�ȼ�log_levelʱ���
         *
         * \param
         * \param
         * \return
         *
         */
        bool output_log_with_level_time_line(int level, char *log_txt, ...);
        bool voutput_log_with_level_time_line(int level, char *log_txt, va_list va);
        /** \ �����־�����ڽ�β���ʱ���һ������\n��������ÿ�ַ�ʽ�����жϵ�ǰ����־�ȼ�
         *
         * \param
         * \param
         * \return
         *
         */
        bool output_log_with_time_line(char *log_txt, ...);
        bool voutput_log_with_time_line(char *log_txt, va_list va);

        bool debug(char *log_txt, ...);
        bool info(char *log_txt, ...);
        bool warn(char *log_txt, ...);
        bool error(char *log_txt, ...);

        bool debug_line(char *log_txt, ...);
        bool info_line(char *log_txt, ...);
        bool warn_line(char *log_txt, ...);
        bool error_line(char *log_txt, ...);

        bool debug_time_line(char *log_txt, ...);
        bool info_time_line(char *log_txt, ...);
        bool warn_time_line(char *log_txt, ...);
        bool error_time_line(char *log_txt, ...);
    protected:
    private:
        /** Default constructor */
        log();
};

/**< ֱ�������־�ĺ� */

///**< ������־�ȼ������־��ֻ������ĵȼ����ڵ���ϵͳ��־�ȼ�log_levelʱ��� */
//#define log_debug(log_txt,args...) log::getInstance()->debug(log_txt,##args)
//#define log_info(log_txt,args...) log::getInstance()->info(log_txt,##args)
//#define log_warn(log_txt,args...) log::getInstance()->warn(log_txt,##args)
//#define log_error(log_txt,args...) log::getInstance()->error(log_txt,##args)
///**< ������־�ȼ������־�������һ������\n����ֻ������ĵȼ����ڵ���ϵͳ��־�ȼ�log_levelʱ��� */
//#define log_debug_line(log_txt,args...) log::getInstance()->debug_line(log_txt,##args)
//#define log_info_line(log_txt,args...) log::getInstance()->info_line(log_txt,##args)
//#define log_warn_line(log_txt,args...) log::getInstance()->warn_line(log_txt,##args)
//#define log_error_line(log_txt,args...) log::getInstance()->error_line(log_txt,##args)
///**< ������־�ȼ������־�����ڽ�β���ʱ���һ������\n����ֻ������ĵȼ����ڵ���ϵͳ��־�ȼ�log_levelʱ��� */
//#define log_debug_time_line(log_txt,args...) log::getInstance()->debug_time_line(log_txt,##args)
//#define log_info_time_line(log_txt,args...) log::getInstance()->info_time_line(log_txt,##args)
//#define log_warn_time_line(log_txt,args...) log::getInstance()->warn_time_line(log_txt,##args)
//#define log_error_time_line(log_txt,args...) log::getInstance()->error_time_line(log_txt,##args)



/**< ������־�ȼ������־��ֻ������ĵȼ����ڵ���ϵͳ��־�ȼ�log_levelʱ��� */
#define log_debug(log_txt,...) log::getInstance()->debug(log_txt,##__VA_ARGS__)
#define log_info(log_txt,...) log::getInstance()->info(log_txt,##__VA_ARGS__)
#define log_warn(log_txt,...) log::getInstance()->warn(log_txt,##__VA_ARGS__)
#define log_error(log_txt,...) log::getInstance()->error(log_txt,##__VA_ARGS__)
/**< ������־�ȼ������־�������һ������\n����ֻ������ĵȼ����ڵ���ϵͳ��־�ȼ�log_levelʱ��� */
#define log_debug_line(log_txt,args...) log::getInstance()->debug_line(log_txt,##args)
#define log_info_line(log_txt,args...) log::getInstance()->info_line(log_txt,##args)
#define log_warn_line(log_txt,args...) log::getInstance()->warn_line(log_txt,##args)
#define log_error_line(log_txt,args...) log::getInstance()->error_line(log_txt,##args)
/**< ������־�ȼ������־�����ڽ�β���ʱ���һ������\n����ֻ������ĵȼ����ڵ���ϵͳ��־�ȼ�log_levelʱ��� */
#define log_debug_time_line(log_txt,args...) log::getInstance()->debug_time_line(log_txt,##args)
#define log_info_time_line(log_txt,args...) log::getInstance()->info_time_line(log_txt,##args)
#define log_warn_time_line(log_txt,args...) log::getInstance()->warn_time_line(log_txt,##args)
#define log_error_time_line(log_txt,args...) log::getInstance()->error_time_line(log_txt,##args)


/**< ������־�ȼ���ֱ�������־ */
#define normal_log(log_txt,args...) log::getInstance()->output_log(log_txt,##args)
/**< ������־�ȼ���ֱ�������־�����ڽ�β���ʱ���һ������\n�� */
#define normal_log_time_line(log_txt,args...) log::getInstance()->output_log_with_time_line(log_txt,##args)

#endif // LOG_H
