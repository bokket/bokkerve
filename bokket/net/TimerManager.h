//
// Created by bokket on 2021/3/1.
//

#ifndef BOKKET_TIMERMANAGER_H
#define BOKKET_TIMERMANAGER_H

#include <vector>
#include <memory>
#include <queue>


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

class TimerQueue: public noncopyable
{
private:
    std::priority_queue<Timer::ptr,std::vector<Timer::ptr,TimerCompare>> timers_;
    EventLoop* loop_;


    int timerFd_;
    Channel::ptr timerfdChannel_;


};

}



#endif //BOKKET_TIMERMANAGER_H
