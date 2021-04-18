//
// Created by bokket on 2021/2/21.
//

#ifndef BOKKERVE_CURRENTTHREAD_H
#define BOKKERVE_CURRENTTHREAD_H

#include <unistd.h>
#include <sys/syscall.h>
#include <pthread.h>
#include <sys/types.h>
#include <string>
using namespace std;
namespace bokket
{
namespace CurrentThread
{
    // __thread修饰的变量是线程局部存储的
    extern thread_local int t_cachedTid;
    // 线程真实pid（tid）的缓存，
    // 是为了减少::syscall(SYS_gettid)系统调用的次数
    // 提高获取tid的效率
    extern thread_local string t_tidString;


    void cacheTid();

    inline int tid()
    {
        if(t_cachedTid==0)
            cacheTid();
        return t_cachedTid;
    }

    inline const string tidString() // for logging
    {
        //return t_tidString.c_str();
        return t_tidString;
    }

    bool isMainThread();
}

}

#endif //BOKKERVE_CURRENTTHREAD_H
