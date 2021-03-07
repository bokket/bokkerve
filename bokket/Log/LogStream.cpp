//
// Created by bokket on 2021/2/17.
//

#include "LogStream.h"
#include <limits>
#include <assert.h>
#include <stdio.h>
#include <algorithm>
#include <stdint.h>
#include <cmath>

using namespace std;
using namespace bokket;
using namespace bokket::detail;

namespace bokket
{
namespace detail
{


const char digits[]="9876543210123456789";
const char digitsHex[] = "0123456789ABCDEF";

//9876543210123456789
//         ^
//         |
//        zero
const char* zero=digits+9;

    template class FixedBuffer<kSmallBuffer>;
    template class FixedBuffer<kLargeBuffer>;

/*
    template<int SIZE>
    const char * FixedBuffer<SIZE>::debugString()
    {
        *cur_ = '\0';
        return data_;
    }*/


    template<typename T>
    size_t convert(char buf[],T value)
    {
        T i=value;
        char* p=buf;

        //123
        do
        {
            int lsd=static_cast<int>(i%10);
            i/=10;
            *p++=zero[lsd];
            //zero[9]
            //zero[12]
            //9876543210123456789
            //         ^  ^
            //         |  |
            //        zero|
            //            |
            //            |
            //           zero
            //321
        }while (i!=0);

        if(value<0)
        {
            *p++='-';
        }
        *p='\0';

        reverse(buf,p);

        return p-buf;
    }

    // uintptr_t对于32平台来说就是unsigned int, 对于64位平台来说unsigned long int
    size_t convertHex(char buf[], uintptr_t value)
    {
        uintptr_t i = value;
        char* p = buf;

        do
        {
            int lsd = i % 16;
            i /= 16;
            *p++ = digitsHex[lsd];
        } while (i != 0);

        *p = '\0';
        std::reverse(buf, p);

        return p - buf;
    }


    template <typename T>
    void LogStream::formatInteger(T v)
    {
        //buffer_可用的是否大于或等于32
        if(buffer_.avail() >= kMaxNumericSize )
        {
            //整数转换成字符串
            //--->123
            //--->"123"

            //返回buffer_自己的cur_指针
            size_t len=convert(buffer_.currunt(),v);
            //不用说了直接放
            buffer_.add(len);
            cout<<buffer_.toString()<<endl;
        }
    }

    LogStream& LogStream::operator<<(short v)
    {
        *this << static_cast<int>(v);
        return *this;
    }
    LogStream& LogStream::operator<<(unsigned short v)
    {
        *this << static_cast<unsigned int>(v);
        return *this;
    }

    LogStream& LogStream::operator<<(int v)
    {
        formatInteger(v);
        return *this;
    }

    LogStream& LogStream::operator<<(unsigned int v)
    {
        formatInteger(v);
        return *this;
    }

    LogStream& LogStream::operator<<(long v)
    {
        formatInteger(v);
        return *this;
    }

    LogStream& LogStream::operator<<(unsigned long v)
    {
        formatInteger(v);
        return *this;
    }

    LogStream& LogStream::operator<<(long long v)
    {
        formatInteger(v);
        return *this;
    }

    LogStream& LogStream::operator<<(unsigned long long v)
    {
        formatInteger(v);
        return *this;
    }


    LogStream& LogStream::operator<<(double v)
    {
        if(buffer_.avail()>=kMaxNumericSize)
        {
            int len=snprintf(buffer_.currunt(),kMaxNumericSize,"%.12g",v);
            //返回v的字符串大小
            buffer_.add(len);
            cout<<buffer_.toString()<<endl;
        }
        return *this;
    }

    LogStream& LogStream::operator<<(const void * p)
    {
        uintptr_t v=reinterpret_cast<uintptr_t>(p);

        if(buffer_.avail()>=kMaxNumericSize)
        {
            //cur_
            char* buf=buffer_.currunt();
            buf[0]='0';
            buf[1]='x';

            size_t len=convertHex(buf+2,v);
            buffer_.add(len+2);
            cout<<buffer_.toString()<<endl;
        }
        return *this;
    }


}
}
