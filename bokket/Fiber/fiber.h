//
// Created by bokket on 2021/5/10.
//

#ifndef BOKKERVE_FIBER_H
#define BOKKERVE_FIBER_H

#include <memory>
#include <functional>
#include <ucontext.h>

namespace bokket
{
class Fiber: public std::enable_shared_from_this<Fiber>
{
public:
    using ptr = std::shared_ptr<Fiber>;
    enum class Status :uint8_t
    {
        INIT,
        HOLD,
        EXEC,
        TERM,
        READY,
        EXCEPT
    };

private:
    Fiber();

public:
    Fiber(std::function<void()>cb, size_t stacksize =0,bool use = false);
    ~Fiber();

    void reset(std::function<void()> cb);

    void swapIn();

    void swapOut();

    void call();

    void back();

    uint64_t getId() const { return id_; }

    Status getStatus() const { return status_; }
public:
    static void setThis(Fiber* fiber);

    static Fiber::ptr getThis();

    static void yieldToReady();

    static uint64_t totalFibers();

    static void mainFunc();

    static void callerMainFunc();

    static uint64_t getFiberId();
private:
    uint64_t id_ = 0;
    uint32_t stackSize_=0;

    Status status_ = Status::INIT;

    ucontext_t ctx_;

    void *stack_ = nullptr;
    std::function<void()> cb_;
};
}


#endif //BOKKERVE_FIBER_H
