//
// Created by bokket on 2020/12/24.
//

#ifndef C_2_LOG_H
#define C_2_LOG_H

#include <string>
#include <memory>
#include <list>
#include <iostream>
#include <vector>
#include <stdarg.h>
#include <map>
#include <fstream>
#include <sstream>
#include <time.h>
#include <singleton.h>
#include <stdarg.h>
using namespace std;


//LogEventWrap对象初始化 传参为LogEvent::ptr智能指针 
//LogEvent::ptr new() 一个指针初始化为其本身对象LogEvent(把LogEvent传入LogEvent::ptr)
#define BOKKET_LOG_LEVEL(logger,level) \
    if(logger->getLevel() <= level )   \
        bokket::LogEventWrap(bokket::LogEvent::ptr new(bokket::LogEvent(logger,level,\
                __FILE__,__LINE__,0,bokket::LogEvent::getThreadId(),bokket::LogEvent::getCoreadId(),\
                time(NULL)) )).getStringStream()

#define BOKKET_LOG_DEBUG(logger) BOKKET_LOG_LEVEL(logger,bokket::LogLevel::DEBUG)
#define BOKKET_LOG_INFO(logger) BOKKET_LOG_LEVEL(logger,bokket::LogLevel::INFO)
#define BOKKET_LOG_WARN(logger) BOKKET_LOG_LEVEL(logger,bokket::LogLevel::WARN)
#define BOKKET_LOG_ERROR(logger) BOKKET_LOG_LEVEL(logger,bokket::LogLevel::ERROR)
#define BOKKET_LOG_FATAL(logger) BOKKET_LOG_LEVEL(logger,bokket::LogLevel::FATAL)


#define BOKKET_LOG_FMT_LEVEL(logger,level,fmt,...) \
    if(logger->getLevel() <= level  ) \
        bokket::LogEventWrap(bokket::LogEvent::ptr new(bokket::LogEvent(logger,level,\
                __FILE__,__LINE__,0,bokket::LogEvent::getThreadId(),bokket::LogEvent::getCoreadId(),\
                time(NULL)  ))).getEvent->fmt(fmt,__VA_ARGS__)

#define BOKKET_LOG_FMT_DEBUG(logger,fmt,...) BOKKET_LOG_FMT_LEVEL(logger,bokket::LogLevel::DEBUG,fmt,_VA_ARGS_)
#define BOKKET_LOG_FMT_INFO(logger,fmt,...) BOKKET_LOG_FMT_LEVEL(logger,bokket::LogLevel::INFO,fmt,_VA_ARGS_)
#define BOKKET_LOG_FMT_WARN(logger,fmt,...) BOKKET_LOG_FMT_LEVEL(logger,bokket::LogLevel::WARN,fmt,_VA_ARGS_)
#define BOKKET_LOG_FMT_ERROR(logger,fmt,...) BOKKET_LOG_FMT_LEVEL(logger,bokket::LogLevel::ERROR,fmt,_VA_ARGS_)
#define BOKKET_LOG_FMT_FATAL(logger,fmt,...) BOKKET_LOG_FMT_LEVEL(logger,bokket::LogLevel::FATAL,fmt,_VA_ARGS_)


#define BOKKET_LOG_ROOT() bokket::LoggerMgr::GetInstance()->getRoot()

namespace bokket
{


class Logger;



//日志级别
class LogLevel
{
public:
    enum Level
    {
        UNKNOW=0,
        DEBUG=1,
        INFO=2,
        WARN=3,
        ERROR=4,
        FATAL=5
    };

    static const char* ToString(LogLevel::Level level);
};



//日志事件
class LogEvent
{
public:
    typedef shared_ptr<LogEvent> ptr;
public:
    LogEvent(shared_ptr<Logger> logger,LogLevel::Level level
             ,const char* file,int32_t line,uint32_t msec
             ,uint32_t thread_id,uint32_t coread_id,uint64_t time);

    const char* getFile() const { return m_file; }
    int32_t getLine() const { return m_line; }
    uint32_t getMsec() const { return m_msec; }
    uint32_t getThreadId() const { return m_threadId; }
    uint32_t getCoreadId() const { return m_coreadId; }
    uint64_t getTime() const { return m_time; }
    string getContent() const { return m_stream.str(); }

    shared_ptr<Logger> getLogger() const { return m_logger; }
    LogLevel::Level getLevel() const { return m_level; }

