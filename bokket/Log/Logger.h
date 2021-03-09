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
#include <list>
#include <map>
#include "LogAppender.h"
#include "LogLevel.h"
#include "LogStream.h"
#include "LogEvent.h"
#include "../base/noncopyable.h"

using namespace bokket;


namespace bokket
{

class LogSteam;
class LogEvent;

class Logger: public std::enable_shared_from_this<Logger>
{
    friend class LoggerManager;
public:
    using ptr=std::shared_ptr<Logger>;
public:
    Logger(const std::string& basename="root");
    ~Logger();

    void append(LogLevel level,LogEvent::ptr event);

    void info(LogEvent::ptr event);
    void debug(LogEvent::ptr event);
    void error(LogEvent::ptr event);
    void fatal(LogEvent::ptr event);
    void warn(LogEvent::ptr event);

    void addAppender(const std::string& appendername
                     ,LogAppender::ptr appender);

    void delAppender(const std::string& appendername
                     ,LogAppender::ptr appender);

    void clearAppender();

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

    Logger::ptr root_;

};


}


#endif //BOKKET_LOGGER_H
