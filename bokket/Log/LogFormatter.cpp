//
// Created by bokket on 2021/3/8.
//

#include "LogFormatter.h"
#include "LogEvent.h"
#include "Logger.h"

class MessageFormatImpl: public LogFormatter::Impl
{
public:
    MessageFormatImpl(const std::string& str="")
    {}

    void format(std::ostream& stream,LogEvent::ptr event) override
    {
        stream<<event->getContent();
    }
};


class LevelFormatImpl: public LogFormatter::Impl
{
public:
    LevelFormatImpl(const std::string& str="")
    {}
    void format(std::ostream& stream,LogEvent::ptr event) override
    {
        stream<<bokket::getLogLevelToString(event->getLevel());
    }
};

class FileNameFormatImpl: public LogFormatter::Impl
{
public:
    FileNameFormatImpl(const std::string& str="")
    {}

    void format(std::ostream& stream,LogEvent::ptr event) override
    {
        stream<<event->getFilename();
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
    void formatByStrf(std::ostream& stream,LogEvent::ptr event)
    {
        std::tm tm;
        time_t time=static_cast<time_t>(event->getTime());
        ::localtime_r(&time,&tm);

        char buf[128]={0};
        strftime(buf,sizeof(buf),format_.c_str(),&tm);
        stream<<buf;
    }
    void format(std::ostream& stream,LogEvent::ptr event) override
    {
        //now() 当前时间time_point
        //to_time_t() time_point转换成time_t秒
        //auto now=std::chrono::system_clock::now();
        //auto now_time_t=std::chrono::system_clock::to_time_t(now);
        std::tm tm;
        time_t time=static_cast<time_t>(event->getTime());

        ::localtime_r(&time,&tm);

        auto now=event->getTimeNow();


        uint64_t microseconds=std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count()
                              -std::chrono::duration_cast<std::chrono::seconds>(now.time_since_epoch()).count()*1000;

        localtime_r(&time,&tm);

        char buf[32]={0};
        snprintf(buf,sizeof(buf),"%04d-%02d-%02d-%02d-%02d-%02d-%06d", 1900 + tm.tm_year,
                 1 + tm.tm_mon, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec,static_cast<int>(microseconds));

        stream<<buf;
    }

private:
    std::string format_;
};


class FunctionFormatImpl: public LogFormatter::Impl
{
public:
    FunctionFormatImpl(const std::string="")
    {}
    void format(std::ostream& stream,LogEvent::ptr event) override
    {
        stream<<event->getFunc();
    }
};

class LogNameFormatImpl: public LogFormatter::Impl
{
public:
    LogNameFormatImpl(const std::string="")
    {}
    void format(std::ostream& stream,LogEvent::ptr event) override
    {
        stream<<event->getLogger()->getBaseName();
    }
};

class LineFormatImpl: public LogFormatter::Impl
{
public:
    LineFormatImpl(const std::string="")
    {}
    void format(std::ostream& stream,LogEvent::ptr event) override
    {
        stream<<event->getLine();
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

