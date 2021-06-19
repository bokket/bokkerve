//
// Created by bokket on 2021/6/19.
//

#ifndef BOKKERVE_PLUS_PLUS_EVENTLOOPTHREADPOOL_H
#define BOKKERVE_PLUS_PLUS_EVENTLOOPTHREADPOOL_H

#include <vector>
#include <memory>
#include <functional>

#include "EventLoopThread.h"
#include "../base/noncopyable.h"


namespace bokket
{

class EventLoop;
class EventLoopThread;

class EventLoopThreadPool: public noncopyable
{
public:
    using ThreadInitCallback=std::function<void(EventLoop*)>;
public:
    //explicit EventLoopThreadPool(EventLoop* baseLoop);
    EventLoopThreadPool(std::size_t threadNum,const std::string& name="EventLoopThreadPool");
    ~EventLoopThreadPool();

    void setThreadNum(std::size_t threadNums) {
        threadNums_=threadNums;
    }

    void start();

    void wait();

    std::size_t size() {
        return threads_.size();
    }

    EventLoop* getLoop(std::size_t id);

    EventLoop* getNextLoop();

    std::vector<EventLoop*> getLoops() const;

    bool started() const { return started_; }

private:
    EventLoop* baseLoop_;
    bool started_;
    std::size_t threadNums_;
    std::size_t loopIndex_;
    //std::size_t next_;
    std::vector<std::unique_ptr<EventLoopThread>> threads_;
    //std::vector<EventLoop*> loops_;

};

}



#endif //BOKKERVE_PLUS_PLUS_EVENTLOOPTHREADPOOL_H
