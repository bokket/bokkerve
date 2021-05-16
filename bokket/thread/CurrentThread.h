//
// Created by bokket on 2021/5/16.
//

#ifndef BOKKERVE_CURRENTTHREAD_H
#define BOKKERVE_CURRENTTHREAD_H

#include <string>
#include "./util.h"

namespace bokket
{

namespace CurrentThread
{
    /*
extern thread_local int t_cachedThreadId;
extern thread_local std::string t_threadName;
*/

void cacheThreadId();


}

}



#endif //BOKKERVE_CURRENTTHREAD_H
