//
// Created by bokket on 2021/2/22.
//

#ifndef BOKKET_CONDITION_H
#define BOKKET_CONDITION_H

#include "../MutexLock/MutexLock.h"
#include "../base/noncopyable.h"
#include <pthread.h>
#include <errno.h>

namespace bokket
{

class Condition: public noncopyable
{
public:
    explicit Condition(MutexLock& mutex)
                      :mutex_(mutex)
    {
        pthread_cond_init(&cond_, nullptr);
    }
    ~Condition()
    {
        pthread_cond_destroy(&cond_);
    }
    void wait()
    {
        pthread_cond_wait(&cond_,mutex_.get());
    }

    void notify()
    {
        pthread_cond_signal(&cond_);
    }

    void notifyAll()
    {
        pthread_cond_broadcast(&cond_);
    }
    bool waitForSeconds(int seconds)
    {
        struct timespec timespec;
        clock_gettime(CLOCK_REALTIME,&timespec);
        timespec.tv_sec+=seconds;
        return ETIMEDOUT==pthread_cond_timedwait(&cond_,mutex_.get(),&timespec);
    }


private:
    MutexLock& mutex_;
    pthread_cond_t cond_;
};


}

#endif //BOKKET_CONDITION_H
