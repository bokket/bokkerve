//
// Created by bokket on 2021/5/26.
//

#ifndef BOKKERVE_PLUS_PLUS_ASSERT_H
#define BOKKERVE_PLUS_PLUS_ASSERT_H


#include <cstring>
#include <cassert>
#include "util.h"



#define LIKELY(x) __builtin_expect(!!(x),1)

#define UNLIKELY(x) __builtin_expect(!!(x),1)


#define ASSERT(x) \
    do            \
    {\
        if(UNLIKELY(!(x))) \
        {\
            BOKKET_LOG_ERROR(BOKKET_LOG_ROOT)<<"ASSERTION:" #x \
            <<"\nbacktrace:\n"\
            <<bokket::backTraceToString(100,2,"  ");\
            assert(x);\
        }         \
    }while(0)



#define ASSERT_MSG(x,w) \
    do                  \
    {\
        if(UNLIKELY(!(x))) \
        {\
            BOKKET_LOG_ERROR(BOKKET_LOG_ROOT)<<"ASSERTION:" #x \
            <<"\n"<<w                  \
            <<"\nbacktrace:\n"\
            <<bokket::backTraceToString(100,2,"  ");\
            assert(x);\
        }               \
    }while(0)
#endif //BOKKERVE_PLUS_PLUS_ASSERT_H
