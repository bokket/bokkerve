//
// Created by bokket on 2021/4/7.
//


#include <map>
#include <tuple>
#include <string>
#include <iomanip>
#include <iostream>
#include <functional>

#include "Log.h"
#include "./FileWriterType.h"


namespace bokket 
{

std::string getHostName()
{
    char buf[256];
    //返回本地主机的标准主机名
    if(::gethostname(buf,sizeof(buf))==0)
    {
        buf[sizeof(buf)-1]='\0';
        return buf;
    }
    else
    {
        return "unknowhost";
    }
}


std::string getLogFileName(const std::string& basename,std::time_t* now)
{
    std::string filename;
    //字符串的容量设置为至少size. 如果size指定的数值要小于当前字符串中的字符数
    // (亦即size<this→size()), 容量将被设置为可以恰好容纳字符的数值.
    filename.reserve(basename.size()+64);

    filename=basename;

    //char timebuf[32];
    std::stringstream ss;
    std::tm tm;
    //gmtime_r转出来的是0时区的标准时间
    //::gmtime_r(now,&tm);


    ss<<filename;
    ss<<getHostName();
    //ss<<std::put_time(::localtime_r(now,&tm),".%04d-%02d-%02d-%02d-%02d-%02d-%06d");
    ss<<std::put_time(::gmtime_r(now,&tm),".%Y-%m-%d %H:%M:%S");
    ss<<".log";

    return ss.str();
    //strftime(timebuf,sizeof(timebuf),".%Y%m%d-%H%M%S.", &tm);
    //filename+=timebuf;


    //filename+=getHostName();

    /*char pidbuf[32];
    snprintf(pidbuf, sizeof pidbuf, ".%d", ::getpid());
    filename += pidbuf;*/

    //filename+=".log";
    //return filename;
}


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
    if(level>=level_) {
        //return shared_ptr<Logger>(this->_M_weak_this);
        auto self = shared_from_this();
        std::lock_guard <std::mutex> lockGuard(mutex_);

        //auto log=self->getLogFormatter()->format(event);

        if(! appenders_.empty()) {
            for(auto &it: appenders_) {
                it->append(self,level,event);
            }
        } else if(root_) {
            root_->append(level,event);
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
    std::stringstream ss;

    std::tm tm;
    std::time_t time=static_cast<time_t>(event->getTime());
    //%Y-%m-%d
    //带世纪部分的十进制年份-十进制的月份-十进制表示的每月的第几天

    //%H:%M:%S
    //24小时制的小时:十时制表示的分钟:十进制的秒数
    //format:yyyy-mm-dd hh:mm::ss
    ss<<std::put_time(::localtime_r(&time,&tm),"%Y-%m-%d %H:%M:%S");


    std::string prefix;

    prefix.append("/t");
    prefix.append(event->getThreadName());

    prefix.append("/t");
    prefix.append(std::to_string(event->getFiberId()));

    prefix.append("/t");
    prefix.append("[");
    prefix.append(bokket::getLogLevelToString(event->getLevel()));
    prefix.append("]");

    prefix.append("/t");
    prefix.append("[");
    prefix.append(event->getLogger()->getBaseName());
    prefix.append("]");

    prefix.append("/t");
    prefix.append(event->getFilename());
    prefix.append(":");
    prefix.append(std::to_string(event->getLine()));

    prefix.append("/t");
    prefix.append(event->getContent());
    prefix.append("/n");

    while(!functors_.empty()) {
        std::function<void()> task = std::move(functors_.front());
        functors_.pop_front();
        task();
    }
    std::lock_guard<std::mutex> lockGuard(mutex_);
    ss<<prefix.c_str();
    return ss.str();
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
    

    while(!functors_.empty()) {
        std::function<void()> task = std::move(functors_.front());
        functors_.pop_front();
        task();
    }
    std::lock_guard<std::mutex> lockGuard(mutex_);
    ostream<<prefix.c_str();
    return ostream;
}


LogAppenderFile::LogAppenderFile(const std::string basename, size_t rollSize, int flushInterval, int check_freq_count,
                                 FileWriterType writerType)
                                 :basename_(basename),rollSize_(rollSize),flushInterval_(flushInterval),check_freq_count_(check_freq_count)
                                 ,count_(0),startOfPeriod_(0),lastRoll_(0),lastFlush_(0)
                                 //,stream_()
                                 //,mutex_(std::make_unique<std::mutex>())
{
    /*auto now=std::chrono::system_clock::now();
    time_t time=std::chrono::system_clock::to_time_t(now);
    std::string filename=getLogFileName(basename_,&time);*/
    //time_t time=static_cast<time_t>(event->getTime());
    std::time_t time=std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::string filename=getLogFileName(basename_,&time);

    if(writerType==FileWriterType::MMAPFILE)
        file_=std::make_unique<MmapFileWrite>(filename,rollSize_);
    else
        file_=std::make_unique<AppendFileWriter>(filename);

    writerType_=writerType;

    rollFile();
}


void LogAppenderFile::append(Logger::ptr logger, LogLevel level, LogEvent::ptr event)
{
    /*if(mutex_)
    {
        //std::unique_lock<std::mutex> lock(*mutex_);
        append_unlocked(msg,len);
    }*/
    //LogAppender的锁
    std::lock_guard<std::mutex> uniqueLock(mutex_);
    //formatter_->format(filestream_,event);
    auto log=logger->getLogEvent()->format(event);
    //auto log=logger->getLogFormatter()->format(event);
    append_unlocked(log.data(),log.size());
    
}

void LogAppenderFile::append_unlocked(const std::string &msg, int32_t len)
{
    file_->append(msg,len);

    if(file_->getWrittenBytes() > rollSize_) {
        rollFile();
    } else {
        ++count_;
        //隔多久需要检查一下
        if(count_ >= check_freq_count_) {
            count_=0;
            //time_t now = ::time(nullptr);
            std::time_t now=std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
            time_t thisPeriod=now/kRollPerSeconds*kRollPerSeconds;
            if(thisPeriod!=startOfPeriod_) {
                rollFile();
            } else if(now-lastFlush_ > flushInterval_) {
                lastFlush_=now;
                file_->flush();
            }
        }
    }
}


void LogAppenderFile::flush()
{
    /*if(mutex_)
    {
        std::unique_lock<std::mutex> lock(*mutex_);
        file_->flush();
    }*/
    std::lock_guard<std::mutex> uniqueLock(mutex_);
    file_->flush();
}

bool LogAppenderFile::rollFile()
{

    //time_t now=static_cast<time_t>(event->getTime());
    time_t now=0;
    std::string filename=getLogFileName(basename_,&now);

    time_t start=now/kRollPerSeconds*kRollPerSeconds;

    if(now>lastRoll_)
    {
        lastRoll_=now;
        lastFlush_=now;
        startOfPeriod_=start;

        if(writerType_==FileWriterType::APPENDFILE)
            file_.reset(new AppendFileWriter(filename));
        else
            file_.reset(new MmapFileWrite(filename,rollSize_));
        return true;
    }
    return false;
}




void LogAppenderStdout::append(Logger::ptr logger, LogLevel level, LogEvent::ptr event)
{
    if(level_<=level) {
        std::lock_guard<std::mutex> lockGuard(mutex_);
        //formatter_->format(std::cout,event);
        event->format(std::cout,event);
    }
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

    Logger::ptr logger=std::make_shared<Logger>(basename);
    logger->root_ = root_;
    loggers_[basename] = logger;
    return logger;
}

}

