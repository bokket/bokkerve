//
// Created by bokket on 2021/5/11.
//

#ifndef BOKKERVE_SEMAPHORE_H
#define BOKKERVE_SEMAPHORE_H
#include <mutex>
#include <condition_variable>

namespace bokket
{
class Semaphore
{
public:
    explicit Semaphore(int count=0)
                      :count_(count)
    {}

    void notify() {
        std::unique_lock<std::mutex> uniqueLock(mutex_);
        ++count_;
        cv_.notify_one();
    }

    void wait() {
        std::unique_lock<std::mutex> uniqueLock(mutex_);
        cv_.wait(uniqueLock,
                 [=] {
            return count_>0;
                });
        --count_;
    }
private:
    std::mutex mutex_;
    std::condition_variable cv_;
    int count_;
};
}

#endif //BOKKERVE_SEMAPHORE_H
