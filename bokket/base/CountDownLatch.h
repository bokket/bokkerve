//
// Created by bokket on 2021/2/22.
//

#ifndef BOKKET_COUNTDOWNLATCH_H
#define BOKKET_COUNTDOWNLATCH_H

#include "MutexLock.h"
#include "Condition.h"
#include "noncopyable.h"


//  锁住mutex                             锁住mutex
//      while(条件)                           更改条件
//          等待                              signal或broadcast
//  解锁mutex                             解锁mutex

namespace bokket
{
    class CountDownLatch: public noncopyable
    {
    public:
        explicit CountDownLatch(int count)
                               :mutex_()
                               ,condition_(mutex_)
                               ,count_(count)
        {}


        void wait()
        {
            MutexLockGuard lockGuard(mutex_);
            while(count_>0)
                condition_.wait();
        }

        void countDown()
        {
            MutexLockGuard lockGuard(mutex_);
            --count_;
            if(count_==0)
                condition_.notifyAll();
        }

        int getCount() const
        {
            MutexLockGuard mutexLockGuard(mutex_);
            return count_;
        }

    private:
        mutable MutexLock mutex_;
        Condition condition_;
        int count_;
    };
}


#endif //BOKKET_COUNTDOWNLATCH_H
