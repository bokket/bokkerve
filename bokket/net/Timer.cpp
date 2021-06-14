//
// Created by bokket on 2021/2/23.
//

#include "Timer.h"


namespace bokket
{
std::atomic<uint64_t> Timer::timersCreated_={0};


Timer::Timer(const std::function<void()> &cb, const std::chrono::steady_clock::time_point &when,
             const std::chrono::microseconds &interval)
             :cb_(cb)
             ,when_(when)
             ,interval_(interval)
             ,repeat_(interval.count()>std::chrono::microseconds ::zero())
             ,id_(++timersCreated_)
{}

void Timer::run() const {
    if()
}


}
atomic<int64_t> numCreated_;