//
// Created by bokket on 2021/5/11.
//

#include "../Log/Log.h"
#include "../Fiber/fiber.h"

namespace bokket
{

static bokket::Logger::ptr g_logger ;

pid_t getThreadId() {
    return ::syscall(SYS_gettid);
}

uint32_t getFiberId() {
    //return bokket::Fiber::getFiberId();
    return 0;
}



}