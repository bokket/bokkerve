//
// Created by bokket on 2021/2/23.
//

#include "EventLoop.h"
#include "Channel.h"
#include "Epoller.h"

#include <iostream>
#include <sstream>

#include <signal.h>
#include <sys/eventfd.h>


#include "../Log/Log.h"
#include "../thread/util.h"
#include "../thread/thread.h"



namespace bokket
{

static thread_local EventLoop *t_EventLoop = nullptr;

static bokket::Logger::ptr g_logger=BOKKET_LOG_NAME("net");



int createEventfd() {
    int evfd = ::eventfd(0, EFD_NONBLOCK | EFD_CLOEXEC);
    if (evfd < 0) {
        BOKKET_LOG_FATAL(g_logger)<<"eventfd create fail";
    }
    return evfd;
}

class IgnoreSigPipe {
public:
    IgnoreSigPipe() {
        ::signal(SIGPIPE, SIG_IGN);
    }
};
    
IgnoreSigPipe ignore;


EventLoop * EventLoop::getEventLoopOfCurrentThread() {
    return t_EventLoop;
}


EventLoop::EventLoop()
           :tid_(bokket::getThreadId())
           ,looping_(false)
           ,quit_(false)
           ,callingPendingTasks_(false)
           ,eventHandling_(false)
           ,epoller_(new Epoller(this))
           ,timerManager_(new TimerManager(this))
           //,epoller_(std::make_unique<Epoller>(this))
           ,wakeupfd_(createEventfd())
           ,wakeupChannel_(new Channel(this,wakeupfd_))
           //,wakeupChannel_(std::make_unique<Channel>(this,wakeupFd_))
           ,currentActiveChannel_(nullptr)
{

    if(t_EventLoop) {
        BOKKET_LOG_FATAL(g_logger)<<"Another event Loop "<<t_EventLoop
        <<"exists in this thread"<<tid_;
    }
    else {
        t_EventLoop=this;
    }
    //wakeupChannel_->setReadCallback(std::bind(&EventLoop::handleRead,this));
    wakeupChannel_->setReadCallback(
            [this] { handleRead(); }
            );
    wakeupChannel_->enableReading();
}


EventLoop::~EventLoop()
{
    wakeupChannel_->disableAll();
    wakeupChannel_->remove();
    ::close(wakeupfd_);

    t_EventLoop= nullptr;
}


size_t EventLoop::queueSize() const
{
    return pendingTasks_.size();
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

        printActiveChannels();

        eventHandling_= true;

        for(Channel* channel:activeChannels_)
        {
            currentActiveChannel_=channel;
            currentActiveChannel_->handleEvent();
        }

        currentActiveChannel_= nullptr;
        eventHandling_= false;

        doPendingTasks();

    }
    looping_= false;


    std::ostringstream oss;
    oss<<std::this_thread::get_id();
    std::string stid=oss.str();

    //LOG " Exiting Loop , EventLoop object : 0x%x, threadID: %s ",this,stid.str();
}

void EventLoop::quit()
{
    quit_= true;
    if(!isInLoopThread())
        wakeup();
}

void EventLoop::runInLoop(const Task &cb)
{
    if(isInLoopThread())
        cb();
    else
    {
        queueInLoop(std::move(cb));
    }
}

void EventLoop::setFrameFunctor(const Functor cb)
{
    functor_=cb;
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

void EventLoop::updateChannel(Channel *channel)
{
    assert(channel->ownerLoop()== this);
    assertInLoopThread();
    epoller_->updateChannel(channel);
}

void EventLoop::removeChannel(Channel *channel)
{
    assert(channel->ownerLoop()==this);
    assertInLoopThread();
    epoller_->removeChannel(channel);
}

bool EventLoop::hasChannel(Channel *channel)
{
    assert(channel->ownerLoop()==this);
    assertInLoopThread();
    return epoller_->hasChannel(channel);
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

void EventLoop::doPendingTasks() {
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

void EventLoop::printActiveChannels() const {
    for(auto item:activeChannels_)
    {
        <<item->reventsToString()<<"}  ";
    }
}

}