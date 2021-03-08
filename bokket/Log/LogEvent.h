//
// Created by bokket on 2021/3/8.
//

#ifndef BOKKET_LOGEVENT_H
#define BOKKET_LOGEVENT_H

#include <memory>
#include <chrono>
#include <string>
#include <sstream>
#include <thread>
#include "Logger.h"
#include "LogStream.h"

namespace bokket
{

class LogStream;
class LogEvent
{
public:
    using ptr=std::shared_ptr<LogEvent>;

public:
    LogEvent(std::shared_ptr<Logger> logger,LogLevel level
             ,const std::string& filename,int32_t line
             ,uint32_t sec_,uint64_t time_);
    ~LogEvent();

    const std::string& getFilename() const { return filename_; }

    int32_t getLine() const { return line_; }

    uint32_t getTime() const { return time_; }

    std::string getContent() const { return stringStream_.str(); }

    LogLevel getLevel() const { return level_; }

    std::shared_ptr<Logger> getLogger() const { return logger_; }

    std::stringstream& getStringStream() { return stringStream_; }

    void format(const char* fmt,...);
    void format(const char* fmt,va_list al);

private:
    const std::string& filename_;
    int32_t line_;
    //uint32_t sec_;
    uint64_t time_;



    std::stringstream stringStream_;

    std::shared_ptr<Logger> logger_;
    LogLevel level_;
};

}
#endif //BOKKET_LOGEVENT_H
