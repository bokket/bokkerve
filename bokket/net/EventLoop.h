//
// Created by bokket on 2021/2/23.
//

#ifndef BOKKET_EVENTLOOP_H
#define BOKKET_EVENTLOOP_H


#include "../base/noncopyable.h"
#include "../base/Timestamp.h"
#include "../base/Thread.h"
#include "../base/MutexLock.h"
#include <vector>
#include <functional>
#include <atomic>
#include <thread>

using namespace std;

namespace bokket
{
namespace net
{
class EventLoop: public noncopyable
{
public:
    using Functor=function<void()>;

public:
    EventLoop();
    ~EventLoop();


    Timestamp pollReturnTime() const { return pollReturnTime_; }

    size_t queueSize() const;

    void loop();
    void quit();
    void runInLoop(Functor cb);
    void queueInLoop(Functor cb);

    void wakeup();
    void updateChannel(Channel* channel);
    void removeChannel(Channel* channel);
    bool hasChannel(Channel* channel);

    void assertInLoopThread();


    bool isInLoopThread() const { return threadId==CurrentThread::tid(); }
    bool eventHandling() const { return eventHandling_; }

    static EventLoop* getEventLoopOfCurrentThread();
private:
    void handleRead();
    void doPendingFunctors();

    //atomic<bool> quit_;
    atomic_bool quit_;
    //atomic<bool> eventHandling_;
    atomic_bool eventHandling_;
    //atomic<bool> callingPendingFunctors_;
    atomic_bool callingPendingFunctors_;

    const pid_t threadId;
    int wakeupFd_;// 用于eventfd
    unique_ptr<Channel> wakeupChannel_;// 该通道将会纳入poller_来管理
    unique_ptr<Epoller> poller_;
    unique_ptr<TimerQueue> timerQueue_;

    mutable MutexLock mutex_;
    vector<Functor> pendingFunctors_;//小任务列表

    using ChannelList=vector<Channel*>;
    ChannelList activeChannels_;//活跃通道列表 // Poller返回的活动通道
    Channal* currentActiveChannel_;//当前正在处理的通道

    Timestamp pollReturnTime_;//epoll_wait返回时间
};
}
}



#endif //BOKKET_EVENTLOOP_H
