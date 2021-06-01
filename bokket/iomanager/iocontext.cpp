//
// Created by bokket on 2021/6/1.
//

#include "iocontext.h"

namespace bokket
{

static bokket::Logger::ptr g_logger= BOKKET_LOG_NAME("system");

IOContext::IOContext(int fd)
                    :fd_(fd)
{}

Context & IOContext::getContext(Event event) {
    switch (event) {
        case Event::READ:
            return context_[0];
        case Event::WRITE:
            return context_[1];
        default:
            ASSERT_MSG(false,"IOContext::getContext");
            break;
    }
    throw std::invalid_argument("IOContext::get invalid event");
}

void IOContext::resetContext(Event event) {
    switch (event) {
        case Event::READ:
            context_[0].scheduler= nullptr;
            context_[0].fiber.reset();
            context_[0].cb = nullptr;
            break;
        case Event::WRITE:
            context_[0].scheduler= nullptr;
            context_[0].fiber.reset();
            context_[0].cb = nullptr;
            break;
        default:
            ASSERT_MSG(false,"IOContext::resetContext");
            break;
    }
    throw std::invalid_argument("IOContext::reset invalid event");
}

IOContext::reset() {
    reset(Event::READ);
    reset(Event::WRITE);
}

void IOContext::triggerEvent(Event event) {

    ASSERT(events_&event);

    events_=events_& ~ event;

    Context& ctx=getContext(event);

    if(ctx.cb) {
        ctx.scheduler->schedule(&ctx.cb);
    } else {
        ctx.scheduler->schedule(&ctx.fiber);
    }

    ctx.scheduler= nullptr;
    return;
}

}