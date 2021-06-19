//
// Created by bokket on 2021/6/19.
//

#include "EventLoopThread.h"

#include <sys/prctl.h>


#include "../Log/Log.h"


namespace bokket
{


static bokket::Logger g_logger=BOKKET_LOG_NAME("net");

EventLoopThread::EventLoopThread(const std::string &threadName)
                                :loop_(nullptr)
                                ,loopThreadName_(threadName)
                                ,thread_([this] { loopTasks(); } )
{
    auto f=promiseForLoopPointer_.get_future();
    loop_=f.get();
}

EventLoopThread::~EventLoopThread() {
    run();

    if(loop_) {
        loop_->quit();
    }
    if(!thread_.joinable()) {
        thread_.join();
    }
}

void EventLoopThread::wait() {
    thread_.join();
}

void EventLoopThread::loopTasks() {
    thread_.setName(loopThreadName_);

    EventLoop loop;
    loop.queueInLoop( [this]
                      {
        promiseForLoop_.set_value(1);
                      });

    promiseForLoopPointer_.set_value(&loop);
    auto f=promiseForRun_.get_future();
    (void)f.get();

    loop.loop();
    BOKKET_LOG_DEBUG(g_logger)<<"EventLoopThread::loopTasks()";
    loop_= nullptr;

}

void EventLoopThread::run() {
    std::call_once(once_,[this]() {
        auto f=promiseForLoop_.get_future();
        promiseForRun_.set_value(1);

        (void)f.get();
    });
}


}