//
// Created by bokket on 2021/5/12.
//

#include "scheduler.h"

namespace bokket
{

static Logger::ptr g_logger=BOKKET_LOG_NAME("system");

static thread_local Scheduler* t_scheduler= nullptr;
static thread_local Fiber* t_scheduler_fiber= nullptr;

Fiber * Scheduler::getMainFiber() {
    return t_scheduler_fiber;
}

}