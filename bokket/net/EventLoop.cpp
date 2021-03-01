//
// Created by bokket on 2021/2/23.
//

#include "EventLoop.h"
#include "Channel.h"
#include "Epoller.h"
#include <signal.h>
#include <sys/eventfd.h>


using namespace bokket;
using namespace bokket::net;

namespace
{
    thread_local EventLoop *t_EventLoop = nullptr;



    int CreateEventFd() {
        int evfd = ::eventfd(0, EFD_NONBLOCK | EFD_CLOEXEC);
        if (evfd < 0) {}
        return evfd;
    }

    class IgnoreSigPipe {
    public:
        IgnoreSigPipe() {
            ::signal(SIGPIPE, SIG_IGN);
        }
    };
    IgnoreSigPipe ignore;
}

EventLoop * EventLoop::getEventLoopOfCurrentThread()
{
    return t_EventLoop;
}


EventLoop::EventLoop()
           :threadId_(bokket::CurrentThread::tid())
           ,looping_(false)
           ,quit_(false)
           ,callingPendingFunctors_(false)
           ,eventHandling_(false)
           ,epoller_(std::make_unique<Epoller>(this))
           ,wakeupFd_(CreateEventFd())
           ,wakeupChannel_(std::make_unique<Channel>(this,wakeupFd_))
           ,currentActiveChannel_(nullptr)
{

    if(t_EventLoop)
    {}
    else
    {
        t_EventLoop=this;
    }
    wakeupChannel_->setReadCallback(std::bind(&EventLoop::handleRead,this));
    wakeupChannel_->enableReading();
}


EventLoop::~EventLoop()
{
    wakeupChannel_->disableAll();
    wakeupChannel_->remove();
    ::close(wakeupFd_);

    t_EventLoop= nullptr;
}


size_t EventLoop::queueSize() const
{
    return pendingFunctors_.size();
}

// 事件循环，该函数不能跨线程调用
// 只能在创建该对象的线程中调用
void EventLoop::loop()
{
    assert(!looping_);

    // 断言当前处于创建该对象的线程中
    assertInLoopThread();
    looping_= true;
    quit_= false;

    while(!quit_)
    {
        activeChannels_.clear();
        //epoller_->poll(10s);
        epoller_->poll(&activeChannels_);

        eventHandling_= true;

        for(Channel* channel:activeChannels_)
        {
            currentActiveChannel_=channel;
            currentActiveChannel_->handleEvent();
        }

        currentActiveChannel_= nullptr;
        eventHandling_= false;

        doPendingFunctors();

    }
    looping_= false;
}

void EventLoop::quit()
{
    quit_= true;
    if(!isInLoopThread())
        wakeup();
}

void EventLoop::runInLoop(Functor cb)
{
    if(isInLoopThread())
        cb();
    else
    {
        queueInLoop(std::move(cb));
    }
}


void EventLoop::queueInLoop(const Functor& cb)
{
    {
        std::lock_guard <std::mutex> guard(mutex_);
        pendingFunctors_.emplace_back(cb);
    }

    // 调用queueInLoop的线程不是IO线程需要唤醒
    // 或者调用queueInLoop的线程是IO线程，并且此时正在调用pending functor，需要唤醒
    // 只有IO线程的事件回调中调用queueInLoop才不需要唤醒
    if(!isInLoopThread() || callingPendingFunctors_)
    {
        wakeup();
    }
}

void EventLoop::wakeup()
{
    uint64_t one=1;

    ssize_t n=::write(wakeupFd_,&one,sizeof(one));
    if(n!=sizeof(one))
    {}
}

void EventLoop::assertInLoopThread()
{
    assert(isInLoopThread());
}


void EventLoop::handleRead()
{
    uint64_t one=1;

    ssize_t n=::read(wakeupFd_,&one,sizeof(one));
    if(n!=sizeof(one))
    {}
}

void EventLoop::doPendingFunctors()
{
    assertInLoopThread();
    vector<Functor> functors;

    {
        std::lock_guard<std::mutex> guard(mutex_);
        functors.swap(pendingFunctors_);
    }

    for(Functor& functor:functors)
        functor();

    callingPendingFunctors_=false;
}

void EventLoop::printActiveChannels() const
{
    for(auto item:activeChannels_)
    {
        <<item->reventsToString()<<"}  ";
    }
}