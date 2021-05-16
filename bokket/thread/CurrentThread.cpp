//
// Created by bokket on 2021/5/16.
//

#include "CurrentThread.h"
#include "./util.h"


namespace bokket
{
namespace CurrentThread
{

void cacheThreadId()
{
    if(t_cachedThreadId == 0) {
        t_cachedThreadId = getThreadId();
        t_threadName = std::to_string(t_cachedThreadId);
    }
}


}

}