//
// Created by bokket on 2021/5/12.
//

#ifndef BOKKERVE_SCHEDULER_H
#define BOKKERVE_SCHEDULER_H

#include <memory>
#include <vector>
#include <list>
#include <mutex>

#include "../Log/Log.h"
#include "../Fiber/fiber.h"
#include "../thread/thread.h"
namespace bokket
{

class Scheduler
{
public:
    using ptr= std::shared_ptr<Scheduler>;

public:
    Scheduler(size_t threads=1,bool useCaller=true,const std::string& name="UNKNOW");

    virtual ~Scheduler();

public:
    const std::string& getName() const { return name_; }
    static Scheduler* getThis();
    static Fiber* getMainFiber();

    void start();
    void stop();


    template<class T>
    void schedule(T fc) {
        bool need_tickle= false;
        {
            std::lock_guard<std::mutex> lockGuard(mutex_);
            need_tickle=scheduleNoLock(fc);
        }
        if(need_tickle) {
            tickle();
        }
    }

    template<class Iterator>
    void schedule(Iterator begin,Iterator end) {
        bool need_tickle= false;
        {
            std::lock_guard <std::mutex> lockGuard(mutex_);
            while (begin != end) {
                need_tickle=scheduleNoLock(&*begin) || need_tickle;
                ++begin;
            }
        }
        if(need_tickle) {
            tickle();
        }
    }


protected:
    virtual void tickle();
    void run();
    virtual bool stopping();
    virtual void idle();

    void setThis();

    bool hasIdleThreads() { return idleThreadCount>0; }

private:
    template<class T>
    bool scheduleNoLock(T fc,int thread) {
        bool need_tickle=fibers_.empty();
        //FiberAndThread fat(fc,thread);
        FiberOrCb ft(fc);
        if(ft.fiber_ || ft.cb_ ) {
            fibers_.push_back(ft);
        }
        return need_tickle;
    }

private:
    struct FiberOrCb {
        Fiber::ptr fiber_;
        std::function<void()> cb_;
        //int thread_;
        /*FiberOrCb(Fiber::ptr fiber,int thread)
                      :fiber_(fiber),thread_(thread)
        {}

        FiberOrCb(Fiber::ptr* fiber,int thread)
                 :thread_(thread)
        {
            fiber_.swap(*fiber);
        }*/
        FiberOrCb() {}

        FiberOrCb(Fiber::ptr fiber)
                 :fiber_(fiber) {}

        FiberOrCb(std::function<void()> cb)
                 :cb_(cb)

                //:thread_(-1)
        {}

        void reset() {
            fiber_= nullptr;
            cb_= nullptr;
            //thread_=-1;
        }
    };
private:
    std::mutex mutex_;
    std::vector<Thread::ptr> threads_;
    std::list<FiberOrCb> fibers_;

    Fiber::ptr rootFiber_;

    std::string name_;
protected:
    std::vector<int> threadIds_;
    size_t threadCount_=0;
    std::atomic<size_t> activeThreadCount={0};
    std::atomic<size_t> idleThreadCount={0};

    bool stopping_= true;
    bool autoStop_= false;

    int rootThread_ =0;

};

}


#endif //BOKKERVE_SCHEDULER_H
