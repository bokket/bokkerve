//
// Created by bokket on 2021/2/23.
//

#include "Timer.h"


namespace bokket
{
std::atomic<uint64_t> Timer::timersCreated_={0};


/*Timer::Timer(const std::function<void()> &cb, const std::chrono::steady_clock::time_point &when,
             const std::chrono::microseconds &interval)*/
Timer::Timer(std::function<void()> cb, const std::chrono::steady_clock::time_point &when,
             const std::chrono::microseconds &interval)
             :cb_(std::move(cb))
             ,when_(when)
             ,interval_(interval)
             ,repeat_(interval.count()>std::chrono::microseconds::zero().count())
             ,id_(++timersCreated_)
{
}

void Timer::run() const {
    if(cb_) {
        cb_;
    }
}

void Timer::restart(const std::chrono::steady_clock::time_point &now) {
    if(repeat_) {
        when_=now+interval_;
    }
    else
        when_=std::chrono::steady_clock::now();
}

/*
bool Timer::operator<(const Timer &timer) const {
    return when_<timer.when_;
}

bool Timer::operator>(const Timer &timer) const {
    return when_>timer.when_;
}*/


}
