//
// Created by bokket on 2021/3/7.
//

#ifndef BOKKET_LOGGER_H
#define BOKKET_LOGGER_H


#include <string>
#include <sstream>
#include <memory>
#include <mutex>
#include <thread>
#include <sstream>
#include <list>
#include "LogAppender.h"
#include "LogLevel.h"
#include "LogStream.h"
#include "LogEvent.h"
#include "../base/noncopyable.h"
#include "../net/Timestamp.h"

using namespace bokket;
using namespace bokket::detail;

namespace bokket
{

class LogSteam;

class Logger: public std::enable_shared_from_this<Logger>
{
    friend class LoggerManager;
public:
    using ptr=std::shared_ptr<Logger>;
public:
    Logger(const std::string& name="root");
    ~Logger();

    void append(LogLevel level,LogEvent::ptr event);

    void info(LogEvent::ptr);
    void debug(LogEvent::ptr);
    void error(LogEvent::ptr);
    void fatal(LogEvent::ptr);
    void warn(LogEvent::ptr);

    void addAppender(const std::string& appendername
                     ,LogAppender::ptr appender);

    void delAppender(const std::string& appendername
                     ,LogAppender::ptr appender);

    void clearAppender();

    LogLevel getLogLevel() const;


    LogSteam &steam() { return stream_; }


private:
    const char* logLevelToString(LogLevel level) const;
    LogLevel FromString(const std::string & str) const;


    LogLevel level_;
    LogStream stream_;


    //std::string filename_;

    // 日志名称
    std::string basename_;
    std::stringstream  ss_;
    std::mutex mutex_;
    std::map<std::string,LogAppender::ptr> appenders_;

    Logger::ptr root_;

};


}


#endif //BOKKET_LOGGER_H
