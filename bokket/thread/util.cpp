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

void backTrace(std::vector<std::string>& bt,int size=64,int skip=1) {
    void** array=static_cast<void**>malloc(sizeof(void*)size);

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

std::string backTraceToString(int size=64,int skip=2,const std::string& prefix="") {
    std::vector<std::string> bt;
    backTrace(bt,size,skip);
    std::stringstream ss;
    for(size_t i=0;i<bt.size();i++) {
        ss<<prefix<<bt[i]<<std::endl;
    }
    return ss.str();
}

}