//
// Created by bokket on 2021/5/10.
//

#include "thread.h"

namespace bokket
{

static thread_local Thread* t_thread= nullptr;
static thread_local std::string t_threadName= "UNKNOW";

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
}