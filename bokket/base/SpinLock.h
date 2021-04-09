//
// Created by bokket on 2021/4/9.
//

#ifndef BOKKERVE_SPINLOCK_H
#define BOKKERVE_SPINLOCK_H

#include <atomic>
#include "noncopyable.h"

namespace bokket
{
class SpinLock : public noncopyable
{
private:
    std::atomic_flag flag;
public:
    void lock()
    {
        while(flag.test_and_set(std::memory_order_acquire))
        ;
    }
    bool try_lock()
    {
        return !flag.test_and_set(std::memory_order_acquire);
    }
    void unlock()
    {
        flag.clear(std::memory_order_release);
    }
};

}


#endif //BOKKERVE_SPINLOCK_H
