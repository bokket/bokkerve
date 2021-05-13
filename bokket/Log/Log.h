//
// Created by bokket on 2021/4/7.
//

#ifndef BOKKERVE_LOG_H
#define BOKKERVE_LOG_H


#include <map>
#include <list>
#include <mutex>
#include <vector>
#include <string>
#include <memory>
#include <thread>
#include <chrono>
#include <cstdarg>
#include <sstream>
#include <fstream>
#include <condition_variable>
#include "./LogLevel.h"
//#include "./LogStream.h"
#include "./FileWriterType.h"
#include "../base/noncopyable.h"
#include "../base/SpinLock.h"
#include "../base/Singleton.h"
#include "../thread/util.h"



//LogEventWrap对象初始化 传参为LogEvent::ptr智能指针 
//LogEvent::ptr new() 一个指针初始化为其本身对象LogEvent(把LogEvent传入LogEvent::ptr)
#define BOKKET_LOG_LEVEL(logger,level) \
    if(logger->getLevel() <= level )   \
        bokket::LogEventWrap(bokket::LogEvent::ptr (new bokket::LogEvent(logger,level,\
                __FILE__,__FUNCTION__,__LINE__,bokket::getThreadId(),bokket::getFiberId(),\
                0,std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()) ) )).stream()
//bokket::getFiberId()


#define BOKKET_LOG_DEBUG(logger) BOKKET_LOG_LEVEL(logger,bokket::LogLevel::DEBUG)
#define BOKKET_LOG_INFO(logger) BOKKET_LOG_LEVEL(logger,bokket::LogLevel::INFO)
#define BOKKET_LOG_WARN(logger) BOKKET_LOG_LEVEL(logger,bokket::LogLevel::WARNNING)
#define BOKKET_LOG_ERROR(logger) BOKKET_LOG_LEVEL(logger,bokket::LogLevel::ERROR)
#define BOKKET_LOG_FATAL(logger) BOKKET_LOG_LEVEL(logger,bokket::LogLevel::FATAL)


#define BOKKET_LOG_FMT_LEVEL(logger,level,fmt,...) \
    if(logger->getLevel() <= level )   \
        bokket::LogEventWrap(bokket::LogEvent::ptr std::make_shared<bokket::LogEvent>(logger,level,\
                __FILE__,__FUNCTION__,__LINE__,bokket::threadId(),bokket::Thread::getNowThreadName(),\
                0,time(nullptr)) ).getEvent->formt(fmt,__VA_ARGS__)

//bokket::getFiberId()
#define BOKKET_LOG_FMT_DEBUG(logger,fmt,...) BOKKET_LOG_FMT_LEVEL(logger,bokket::LogLevel::DEBUG,fmt,_VA_ARGS_)
#define BOKKET_LOG_FMT_INFO(logger,fmt,...) BOKKET_LOG_FMT_LEVEL(logger,bokket::LogLevel::INFO,fmt,_VA_ARGS_)
#define BOKKET_LOG_FMT_WARN(logger,fmt,...) BOKKET_LOG_FMT_LEVEL(logger,bokket::LogLevel::WARNNING,fmt,_VA_ARGS_)
#define BOKKET_LOG_FMT_ERROR(logger,fmt,...) BOKKET_LOG_FMT_LEVEL(logger,bokket::LogLevel::ERROR,fmt,_VA_ARGS_)
#define BOKKET_LOG_FMT_FATAL(logger,fmt,...) BOKKET_LOG_FMT_LEVEL(logger,bokket::LogLevel::FATAL,fmt,_VA_ARGS_)


#define BOKKET_LOG_ROOT() bokket::LoggerMgr::GetInstance()->getRoot()

#define BOKKET_LOG_NAME(name) bokket::LoggerMgr::GetInstance()->getLogger()


