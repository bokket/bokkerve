//
// Created by bokket on 2021/2/23.
//

#ifndef BOKKET_TIMER_H
#define BOKKET_TIMER_H


#include <atomic>
#include <chrono>
#include <cassert>
#include <functional>


#include "CallBack.h"

#include "../base/noncopyable.h"

namespace bokket
{

class Timer: public noncopyable
{
public:
    using ptr=std::shared_ptr<Timer>;
    using TimerCallback=std::function<void()>;
public:

    Timer(const std::function<void()>& cb,const std::chrono::steady_clock::time_point & when
          ,const std::chrono::microseconds & interval)
         :callback_(std::move(callback))
         ,when_(when)
         ,interval_(interval)
         ,repeat_(interval_>chrono::nanoseconds::zero())
         ,canceled_(false)
         ,sequence_(++numCreated_)
    {}

    ~Timer();

    void run() const
    {
        if(callback_)
            callback_();
    }

    //是否超时失效
    bool expired(const std::chrono::steady_clock::time_point& now) const { return now>=when_; }

    void restart(const std::chrono::steady_clock::time_point& now);
    {
        assert(repeat_);
        when_+=interval_;
    }

    /*void setCancel(bool op)
    {
        //assert(!canceled_);
        canceled_= op;
    }*/

    const std::chrono::steady_clock::time_point & when() const { return when_; }

    bool isRepeat() { return repeat_; }

    //bool isCanceled() { return canceled_; }

    uint64_t getTimerId() const { return id_; }

    static int64_t getTimerCreated() { return timersCreated_; }

private:
    std::function<void()> call_;// 定时器回调函数
    std::chrono::steady_clock::time_point when_;// 下一次的超时时刻
    const std::chrono::microseconds interval_;// 超时时间间隔，如果是一次性定时器，该值为0
    bool repeat_; //是否重复
    bool canceled_;
    //long long	Signed
    static atomic<uint64_t> timersCreated_;//创建个数
    const uint64_t id_;    // 定时器序列号
};


}

#endif //BOKKET_TIMER_H
