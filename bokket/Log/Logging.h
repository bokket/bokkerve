//
// Created by bokket on 2021/2/17.
//

#ifndef BOKKET_LOGGING_H
#define BOKKET_LOGGING_H

#pragma once


#include "LogStream.h"
#include <pthread.h>
#include <string>
#include <stdio.h>

using namespace std;

namespace bokket
{
    class Logging
    {
    public:
        enum LogLevel
        {
            TRACE,
            DEBUG,
            INFO,
            WARN,
            ERROR,
            FATAL,
            NUM_LOG_LEVELS,
        };
    public:

        Logging(const char *filename, int line);

        ~Logging();

        LogStream &stream() { return impl_.stream_; }

    private:
        class Impl
        {
        public:
            Impl(const char *filename, int line);

            void formatTime();

            LogStream stream_;
            int line_;
            string basename_;
        };

        Impl impl_;
    };


#define LOG Logger(__FILE__,__LINE__).stream()

}

#endif //BOKKET_LOGGING_H
