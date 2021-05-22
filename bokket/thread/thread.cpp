//
// Created by bokket on 2021/5/10.
//

#include <sys/prctl.h>
#include <atomic>
#include "thread.h"
//#include "./util.h"

namespace bokket
{

std::atomic<int> threadCount(0);

static thread_local Thread* t_thread= nullptr;
static thread_local pid_t t_cachedThreadId = 0;
static thread_local std::string t_threadName= "UNKNOW";

pid_t Thread::currentThreadTid() {
    if(t_cachedThreadId==0) {
        t_cachedThreadId = getThreadId();
        t_threadName=std::to_string(t_cachedThreadId);
    }
    return t_cachedThreadId;
}



static bokket::Logger::ptr g_logger = BOKKET_LOG_NAME("system");


Thread * Thread::getThis() {
    return t_thread;
}
const std::string & Thread::getNowThreadName() {
    return t_threadName;
}

void Thread::setName(const std::string &name) {
    if(name.empty()) {
        return ;
    }
    if(t_thread) {
        t_thread->threadName_ = name;
    }
    t_threadName = name;
}

Thread::Thread(std::function<void()> cb, const std::string &name)
              :started_(false),joined_(false)
              ,cb_(std::move(cb))
              ,threadName_(name) {
    if(name.empty()) {
        threadName_ = "UNKNOW" ;
        int num = threadCount.fetch_add(1);
        threadName_=std::to_string(num);
    }
}

void Thread::start() {
    started_= true;
    int ret = pthread_create(&thread_, nullptr,&Thread::run,this);

    if(ret) {
        started_= false;
        BOKKET_LOG_ERROR(g_logger) << " pthread_create failed,return ret="<< ret
                                   <<"name=" <<threadName_;
        throw std::logic_error("pthread_create error");
    }
    semaphore_.wait();
}

Thread::~Thread() {
    /*if(thread_) {
        ::pthread_detach(thread_);
    }*/
    if(started_ && !joined_) {
        ::pthread_detach(thread_);
    }
}

bool Thread::isStarted() {
    return started_;
}

void Thread::join() {
    if(thread_) {
        joined_= true;
        int ret = pthread_join(thread_, nullptr);
        if(ret) {
            joined_= false;
            BOKKET_LOG_ERROR(g_logger) << " pthread_join failed,return ret="<< ret
                                       <<"name=" << threadName_;
            throw std::logic_error("pthread_create error");
        }
        thread_ = 0;
    }
}

void * Thread::run(void *arg) {
    Thread* thread= static_cast<Thread*>(arg);

    t_thread = thread;
    t_threadName = thread->threadName_.empty() ? "bokketThread" : thread->threadName_;
    ::prctl(PR_SET_NAME,bokket::t_threadName.c_str());

    thread->tid_ = bokket::getThreadId();

    //设置该进程线程的名字
    ::pthread_setname_np(::pthread_self(),thread->threadName_.substr(0,15).c_str());

    std::function<void()> cb;
    cb.swap(thread->cb_);

    thread->semaphore_.notify();

    cb();

    return nullptr;
}

}