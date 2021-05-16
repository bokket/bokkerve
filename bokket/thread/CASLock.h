//
// Created by bokket on 2021/5/15.
//

#ifndef BOKKERVE_CASLOCK_H
#define BOKKERVE_CASLOCK_H

#include <atomic>

namespace bokket
{
class CASLock
{
public:
    CASLock() {
        atomicMutex_.clear();
    }
    ~CASLock()=default;

    void lock() {
        while(std::atomic_flag_test_and_set_explicit(&atomicMutex_,std::memory_order_acquire));
    }
    void unlock() {
        std::atomic_flag_clear_explicit(&atomicMutex_,std::memory_order_release);
    }
private:
    volatile std::atomic_flag atomicMutex_;
}
}

#endif //BOKKERVE_CASLOCK_H
