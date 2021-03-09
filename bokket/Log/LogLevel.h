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

const std::string getLogLevelToString(LogLevel level) 
{
    switch(level) {
#define XX(name) \
    case LogLevel::name: \
        return #name; \
        break;

        XX(DEBUG);
        XX(INFO);
        XX(WARNNING);
        XX(ERROR);
        XX(FATAL);
#undef XX
        default:
            return "UNKNOW";
    }
    return "UNKNOW";
}

LogLevel logLevelFromString(const std::string &str) 
{
#define XX(level, v) \
    if(str == #v) { \
        return LogLevel::level; \
    }
    XX(DEBUG, debug);
    XX(INFO, info);
    XX(WARNNING, warn);
    XX(ERROR, error);
    XX(FATAL, fatal);

    XX(DEBUG, DEBUG);
    XX(INFO, INFO);
    XX(WARNNING, WARN);
    XX(ERROR, ERROR);
    XX(FATAL, FATAL);
    return LogLevel::UNKNOW;
#undef XX
}
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
