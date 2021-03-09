//
// Created by bokket on 2021/3/7.
//

#include "Logger.h"


namespace bokket
{
Logger::Logger(const std::string &basename)
              :basename_(basename)
              ,level_(LogLevel::DEBUG)
{

}

void Logger::append(LogLevel level, LogEvent::ptr event)
{
    if(level>=level_)
    {
        //return shared_ptr<Logger>(this->_M_weak_this);
        auto self=shared_from_this();
        std::lock_guard<std::mutex> lockGuard(mutex_);

        if(!appenders_.empty())
        {
            for(auto & item : appenders_)
            {
                item->append(self,level,event);
            }
        }
        else if()
    }
}

void Logger::addAppender(const std::string &appendername, LogAppender::ptr appender)
{
    std::lock_guard<std::mutex> lockGuard(mutex_);
    appenders_[appendername]=appender;
}

void Logger::delAppender(const std::string &appendername, LogAppender::ptr appender)
{
    std::lock_guard<std::mutex> lockGuard(mutex_);
    for(auto item=appenders_.begin();item!=appenders_.end();item++)
    {
        if(item)
    }
}

void Logger::clearAppender()
{

}

}