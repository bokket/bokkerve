//
// Created by bokket on 2021/5/10.
//

#include "fiber.h"

#include <atomic>


#include "../Log/Log.h"

namespace bokket
{

static Logger::ptr g_logger= BOKKET_LOG_NAME("system");

static std::atomic<uint64_t> fiberId_ {0};
static std::atomic<uint64_t> fiberCount_ {0};

static thread_local Fiber* t_fiber = nullptr;
static thread_local Fiber::ptr t_threadFiber = nullptr;

class MallocStackAllocator {
public:
    static void* Alloc(size_t size) {
        return malloc(size);
    }
    static void Dealloc(void* vp,size_t size) {
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

Fiber::ptr Fiber::getThis() {
    if(t_fiber) {
        return t_fiber->shared_from_this();
    }
    Fiber::ptr main_fiber(new Fiber);

    t_threadFiber=main_fiber;
    return t_fiber->shared_from_this();
}


void Fiber::yieldToReady() {
    Fiber::ptr cur=getThis();
    cur->status_=Status::READY;
    cur->swapOut();
}

void Fiber::yieldToHold() {
    Fiber::ptr cur=getThis();
    cur->status_=Status::HOLD;
    cur->swapOut();
}

uint64_t Fiber::totalFibers() {
    return fiberCount_;
}

Fiber::Fiber() {
    status_=Status::EXEC;
    setThis(this);

    if(getcontext(&ctx_)) {

    }

    ++fiberCount_;

    BOKKET_LOG_DEBUG(g_logger)<<"Fiber::Fiber main,id="<<id_;
}

Fiber::Fiber(std::function<void()> cb, size_t stacksize, bool useCaller)
            :id_(++fiberId_)
            ,cb_(cb) {
    ++fiberCount_;
    stackSize_= stacksize ? stacksize : 128*1024;

    stack_ = StackAllocator::Alloc(stacksize);

    if(getcontext(&ctx_)) {

    }
    ctx_.uc_link = nullptr;
    ctx_.uc_stack.ss_sp = stack_;
    ctx_.uc_stack.ss_size = stackSize_;

    if(use == nullptr ) {
        makecontext(&ctx_,&Fiber::mainFunc,0);
    } else {
        makecontext(&ctx_,&Fiber::callerMainFunc,0);
    }

    BOKKET_LOG_DEBUG(g_logger)<<"Fiber::Fiber id="<<id_;
}

Fiber::~Fiber() {
    --fiberCount_;
    if(stack_) {
        static_assert(  status_ == Status::TERM
                    ||  status_ == Status::EXCEPT
                    ||  status_ == Status::INIT,"Fiber status ");
        StackAllocator::Dealloc(stack_,stackSize_);
    } else {
        static_assert(!cb_,"functional wrang");
        static_assert(status_==Status::EXEC,"");

        Fiber* cur = t_fiber;
        if(cur == this) {
            setThis(nullptr);
        }
    }
    BOKKET_LOG_DEBUG(g_logger)<<"Fiber::~Fiber id="<<id_;
}

void Fiber::reset(std::function<void()> cb) {
    cb_=cb;

    if(getcontext(&ctx_)) {

    }

    ctx_.uc_link=nullptr;
    ctx_.uc_stack.ss_sp=stack_;
    ctx_.uc_stack.ss_size=stackSize_;

    makecontext(&ctx_,&Fiber::mainFunc,0);
    status_=Status::INIT;

}

void Fiber::call() {
    setThis(this);
    status_=Status::EXEC;
    BOKKET_LOG_ERROR(g_logger)<<getFiberId();

    if(swapcontext(&ctx_,&t_threadFiber->ctx_)) {

    }
}

void Fiber::swapIn() {
    setThis(this);

    status_=Status::EXEC;
    if(swapcontext(&,&ctx_)) {

    }
}

void Fiber::swapOut() {

}


void Fiber::mainFunc() {
    Fiber::ptr cur=getThis();

    try {
        cur->cb_();
        cur->cb_= nullptr;
        cur->status_=Status::TERM;
    } catch (std::exception& ex) {
        cur->status_=Status::EXCEPT;

    } catch(...) {
        cur->status_=Status::EXCEPT;

    }

    auto raw_ptr=cur.get();

    cur.reset();
    raw_ptr->swapOut();

}


void Fiber::callerMainFunc() {
    Fiber::ptr cur=getThis();

    try {
        cur->cb_();
        cur->cb_= nullptr;
        cur->status_=Status::TERM;
    } catch (std::exception& ex) {
        cur->status_=Status::EXCEPT;

    } catch(...) {
        cur->status_=Status::EXCEPT;

    }

    auto raw_ptr=cur.get();

    cur.reset();
    raw_ptr->back();

}

}