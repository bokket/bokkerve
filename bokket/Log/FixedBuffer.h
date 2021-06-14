//
// Created by bokket on 2021/2/19.
//

#ifndef BOKKERVE_FIXEDBUF_H
#define BOKKERVE_FIXEDBUF_H


#include <string>
#include <cstdio>
#include <cstring>
#include <streambuf>
#include "../base/noncopyable.h"

using namespace std;

namespace bokket
{


namespace detail
{
    const int kSmallBuffer=4000;
    const int kLargeBuffer=4000*1000;

    //非类型参数
    template <int SIZE>
class FixedBuffer: public streambuf ,public noncopyable
{
public:
    FixedBuffer(size_t total=1024*1024*10)
               :total_(total)
               ,available_(total)
               ,cur_(0)
    {
        data_=new char[total];
    }
    ~FixedBuffer() {
        delete[] data_;
    }

    void append(const char* buf,size_t len)
    {
        /*if(avail() > len)
        {
            memcpy(cur_,buf,len);
            cur_+=len;
        }*/
        memcpy(data_+cur_,data_,len);
        cur_+=len;
        available_-=len;
    }

    /*void append(string& s,size_t len)
    {
        if(avail() > len )
        {
            memcpy(cur_,s.c_str(),len);
            cur_+=len;
        }
    }*/
    const char* data() const { return data_; }
    int available() const
    {
        //向上转换
        //return static_cast<int>(end()-cur_);
        return available_;
    }
    size_t length() const
    {
        //return static_cast<int>(cur_-data_);
        return cur_;
    }
    //char* currunt() { return cur_; }

    //void add(size_t len) { cur_+=len; }

    //void reset() { cur_=data_; }

    //void memset() { ::memset(data_,0,sizeof(data_) ); }

    void clear() {
        //::bzero(data_,sizeof(data_) );
        cur_=0;
        available_=total_;
    }

    std::string toString() const { return std::string(data_,length() ); }

    
    const char* debugString() const
    {
        fprintf(stderr, "data:%s\nsize:%d\n", data_, length());
        //*cur_ = '\0';
        return data_;
    }
private:
    //const char* end() const { return data_+sizeof(data_); }
    char* data_;
    const size_t total_;
    size_t available_;
    size_t cur_;
    //char data_[SIZE];
    //char* cur_;
};

}

}

#endif //BOKKERVE_FIXEDBUF_H
