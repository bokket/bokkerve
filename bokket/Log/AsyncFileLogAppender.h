//
// Created by bokket on 2021/3/8.
//

#ifndef BOKKET_ASYNCFILELOGAPPENDER_H
#define BOKKET_ASYNCFILELOGAPPENDER_H

#include "LogAppender.h"
#include "../net/CountDownLatch.h"
#include <string>
#include <mutex>
#include <thread>
#include <condition_variable>
#include <atomic>


namespace bokket
{

class AsyncFileLogAppender: public LogAppender
{
public:
    AsyncFileLogAppender(const std::string& filename);
    ~AsyncFileLogAppender();

    void append(const std::string& msg,int32_t line) override;

    void start();
    void stop();

private:
    void threadFunc();
    

    std::atomic_bool started_;

    time_t ;
    std::string filename_;

    mutable std::mutex mutex_;
    std::thread thread_;
    std::condition_variable conditionVariable_;
    bokket::net::CountDownLatch latch_;

};

}

#endif //BOKKET_ASYNCFILELOGAPPENDER_H
