//
// Created by bokket on 2021/5/11.
//

#ifndef BOKKERVE_UTIL_H
#define BOKKERVE_UTIL_H


#include <string>
#include <vector>

#include <stdint.h>
#include <assert.h>
#include <unistd.h>
#include <syscall.h>


namespace bokket
{



/*extern thread_local int t_cachedThreadId;
extern thread_local std::string t_threadName;
*/

/*
void cacheThreadId();

int threadId();*/
/*
extern void cacheThreadId();

//void cacheThreadId();

int threadId() {
    if(t_cachedThreadId == 0) {
        bokket::CurrentThread::cacheThreadId();
    }
    return t_cachedThreadId;
}*/


pid_t getThreadId();


uint32_t getFiberId();

void backTrace(std::vector<std::string>& bt,int size=64,int skip=1);
std::string backTraceToString(int size=64,int skip=2,const std::string& prefix="");


}

#endif //BOKKERVE_UTIL_H
