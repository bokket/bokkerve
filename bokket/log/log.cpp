//
// Created by bokket on 2020/12/24.
//

#include "log.h"

#include <map>
#include <iostream>
#include <functional>
#include <time.h>
#include <cstring>

namespace bokket
{
const char * LogLevel::ToString(LogLevel::Level level)
{

}

//
LogEventWrap::LogEventWrap(LogEvent::ptr ptr)
                            :m_event(ptr)
{
}

//
LogEventWrap::~LogEventWrap()
{
    m_event->getLogger()->log(m_event->getLevel(),m_event);

}


stringstream & LogEventWrap::getStringStream()
{
    return m_event->getStringStream();
}

class MessgeFmtItem: public LogFmtter::FmtItem
{
public:
    MessgeFmtItem(const string& str="")
    {}
    void fmt(ostream & os,Logger::ptr logger,LogLevel::Level,LogEvent::ptr event) override
    {
        os<<event->getContent();
    }
};

class LevelFmtItem: public LogFmtter::FmtItem
{
public:
    LevelFmtItem(const string& str="")
    {}
    void fmt(ostream & os,Logger::ptr logger,LogLevel::Level,LogEvent::ptr event) override
    {
        os<<event->getLevel();
    }
};

class MsecFmtItem: public LogFmtter::FmtItem
{
public:
    MsecFmtItem(const string& str="")
    {}
    void fmt(ostream & os,Logger::ptr logger,LogLevel::Level,LogEvent::ptr event) override
    {
        os<<event->getMsec();
    }
};

class NameFmtItem: public LogFmtter::FmtItem
{
public:
    NameFmtItem(const string & str="")
    {}
    void fmt(ostream & os,Logger::ptr logger,LogLevel::Level,LogEvent::ptr event) override
    {
        os<<event->getFile();
    }
};
class ThreadIdFmtItem: public LogFmtter::FmtItem
{
public:
    ThreadIdFmtItem(const string & str="")
    {}
    void fmt(ostream & os,Logger::ptr logger,LogLevel::Level,LogEvent::ptr event) override
    {
        os<<event->getThreadId();
    }
};
class CoreadIdFmtItem: public LogFmtter::FmtItem
{
public:
    CoreadIdFmtItem(const string & str="")
    {}
    void fmt(ostream & os,Logger::ptr logger,LogLevel::Level,LogEvent::ptr event) override
    {
        os<<event->getCoreadId();
    }
};


//%Y-%m-%d
//带世纪部分的十进制年份-十进制的月份-十进制表示的每月的第几天

//%H:%M:%S
//24小时制的小时:十时制表示的分钟:十进制的秒数
class TimeFmtItem: public LogFmtter::FmtItem
{
public:
    TimeFmtItem(const string & fmt="%Y-%m-%d %H:%M:%S")
                :m_string(fmt)
    {
        if(m_string.empty())
            m_string="%Y-%m-%d %H:%M:%S";
    }
    void fmt(ostream & os,Logger::ptr logger,LogLevel::Level,LogEvent::ptr event) override
    {
        struct tm tm;
        time_t time=event->getTime();
        localtime_r(&time,&tm);
        char buf[128];
        strftime(buf,sizeof(buf),m_string.c_str(),&tm);
        os<<buf;
    }
private:
    string m_string;
};

class FilenameFmtItem: public LogFmtter::FmtItem
{
public:
    FilenameFmtItem(const string & str="")
    {}
    void fmt(ostream & os,Logger::ptr logger,LogLevel::Level,LogEvent::ptr event) override
    {
        os<<event->getFile();
    }
};

class LineFmtItem: public LogFmtter::FmtItem
{
public:
    LineFmtItem(const string & str="")
    {}
    void fmt(ostream & os,Logger::ptr logger,LogLevel::Level,LogEvent::ptr event) override
    {
        os<<event->getLine();
    }
};

class NewLineFmtItem: public LogFmtter::FmtItem
{
public:
    NewLineFmtItem(const string & str="")
    {}
    void fmt(ostream & os,Logger::ptr logger,LogLevel::Level,LogEvent::ptr event) override
    {
        os<<endl;
    }
};

class StringFmtItem: public LogFmtter::FmtItem
{
public:
    StringFmtItem(const string & str="")
                :m_string(str)
    {}
    void fmt(ostream & os,Logger::ptr logger,LogLevel::Level,LogEvent::ptr event) override
    {
        os<<m_string;
    }
private:
    string m_string;
};

class TabFmtItem: public LogFmtter::FmtItem
{
public:
    TabFmtItem(const string & str="")
    {}
    void fmt(ostream & os,Logger::ptr logger,LogLevel::Level,LogEvent::ptr event) override
    {
        os<<"\t";
    }
};

void LogEvent::fmt(const char *format, ...)
{
    va_list vl;
    va_start(vl,format);
    fmt(format,vl);
    va_end(vl);
}


void LogEvent::fmt(const char *format, va_list vaList)
{
    char* buf=NULL;
    int len=vasprintf(&buf,format,vaList);
    if(len!=-1)
    {
        m_stream<<string(buf,len);
        free(buf);
    }
}

LogEvent::LogEvent(shared_ptr <Logger> logger, LogLevel::Level level
                   , const char *file, int32_t line, uint32_t msec,
                   uint32_t thread_id, uint32_t coread_id, uint64_t time)
                   :m_logger(logger)
                   ,m_level(level)
                   ,m_file(file)
                   ,m_line(line)
                   ,m_msec(msec)
                   ,m_threadId(thread_id)
                   ,m_coreadId(coread_id)
                   ,m_time(time)
{}
Logger::Logger(const string & name)
            :m_name(name)
            ,m_lever(LogLevel::DEBUG)
            ,m_fmtter()
{
}
void Logger::addPrint(LogPrint::ptr printer)
{
    if(!printer->getFmtter())
        printer->setFmtter(m_fmtter);
    else
    {
        m_printers.push_back(printer);
    }
    
}
void Logger::delPrint(LogPrint::ptr printer)
{
    for(auto it=m_printers.begin();it!=m_printers.end();it++)
    {
        if(*it==printer)
        {
            m_printers.erase(it);
            break;
        }
    }
}
void Logger::log(LogLevel::Level level, LogEvent::ptr event)
{
    if(level>=m_lever)
    {
        auto it=shared_from_this();
        for(auto & i:m_printers)
            i->log(it,level,event);
    }
}
void Logger::debug(LogEvent::ptr event)
{
    log(LogLevel::DEBUG,event);
}
void Logger::info(LogEvent::ptr event)
{
    log(LogLevel::INFO,event);
}
void Logger::warn(LogEvent::ptr event)
{
    log(LogLevel::WARN,event);
}
void Logger::error(LogEvent::ptr event)
{
    log(LogLevel::ERROR,event);
}
void Logger::fatal(LogEvent::ptr event)
{
    log(LogLevel::FATAL,event);
}
LogPrintFile::LogPrintFile(const string &filename)
                            :m_filename(filename)
{
    reopen();
}

void LogPrintFile::log(Logger::ptr logger, LogLevel::Level level, LogEvent::ptr event)
{
    if(level>=m_level)
        m_filestream<<m_fmtter->fmt(logger,level,event);
}

bool LogPrintFile::reopen()
{
    if(m_filestream)
        m_filestream.close();

    m_filestream.open(m_filename,ios::app);

    return !!m_filestream;
}
void LogPrintStdout::log(Logger::ptr logger, LogLevel::Level level, LogEvent::ptr event)
{
    if(level>=m_level)
        cout<<m_fmtter->fmt(logger,level,event);
}


LogFmtter::LogFmtter(const string &pattern)
                    :m_pattern(pattern)
{
    init();
}

string LogFmtter::fmt(shared_ptr <Logger> logger, LogLevel::Level level, LogEvent::ptr event)
{
    stringstream ss;
    for(auto & i:m_items)
    {
        i->fmt(ss,logger,level,event);
    }
    return ss.str();
}


//%xxx %xxx{xxx} %%
//标准的日期串
void LogFmtter::init()
{
    string nstr;


    vector< tuple< string ,string ,int > >vec;

    for(auto i=0;i<m_pattern.size();i++)
    {
        if(m_pattern[i]!='%')
        {
            nstr.append(1,m_pattern[i]);
            continue;
        }
        if(i+1 < m_pattern.size())
        {
            if (m_pattern[i + 1] == '%')
            {
                nstr.append(1, '%');
                continue;
            }
        }


        size_t n=i+1;
        int fmt_status=0;
        size_t fmt_begin=0;

        string str;
        string fmt;
        while( n < m_pattern.size() )
        {
            if(!fmt_status && (!isalpha(m_pattern[n]) &&  m_pattern[n]!='{' && m_pattern[n]!='}' ) )
            {

                str=m_pattern.substr(n,n-i-1);
                break;
            }

            if(fmt_status==0)
            {
                if(m_pattern[n]=='{')
                {
                    str=m_pattern.substr(n,n-i-1);

                    fmt_status=1;
                    fmt_begin=n;
                    ++n;
                    continue;
                }
            }
            else if(fmt_status==1)
            {
                if(m_pattern[n]=='}')
                {
                    fmt=m_pattern.substr(fmt_begin+1,n-fmt_begin-1);
                    fmt_status=0;
                    ++n;
                    break;;
                }
            }
            ++n;
            if(n==m_pattern.size())
            {
                if(!str.empty())
                    str=m_pattern.substr(i+1);
            }
        }

        if(fmt_status==0)
        {
            if(!nstr.empty())
            {
                vec.push_back(make_tuple(nstr,string(),0));
                nstr.clear();
            }
            vec.push_back(make_tuple(str,fmt,1));
            i=n-1;
        }
        else if(fmt_status==1)
        {
            cout<<"pattern parse error: "<<m_pattern<<"-"<<m_pattern.substr(i)<<endl;
            vec.push_back(make_tuple("<<pattern_error>>" , fmt ,0));
        }
    }

    if(!nstr.empty())
        vec.push_back(make_tuple(nstr,"",0));

    



}

LoggerManager::LoggerManager()
{
    m_root.reset(new Logger);
    m_root->addPrint(LogPrint::ptr (new LogPrintStdout));

}

Logger::ptr LoggerManager::getLogger(const string &name)
{
    auto it=m_loggers.find(name);
    if(it==m_loggers.end())
        return m_root;
    else
    {
        return it->second;
    }
}
