//
// Created by bokket on 2021/6/7.
//
#include <chrono>
#include <iostream>

#include "../bokket/Log/Log.h"
#include "../bokket/Fiber/fiber.h"
#include "../bokket/Scheduler/scheduler.h"

static bokket::Logger::ptr g_logger = BOKKET_LOG_NAME("test");

static uint64_t cnt=0;

void run() {
    while(true) {
        cnt++;
        //bokket::Fiber::getThis()->yield();
        bokket::Fiber::getThis()->call();
    }
}

int main() {
    auto start=std::chrono::system_clock::now();
    bokket::Fiber::getThis();
    bokket::Fiber::ptr fiber(new bokket::Fiber(run,0));

    while(cnt<10000) {
        cnt++;
        //fiber->resume();
        fiber->call();
    }
    auto end=std::chrono::system_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

    std::cout << cnt << "次运行耗时" << duration.count() << "us" << std::endl;

}