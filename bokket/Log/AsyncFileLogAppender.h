//
// Created by bokket on 2021/3/8.
//

#ifndef BOKKET_ASYNCFILELOGAPPENDER_H
#define BOKKET_ASYNCFILELOGAPPENDER_H

#include "Log.h"
#include <string>
#include <mutex>
#include <thread>
#include <condition_variable>
#include <atomic>
#include <iostream>
#include <sys/stat.h>
#include "../thread/thread.h"
#include "../net/CountDownLatch.h"

namespace bokket
{

class LogAppenderAsyncFile: public LogAppender
{
public:
    using ptr=std::shared_ptr<LogAppenderAsyncFile>;
public:
    LogAppenderAsyncFile(const std::string& filename,std::time_t persistPeriod=3);
    ~LogAppenderAsyncFile();

    //void append(const std::string& msg,int32_t line,LogLevel level,LogEvent::ptr event) override;
    void append(Logger::ptr logger,LogLevel level,LogEvent::ptr event) override;


    void start();
    void stop();

private:
    void threadFunc();
    

    bool started_;
    bool running_;

    std::time_t persistPeriod_;
    std::string filename_;

    std::mutex mutex_;
    //bokket::Thread persitThread_;
    Thread persitThread_;
    std::condition_variable cv_;
    bokket::net::CountDownLatch latch_;


    using LogBuffer=bokket::detail::FixedBuffer<bokket::detail::kLargeBuffer>;
    std::unique_ptr<LogBuffer> curBuffer_;
    std::vector<std::unique_ptr<LogBuffer>> buffers_;

};

}

#endif //BOKKET_ASYNCFILELOGAPPENDER_H
