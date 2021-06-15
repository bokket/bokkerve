//
// Created by bokket on 2021/5/12.
//

#ifndef BOKKERVE_SCHEDULER_H
#define BOKKERVE_SCHEDULER_H

#include <memory>
#include <vector>
#include <list>
#include <mutex>


#include "../Hook/hook.h"
#include "../Fiber/fiber.h"
#include "../thread/thread.h"

namespace bokket
{

class Scheduler
{


public:
    using ptr= std::shared_ptr<Scheduler>;

public:
    Scheduler(size_t threadCount=1,bool useCaller=true,const std::string& name="Scheduler");

    virtual ~Scheduler();

public:
    const std::string& getName() const { return name_; }
    static Scheduler* getThis();

    static Fiber* getMainFiber();
    void start();
    void stop();

    std::string toString();


    template<class T>
    void schedule(T fc,int thread=-1) {
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

    //void setThis(Scheduler* scheduler);
    void setThis();

    bool hasIdleThreads() { return idleThreadCount>0; }

private:
    template<class T>
    bool scheduleNoLock(T t,int thread) {
        bool need_tickle=tasks_.empty();
        //FiberAndThread fat(fc,thread);
        //FiberOrCb ft(fc);
        SchedulerTask task(t,thread);
        if(task.fiber_ || task.cb_ ) {
            tasks_.push_back(task);
        }
        return need_tickle;
    }

private:
    struct SchedulerTask {
        public:
        Fiber::ptr fiber_;
        std::function<void()> cb_;
        int thread_;
        SchedulerTask(Fiber::ptr fiber,int thread)
                      :fiber_(fiber),thread_(thread)
        {}

        SchedulerTask(Fiber::ptr* fiber,int thread)
                 :thread_(thread)
        {
            fiber_.swap(*fiber);
        }
        /*FiberOrCb() {}

        FiberOrCb(Fiber::ptr f)
                 :fiber(f) {}
        */
        SchedulerTask(std::function<void()> b,int thread)
                 :cb_(cb)
                 ,thread_(thread)
        {}

        SchedulerTask() {
            thread_=-1;
        }

        void reset() {
            fiber= nullptr;
            cb= nullptr;
            thread_=-1;
        }
    };
private:
    std::mutex mutex_;
    std::vector<Thread::ptr> threads_;
    std::list<SchedulerTask> tasks_;

    Fiber::ptr rootFiber_;

    std::string name_;
private:
    std::vector<int> threadIds_;
    size_t threadCount_=0;
    std::atomic<size_t> activeThreadCount={0};
    std::atomic<size_t> idleThreadCount={0};

    bool stopping_= false;
    //bool autoStop_= false;

    bool useCaller_;

    int rootThread_ =0;

};

}


#endif //BOKKERVE_SCHEDULER_H
