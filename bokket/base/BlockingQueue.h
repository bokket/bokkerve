//
// Created by bokket on 2021/2/22.
//

#ifndef BOKKET_BLOCKINGQUEUE_H
#define BOKKET_BLOCKINGQUEUE_H

#include "noncopyable.h"
#include "Condition.h"
#include "MutexLock.h"
#include <cassert>
#include <deque>

namespace bokket
{

template <typename T>
class BlockingQueue: public noncopyable
{
public:
    BlockingQueue()
                 :mutex_()
                 ,notEmpty_(mutex_)
                 ,deque_()
    {}
    void put(const T& x)
    {
        {
            MutexLockGuard lockGuard(mutex_);
            deque_.push_back(x);
        }
        //通知消费者线程去消费
        notEmpty_.notify();
    }

    T take()
    {
        MutexLockGuard lockGuard(mutex_);

        //如果没有产品就阻塞 直到有为主
        while(deque_.empty())
            notEmpty_.wait();
        assert(!deque_.empty());

        T front(deque_.front());
        deque_.pop_front();
        return front;
    }

    size_t size()const
    {
        MutexLockGuard lockGuard(mutex_);
        return deque_.size();
    }
private:
    mutable MutexLock mutex_;
    Condition notEmpty_;
    deque<T> deque_;
};
}

#endif //BOKKET_BLOCKINGQUEUE_H
