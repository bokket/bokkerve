//
// Created by bokket on 2021/3/6.
//

#ifndef BOKKET_COUNTDOWNLATCH_H
#define BOKKET_COUNTDOWNLATCH_H

#include <mutex>
#include <condition_variable>
#include "../base/noncopyable.h"
namespace bokket
{
namespace net
{
class CountDownLatch: public noncopyable
{
public:
    explicit CountDownLatch(int count)
                           :count_(count)
    {}

    void wait()
    {
        std::unique_lock<std::mutex> uniqueLock(mutex_);
        while(count_>0)
            cond_.wait(uniqueLock);
    }

    void countLatch()
    {
        std::unique_lock<std::mutex> uniqueLock(mutex_);
        count_--;
        if(count_<=0)
            cond_.notify_all();
    }

    int getCount() const
    {
        std::lock_guard<std::mutex> lockGuard(mutex_);
        return count_;
    }
private:
    int count_;
    mutable std::mutex mutex_;
    std::condition_variable cond_;
};
}

}

#endif //BOKKET_COUNTDOWNLATCH_H
