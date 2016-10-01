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

        /** \初始化日志
         *
         * \param log_first 指向不同log方式的第一个方式地址的指针
         * \param num log方式的个数
         * \param level 日志等级 取值LOG_LEVEL_DEBUG与LOG_LEVEL_NONE之间
         * \return
         *
         */
        bool init(logBase **log_first, int num, int level);
        /** \ 输出日志，对于每种方式，不判断当前的日志等级
         *
         * \param
         * \param
         * \return
         *
         */
        bool output_log(char *log_txt, ...);
        bool voutput_log(char *log_txt, va_list va);
        /** \根据日志等级输出日志，只在输出的等级level大于等于系统日志等级log_level时输出
         *
         * \param
         * \param
         * \return
         *
         */
        bool output_log_with_level(int level, char *log_txt, ...);
        bool voutput_log_with_level(int level, char *log_txt, va_list va);
        /** \根据日志等级输出日志（并输出一个换行\n），只在输出的等级level大于等于系统日志等级log_level时输出
         *
         * \param
         * \param
         * \return
         *
         */
        bool output_log_with_level_line(int level, char *log_txt, ...);
        bool voutput_log_with_level_line(int level, char *log_txt, va_list va);
        /** \根据日志等级输出日志（并在结尾输出时间和一个换行\n），只在输出的等级level大于等于系统日志等级log_level时输出
         *
         * \param
         * \param
         * \return
         *
         */
        bool output_log_with_level_time_line(int level, char *log_txt, ...);
        bool voutput_log_with_level_time_line(int level, char *log_txt, va_list va);
        /** \ 输出日志（并在结尾输出时间和一个换行\n），对于每种方式，不判断当前的日志等级
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

/**< 直接输出日志的宏 */

///**< 根据日志等级输出日志，只在输出的等级大于等于系统日志等级log_level时输出 */
//#define log_debug(log_txt,args...) log::getInstance()->debug(log_txt,##args)
//#define log_info(log_txt,args...) log::getInstance()->info(log_txt,##args)
//#define log_warn(log_txt,args...) log::getInstance()->warn(log_txt,##args)
//#define log_error(log_txt,args...) log::getInstance()->error(log_txt,##args)
///**< 根据日志等级输出日志（并输出一个换行\n），只在输出的等级大于等于系统日志等级log_level时输出 */
//#define log_debug_line(log_txt,args...) log::getInstance()->debug_line(log_txt,##args)
//#define log_info_line(log_txt,args...) log::getInstance()->info_line(log_txt,##args)
//#define log_warn_line(log_txt,args...) log::getInstance()->warn_line(log_txt,##args)
//#define log_error_line(log_txt,args...) log::getInstance()->error_line(log_txt,##args)
///**< 根据日志等级输出日志（并在结尾输出时间和一个换行\n），只在输出的等级大于等于系统日志等级log_level时输出 */
//#define log_debug_time_line(log_txt,args...) log::getInstance()->debug_time_line(log_txt,##args)
//#define log_info_time_line(log_txt,args...) log::getInstance()->info_time_line(log_txt,##args)
//#define log_warn_time_line(log_txt,args...) log::getInstance()->warn_time_line(log_txt,##args)
//#define log_error_time_line(log_txt,args...) log::getInstance()->error_time_line(log_txt,##args)



/**< 根据日志等级输出日志，只在输出的等级大于等于系统日志等级log_level时输出 */
#define log_debug(log_txt,...) log::getInstance()->debug(log_txt,##__VA_ARGS__)
#define log_info(log_txt,...) log::getInstance()->info(log_txt,##__VA_ARGS__)
#define log_warn(log_txt,...) log::getInstance()->warn(log_txt,##__VA_ARGS__)
#define log_error(log_txt,...) log::getInstance()->error(log_txt,##__VA_ARGS__)
/**< 根据日志等级输出日志（并输出一个换行\n），只在输出的等级大于等于系统日志等级log_level时输出 */
#define log_debug_line(log_txt,args...) log::getInstance()->debug_line(log_txt,##args)
#define log_info_line(log_txt,args...) log::getInstance()->info_line(log_txt,##args)
#define log_warn_line(log_txt,args...) log::getInstance()->warn_line(log_txt,##args)
#define log_error_line(log_txt,args...) log::getInstance()->error_line(log_txt,##args)
/**< 根据日志等级输出日志（并在结尾输出时间和一个换行\n），只在输出的等级大于等于系统日志等级log_level时输出 */
#define log_debug_time_line(log_txt,args...) log::getInstance()->debug_time_line(log_txt,##args)
#define log_info_time_line(log_txt,args...) log::getInstance()->info_time_line(log_txt,##args)
#define log_warn_time_line(log_txt,args...) log::getInstance()->warn_time_line(log_txt,##args)
#define log_error_time_line(log_txt,args...) log::getInstance()->error_time_line(log_txt,##args)


/**< 不论日志等级，直接输出日志 */
#define normal_log(log_txt,args...) log::getInstance()->output_log(log_txt,##args)
/**< 不论日志等级，直接输出日志（并在结尾输出时间和一个换行\n） */
#define normal_log_time_line(log_txt,args...) log::getInstance()->output_log_with_time_line(log_txt,##args)

#endif // LOG_H
