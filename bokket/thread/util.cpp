//
// Created by bokket on 2021/5/11.
//

#include "util.h"


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
    //return bokket::Fiber::getFiberId();
    return 0;
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