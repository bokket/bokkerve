//
// Created by bokket on 2021/2/19.
//

#ifndef BOKKERVE_FIXEDBUF_H
#define BOKKERVE_FIXEDBUF_H

#pragma once

#include <string>
#include <cstdio>
#include <cstring>
#include "../MutexLock/noncopyable.hpp"

using namespace std;

namespace bokket
{


namespace detail
{
    const int kSmallBuffer=4000;
    const int kLargeBuffer=4000*1000;

    //非类型参数
    template <int SIZE>
class FixedBuffer: public noncopyable
{
public:
    FixedBuffer()
               :cur_(data_)
    {}

    ~FixedBuffer() {}

    void append(const char* buf,size_t len)
    {
        if(avail() > len)
        {
            memcpy(cur_,buf,len);
            cur_+=len;
        }
    }
    int avail() const
    {
        //向上转换
        return static_cast<int>(end()-cur_);
    }
    int length() const
    {
        return static_cast<int>(cur_-data_);
    }
    char* currunt() { return cur_; }

    void add(size_t len) { cur_+=len; }

    void reset() { cur_=data_; }

    void memset() { ::memset(data_,0,sizeof(data_) ); }

    void bzero() { ::bzero(data_,sizeof(data_) ); }

    string asString() const { return string(data_,length() ); }
private:

    const char* end() const { return data_+sizeof(data_); }
    char data_[SIZE];
    char* cur_;
};
}

}

#endif //BOKKERVE_FIXEDBUF_H