    stringstream & getStringStream() { return m_stream; }

    void fmt(const char* format,...);
    void fmt(const char* format,va_list vaList);

private:
    const char* m_file=NULL;  //文件名
    int32_t m_line=0;         //行号
    uint32_t m_msec=0;        //程序启动开始到现在的毫秒
    uint32_t m_threadId=0;    //线程ID
    uint32_t m_coreadId=0;    //协程ID
    uint64_t m_time=0;        //时间戳

    stringstream m_stream;
    shared_ptr<Logger> m_logger;
    LogLevel::Level m_level;
};


class LogEventWrap
{
public:
    LogEventWrap(LogEvent::ptr ptr);
    ~LogEventWrap();
    LogEvent::ptr getEvent() const { return m_event; }
    stringstream & getStringStream();

private:
    LogEvent::ptr m_event;
};


//日志格式器
class LogFmtter
{
public:
    typedef shared_ptr<LogFmtter> ptr;
public:
    LogFmtter(const string & pattern);

    string fmt(shared_ptr<Logger> logger
               ,LogLevel::Level level
               ,LogEvent::ptr event);

public:
    class FmtItem
    {
    public:
        typedef shared_ptr<FmtItem> ptr;
    public:
        virtual ~FmtItem() {}
        //virtual void fmt(ostream& os,shared_ptr<Logger> logger,LogLevel::Level level
        //                 ,LogEvent::ptr event)=0;
        virtual string fmt(LogEvent::ptr event)=0;
    };
    void init();
private:
    string m_pattern;
    vector<FmtItem::ptr> m_items;

};


//日志输出器
class LogAppender
{
public:
    typedef shared_ptr<LogAppender> ptr;
public:
    virtual ~LogAppender() {}
    virtual void log(shared_ptr<Logger> logger,LogLevel::Level level
                     ,LogEvent::ptr event)=0;
    void setFmtter(LogFmtter::ptr val) { m_fmtter=val; }
    LogFmtter::ptr getFmtter() const { return m_fmtter; }

    void setLevel(LogLevel::Level level) { m_level=level; }
    LogLevel::Level getLevel() const { return m_level; }

protected:
    LogLevel::Level m_level=LogLevel::DEBUG;
    LogFmtter::ptr  m_fmtter;
};


//日志器
class Logger: public enable_shared_from_this<Logger>
{
public:
    typedef shared_ptr<Logger> ptr;
public:
    Logger(const string & name="root");
    void log(LogLevel::Level level,LogEvent::ptr event);

    void debug(LogEvent::ptr event);
    void info(LogEvent::ptr event);
    void warn(LogEvent::ptr event);
    void error(LogEvent::ptr event);
    void fatal(LogEvent::ptr event);

    void addAppender(LogPrint::ptr appender);
    void delAppender(LogPrint::ptr appender);

    LogLevel::Level getLevel() const { return m_lever; }
    void setLevel(LogLevel::Level level) { m_lever=level; }
    const string & getName() const { return m_name; }

private:
    string m_name;                      //日志名称
    LogLevel::Level m_lever;            //日志级别
    list<LogAppender::ptr> m_appenders;     //Appender集合，保存Appender地址
    LogFmtter::ptr m_fmtter;
};

//输出到控制台的Appender
class LogPrintStdout: public LogAppender
{
public:
    typedef shared_ptr<LogPrintStdout> ptr;
public:
    void log(Logger::ptr logger,LogLevel::Level level,LogEvent::ptr event) override;
};

//定义输出到文件的Appender
class LogPrintFile: public LogAppender
{
public:
    typedef shared_ptr<LogPrintFile> ptr;
public:
    LogPrintFile(const string& filename);
    void log(Logger::ptr logger,LogLevel::Level level,LogEvent::ptr event) override;
    
    //重新打开文件，文件打开成功返回true
    bool reopen();

private:
    string m_filename;
    ofstream m_filestream;
};

class LoggerManager
{
public:
    LoggerManager();
    Logger::ptr getLogger(const string& name);


    void init();
    Logger::ptr getRoot() const { return m_root; }
    void setRoot(Logger::ptr root) { m_root=root; }

private:
    map<string,Logger::ptr> m_loggers;
    Logger::ptr m_root;
};

    typedef bokket::Singleton<LoggerManager> LoggerMgr;
}
#endif //C_2_LOG_H
