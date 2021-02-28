//
// Created by bokket on 2021/2/28.
//

#ifndef BOKKET_TIMERLD_H
#define BOKKET_TIMERLD_H

#include "../base/copyable.h"

namespace bokket
{
namespace net
{

class Timer;

class TimerId: public copyable
{
public:
    TimerId()
           :timer_(nullptr)
           ,sequence_(0)
    {}
    TimerId(Timer* timer,int64_t seq)
           :timer_(timer)
           ,sequence_(seq);
    Timer* getTimer()
    {
        return timer_;
    }


    friend class TimerQueue;
private:
    Timer* timer_;
    int64_t sequence_;
};

}
}

#endif //BOKKET_TIMERLD_H
