//
// Created by bokket on 2021/3/8.
//

#ifndef BOKKET_LOGLEVEL_H
#define BOKKET_LOGLEVEL_H

#include <stdint.h>
#include <string>

namespace bokket
{

enum class LogLevel:uint8_t
{
    UNKNOW,
    DEBUG,
    INFO,
    WARNNING,
    ERROR,
    FATAL
};

/*
    static const char* ToString(LogLevel::Level level);

    static LogLevel::Level FromString(const std::string & str);

    static void setLogLevel(LogLevel::Level level)
    {
        g_logLevel=level;
    }

    LogLevel::Level getLogLevel();

    void initLogLevel();*/

}

#endif //BOKKET_LOGLEVEL_H