namespace bokket
{

class Logger;


class LogEvent
{
public:
    using ptr=std::shared_ptr<LogEvent>;

public:
    LogEvent(std::shared_ptr<Logger> logger,LogLevel level
             ,const std::string& filename,const std::string& func,int32_t line
             ,int threadId,uint32_t fiberId
             ,uint32_t elapse,std::time_t time);
             //thread::id threadId
    //uint32_t fiberId
    //const std::string& threadName
    ~LogEvent();

    const std::string& getFilename() const { return filename_; }

    const std::string& getFunc() const { return func_; }

    uint32_t getElapse() const { return elapse_; }

    //thread::id getThreadId() const { return threadId_; }
    int getThreadId() const { return threadId_; }

    uint32_t getFiberId() const { return fiberId_; }

    const std::string& getThreadName() const { return threadName_; }

    int32_t getLine() const { return line_; }

    std::time_t getTime() const { return time_; }

    std::string getContent() const { return stringStream_.str(); }

    LogLevel getLevel() const { return level_; }

    std::shared_ptr<Logger> getLogger() const { return logger_; }

    std::stringstream& getStringStream() { return stringStream_; }
    //bokket::detail::LogStream & steam() { return stream_;}


    void format(const char* fmt,...);
    void format(const char* fmt,va_list al);


    //static std::chrono::system_clock::time_point getTimeNow();

private:

    const std::string loggername_;

    const std::string filename_;
    const std::string func_;
    int32_t line_;

    std::string threadName_;
    //thread::id threadId_;
    int threadId_;
    uint32_t fiberId_;

    uint32_t elapse_;
    std::time_t time_;

    std::stringstream stringStream_;
    //bokket::detail::LogStream stream_;

    std::shared_ptr<Logger> logger_;
    LogLevel level_;
};

class LogEventWrap
{
public:
    LogEventWrap(LogEvent::ptr event);
    ~LogEventWrap();
    LogEvent::ptr getEvent() const { return event_; }
    std::stringstream & stream();

    //std::ostream& stream();
    //bokket::detail::LogStream & stream();
private:
    LogEvent::ptr event_;
};


class LogFormatter
{
public:
    using ptr=std::shared_ptr<LogFormatter>;
public:
    LogFormatter(const std::string& pattern);
public:
    class Impl
    {
    public:
        using ptr=std::shared_ptr<Impl>;
    public:
        virtual ~Impl() {}
        virtual void format(std::ostream& os,LogEvent::ptr event)=0;
    };
public:
    void init();

    std::vector<std::tuple<std::string,std::string,int>>& parse();

    std::string format(LogEvent::ptr event);

    std::ostream& format(std::ostream& ostream,LogEvent::ptr event);

    //void format(const std::string& msg,int32_t len);

    bool getError() const { return error_; }

private:
    std::vector<std::tuple<std::string,std::string,int>> vec_;
    std::vector<Impl::ptr> items_;
    std::string pattern_;
    bool error_=false;
};


class LogAppender
{
public:
    using ptr=std::shared_ptr<LogAppender>;
public:
    virtual ~LogAppender();
    //virtual void append(const std::string& msg,int32_t len,LogLevel level,LogEvent::ptr event)=0;
    virtual void append(std::shared_ptr<Logger> logger,LogLevel level,LogEvent::ptr event)=0;

    //virtual void flush() =0;

    void setFormatter(LogFormatter::ptr val) ;
    void setLevel(LogLevel level) { level_=level; }

    LogFormatter::ptr getFormatter() ;
    LogLevel getLevel() const { return level_; }

    std::mutex& getMutex() { return mutex_; }

    bool getError() { return hasFormatter_; }

    void setError(bool hasFormatter) { hasFormatter_=hasFormatter; } 
protected:
    LogLevel level_;
    LogFormatter::ptr formatter_;
    std::mutex mutex_;
    bool hasFormatter_= false;
};


class Logger: public std::enable_shared_from_this<Logger>
{
    friend class LoggerManager;
public:
    using ptr=std::shared_ptr<Logger>;
public:
    Logger(const std::string& basename="root");
    ~Logger();

    void append(LogLevel level,LogEvent::ptr event);

