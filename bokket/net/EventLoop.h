//
// Created by bokket on 2021/2/23.
//

#ifndef BOKKET_EVENTLOOP_H
#define BOKKET_EVENTLOOP_H


#include <mutex>
#include <vector>
#include <atomic>
#include <memory>
#include <atomic>
#include <thread>
#include <functional>

#include "../thread/thread.h"
#include "../base/noncopyable.h"


namespace bokket
{


class Channel;
class Epoller;
class TimerQueue;

class EventLoop: public noncopyable
{
public:
    using Task=function<void()>;

public:
    EventLoop();
    ~EventLoop();


    //Timestamp pollReturnTime() const { return pollReturnTime_; }

    size_t queueSize() const;

    void loop();
    void quit();

    void runInLoop(const Task& cb);
    void runInLoop(Task && cb);

    void setFrameTask(const Task cb);

    void queueInLoop(const Task& cb);
    void queueInLoop(Task && cb);

    void wakeup();
    void updateChannel(Channel* channel);
    void removeChannel(Channel* channel);
    bool hasChannel(Channel* channel);

    void assertInLoopThread();

    bool isInLoopThread() const { return tid_==bokket::Thread::currentThreadTid(); }

    bool eventHandling() const { return eventHandling_; }


    static EventLoop* getEventLoopOfCurrentThread();

    void printActiveChannels() const;
private:
    void handleRead();
    void doPendingTasks();


    bool looping_;
    //atomic<bool> quit_;
    std::atomic_bool quit_;
    //atomic<bool> eventHandling_;
    atomic_bool eventHandling_;
    //atomic<bool> callingPendingTasks_;
    atomic_bool callingPendingTasks_;

    const pid_t tid_;
    //const thread::id threadId_;
    int wakeupFd_;// 用于eventfd
    std::unique_ptr<Channel> wakeupChannel_;// 该通道将会纳入poller_来管理
    std::unique_ptr<Epoller> epoller_;
    //unique_ptr<TimerQueue> timerQueue_;

    //mutable MutexLock mutex_;
    mutable std::mutex mutex_;
    vector<Task> pendingTasks_;//小任务列表

    using ChannelList=vector<Channel*>;
    ChannelList activeChannels_;//活跃通道列表 // Epoller返回的活动通道


    Channel* currentActiveChannel_;//当前正在处理的通道

    //Timestamp pollReturnTime_;//epoll_wait返回时间

    Task Task_;
};

}



#endif //BOKKET_EVENTLOOP_H
