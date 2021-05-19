//
// Created by bokket on 2021/4/7.
//


#include <map>
#include <tuple>
#include <string>
#include <iomanip>
#include <iostream>
#include <functional>

#include <sys/stat.h>

#include "Log.h"
//#include "./FileWriterType.h"


namespace bokket 
{


LogEventWrap::LogEventWrap(LogEvent::ptr event)
                          :event_(event)
{}

LogEventWrap::~LogEventWrap()
{
    event_->getLogger()->append(event_->getLevel(),event_);
}

std::stringstream & LogEventWrap::stream()
{
    return event_->getStringStream();
}

/*
std::ostream & LogEventWrap::stream()
{
    return event_->steam();
}*/

/*bokket::detail::LogStream & LogEventWrap::stream()
{
    return event_->steam();
}*/



Logger::Logger(const std::string &basename)
              :basename_(basename)
              ,level_(LogLevel::DEBUG)
{
    //formatter_.reset(new LogFormatter("%Y-%m-));
}

void Logger::append(LogLevel level, LogEvent::ptr event) 
{
    if(level<=level_) {
        //return shared_ptr<Logger>(this->_M_weak_this);
        auto self = shared_from_this();
        std::lock_guard <std::mutex> lockGuard(mutex_);

        //auto log=self->getLogFormatter()->format(event);

        if(! appenders_.empty()) {
            for(auto &it: appenders_) {
                it->append(self,level,event);
            }
        } else if(root_) {
            //root_->append(level,event);
            root_->
        }
        /*for(const auto & [appendname,append] : appenders_) {
            append->append(self,level,event);
        }*/
       // for (const auto &appender:appenders_) {
       //     appender.second->append(log,log.size());
       // }
        /*
        if(!appenders_.empty())
        {
            for(auto & item : appenders_)
            {
                item->append(self,level,event);
            }
        }
        else if()*/
    }
}

void Logger::unkown(LogEvent::ptr event)
{
    append(LogLevel::UNKNOW,event);
}

void Logger::debug(LogEvent::ptr event)
{
    append(LogLevel::DEBUG,event);
}

void Logger::info(LogEvent::ptr event)
{
    append(LogLevel::INFO,event);
}

void Logger::warn(LogEvent::ptr event)
{
    append(LogLevel::WARNNING,event);
}

void Logger::error(LogEvent::ptr event)
{
    append(LogLevel::ERROR,event);
}

void Logger::fatal(LogEvent::ptr event)
{
    append(LogLevel::FATAL,event);
}







/*
void Logger::addAppender(const std::string &appendername, LogAppender::ptr appender)
{
    std::lock_guard<std::mutex> lockGuard(mutex_);
    if(!appender->getFormatter()) {
        std::lock_guard<std::mutex> lockGuardll(appender->getMutex());
        appender->setFormatter(formatter_);

    }
    appenders_[appendername]=appender;
}*/

/*
void Logger::delAppender(const std::string &appendername, LogAppender::ptr appender)
{
    std::lock_guard<std::mutex> lockGuard(mutex_);*/
    /*for(auto item=appenders_.begin();item!=appenders_.end();)
    {
        if(item->first==appendername) {
            item=appenders_.erase(item);
        } else {
            ++item;
        }
    }*/
    /*
    for(auto& [appendername_,appender_] : appenders_) {
        if(appendername_==appendername) {
            appenders_.erase(appendername);
            break;
        }
    }
}*/

void Logger::addAppender(LogAppender::ptr appender) {
    std::lock_guard<std::mutex> lockGuard(mutex_);
    appenders_.emplace_back(appender);
}

void Logger::delAppender(LogAppender::ptr appender) {
    std::lock_guard<std::mutex> lockGuard(mutex_);
    for(auto it=appenders_.begin();it != appenders_.end();++it) {
        if(*it==appender) {
            appenders_.erase(it);
            break;
        }
    }
}

void Logger::clearAppender()
{
    std::lock_guard<std::mutex> lockGuard(mutex_);
    appenders_.clear();
}


/*LogEvent::LogEvent(std::shared_ptr <Logger> logger, LogLevel level, const std::string &filename,
                   const std::string &func, int32_t line, thread::id threadId,
                   const std::string &threadName, uint32_t elapse, uint64_t time)*/
LogEvent::LogEvent(std::shared_ptr <Logger> logger, LogLevel level, const std::string &filename,
                   const std::string &func, int32_t line, int threadId, uint32_t fiberId, uint32_t elapse,
                   std::time_t time)
                   :logger_(logger)
                   ,level_(level)
                   ,filename_(filename)
                   ,func_(func)
                   ,line_(line)
                   ,threadId_(threadId)
                   ,fiberId_(fiberId)
                   //,threadName_(threadName)
                   ,elapse_(elapse)
                   ,time_(time)
{}

void LogEvent::format(const char *fmt, ...)
{
    if(!fmt) {
        return;
    }
    ::va_list al;
    va_start(al,fmt);
    format(fmt,al);
    va_end(al);
}

void LogEvent::format(const char *fmt, va_list al)
{
    if(!fmt) {
        return;
    }
    char*buf=nullptr;
    int len=::vasprintf(&buf,fmt,al);
    if(len!=-1)
    {
        stringStream_<<std::string(buf,len);
        free(buf);
    }
}

/*std::chrono::system_clock::time_point LogEvent::getTimeNow()
{
    return std::chrono::system_clock::now();
}*/

std::string LogEvent::format(LogEvent::ptr event) {
    //std::stringstream ss;

    std::tm tm;
    std::time_t time=static_cast<time_t>(event->getTime());
    //%Y-%m-%d
    //带世纪部分的十进制年份-十进制的月份-十进制表示的每月的第几天

    //%H:%M:%S
    //24小时制的小时:十时制表示的分钟:十进制的秒数
    //format:yyyy-mm-dd hh:mm::ss
    stringStream_<<std::put_time(::localtime_r(&time,&tm),"%Y-%m-%d %H:%M:%S");


    std::string prefix;

    prefix.append("  ");
    prefix.append(event->getThreadName());

    prefix.append("  ");
    prefix.append(std::to_string(event->getFiberId()));

    prefix.append("  ");
    prefix.append("[");
    prefix.append(bokket::getLogLevelToString(event->getLevel()));
    prefix.append("]");

    prefix.append("  ");
    prefix.append("[");
    prefix.append(event->getLogger()->getBaseName());
    prefix.append("]");

    prefix.append("  ");
    prefix.append(event->getFilename());
    prefix.append(":");
    prefix.append(std::to_string(event->getLine()));

    prefix.append("  ");
    prefix.append(event->getContent());
    //prefix.append("/n");

    /*while(!functors_.empty()) {
        std::function<void()> task = std::move(functors_.front());
        functors_.pop_front();
        task();
    }*/
    std::lock_guard<std::mutex> lockGuard(mutex_);
    stringStream_<<prefix.c_str();
    stringStream_<<std::endl;
    return stringStream_.str();
}

std::ostream & LogEvent::format(std::ostream &ostream, LogEvent::ptr event) {
    std::tm tm;
    std::time_t time=static_cast<time_t>(event->getTime());
    //%Y-%m-%d
    //带世纪部分的十进制年份-十进制的月份-十进制表示的每月的第几天

    //%H:%M:%S
    //24小时制的小时:十时制表示的分钟:十进制的秒数
    //format:yyyy-mm-dd hh:mm::ss
    ostream<<std::put_time(::localtime_r(&time,&tm),"%Y-%m-%d %H:%M:%S");

    std::string prefix;

    prefix.append("  ");
    prefix.append(event->getThreadName());

    prefix.append("  ");
    prefix.append(std::to_string(event->getFiberId()));

    prefix.append("  ");
    prefix.append("[");
    prefix.append(bokket::getLogLevelToString(event->getLevel()));
    prefix.append("]");

    prefix.append("  ");
    prefix.append("[");
    prefix.append(event->getLogger()->getBaseName());
    prefix.append("]");

    prefix.append("  ");
    prefix.append(event->getFilename());
    prefix.append(":");
    prefix.append(std::to_string(event->getLine()));

    prefix.append("  ");
    prefix.append(event->getContent());
    //prefix.append("/n");
    

    /*while(!functors_.empty()) {
        std::function<void()> task = std::move(functors_.front());
        functors_.pop_front();
        task();
    }*/
    std::lock_guard<std::mutex> lockGuard(mutex_);
    ostream<<prefix.c_str();
    ostream<<std::endl;
    return ostream;
}



void LogAppenderStdout::append(Logger::ptr logger, LogLevel level, LogEvent::ptr event)
{
    if(level_<=level) {
        std::lock_guard<std::mutex> lockGuard(mutex_);
        //formatter_->format(std::cout,event);
        //event->format(std::cout,event);
        std::cout<<event->format(event);
    }
}

LogAppenderAsyncFile::LogAppenderAsyncFile(const std::string &filename)
                                          :started_(false),running_(false)
                                          ,persistPeriod_()
                                          ,filename_(filename)
                                          ,latch_(1)
                                          ,persitThread_(std::bind(&LogAppenderAsyncFile::threadFunc,this),"AsyncLogging")
                                          ,curBuffer_(new LogBuffer)
{
    ::mkdir(filename_.c_str(),0755);
    start();
}

LogAppenderAsyncFile::~LogAppenderAsyncFile() noexcept {
    if(started_) {
        stop();
    }
}

void LogAppenderAsyncFile::append(Logger::ptr logger, LogLevel level, LogEvent::ptr event) {
    std::lock_guard<std::mutex> lockGuard(mutex_);


    auto log=logger->getLogEvent()->format(event);
    if(curBuffer_->avail() >= len) {
        curBuffer_->append(log.data(),log.size());
    } else {
        buffers_.emplace_back(std::move(curBuffer_));

        curBuffer_.reset(new LogBuffer);
        curBuffer_->append(log.data(),log.size());

        cv_.notify_one();
    }
}


void LogAppenderAsyncFile::start() {
    started_= true;
    running_= true;
    persitThread_.start();
    latch_.wait();
}


void LogAppenderAsyncFile::stop() {
    started_= false;
    cv_.notify_one();
    persitThread_.join();
}

void LogAppenderAsyncFile::threadFunc() {
    
}

LoggerManager::LoggerManager() {
    root_.reset(new Logger);

    root_->addAppender(LogAppender::ptr (new LogAppenderStdout));

    loggers_[root_->basename_] = root_;
}

Logger::ptr LoggerManager::getLogger(const std::string &basename) {
    std::lock_guard<std::mutex> lockGuard(mutex_);
    auto it = loggers_.find(basename);

    if(it != loggers_.end() ) {
        return it->second;
    }

    Logger::ptr logger(new Logger(basename));
    logger->root_ = root_;
    loggers_[basename] = logger;
    return logger;
}

}