    void unkown(LogEvent::ptr event);
    void debug(LogEvent::ptr event);
    void info(LogEvent::ptr event);
    void warn(LogEvent::ptr event);
    void error(LogEvent::ptr event);
    void fatal(LogEvent::ptr event);

    /*void addAppender(const std::string& appendername
                     ,LogAppender::ptr appender);

    void delAppender(const std::string& appendername
                     ,LogAppender::ptr appender);*/
    void addAppender(LogAppender::ptr appender);

    void delAppender(LogAppender::ptr appender);

    void clearAppender();

    void setFormatter(LogFormatter::ptr val);

    void setFormatter(const std::string& str);

    LogFormatter::ptr getLogFormatter();

    LogLevel getLevel() const { return level_; }


    //bokket::detail::LogStream &steam() { return stream_; }

    std::string& getBaseName() { return basename_; }



private:
    LogLevel level_;
    //bokket::detail::LogStream stream_;


    //std::string filename_;

    // 日志名称
    std::string basename_;
    std::stringstream  ss_;
    std::mutex mutex_;
    //std::map<std::string,LogAppender::ptr> appenders_;
    std::list<LogAppender::ptr> appenders_;

    LogFormatter::ptr formatter_;

    Logger::ptr root_;
};

class LogAppenderFile: public LogAppender
{
public:
    using ptr=std::shared_ptr<LogAppenderFile>;
public:
    LogAppenderFile(const std::string basename,size_t rollSize,int flushInterval,int check_freq_count,FileWriterType writerType);


    ~LogAppenderFile();

    //void append(const std::string& msg,int32_t len,LogLevel level) override;
    void append(Logger::ptr logger,LogLevel level,LogEvent::ptr event) override;

    void flush();

    bool rollFile();

    //bool reopen();
private:

    void append_unlocked(const std::string& msg,int32_t len);

    const std::string basename_;

    std::ofstream filestream_;
    //bokket::detail::LogStream stream_;


    const size_t rollSize_; // 日志文件达到rolSize_换一个新文件
    const int flushInterval_;
    const int check_freq_count_;

    int count_;
    //std::unique_ptr<std::mutex> mutex_;
    time_t startOfPeriod_;	// 开始记录日志时间（调整至零点的时间）
    time_t lastRoll_;			// 上一次滚动日志文件时间
    time_t lastFlush_;		// 上一次日志写入文件时间

    std::unique_ptr<FileWriter> file_;

    FileWriterType writerType_;
    constexpr static int kRollPerSeconds=60*60*24;
};

class LogAppenderStdout: public LogAppender
{
public:
    using ptr=std::shared_ptr<LogAppenderStdout>;
public:
     //void append(const std::string& msg,int32_t len,LogLevel level,LogEvent::ptr event) override;
     void append(Logger::ptr logger,LogLevel level,LogEvent::ptr event) override;
};

/*
class LogAppenderAsyncFile: public LogAppender
{
public:
    LogAppenderAsyncFile(const std::string& filename);
    ~LogAppenderAsyncFile();

    //void append(const std::string& msg,int32_t line,LogLevel level,LogEvent::ptr event) override;
    void append(shared_ptr<Logger> logger,LogLevel level,LogEvent::ptr event) override;


    void start();
    void stop();

private:
    void threadFunc();
    

    std::atomic_bool started_;

    //time_t ;
    std::string filename_;

    mutable std::mutex mutex_;
    std::thread thread_;
    std::condition_variable conditionVariable_;
    //bokket::net::CountDownLatch latch_;

};*/

/*
class LogAppenderAsyncFile: public LogAppender
{
public:
    LogAppenderAsyncFile();

}*/

class LoggerManager
{
public:
    LoggerManager();

    Logger::ptr getLogger(const std::string& basename);

    Logger::ptr getRoot() const { return root_; }

private:
    std::mutex mutex_;
    std::map<std::string,Logger::ptr> loggers_;
    Logger::ptr root_;
};


using LoggerMgr=bokket::Singleton<LoggerManager>;

}


#endif //BOKKERVE_LOG_H
