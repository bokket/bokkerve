//
// Created by bokket on 2021/3/1.
//

#ifndef BOKKET_TIMERMANAGER_H
#define BOKKET_TIMERMANAGER_H

#include <vector>
#include <memory>
#include <queue>
#include <unordered_set>


#include "Timer.h"
#include "../base/noncopyable.h"

namespace bokket
{

struct TimerCompare {
    bool operator()(const Timer::ptr& lhs, const Timer::ptr &rhs) const {
        if(lhs->when()==rhs->when()) {
            return lhs->getTimerId() > rhs->getTimerId();
        } else {
            return lhs->when() > rhs->when();
        }
    }
};

class EventLoop;
class Channel;

class TimerManager: public noncopyable
{
public:
    explicit TimerManager(EventLoop* loop);
    ~TimerManager();

    uint64_t addTimer(const std::function<void()> & cb
                      ,const std::chrono::steady_clock::time_point & when
                      ,const std::chrono::microseconds & interval);


    void addTimerInLoop(const Timer::ptr& timer);
    void invalidTimer(uint64_t id);

    void reset();

protected:
    void handleRead();
    bool insert(const Timer::ptr & timer);

protected:
    std::vector<Timer::ptr> getExpired();
    void reset(const std::vector<Timer::ptr>& expired,const std::chrono::steady_clock::time_point & now);
    std::vector<Timer::ptr> getExpired(const std::chrono::steady_clock::time_point& now);

protected:
    bool callingExpiredTimers_;
private:
    std::priority_queue<Timer::ptr,std::vector<Timer::ptr,TimerCompare>> timers_;
    EventLoop* loop_;


    int timerfd_;
    Channel::ptr timerfdChannel_;

    std::unordered_set<uint64_t> timerIdSet_;

};

}



#endif //BOKKET_TIMERMANAGER_H
