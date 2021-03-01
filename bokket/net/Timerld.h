//
// Created by bokket on 2021/2/28.
//

#ifndef BOKKET_TIMERLD_H
#define BOKKET_TIMERLD_H

#include "../base/copyable.h"
#include <memory>

namespace bokket
{
namespace net
{

class Timer;

class TimerId: public copyable
{
public:
    /*TimerId()
           :timer_(nullptr)
           ,sequence_(0)
    {}
    TimerId(std::weak_ptr<Timer>& timer,int64_t seq)
           :timer_(timer)
           ,sequence_(seq);

    Timer* getTimer()
    {
        return timer_;
    }*/

    TimerId(std::weak_ptr<Timer>& timer,int64_t seq)
            :timer_(timer)
            ,sequence_(timer.lock() ? timer.lock()->getSequence() : 0)
    {}


    friend class TimerQueue;
private:
    std::weak_ptr<Timer> timer_;
    int64_t sequence_;
};

}
}

#endif //BOKKET_TIMERLD_H
