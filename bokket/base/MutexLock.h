//
// Created by bokket on 2021/2/18.
//

#ifndef BOKKET_MUTEXLOCK_H
#define BOKKET_MUTEXLOCK_H


#include "noncopyable.h"
#include "CurrentThread.h"
#include <pthread.h>
#include <assert.h>


namespace bokket {
    class MutexLock : public noncopyable {
    public:
        MutexLock()
                :holder_(0)
        {
            pthread_mutex_init(&mutex_, NULL);
        }

        ~MutexLock() {
            assert(holder_==0);
            //pthread_mutex_lock(&mutex);
            pthread_mutex_destroy(&mutex_);
        }

        bool isLockedByThisThread()
        {
            return holder_==bokket::CurrentThread::tid();
        }

        void assertLocked()
        {
            assert(isLockedByThisThread());
        }

        void lock() {
            pthread_mutex_lock(&mutex_);
            holder_=bokket::CurrentThread::tid();
        }

        void unlock() {
            holder_=0;
            pthread_mutex_unlock(&mutex_);
        }

        pthread_mutex_t *get() {
            return &mutex_;
        }

    private:
        pthread_mutex_t mutex_;
        pid_t holder_;

    private:
        friend class Condition;
    };

    class MutexLockGuard : public noncopyable {
    public:
        explicit MutexLockGuard(MutexLock &mutex)
                : mutex_(mutex) {
            mutex_.lock();
        }

        ~MutexLockGuard() {
            mutex_.unlock();
        }

    private:
        MutexLock &mutex_;
    };
}
#endif //BOKKET_MUTEXLOCK_H
