//
// Created by bokket on 2021/6/1.
//

#include "iocontext.h"

#include "../thread/Assert.h"

namespace bokket
{

static bokket::Logger::ptr g_logger= BOKKET_LOG_NAME("system");

IOContext::IOContext(int fd)
                    :fd_(fd)
{}


IOContext::Context & IOContext::getContext(Event event) {
    switch (event) {
        case Event::READ:
            return read_;
            //return context_[0];
        case Event::WRITE:
            return write_;
            //return context_[1];
        default:
            ASSERT_MSG(false,"IOContext::getContext");
            break;
    }
    throw std::invalid_argument("IOContext::get invalid event");
}

/*void IOContext::resetContext(Event event) {
    switch (event) {
        case Event::READ:
            context_[0].scheduler= nullptr;
            context_[0].fiber.reset();
            context_[0].cb = nullptr;
            break;
        case Event::WRITE:
            context_[1].scheduler= nullptr;
            context_[1].fiber.reset();
            context_[1].cb = nullptr;
            break;
        default:
            ASSERT_MSG(false,"IOContext::resetContext");
            break;
    }
    throw std::invalid_argument("IOContext::reset invalid event");
}*/

void IOContext::resetContext(Context &ctx) {
    ctx.scheduler= nullptr;
    ctx.cb= nullptr;
    ctx.fiber.reset();
}

/*IOContext::reset() {
    resetContext(Event::READ);
    resetContext(Event::WRITE);
}*/

void IOContext::triggerEvent(Event event) {

    ASSERT(events_&event);

    events_=(IOContext::Event)(events_& ~ event);

    Context& ctx=getContext(event);

    if(ctx.cb) {
        ctx.scheduler->schedule(ctx.cb);
    } else {
        ctx.scheduler->schedule(ctx.fiber);
        //ctx.fiber.reset();
    }

    resetContext(ctx);
    //resetContext(event);
    //ctx.scheduler= nullptr;
    return;
}

}