//
// Created by bokket on 2021/2/21.
//

#include "Thread.h"
#include "CurrentThread.h"
#include <string>
#include <assert.h>

using namespace std;

namespace bokket {


    namespace CurrentThread
    {
        // __thread修饰的变量是线程局部存储的
        thread_local int t_cachedTid = 0;
        // 线程真实pid（tid）的缓存，
        // 是为了减少::syscall(SYS_gettid)系统调用的次数
        // 提高获取tid的效率
        thread_local string t_tidString="UNKOWN";
    }
    namespace detail
    {
        pid_t gettid()
        {
            return static_cast<pid_t>(::syscall(SYS_gettid));
        }

    }
}


using namespace bokket;

atomic<int> numCreated_(0);

void CurrentThread::cacheTid()
{
    if (t_cachedTid == 0)
    {
        t_cachedTid = bokket::detail::gettid();
        t_tidString = std::to_string(t_cachedTid) + '\0';
    }
}

bool CurrentThread::isMainThread()
{
    //判断这个线程的tid是不是主线程的pid，如果是，就说明是主线程
    return tid()==::getpid();
}


Thread::Thread(const ThreadFunc & func, const string &name)
              :started_(false)
              ,joined_(false)
              ,pthreadId_(0)
              ,tid_(0)
              ,func_(func)
              ,name_(name)
              ,latch_(1)
{
    numCreated_++;
}

Thread::~Thread()
{
    if(started_&& !joined_)
        pthread_detach(pthreadId_);
        //pthread_join(pthreadId_, nullptr);
}


void Thread::start()
{
    //如果started_为假就继续
    assert(!started_);
    started_= true;
    errno=pthread_create(&pthreadId_, nullptr,&startThread,this);
    if(errno!=0)
    {
        return;
    }
}

int Thread::join()
{
    assert(started_);
    assert(!joined_);
    joined_= true;

    return pthread_join(pthreadId_, nullptr);
}

void * Thread::startThread(void *thread)
{
    Thread* thread_=static_cast<Thread*>(thread);

    thread_->runInThread();
    return nullptr;
}

void Thread::runInThread()
{
    tid_=CurrentThread::tid();
    latch_.countDown();
    try {
        func_();
    }
    catch (const std::exception& ex) {
        fprintf(stderr, "exception caught in Thread %s ID :%d\n", CurrentThread::tidString(),CurrentThread::tid());
        fprintf(stderr, "reason: %s\n", ex.what());
        abort();
    }
    catch (...) {
        fprintf(stderr, "unknown exception caught in Thread %d\n", CurrentThread::tid());
        throw; // rethrow
    }
}

