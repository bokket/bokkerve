//
// Created by bokket on 2021/5/10.
//

#include "fiber.h"

#include <atomic>


#include "../Log/Log.h"

namespace bokket
{

static Logger::ptr g_logger= BOKKET_LOG_NAME("system");

static Config<uint32_t>::ptr g_fiber_stack_size=
        CONFIG()->Lookup("system.fiber_stack_size",static_cast<uint32_t>(128*1024),"system fiber stack size config");

static std::atomic<uint64_t> fiberId_ {0};
static std::atomic<uint64_t> fiberCount_ {0};

static thread_local Fiber* t_fiber = nullptr;
static thread_local Fiber::ptr t_threadFiber = nullptr;

//static std::mutex mutex_;

class MallocStackAllocator {
public:
    static void* Malloc(size_t size) {
        return malloc(size);
    }
    static void Free(void* vp,size_t size) {
        return free(vp);
    }
};


using StackAllocator = MallocStackAllocator;

uint64_t Fiber::getFiberId() {
    if(t_fiber) {
        return t_fiber->getId();
    }
    return 0;
}

void Fiber::setThis(Fiber *fiber) {
    t_fiber=fiber;
}

Fiber::ptr Fiber::getThis() {
    if(t_fiber) {
        return t_fiber->shared_from_this();
    }
    Fiber::ptr main_fiber(new Fiber);
    ASSERT(t_fiber==main_fiber.get());

    t_threadFiber=main_fiber;
    return t_fiber->shared_from_this();
}


void Fiber::yieldToReady() {
    Fiber::ptr cur=getThis();
    ASSERT(cur->status_==Status::READY);
    cur->status_=Status::READY;
    cur->swapOut();
}

void Fiber::yieldToHold() {
    Fiber::ptr cur=getThis();
    ASSERT(cur->status_==Status::HOLD);
    cur->status_=Status::HOLD;
    cur->swapOut();
}

uint64_t Fiber::totalFibers() {
    return fiberCount_;
}


Fiber::Fiber() {
    status_=Status::EXEC;
    if(getcontext(&ctx_)) {
        ASSERT_MSG(false,"getcontext");
    }

    ++fiberCount_;

    BOKKET_LOG_INFO(g_logger)<<"Fiber::Fiber private main,id= "<<id_;
}


Fiber::Fiber(std::function<void()> cb, size_t stacksize, bool useCaller)
            :id_(++fiberId_)
            ,cb_(cb) {

    //std::lock_guard<std::mutex> lc(mutex_);
    ++fiberCount_;

    BOKKET_LOG_INFO(g_logger)<<"fiber stack size="<<g_fiber_stack_size->getValue();

    stackSize_= stacksize ? stacksize : g_fiber_stack_size->getValue();

    stack_ = StackAllocator::Malloc(stacksize);

    if(getcontext(&ctx_)) {
        ASSERT_MSG(false,"getcontext");
    }
    ctx_.uc_link = nullptr;
    ctx_.uc_stack.ss_sp = stack_;
    //ctx_.uc_stack.ss_size = stackSize_;
    ctx_.uc_stack.ss_size=g_fiber_stack_size->getValue();


    if(useCaller == false ) {
        makecontext(&ctx_,&Fiber::run,0);
        BOKKET_LOG_INFO(g_logger)<<"Fiber::Fiber public main id="<<id_<<" (This is Fiber run)";
    } else {
        makecontext(&ctx_,&Fiber::mainRun,0);
        BOKKET_LOG_INFO(g_logger)<<"Fiber::Fiber public main id="<<id_<<" (This is main Fiber run)";
    }

    BOKKET_LOG_DEBUG(g_logger)<<"Fiber::Fiber id="<<id_;
}

Fiber::~Fiber() {
    --fiberCount_;


    if(stack_) {
        //static_assert(  status_ == Status::TERM
        //            ||  status_ == Status::EXCEPT
        //            ||  status_ == Status::INIT,"Fiber status ");
        ASSERT(      status_ == Status::TERM
                    ||  status_ == Status::EXCEPT
                    ||  status_ == Status::INIT);

        StackAllocator::Free(stack_,stackSize_);
    } else {
        //static_assert(!cb_,"functional wrang");
        //static_assert(status_==Status::EXEC,"");
        ASSERT(!cb_);
        ASSERT(status_==Status::EXEC);

        Fiber* cur = t_fiber;
        if(cur == this) {
            setThis(nullptr);
        }
    }
    BOKKET_LOG_DEBUG(g_logger)<<"Fiber::~Fiber id="<<id_ <<" ,fiber total="<<fiberCount_;
}

void Fiber::reset(std::function<void()> cb) {
    ASSERT(stack_);
    ASSERT(  status_==Status::INIT
            || status_==Status::TERM
            || status_==Status::EXCEPT);

    cb_=cb;

    if(getcontext(&ctx_)) {
        ASSERT_MSG(false,"getcontext");
    }

    ctx_.uc_link=nullptr;
    ctx_.uc_stack.ss_sp=stack_;
    ctx_.uc_stack.ss_size=stackSize_;

    makecontext(&ctx_,&Fiber::mainRun,0);
    status_=Status::INIT;

}

void Fiber::call() {
    setThis(this);
    status_=Status::EXEC;
    BOKKET_LOG_INFO(g_logger)<<"Fiber::call"<<" fiber id="<<getFiberId();

    if(swapcontext(&t_threadFiber->ctx_,&ctx_)) {
        ASSERT_MSG(false,"swapcontext");
    }
}

void Fiber::back() {
    setThis(t_threadFiber.get());
    if(swapcontext(&ctx_,&t_threadFiber->ctx_)) {
        ASSERT_MSG(false,"swapcontext");
    }
}


void Fiber::swapIn() {
    setThis(this);

    ASSERT(status_!=Status::EXEC);

    status_=Status::EXEC;
    if(swapcontext(&Scheduler::getMainFiber()->ctx_,&ctx_)) {
        ASSERT_MSG(false,"swapcontext");
    }
}

void Fiber::swapOut() {
    setThis(Scheduler::getMainFiber());

    if(swapcontext(&ctx_,&Scheduler::getMainFiber()->ctx_)) {
        ASSERT_MSG(false,"swapcontext");
    }
}


void Fiber::run() {
    Fiber::ptr cur=getThis();
    ASSERT(cur);
    try {
        cur->cb_();
        cur->cb_= nullptr;
        cur->status_=Status::TERM;
    } catch (std::exception& ex) {
        cur->status_=Status::EXCEPT;
        BOKKET_LOG_ERROR(g_logger)<<"Fiber Except:"<<ex.what()<<" ,fiber id="<<cur->getId()
        <<std::endl<<backTraceToString();
        cur->status_=Status::EXCEPT;

    } catch(...) {
        cur->status_=Status::EXCEPT;
        BOKKET_LOG_ERROR(g_logger)<<"Fiber Except:"<<" ,fiber id="<<cur->getId()
        <<std::endl<<backTraceToString();

    }

    auto raw_ptr=cur.get();

    cur.reset();
    raw_ptr->swapOut();


    ASSERT_MSG(false,"never reach fiber id="+std::to_string(raw_ptr->getId()));
}


void Fiber::mainRun() {
    Fiber::ptr cur=getThis();
    ASSERT(cur);

    try {
        cur->cb_();
        cur->cb_= nullptr;
        cur->status_=Status::TERM;
    } catch (std::exception& ex) {
        cur->status_=Status::EXCEPT;
        BOKKET_LOG_ERROR(g_logger)<<"Fiber Except:"<<ex.what()<<" ,fiber id="<<cur->getId()
        <<std::endl<<backTraceToString();

    } catch(...) {
        cur->status_=Status::EXCEPT;
        BOKKET_LOG_ERROR(g_logger)<<"Fiber Except:"<<" ,fiber id="<<cur->getId()
        <<std::endl<<backTraceToString();
    }

    auto raw_ptr=cur.get();

    cur.reset();
    raw_ptr->back();

    ASSERT_MSG(false,"never reach fiber id="+std::to_string(raw_ptr->getId()));
}

}