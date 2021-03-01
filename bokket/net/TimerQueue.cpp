//
// Created by bokket on 2021/2/28.
//

#include "TimerQueue.h"

#include <cstring>
#include <sys/timerfd.h>
#include <unistd.h>
#include <ratio>
#include "EventLoop.h"


using namespace std::chrono_literals;

namespace bokket
{
namespace net
{

namespace detail
{
int createTimerfd()
{
    int timerfd=::timerfd_create(CLOCK_MONOTONIC,TFD_NONBLOCK|TFD_CLOEXEC);

    if(timerfd<0)
    {
    }

    return timerfd;
}

// 清除定时器，避免一直触发
void readTimerfd(int timerfd)
{
    uint64_t val;
    ssize_t n=::read(timerfd,&val,sizeof(val));
    if(n!=sizeof(val))
    {}
}

struct timespec howMuchTimeFromNow(Timestamp when)
{
    struct timespec sec;
    chrono::nanoseconds ns=when-bokket::clock::now();
    auto one_microseconds=1ms;
    if(ns<one_microseconds)
        ns=one_microseconds;

    //对于一个具体的ratio来说，可以通过den获取分母的值，num获取分子的值。
    //<ratio>头文件中包含了很多以10为基数的各种分数
    //我们只需要通过：std::milli，std::micro，std::nano使用就好了。
    //std::ratio<1, 1000000000> nanoseconds;
    sec.tv_sec=static_cast<time_t>(ns.count()/std::nano::den);
    sec.tv_nsec=static_cast<long>(ns.count()%std::nano::den);

    return sec;
}

void resetTimerfd(int timerfd,Timestamp when)
{
    struct itimerspec newValue;
    struct itimerspec oldValue;
    ::memset(&newValue,0,sizeof(newValue));
    ::memset(&oldValue,0,sizeof(oldValue));

    newValue=howMuchTimeFromNow(when);

    int ret=timerfd_settime(timerfd,0,&newValue,&oldValue);
    if(ret==-1)
    {
    }
}

}
}
}

using namespace bokket;
using namespace bokket::net;
using namespace bokket::net::detail;

TimerQueue::TimerQueue(EventLoop *loop)
                      :loop_(loop)
                      ,timerfd_(createTimerfd())
                      ,timerfdChannel_(loop_,timerfd_)
{
    loop_->assertInLoopThread();

    //捕获this指针
    //闭包
    // std::function<void()>;

    timerfdChannel_.setReadCallback(bind(&TimerQueue::handleRead,this));
    //timerfdChannel_.setReadCallback( [this](){handleRead();} );
    timerfdChannel_.enableReading();
}

TimerQueue::~TimerQueue()
{
    ::close(timerfd_);

    for(auto & item :TimerList)
    {
        delete item.seconde;
    }
}

TimerId TimerQueue::addTimer(TimerCallback cb, Timestamp when, chrono::nanoseconds interval)
{

}


void TimerQueue::handleRead()
{
    loop_->assertInLoopThread();
    readTimerfd(timerfd_);

    
}