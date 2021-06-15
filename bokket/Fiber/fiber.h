//
// Created by bokket on 2021/5/10.
//

#ifndef BOKKERVE_FIBER_H
#define BOKKERVE_FIBER_H

#include <memory>
#include <functional>
#include <ucontext.h>

#include "../Log/Log.h"
#include "../Config/config.h"
#include "../thread/Assert.h"

namespace bokket
{
class Scheduler;    
class Fiber: public std::enable_shared_from_this<Fiber>
{
    friend class Scheduler;
public:
    using ptr = std::shared_ptr<Fiber>;
    enum class Status
    {
        //INIT,
        //HOLD,
        READY,
        EXEC,
        TERM,
        //READY,
        EXCEPT
    };

private:

    Fiber();

public:
    Fiber(std::function<void()>cb, size_t stacksize =0,bool runInScheduler = false);
    //Fiber(std::function<void()>cb, size_t stacksize =0);
    ~Fiber();

    void reset(std::function<void()> cb);

    //执行当前携程
    /*void swapIn();

    //切换到当前线程的协程
    void swapOut();

    void call();

    void back();*/
    void yield();
    void resume();

    uint64_t getId() const { return id_; }

    Status getStatus() const { return status_; }

    void setStatus(Status status) { status_=status; }
public:
    static void setThis(Fiber* fiber);

    static Fiber::ptr getThis();

    //static void yieldToReady();
    //static void yield();
    //static void yieldToHold();

    static uint64_t totalFibers();

    //static void run();

    static void mainRun();

    static uint64_t getFiberId();
private:
    
    uint64_t id_ ;
    uint32_t stackSize_;

    Status status_ = Status::READY;

    ucontext_t ctx_;

    void *stack_ = nullptr;
    std::function<void()> cb_;

    bool runInScheduler_;
};
}


#endif //BOKKERVE_FIBER_H
