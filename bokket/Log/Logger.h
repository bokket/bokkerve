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
#include "../base/noncopyable.h"

namespace bokket
{

enum class LogLevel:int
{
    TRACE,
    DEBUG,
    INFO,
    WARNNING,
    ERROR,
    FATAL
};

class Logger: public std::enable_shared_from_this<Logger>
{
    friend class LoggerManager;
public:
    using ptr=std::shared_ptr<Logger>;
public:
    Logger(const std::string& name="root");

    void log(LogLevel level,Log);

    void
private:
    std::string name_;
    LogLevel level_;
    std::mutex mutex_;



};

class LogAppender
{
public:
    using ptr=std::shared_ptr<LogAppender>;
public:
    virtual ~LogAppender();
    virtual void log(std::shared_ptr<Logger> logger,LogLevel level)=0;

protected:
    LogLevel level_=LogLevel::DEBUG;
    std::mutex mutex_;
};



}


#endif //BOKKET_LOGGER_H
