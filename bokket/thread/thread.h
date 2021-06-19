//
// Created by bokket on 2021/5/10.
//

#ifndef BOKKERVE_THREAD_H
#define BOKKERVE_THREAD_H


#include <memory>
#include <string>
#include <functional>

#include <pthread.h>



#include "./Semaphore.h"
#include "./util.h"
#include "../Log/Log.h"
#include "../base/noncopyable.h"
namespace bokket
{

class Thread: public noncopyable
{
public:
    using ptr=std::shared_ptr<Thread>;
public:
    Thread(std::function<void()> cb,const std::string & name);
    ~Thread();

    pid_t getId() const { return tid_; }

    const std::string & getName() const { return threadName_; }


    bool isStarted();
    bool joinable();
    void start();
    void join();

    static pid_t currentThreadTid();

    static Thread* getThis();
    static const std::string& getNowThreadName();
    static void setName(const std::string& name);
private:
    static void *run(void* arg);
private:
    bool started_=false;
    bool joined_= false;

    pid_t tid_;
    pthread_t thread_;
    std::function<void()> cb_;
    std::string threadName_;
    Semaphore semaphore_;
};


}




#endif //BOKKERVE_THREAD_H
