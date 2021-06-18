//
// Created by bokket on 2021/2/27.
//

#ifndef BOKKET_TIMESTAMP_H
#define BOKKET_TIMESTAMP_H

#include <chrono>

namespace bokket
{

using Timestamp=std::chrono::time_point<std::chrono::system_clock,std::chrono::nanoseconds>;


inline Timestamp now() {
     return std::chrono::system_clock::now();
}
inline Timestamp nowAfter(std::chrono::nanoseconds interval) {
       return std::chrono::system_clock::now()+interval;
}
inline Timestamp nowBefore(std::chrono::nanoseconds interval) {
      return std::chrono::system_clock::now()-interval;
}


}



#endif //BOKKET_TIMESTAMP_H
