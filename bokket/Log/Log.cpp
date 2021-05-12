//
// Created by bokket on 2021/4/7.
//

#include "Log.h"
#include <iostream>
#include <string>
#include <map>
#include <functional>
#include <iomanip>
#include <tuple>


namespace bokket {

LogEventWrap::LogEventWrap(LogEvent::ptr event)
                          :event_(event)
{}

LogEventWrap::~LogEventWrap()
{
    event_->getLogger()->append(event_->getLevel(),event_);
}

std::stringstream & LogEventWrap::getSS()
{
    return event_->getStringStream();
}

std::ostream & LogEventWrap::stream()
{
    return event_->steam();
}

/*bokket::detail::LogStream & LogEventWrap::stream()
{
    return event_->steam();
}*/



Logger::Logger(const std::string &basename)
              :basename_(basename)
              ,level_(LogLevel::DEBUG)
{
    formatter_.reset(new LogFormatter("%d{%Y-%m-%d %H:%M:%S}%T%t%T%N%T%F%T[%p]%T[%c]%T%f:%l%T%m%n"));
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





LogFormatter::ptr Logger::getLogFormatter() 
{
    std::lock_guard<std::mutex> lockGuard(mutex_);
    return formatter_;
}

void Logger::setFormatter(LogFormatter::ptr val)
{
    std::lock_guard<std::mutex> lockGuard(mutex_);
    formatter_=val;

    //for(auto &[appendname,appender] : appenders_) {
    for(auto& it:appenders_) {
        std::lock_guard<std::mutex> lockGuardll(it->getMutex());
        //如果appender没有formatter
        if(!it->getError()) {
            it->setFormatter(formatter_);
        }
    }
}

void Logger::setFormatter(const std::string &str)
{
    std::cout<<"[Logger::setFotmatter]:"<<std::endl;

    LogFormatter::ptr new_formatter=std::make_shared<LogFormatter>(str);

    //是否能设置成功
    if(new_formatter->getError()) {
        std::cout << "Logger name" << basename_
                  << " str=" << str << "set formatter failed"
                  << std::endl;
        return ;
    }
    setFormatter(new_formatter);
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
    if(!appender->getFormatter()) {
        std::lock_guard<std::mutex> lockGuardll(appender->getMutex());
        appender->setFormatter(formatter_);
    }
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
                   const std::string &func, int32_t line, int threadId, const std::string &threadName, uint32_t elapse,
                   std::time_t time)
                   :logger_(logger)
                   ,level_(level)
                   ,filename_(filename)
                   ,func_(func)
                   ,line_(line)
                   ,threadId_(threadId)
                   //,fiberId_(fiberId)
                   ,threadName_(threadName)
                   ,elapse_(elapse)
                   ,time_(time)
{}

void LogEvent::format(const char *fmt, ...)
{
    ::va_list al;
    va_start(al,fmt);
    format(fmt,al);
    va_end(al);
}

void LogEvent::format(const char *fmt, va_list al)
{
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


class MessageFormatImpl: public LogFormatter::Impl
{
public:
    MessageFormatImpl(const std::string& str="")
    {}

    void format(std::ostream& os,LogEvent::ptr event) override
    {
        os<<event->getContent();
    }
};


class LevelFormatImpl: public LogFormatter::Impl
{
public:
    LevelFormatImpl(const std::string& str="")
    {}
    void format(std::ostream& os,LogEvent::ptr event) override
    {
        os<<bokket::getLogLevelToString(event->getLevel());
    }
};


class ElapseFormatImpl: public LogFormatter::Impl
{
public:
    ElapseFormatImpl(const std::string& str="")
    {}
    
    void format(std::ostream& os,LogEvent::ptr event) override
    {
        os<<event->getElapse();
    }
};


class ThreadIdFromatImply: public LogFormatter::Impl
{
public:
    ThreadIdFromatImply(const std::string& str="")
    {}

    void format(std::ostream& os,LogEvent::ptr event) override
    {
        os<<event->getThreadId();
    }
};


class FiberIdFormatImply: public LogFormatter::Impl
{
public:
    FiberIdFormatImply(const std::string& str="")
    {}
    void format(std::ostream& os,LogEvent::ptr event) override
    {
        os<<event->getFiberId();
    }
};


class ThreadNameFormatImply: public LogFormatter::Impl
{
public:
    ThreadNameFormatImply(const std::string& str="")
    {}
    void format(std::ostream& os,LogEvent::ptr event) override
    {
        os<<event->getThreadName();
    }
};

class BaseNameFormatImpl: public LogFormatter::Impl
{
public:
    BaseNameFormatImpl(const std::string& str="")
    {}

    void format(std::ostream& os,LogEvent::ptr event) override
    {
        os<<event->getLogger()->getBaseName();
    }
};


class FileNameFormatImpl: public LogFormatter::Impl
{
public:
    FileNameFormatImpl(const std::string& str="")
    {}

    void format(std::ostream& os,LogEvent::ptr event) override
    {
        os<<event->getFilename();
    }
};

class DateTimeFormatImpl: public LogFormatter::Impl
{
public:
    //%Y-%m-%d
    //带世纪部分的十进制年份-十进制的月份-十进制表示的每月的第几天

    //%H:%M:%S
    //24小时制的小时:十时制表示的分钟:十进制的秒数
    //format:yyyy-mm-dd hh:mm::ss
    DateTimeFormatImpl(const string& str="%Y-%m-%d %H:%M:%S")
                      :format_(str)
    {
        if(format_.empty())
        {
            format_="%Y-%m-%d %H:%M:%S";
        }
    }
    //void formatByStrf(std::ostream& os,LogEvent::ptr event)
    void format(std::ostream& os,LogEvent::ptr event) override
    {
        std::tm tm;
        time_t time=static_cast<time_t>(event->getTime());

        if(format_.compare("%04d-%02d-%02d-%02d-%02d-%02d-%06d"))
        {
            auto now=event->getTime();


            /*uint64_t microseconds=std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count()
                                  -std::chrono::duration_cast<std::chrono::seconds>(now.time_since_epoch()).count()*1000;*/

            os<<std::put_time(::localtime_r(&time,&tm),"%04d-%02d-%02d-%02d-%02d-%02d-%06d");
        }


        ::localtime_r(&time,&tm);
        
        //os<<std::put_time(::localtime_r(&time,&tm),format_.c_str());

        char buf[64]={0};
        ::strftime(buf,sizeof(buf),format_.c_str(),&tm);
        os<<string(buf);
    }
    /*
    void format(std::ostream& os,LogEvent::ptr event) override
    {
        //now() 当前时间time_point
        //to_time_t() time_point转换成time_t秒
        //auto now=std::chrono::system_clock::now();
        //auto now_time_t=std::chrono::system_clock::to_time_t(now);
        std::tm tm;
        time_t time=static_cast<time_t>(event->getTime());

        //::localtime_r(&time,&tm);

        auto now=event->getTimeNow();


        uint64_t microseconds=std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count()
                              -std::chrono::duration_cast<std::chrono::seconds>(now.time_since_epoch()).count()*1000;

        os<<std::put_time(::localtime_r(&time,&tm),"%04d-%02d-%02d-%02d-%02d-%02d-%06d");

        char buf[32]={0};
        snprintf(buf,sizeof(buf),"%04d-%02d-%02d-%02d-%02d-%02d-%06d", 1900 + tm.tm_year,
                 1 + tm.tm_mon, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec,static_cast<int>(microseconds));

        stream<<buf;*/
    //}

private:
    std::string format_;
};


class FunctionFormatImpl: public LogFormatter::Impl
{
public:
    FunctionFormatImpl(const std::string& str="")
    {}
    void format(std::ostream& os,LogEvent::ptr event) override
    {
        os<<event->getFunc();
    }
};

class LineFormatImpl: public LogFormatter::Impl
{
public:
    LineFormatImpl(const std::string& str="")
    {}
    void format(std::ostream& os,LogEvent::ptr event) override
    {
        os<<event->getLine();
    }
};


class NewLineFormatImpl: public LogFormatter::Impl
{
public:
    NewLineFormatImpl(const std::string& str = "")
    {}
    void format(std::ostream& os,LogEvent::ptr event) override
    {
        os<<std::endl;
    }
};

class StringFormatImpl: public LogFormatter::Impl
{
public:
    StringFormatImpl(const std::string& str)
                     :str_(str)
    {}
    void format(std::ostream& os,LogEvent::ptr event) override
    {
        os<<str_;
    }
private:
    std::string str_;
};


class TabFormatImpl : public LogFormatter::Impl
{
public:
    TabFormatImpl(const std::string& str = "")
    {}
    void format(std::ostream& os,LogEvent::ptr event) override
    {
        os << "\t";
    }
};

std::vector<std::tuple<std::string,std::string,int>>& LogFormatter::parse()
{
    enum class parse:uint8_t {
        INIT,
        ITEM, //%
        FORM, //{
        NORM, //[
        TORM, //:
    };
    //%d{%Y-%m-%d %H:%M:%S}%T%t%T%F%T[%p]%T[%c]%T%f:%l%T%m%n
    parse cur=parse::INIT;


    std::string item;
    std::string format;
    std::string::size_type begin;
    std::string::size_type end;
    
    for(auto i=0;i<pattern_.size();i++)
    {
        item=pattern_[i];
        format="";
        if(error_) {
            vec_.emplace_back(std::make_tuple("<<pattern_error>>",format,0));
            break;
        }
        switch(pattern_[i])
        {
            case '{':
                begin=i;
                //{}
                if(pattern_[i+1]=='}')
                {
                    continue;
                }
                //{%s%s%s}
                //|      |
                //|      |
                //begsin end
                while(pattern_[i]!='}') {                      
                    if(pattern_[i]=='['||pattern_[i]==']'){
                        error_=true;
                        break;
                    }
                    i++;
                }
                if(error_) {
                    break;
                }
                item="";
                end=i;
                format=pattern_.substr(begin+1,end-begin-1);    
                vec_.emplace_back(std::make_tuple(item,format,1));
                break;
            case '%':
                item=pattern_[++i];
                vec_.emplace_back(std::make_tuple(item,format,1));
                break;
            case '[':
            //[%p]
                vec_.emplace_back(std::make_tuple(item,format,0));             
                break;
            case ']':
                vec_.emplace_back(std::make_tuple(item,format,0));
                break;
            default:
                break;
        }
    }

    /*
    for(auto i=0;i<pattern_.size();i++)
    {
        switch (cur) {
            case parse::INIT:
                if(pattern_[i]=='%') {
                    cur=parse::ITEM;
                }
                else if(pattern_[i]=='{') {
                    cur= parse::FORM;
                }
                else {
                    cur=parse::INIT;
                }
                break;
            case parse::ITEM:
                if(pattern_[i]) {
                    if(item.empty()) {
                        item=pattern_[i];
                        if(i+1<pattern_.size() && pattern_[i+1] != '{') {
                            vec.emplace_back(std::make_tuple(item,format,1));
                            item="";
                            format="";
                            cur=parse::INIT;
                        } else if(i+1<pattern_.size() && pattern_[i+1]=='{') {
                            begin=i+1;
                            cur=parse::FORM;
                        } else if(i==pattern_.size()-1) {
                            vec.emplace_back(std::make_tuple(item,format,1));
                            item="";
                            format="";
                            cur=parse::INIT;
                        } else if(i+1 < pattern_.size() && pattern_[i+1]== '[' ) {
                            begin=i+1;
                            cur=parse::NORM;
                        } else if(i+1<pattern_.size() && pattern_[i+1] != '[') {
                            vec.emplace_back(std::make_tuple(item, format, 1));
                            item = "";
                            format = "";
                            cur = parse::INIT;
                        } else if(i+1< pattern_.size() && pattern_[i+1] == ':') {
                            begin=i+1;
                            cur=parse::TORM;
                        } else if(i+1< pattern_.size() && pattern_[i+1] != ':') {
                            vec.emplace_back(std::make_tuple(item, format, 1));
                            item = "";
                            format = "";
                            cur = parse::INIT;
                        }
                    }
                }
                break;
            case parse::FORM:
                if(pattern_[i]=='}') {
                    end=i;
                    format=pattern_.substr(begin+1,end-begin-1);
                    vec.emplace_back(std::make_tuple(item,format,1));
                    item="";
                    format="";
                    cur=parse::INIT;
                }
                break;
            case parse::NORM:
                if(pattern_[i]==']') {
                    end=i;
                    format=pattern_.substr(begin+1,end-begin-1);
                    vec.emplace_back(std::make_tuple(item,format,1));
                    item="";
                    format="";
                    cur=parse::INIT;
                }
            case parse::TORM:
                end=i;
                format=pattern_.substr(end,end-begin+1);
                vec.emplace_back(std::make_tuple(item,format,1));
                item="";
                format="";
                cur=parse::INIT;
            default:
                break;
        }
    }*/
    return vec_;
}


void LogFormatter::init()
{

    static std::map<std::string,std::function<Impl::ptr(const std::string& str)>> format_items= {
#define XX(str,C) \
    { #str,[](const std::string& fmt) { return Impl::ptr(std::make_shared<C>(fmt)); } }

            XX(m,MessageFormatImpl),    //  - %m  -- 日志消息
            XX(p,LevelFormatImpl),      //  - %p  -- 日志级别
            XX(r,ElapseFormatImpl),     //  - %r  -- 启动后的毫秒数
            XX(c,BaseNameFormatImpl),   //  - %c  -- 日志名称
            XX(t,ThreadIdFromatImply),  //  - %t  -- 线程id
            XX(n,NewLineFormatImpl),    //  - %n  -- 回车换行
            XX(d,DateTimeFormatImpl),   //  - %d  -- 时间格式
            XX(f,FileNameFormatImpl),   //  - %f  -- 文件名
            XX(l,LineFormatImpl),       //  - %l  -- 行号
            XX(T,TabFormatImpl),        //  - %T  -- tab
            XX(F,FiberIdFormatImply),   //  - %F  -- 协程id
            XX(N,ThreadNameFormatImply),//  - %N  -- 线程名称
#undef XX
    };
    //%d{%Y-%m-%d %H:%M:%S}%T%t%T%F%T[%p]%T[%c]%T%f:%l%T%m%n

    auto vec=LogFormatter::parse();
    for(auto &i:vec) {
        //std::vector<std::tuple<std::string, std::string, int>>

        //"[" ""  0
        //"]" ""  0
        //":" ""  0
        if(std::get<2>(i)==0) {
            //items_.emplace_back(Impl::ptr(new StringFormatImpl(std::get<0>(i))));
            items_.emplace_back(Impl::ptr(std::make_shared<StringFormatImpl>(std::get<0>(i))));
        } else {
            auto it=format_items.find(std::get<0>(i));
            //"d" "" 1
            //"" "%Y-%m-%d %H:%M:%S" 1
            //"T" "" 1
            //"t" "" 1
            //"F" "" 1
            if(it!=format_items.end()) {
                //get map<string,function>
                //return Impl::ptr(new it->second(get<1>(1)))
                // get<1>(1)
                // return string
                items_.emplace_back(it->second(std::get<1>(i)));
            } else {
                //items_.emplace_back(Impl::ptr (new StringFormatImpl("<<error format %"+std::get<0>(i)+">>")));
                items_.emplace_back(Impl::ptr(std::make_shared<StringFormatImpl>("<<error format %"+std::get<0>(i)+">>")));
            }
        }
    }
    /*items_.emplace_back(Impl::ptr(new StringFormatImpl("[")));
    items_.emplace_back(Impl::ptr(new DateTimeFormatImpl(pattern_)));
    items_.emplace_back(Impl::ptr(new StringFormatImpl("]")));
    items_.emplace_back(Impl::ptr(new ThreadIdFromatImply()));*/
}

LogFormatter::LogFormatter(const std::string &pattern)
                          :pattern_(pattern)
                          ,error_(false)
{
    init();
}


std::string LogFormatter::format(LogEvent::ptr event)
{
    std::stringstream ss;
    for(auto &i:items_) {
        i->format(ss,event);
    }
    return ss.str();
}

std::ostream & LogFormatter::format(std::ostream &ostream, LogEvent::ptr event)
{
    for(auto& i:items_) {
        i->format(ostream,event);
    }
    return ostream;
}


void LogAppender::setFormatter(LogFormatter::ptr val)
{
    std::lock_guard<std::mutex> lockGuard(mutex_);
    formatter_=val;
    if(formatter_) {
        hasFormatter_=true;
    } else {
        hasFormatter_= false;
    }
}


LogFormatter::ptr LogAppender::getFormatter() 
{
    std::lock_guard<std::mutex> lockGuard(mutex_);
    return formatter_;
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
    time_t time=0;
    std::string filename=getLogFileName(basename_,&time);

    if(writerType==FileWriterType::MMAPFILE)
        file_=std::make_unique<MmapFileWrite>(filename,rollSize_);
    else
        file_=std::make_unique<AppendFileWriter>(filename);

    writerType_=writerType;

    rollFile();
}


void LogAppenderFile::append(shared_ptr <Logger> logger, LogLevel level, LogEvent::ptr event)
{
    /*if(mutex_)
    {
        //std::unique_lock<std::mutex> lock(*mutex_);
        append_unlocked(msg,len);
    }*/
    //LogAppender的锁
    std::lock_guard<std::mutex> uniqueLock(mutex_);
    //formatter_->format(filestream_,event);
    auto log=logger->getLogFormatter()->format(event);
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
            time_t now = ::time(nullptr);
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

void LogAppenderStdout::append(shared_ptr <Logger> logger, LogLevel level, LogEvent::ptr event)
{
    if(level_>=level) {
        std::lock_guard<std::mutex> lockGuard(mutex_);
        formatter_->format(std::cout,event);
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

