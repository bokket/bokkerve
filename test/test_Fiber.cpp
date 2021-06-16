//
// Created by bokket on 2021/6/3.
//

/*#include "../bokket/Fiber/fiber.h"
#include "../bokket/thread/thread.h"*/
#include "../bokket/bokket.h"
//#include "../bokket/Scheduler/scheduler.h"

#include "../bokket/Log/Log.h"

#include <iostream>
#include <vector>
static bokket::Logger::ptr g_logger = BOKKET_LOG_NAME("test");

void run_in_fiber2() {
    BOKKET_LOG_INFO(g_logger) << "run_in_fiber2 begin";
    BOKKET_LOG_INFO(g_logger) << "run_in_fiber2 end";
}

void run_in_fiber() {
    /*BOKKET_LOG_INFO(g_logger)<<"run in fiber 1";
    //bokket::Fiber::getThis()->back();
    BOKKET_LOG_INFO(g_logger)<<"run in fiber 2";*/

    BOKKET_LOG_INFO(g_logger)<<"run in fiber begin";
    //bokket::Fiber::yieldToHold();
    bokket::Fiber::getThis()->yield();
    BOKKET_LOG_INFO(g_logger)<<"run in fiber yield";

    BOKKET_LOG_INFO(g_logger)<<"run in fiber end";
    //bokket::Fiber::yieldToHold();
}

void test_fiber() {
    BOKKET_LOG_INFO(g_logger)<<"test_fiber begin ";


    bokket::Fiber::getThis();

    bokket::Fiber::ptr fiber(new bokket::Fiber(run_in_fiber,0, false));

    BOKKET_LOG_INFO(g_logger)<<"fiber ptr use_count="<<fiber.use_count();

    //fiber->call();

    BOKKET_LOG_INFO(g_logger)<<"main before resume";
    fiber->resume();
    BOKKET_LOG_INFO(g_logger)<<"main after resume";
    //fiber->call();
    //BOKKET_LOG_INFO(g_logger)<<"main after call2";
    BOKKET_LOG_INFO(g_logger)<<"fiber ptr use_count="<<fiber.use_count();

    //BOKKET_LOG_INFO(g_logger)<<"fiber status:"<<std::to_string(fiber->getStatus());

    BOKKET_LOG_INFO(g_logger)<<"main resume again";
    fiber->resume();
    BOKKET_LOG_INFO(g_logger)<<"main resume again end";

    BOKKET_LOG_INFO(g_logger)<<"fiber ptr use_count="<<fiber.use_count();

    fiber->reset(run_in_fiber2);
    fiber->resume();
    //fiber->swapIn();
    //BOKKET_LOG_INFO(g_logger)<<"main after swapIn";
    /*fiber->swapIn();
    BOKKET_LOG_INFO(g_logger)<<"main after end";
    fiber->swapIn();*/
    BOKKET_LOG_INFO(g_logger)<<"fiber ptr use_count="<<fiber.use_count();

    BOKKET_LOG_INFO(g_logger)<<"test_fiber end ";
}

int main() {
    bokket::Thread::setName("main");

    std::vector<bokket::Thread::ptr> threads;

    for(int i=0;i<2;++i) {
        threads.emplace_back(bokket::Thread::ptr(
            new bokket::Thread(&test_fiber,"name_"+std::to_string(i))));
    }
    for(auto i:threads)
        i->join();

    BOKKET_LOG_INFO(g_logger) << "main end";
}



