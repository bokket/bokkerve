//
// Created by bokket on 2021/5/12.
//

#ifndef BOKKERVE_SCHEDULER_H
#define BOKKERVE_SCHEDULER_H

#include <memory>
#include <vector>
#include <list>
#include <mutex>


#include "../Fiber/fiber.h"
#include "../thread/thread.h"
namespace bokket
{

class Scheduler
{
public:
    using ptr= std::shared_ptr<Scheduler>;

public:
    Scheduler(size_t threads=1,bool useCaller=true,const std::string& name="");

    virtual ~Scheduler();

public:
    const std::string& getName() const { return name_; }
    static Scheduler* getThis();
    static Fiber* getMainFiber();

    void start();
    void stop();


    template<class FiberOrCb>
    void schedule(FiberOrCb fc,int thread=-1) {
        bool need_tickle= false;
        {
            std::lock_guard<std::mutex> lockGuard(mutex_);
            need_tickle=scheduleNoLock(fc,thread);
        }
        if(need_tickle) {
            tickle();
        }
    }

    template<class InputIterator>
    void schedule(InputIterator begin,InputIterator end) {
        bool need_tickle= false;
        {
            std::lock_guard <std::mutex> lockGuard(mutex_);
            while (begin != end) {
                need_tickle=scheduleNoLock(&*begin,-1) || need_tickle;
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
    template<class FiberOrCb>
    bool scheduleNoLock(FiberOrCb fc,int thread) {
        bool need_tickle=fibers_.empty();
        FiberAndThread fat(fc,thread);
        if(fat.fiber_ || fat.cb_ ) {
            fibers_.push_back(fat);
        }
        return need_tickle;
    }

private:
    struct FiberAndThread {
        Fiber::ptr fiber_;
        std::function<void()> cb_;
        int thread_;
        FiberAndThread(Fiber::ptr fiber,int thread)
                      :fiber_(fiber),thread_(thread)
        {}

        FiberAndThread(Fiber::ptr* fiber,int thread)
                      :thread_(thread)
        {
            fiber_.swap(*fiber);
        }

        FiberAndThread()
                      :thread_(-1)
        {}

        void reset() {
            fiber_= nullptr;
            cb_= nullptr;
            thread_=-1;
        }
    };
private:
    std::mutex mutex_;
    std::vector<Thread::ptr> threads_;
    std::list<FiberAndThread> fibers_;

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
