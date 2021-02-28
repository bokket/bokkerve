//
// Created by bokket on 2021/2/27.
//

#ifndef BOKKET_TIMESTAMP_H
#define BOKKET_TIMESTAMP_H

#include <chrono>

using namespace std;

namespace bokket
{
using Timestamp=chrono::time_point<chrono::system_clock,chrono::nanoseconds>;

namespace clock
{
    inline Timestamp now()
    {
        return chrono::system_clock::now();
    }
    inline Timestamp nowAfter(chrono::nanoseconds interval)
    {
        return chrono::system_clock::now()+interval;
    }
    inline Timestamp nowBefore(chrono::nanoseconds interval)
    {
        return chrono::system_clock::now()-interval;
    }
}
}



#endif //BOKKET_TIMESTAMP_H
