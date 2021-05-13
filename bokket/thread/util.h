//
// Created by bokket on 2021/5/11.
//

#ifndef BOKKERVE_UTIL_H
#define BOKKERVE_UTIL_H


#include <string>
#include <stdint.h>
#include <assert.h>
#include <syscall.h>
namespace bokket
{
/*
extern thread_local int t_cachedThreadId;
extern thread_local std::string t_threadName;
void cacheThreadId();

int threadId() {
    if(t_cachedThreadId == 0) {
        cacheThreadId();
    }
    return t_cachedThreadId;
}*/


pid_t getThreadId();


uint32_t getFiberId();
}

#endif //BOKKERVE_UTIL_H
