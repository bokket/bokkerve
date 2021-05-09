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
    typedef std::shared_ptr<Fiber> ptr;

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
    uint64_t id_ = 0;
    uint32_t stackSize_=0;

    Status status_ = Status::INIT;

    ucontext_t ctx_;

    void *stack_ = nullptr;
    std::function<void()> cb_;
};
}


#endif //BOKKERVE_FIBER_H
