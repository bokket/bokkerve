//
// Created by bokket on 2021/2/22.
//

#ifndef BOKKET_CONDITION_H
#define BOKKET_CONDITION_H

#include "MutexLock.h"
#include "noncopyable.h"
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
        pthread_cond_init(&pcond_, nullptr);
    }
    ~Condition()
    {
        pthread_cond_destroy(&pcond_);
    }
    void wait()
    {
        pthread_cond_wait(&pcond_,mutex_.get());
    }

    void notify()
    {
        pthread_cond_signal(&pcond_);
    }

    void notifyAll()
    {
        pthread_cond_broadcast(&pcond_);
    }

        /*
		struct timespec
		{
			time_t tv_sec;  // Seconds - >= 0 秒
			long   tv_nsec; // Nanoseconds - [0, 999999999] 纳秒
		};
		*/

    bool waitForSeconds(int seconds)
    {
        struct timespec timespec;
        //CLOCK_REALTIME 系统当前时间
        clock_gettime(CLOCK_REALTIME,&timespec);//获取系统当前时间
        timespec.tv_sec+=seconds;
        //设置超时时间 如果定时时间到了就会返回true 没有到将会返回flase
        return ETIMEDOUT==pthread_cond_timedwait(&pcond_,mutex_.get(),&timespec);
    }


private:
    MutexLock& mutex_;
    pthread_cond_t pcond_;
};


}

#endif //BOKKET_CONDITION_H
