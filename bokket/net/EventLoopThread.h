//
// Created by bokket on 2021/6/19.
//

#ifndef BOKKERVE_PLUS_PLUS_EVENTLOOPTHREAD_H
#define BOKKERVE_PLUS_PLUS_EVENTLOOPTHREAD_H

#include <mutex>
#include <future>
#include <condition_variable>


#include "EventLoop.h"

#include "../thread/thread.h"
#include "../base/noncopyable.h"


namespace bokket
{

class EventLoopThread: public noncopyable
{
public:
    explicit EventLoopThread(const std::string& threadName="EventLoopThread");
    ~EventLoopThread();

    void wait();


    EventLoop* getLoop() const {

    }

    void run();

private:
    EventLoop* loop_;
    std::string loopThreadName_;
    void loopTasks();
    std::promise<EventLoop*> promiseForLoopPointer_;
    std::promise<int> promiseForRun_;
    std::promise<int> promiseForLoop_;

    std::once_flag once_;
    Thread thread_;


};

}




#endif //BOKKERVE_PLUS_PLUS_EVENTLOOPTHREAD_H
