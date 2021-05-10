//
// Created by bokket on 2021/5/10.
//

#include "fiber.h"
#include <atomic>

namespace bokket
{

static std::atomic<uint64_t> fiberId_ {0};
static std::atomic<uint64_t> fiberCount_ {0};

static thread_local Fiber* fiber_ = nullptr;
static thread_local Fiber::ptr threadFiber_ = nullptr;

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
    if(fiber_) {
        return fiber_->getId();
    }
    return 0;
}

Fiber::Fiber() {
    status_=Status::EXEC;
    setThis(this);

    if(getcontext(&ctx_)) {

    }

    ++fiberCount_;
}

Fiber::Fiber(std::function<void()> cb, int stacksize, bool use)
            :id_(++fiberId_),cb_(cb) {
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

        Fiber* cur = fiber_;
        if(cur == this) {
            setThis(nullptr);
        }
    }
}

void Fiber::reset(std::function<void()> cb) {

}

}