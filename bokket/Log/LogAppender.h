//
// Created by bokket on 2021/3/8.
//

#ifndef BOKKET_LOGAPPENDER_H
#define BOKKET_LOGAPPENDER_H

#include <string>
#include <memory>
#include <mutex>
#include "LogLevel.h"


namespace bokket
{
class LogAppender
{
public:
    using ptr=std::shared_ptr<LogAppender>;
public:
    virtual ~LogAppender();
    virtual void append(const std::string& msg,int32_t line)=0;
/*
protected:
    LogLevel level=LogLevel::DEBUG;
    std::mutex mutex_;*/
};

}

#endif //BOKKET_LOGAPPENDER_H
