//
// Created by bokket on 2021/2/23.
//

#ifndef BOKKET_TIMER_H
#define BOKKET_TIMER_H

#include "../base/noncopyable.h"
#include "Timestamp.h"
#include "CallBack.h"
#include <cassert>
#include <atomic>
#include <chrono>
namespace bokket
{
namespace net
{
class Timer: public noncopyable
{
public:

    Timer(TimerCallback callback,Timestamp when,chrono::nanoseconds interval)
         :callback_(std::move(callback))
         ,when_(when)
         ,interval_(interval)
         ,repeat_(interval_>chrono::nanoseconds::zero())
         ,canceled_(false)
         ,sequence_(++numCreated_)
    {}

    ~Timer();

    void run()
    {
        if(callback_)
            callback_();
    }

    bool repeat() const { return repeat_; }
    //是否超时失效
    bool expired(Timestamp now) const { return now>=when_; }
    Timestamp when() const { return when_; }

    void restart()
    {
        assert(repeat_);
        when_+=interval_;
    }

    void setCancel(bool op)
    {
        //assert(!canceled_);
        canceled_= op;
    }

    bool isCanceled() { return canceled_; }

    int64_t getSequence() const { return sequence_; }

    static int64_t getNumCreated() { return numCreated_; }

private:
    TimerCallback callback_;// 定时器回调函数
    Timestamp when_;// 下一次的超时时刻
    const chrono::nanoseconds interval_;// 超时时间间隔，如果是一次性定时器，该值为0
    bool repeat_; //是否重复
    bool canceled_;
    //long long	Signed
    static atomic<int64_t> numCreated_;
    const int64_t sequence_;
};

}
}

#endif //BOKKET_TIMER_H
