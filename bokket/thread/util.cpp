//
// Created by bokket on 2021/5/11.
//

#include "util.h"

#include <string_view>
#include <execinfo.h>

#include "./thread.h"
#include "../Fiber/fiber.h"

namespace bokket
{


static bokket::Logger::ptr g_logger=BOKKET_LOG_NAME("system");



pid_t getThreadId() {
    return ::syscall(SYS_gettid);
}

uint32_t getFiberId() {
    return bokket::Fiber::getFiberId();
    //return 0;
}



uint64_t getElapsed() {
    //::timespec ts={0};
    //提供最高精度的计时周期

    std::chrono::milliseconds ms=std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::system_clock::now().time_since_epoch()
            );

    return ms.count();

    
    //系统运行时间，从系统启动时开始计时，系统休眠时不再计时
    //单调的时钟？从某个不确定的时刻开始跳动，不受非连续跳跃的影响，但是受增量的调整影响。
    //CLOCK_MONOTONIC_RAW：与CLOCK_MONOTONIC类似，但输出的是原始的时钟，不受其他调整影响
    //clock_gettime(CLOCK_MONOTONIC_RAW,&ts);
    //return ts.tv_sec*1000+ ts.tv_nsec/1000000.0;
}

uint64_t getCurrentMs() {
    struct ::timeval t;
    ::gettimeofday(&t, nullptr);

    return t.tv_sec*1000ul+t.tv_usec/1000;
}

uint64_t getCurrnetUs() {
    struct ::timeval t;
    ::gettimeofday(&t, nullptr);

    return t.tv_sec*1000000ul+t.tv_usec;
}


void backTrace(std::vector<std::string>& bt,int size,int skip) {
    void** array=static_cast<void**>(malloc((sizeof(void*)*size)));

    size_t s=::backtrace(array,size);

    char** strings=backtrace_symbols(array,s);

    if(strings== nullptr) {
        BOKKET_LOG_ERROR(g_logger)<<"backtrace_symbols error";
        return;
    }

    for(size_t i=skip;i<s;i++) {
        bt.emplace_back(strings[i]);
    }

    free(strings);
    free(array);
}

std::string backTraceToString(int size,int skip,const std::string& prefix) {
    std::vector<std::string> bt;
    backTrace(bt,size,skip);
    std::stringstream ss;
    for(size_t i=0;i<bt.size();i++) {
        ss<<prefix<<bt[i]<<std::endl;
    }
    return ss.str();
}

}