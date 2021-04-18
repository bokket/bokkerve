//
// Created by bokket on 2021/2/21.
//

#ifndef BOKKERVE_THREAD_H
#define BOKKERVE_THREAD_H

#include "noncopyable.h"
#include "CountDownLatch.h"
#include <atomic>
#include <functional>
#include <string>
#include <pthread.h>
using namespace std;

namespace bokket
{

class Thread: public noncopyable
{
public:
    using ThreadFunc=function<void()> ;

public:
    explicit Thread(const ThreadFunc&,const string& name=string());
    ~Thread();

    void start();
    int join();
    bool started() { return started_; }
    pid_t getTid() const { return tid_; }
    const string & name() const { return name_; }

    //std::atomic<T>::load
    // 返回当前值
    static int threadNumCreated() { return numCreated_.load(); }

private:

    static void* startThread(void* thread);
    void runInThread();

    bool started_;
    bool joined_;
    pthread_t pthreadId_;
    ThreadFunc func_;
    string name_;
    pid_t tid_;
    CountDownLatch latch_;

    static atomic<int> numCreated_;
};


}

#endif //BOKKERVE_THREAD_H
