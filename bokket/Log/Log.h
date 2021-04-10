//
// Created by bokket on 2021/4/7.
//

#ifndef BOKKERVE_LOG_H
#define BOKKERVE_LOG_H

#include <string>
#include <sstream>
#include <memory>
#include <mutex>
#include <thread>
#include <list>
#include <map>
#include <vector>
#include <condition_variable>
#include "./LogLevel.h"
#include "./LogStream.h"
#include "./FileWriterType.h"
#include "../base/noncopyable.h"
#include "../base/SpinLock.h"


namespace bokket
{

class Logger;


class LogEvent
{
public:
    using ptr=std::shared_ptr<LogEvent>;

public:
    LogEvent(std::shared_ptr<Logger> logger,LogLevel level
             ,const std::string& filename,const std::string& func
             ,thread::id threadId,uint32_t fiber_id,const std::string& threadName
             ,int32_t line,uint32_t elapse,uint64_t time);
    ~LogEvent();

    const std::string& getFilename() const { return filename_; }

    const std::string& getFunc() const { return func_; }

    uint32_t getElapse() const { return elapse_; }

    thread::id getThreadId() const { return threadId_; }

    uint32_t getFiberId() const { return fiberId_; }

    const std::string& getThreadName() const { return threadName_; }

    int32_t getLine() const { return line_; }

    uint64_t getTime() const { return time_; }

    std::string getContent() const { return stringStream_.str(); }

    LogLevel getLevel() const { return level_; }

    std::shared_ptr<Logger> getLogger() const { return logger_; }

    std::stringstream& getStringStream() { return stringStream_; }

    void format(const char* fmt,...);
    void format(const char* fmt,va_list al);


    static std::chrono::system_clock::time_point getTimeNow();

private:

    const std::string loggername_;

    const std::string filename_;
    const std::string func_;
    int32_t line_;

    std::string threadName_;
    thread::id threadId_;
    uint32_t fiberId_;

    uint32_t elapse_;
    uint64_t time_;

    std::stringstream stringStream_;

    std::shared_ptr<Logger> logger_;
    LogLevel level_;
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
    virtual void append(shared_ptr<Logger> logger,LogLevel level,LogEvent::ptr event)=0;

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

    void addAppender(const std::string& appendername
                     ,LogAppender::ptr appender);

    void delAppender(const std::string& appendername
                     ,LogAppender::ptr appender);

    void clearAppender();

    void setFormatter(LogFormatter::ptr val);

    void setFormatter(const std::string& str);

    LogFormatter::ptr getLogFormatter();

    LogLevel getLogLevel() const { return level_; }


    bokket::detail::LogStream &steam() { return stream_; }

    std::string& getBaseName() { return basename_; }



private:
    LogLevel level_;
    bokket::detail::LogStream stream_;


    //std::string filename_;

    // 日志名称
    std::string basename_;
    std::stringstream  ss_;
    std::mutex mutex_;
    std::map<std::string,LogAppender::ptr> appenders_;

    LogFormatter::ptr formatter_;

    //Logger::ptr root_;
};

class LogAppenderFile: public LogAppender
{
public:
    using ptr=std::shared_ptr<LogAppenderFile>;
public:
    LogAppenderFile(const std::string basename,size_t rollSize,int flushInterval,int check_freq_count,FileWriterType writerType);


    ~LogAppenderFile();

    //void append(const std::string& msg,int32_t len,LogLevel level) override;
    void append(shared_ptr<Logger> logger,LogLevel level,LogEvent::ptr event) override;

    void flush();

    bool rollFile();
private:

    void append_unlocked(const std::string& msg,int32_t len);

    const std::string basename_;

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
     void append(shared_ptr<Logger> logger,LogLevel level,LogEvent::ptr event) override;
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


}


#endif //BOKKERVE_LOG_H
