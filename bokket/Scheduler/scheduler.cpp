//
// Created by bokket on 2021/5/12.
//

#include "scheduler.h"

namespace bokket
{

static Logger::ptr g_logger=BOKKET_LOG_NAME("system");

static thread_local Scheduler* t_scheduler= nullptr;
static thread_local Fiber* t_scheduler_fiber= nullptr;

/*
Scheduler::Scheduler(size_t threadCount, bool useCaller, const std::string &name)
                    :name_(name)
{
    ASSERT(threadCount>0);

    if(useCaller) {
        //hook_enable();
        Fiber::getThis();
        --threadCount;

        ASSERT(getThis()== nullptr);
        t_scheduler=this;

        //rootFiber_.reset(new Fiber(std::bind(&Scheduler::run,this), true));
        rootFiber_.reset(new Fiber(std::bind(&Scheduler::run,this), 0, false));
        bokket::Thread::setName(name_);

        t_scheduler_fiber=rootFiber_.get();
        rootThread_=static_cast<int>(bokket::getThreadId());
        //rootThread_=bokket::getThreadId();
        threadIds_.emplace_back(rootThread_);
    } else
        rootThread_=-1;


    threadCount_=threadCount;
}*/

Scheduler::~Scheduler() {
    BOKKET_LOG_DEBUG(g_logger)<<"Scheduler::~Scheduler()";
    ASSERT(stopping_);
    if(getThis()==this)
        t_scheduler= nullptr;
}

/*
void Scheduler::start() {
    std::lock_guard<std::mutex> lc(mutex_);
    if(!stopping_)
        return;

    stopping_= false;

    ASSERT(threads_.empty());

    threads_.resize(threadCount_);

    for(auto i=0;i<threadCount_;++i) {
        threads_[i].reset(new Thread(std::bind(&Scheduler::run,this),
                                     name_+"_"+std::to_string(i)));
        threadIds_.emplace_back(static_cast<int>(threads_[i]->getId()));
    }
}*/

/*
void Scheduler::stop() {
    autoStop_= true;

    if(rootFiber_
    &&threadCount_==0
    && (rootFiber_->getStatus()==Fiber::Status::TERM
    || rootFiber_->getStatus()==Fiber::Status::INIT)) {
        BOKKET_LOG_INFO(g_logger)<<this<<"stopped";
        stopping_= true;

        if(stopping())
            return;
    }

    if(rootThread_!=-1)
        ASSERT(getThis()==this);
    else
        ASSERT(getThis()!=this);


    stopping_= true;

    for(auto i=0;i<threadCount_;++i)
        tickle();

    if(rootFiber_)
        tickle();


    if(rootFiber_ && !stopping_)
        rootFiber_->call();

    std::vector<Thread::ptr> threads;
    {
        std::lock_guard<std::mutex> lc(mutex_);
        threads.swap(threads_);
    }

    for(auto& i:threads)
        i->join();
}*/

std::string Scheduler::toString() {
    std::stringstream ss;

    ss<<"[Scheduler name="<<name_
    <<" ,thread_count="<<threadCount_
    <<" ,active_count="<<activeThreadCount
    <<" ,idle_count="<<idleThreadCount
    <<" ,stopping="<<stopping_
    <<"]";
    return ss.str();
}

void Scheduler::setThis() {
    //t_scheduler=scheduler;
    t_scheduler=this;
}

Scheduler * Scheduler::getThis() {
    return t_scheduler;
}

Fiber * Scheduler::getMainFiber() {
    return t_scheduler_fiber;
}


void Scheduler::tickle() {
    BOKKET_LOG_INFO(g_logger)<<"Scheduler tickle";
}


bool Scheduler::stopping() {
    std::lock_guard<std::mutex> lc(mutex_);
    /*return autoStop_
        && stopping_
        && fibers_.empty()
        && activeThreadCount==0;*/
    return    stopping_
           && tasks_.empty()
           && activeThreadCount==0;
}

void Scheduler::idle() {
    BOKKET_LOG_INFO(g_logger)<<name_<<"idle running";
    while(!stopping())
        Fiber::getThis()->yield();
        //Fiber::yieldToHold();
}
/*
void Scheduler::run() {
    BOKKET_LOG_DEBUG(g_logger)<<name_<<"running";

    //setThis(this);
    setThis();

    if(bokket::getThreadId() != rootThread_)
        t_scheduler_fiber=Fiber::getThis().get();

    Fiber::ptr idle_fiber(new Fiber(std::bind(&Scheduler::idle,this)));
    Fiber::ptr cb_fiber;

    SchedulerTask task;
    while(true) {
        task.reset();

        bool tickle_me=false;
        //bool is_active=false;

        {
            std::lock_guard<std::mutex> lc(mutex_);

            auto it=tasks_.begin();

            while(it!=fibers_.end()) {
                ASSERT(it->fiber_ || it->cb_);



                if(it->fiber && it->fiber->getStatus()==Fiber::Status::EXEC) {
                    ++it;
                    tickle_me= true;
                    continue;
                }
                ft=*it;
                fibers_.erase(it++);
                ++activeThreadCount;
                is_active=true;
                break;
            }
            tickle_me= it!=fibers_.end();
        }
        if(tickle_me)
            tickle();

        if(ft.fiber && ft.fiber->getStatus()!=Fiber::Status::TERM
                     && ft.fiber->getStatus()!=Fiber::Status::EXCEPT) {
            ft.fiber->swapIn();
            activeThreadCount--;
            if(ft.fiber->getStatus()!=Fiber::Status::TERM)
                //schedule(ft.fiber);
                ft.fiber->setStatus(Fiber::Status::INIT);
                //ft.fiber_->getStatus()==Fiber::Status::INIT;
        } else if(ft.cb) {
            if(cb_fiber)
                cb_fiber->reset(ft.cb);
            else
                cb_fiber.reset(new Fiber(ft.cb));

            //ft.reset();
            cb_fiber->swapIn();
            activeThreadCount--;

            if(cb_fiber->getStatus()==Fiber::Status::TERM)
                cb_fiber->reset(nullptr);
            else {
               // cb_fiber->getStatus()=Fiber::Status::INIT;
                cb_fiber->setStatus(Fiber::Status::INIT);
                cb_fiber.reset();
            }
        } else {
            if(is_active) {
                activeThreadCount--;
                continue;
            }
            if(idle_fiber->getStatus()==Fiber::Status::TERM) {
                BOKKET_LOG_INFO(g_logger)<<"idle fiber terminate";
                break;
            }
            ++idleThreadCount;
            //idle_fiber->swapIn();
            idle_fiber->resume();
            --idleThreadCount;
            //if(idle_fiber->getStatus()!=Fiber::Status::TERM)
            //    idle_fiber->getStatus()==Fiber::Status::READY;
        }
    }
    BOKKET_LOG_DEBUG(g_logger)<<"Scheduler::run() exit";
}*/


}