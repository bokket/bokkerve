//
// Created by bokket on 2021/5/10.
//

#include <sys/prctl.h>

#include "thread.h"
#include "util.h"

namespace bokket
{

thread_local Thread* t_thread= nullptr;
thread_local int t_cachedThreadId = 0;
thread_local std::string t_threadName= "UNKNOW";

void cacheThreadId() {
    if(t_cachedThreadId == 0) {
        t_cachedThreadId = getThreadId();
        t_threadName = std::to_string(t_cachedThreadId);
    }
}

static bokket::Logger::ptr g_logger ;

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
              :cb_(cb),threadName_(name) {
    if(name.empty()) {
        threadName_ = "UNKNOW" ;
    }



    int ret = pthread_create(&thread_, nullptr,&Thread::run,this);

    if(ret) {
        BOKKET_LOG_ERROR(g_logger) << " pthread_create failed,return ret="<< ret
        <<"name=" <<name;
        throw std::logic_error("pthread_create error");
    }
    semaphore_.wait();
}

Thread::~Thread() {
    if(thread_) {
        pthread_detach(thread_);
    }
}

void Thread::join() {
    if(thread_) {
        int ret = pthread_join(thread_, nullptr);
        if(ret) {
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

    thread->tid_ = bokket::threadId();

    //设置该进程线程的名字
    ::pthread_setname_np(::pthread_self(),thread->threadName_.substr(0,15).c_str());

    std::function<void()> cb;
    cb.swap(thread->cb_);

    thread->semaphore_.notify();

    cb();

    return nullptr;
}

}