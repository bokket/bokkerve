//
// Created by bokket on 2021/6/19.
//

#include "EventLoopThreadPool.h"

#include "../Log/Log.h"

namespace bokket
{

static Logger::ptr g_logger=BOKKET_LOG_NAME("net");


EventLoopThreadPool::EventLoopThreadPool(std::size_t threadNum, const std::string &name)
                                        :threadNums_(threadNum)
                                        ,loopIndex_(0)
                                        ,started_(false)
{
    for(auto i=0;i<threadNums_;++i) {
        threads_.emplace_back(std::make_unique<EventLoopThread>(name));
    }
}


void EventLoopThreadPool::start() {
    started_= true;
    for(auto i=0;i<threads_.size();++i) {
        threads_[i]->run();
    }
}


void EventLoopThreadPool::wait() {
    for(auto i=0;i<threads_.size();++i) {
        threads_[i]->wait();
    }
}

EventLoop * EventLoopThreadPool::getNextLoop() {
    if(threads_.size()>0) {
        EventLoop* loop=threads_[loopIndex_]->getLoop();
        ++loopIndex_;
        if(loopIndex_>=threads_.size()) {
            loopIndex_=0;
        }
        return loop;
    }
    return nullptr;
}

EventLoop * EventLoopThreadPool::getLoop(std::size_t id) {
    if(id<threads_.size()) {
        return threads_[id]->getLoop();
    }
    return nullptr;
}

std::vector<EventLoop *> EventLoopThreadPool::getLoops() const {
    std::vector<EventLoop*> ret;
    for(auto& loopThread:threads_) {
        ret.emplace_back(loopThread->getLoop());
    }
    return ret;
}

}