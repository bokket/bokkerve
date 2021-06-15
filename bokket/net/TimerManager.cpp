//
// Created by bokket on 2021/3/1.
//

#include "TimerManager.h"


#include "../Log/Log.h"
#include "./Channel.h"
#include "./EventLoop.h"


#include <unistd.h>
#include <sys/timerfd.h>

#include <string>
#include <ratio>
#include <iostream>


namespace bokket::detail
{

static bokket::Logger::ptr g_logger = BOKKET_LOG_NAME("system");



static int createTimerfd() {
    int timerfd=::timerfd_create(CLOCK_MONOTONIC,TFD_NONBLOCK|TFD_CLOEXEC);

    if(timerfd<0) {
        BOKKET_LOG_ERROR(g_logger)<<"timerfd_create error";
    }

    return timerfd;
}

// 清除定时器，避免一直触发
static void readTimerfd(int timerfd) {
    uint64_t val;
    ssize_t n=::read(timerfd,&val,sizeof(val));
    BOKKET_LOG_DEBUG(g_logger)<<"readTimerfd() "<<val;
    if(n!=sizeof(val)) {
        BOKKET_LOG_ERROR(g_logger)<<"readTimerfd() error "<<n<<" bytes instead of 8 bits";
    }
}

static struct timespec howMuchTimeFromNow(const std::chrono::steady_clock::time_point& when) {
    struct ::timespec sec;


    std::chrono::microseconds ms=std::chrono::duration_cast<std::chrono::microseconds>(
            when - std::chrono::steady_clock::now());
    /*auto ms=std::chrono::duration_cast<std::chrono::microseconds>(
            when - std::chrono::steady_clock::now()).count();*/
    //chrono::nanoseconds ns=when-bokket::clock::now();
    auto one_microseconds=100ms;
            //1ms;
    if(ms<one_microseconds)
        ms=one_microseconds;

    //对于一个具体的ratio来说，可以通过den获取分母的值，num获取分子的值。
    //<ratio>头文件中包含了很多以10为基数的各种分数
    //我们只需要通过：std::milli，std::micro，std::nano使用就好了。
    //std::ratio<1, 1000000000> nanoseconds;
    //typedef ratio<1,                   1000000> micro;
    sec.tv_sec=static_cast<time_t>(ms.count()/std::micro::den);
    sec.tv_nsec=static_cast<long>((ms.count()%std::micro::den)*1000);
    /*sec.tv_sec=static_cast<time_t>(ns.count()/std::nano::den);
    sec.tv_nsec=static_cast<long>(ns.count()%std::nano::den);*/

    return sec;
}

static void resetTimerfd(int timerfd,const std::chrono::steady_clock::time_point & expiration)
{
    struct ::itimerspec newValue;
    struct ::itimerspec oldValue;
    ::memset(&newValue,0,sizeof(newValue));
    ::memset(&oldValue,0,sizeof(oldValue));

    newValue.it_value=howMuchTimeFromNow(expiration);

    int ret=::timerfd_settime(timerfd,0,&newValue,&oldValue);
    if(ret) {
        BOKKET_LOG_ERROR(g_logger)<<"resetTimerfd() error";
    }
        
}
}

namespace bokket
{

void TimerManager::handleRead() {
    loop_->assertInLoopThread();
    const auto now=std::chrono::steady_clock::now();

    bokket::detail::readTimerfd(timerfd_);

    std::vector<Timer::ptr> expired=getExpired(now);

    callingExpiredTimers_= true;

    for(const auto & timerPtr: expired) {
        if(timerIdSet_.find(timerPtr->getTimerId()) != timerIdSet_.end()) {
            timerPtr->run();
        }
    }

    callingExpiredTimers_= false;

    reset(expired,now);
}

TimerManager::TimerManager(EventLoop *loop)
                          :loop_(loop)
                          ,timerfd_(bokket::detail::createTimerfd())
                          ,timerfdChannel_(new Channel(loop,timerfd_))
                          ,timers_()
                          ,callingExpiredTimers_(false) {
    timerfdChannel_->setReadCallback(
        [this] { TimerManager::handleRead(); }
        );

    timerfdChannel_->enableReading();
}

void TimerManager::reset() {
    loop_->runInLoop(
            [this]() {
                timerfdChannel_->disableAll();
                timerfdChannel_->remove();
                close(timerfd_);
                timerfd_=bokket::detail::createTimerfd();
                timerfdChannel_=std::make_shared<Channel>(loop_,timerfd_);
                timerfdChannel_->setReadCallback(
                    [this] { TimerManager::handleRead(); }
                    );

                timerfdChannel_->enableReading();

                if(!timers_.empty()) {
                    const auto nextExpire=timers_.top()->when();
                    bokket::detail::resetTimerfd(timerfd_,nextExpire);
                }
                
            }
            );
}

TimerManager::~TimerManager() {
    auto channelPtr=timerfdChannel_;
    auto fd=timerfd_;

    loop_->runInLoop(
        [channelPtr,fd]() {
            channelPtr->disableAll();
            channelPtr->remove();
            ::close(fd);
        }
        );
}

uint64_t TimerManager::addTimer(const std::function<void()> &cb, const std::chrono::steady_clock::time_point &when,
                                const std::chrono::microseconds &interval) {
    Timer::ptr timerPtr=std::make_shared<Timer>(cb,when,interval);

    loop_->runInLoop(
            [this,timerPtr]() {
                addTimerInLoop(timerPtr);
            }
            );
    return timerPtr->getTimerId();
}


void TimerManager::addTimerInLoop(const Timer::ptr &timer) {
    loop_->assertInLoopThread();
    timerIdSet_.insert(timer->getTimerId());
    if(insert(timer)) {
        bokket::detail::resetTimerfd(timerfd_,timer->when());
    }
}

void TimerManager::invalidTimer(uint64_t id) {
    loop_->runInLoop(
            [this,id]() {
                timerIdSet_.erase(id);
            }
            );
}

bool TimerManager::insert(const Timer::ptr &timer) {
    loop_->assertInLoopThread();
    bool earliestChanged=false;
    if(timers_.size()==0 || timers_.top()->when() > timer->when()) {
        earliestChanged=true;
    }
    timers_.push(timer);

    return earliestChanged;
}


std::vector<Timer::ptr> TimerManager::getExpired() {
    auto now=std::chrono::steady_clock::now();
    getExpired(now);
}

std::vector<Timer::ptr> TimerManager::getExpired(const std::chrono::steady_clock::time_point &now) {
    std::vector<Timer::ptr> expired;
    while(!timers_.empty()) {
        if(timers_.top()->when() < now) {
            expired.emplace_back(timers_.top());
            timers_.pop();
        } else
            break;
    }
    return expired;
}


void TimerManager::reset(const std::vector <Timer::ptr> &expired
                         , const std::chrono::steady_clock::time_point &now) {
    loop_->assertInLoopThread();
    for(const auto & timerPtr:expired) {
        auto it=timerIdSet_.find(timerPtr->getTimerId());

        if(it!=timerIdSet_.end()) {
            if(timerPtr->isRepeat()) {
                timerPtr->restart(now);
                insert(timerPtr);
            } else {
                timerIdSet_.erase(it);
            }
        }
    }

    if(!timers_.empty()) {
        const auto nextExpire=timers_.top()->when();
        bokket::detail::resetTimerfd(timerfd_,nextExpire);
    }

}



}