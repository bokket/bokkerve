//
// Created by bokket on 2021/2/17.
//

#ifndef BOKKET_LOGSTREAM_H
#define BOKKET_LOGSTREAM_H


#include "FixedBuffer.h"

namespace bokket
{
namespace detail
{
class LogStream: public noncopyable
{
public:
    typedef LogStream self;

    typedef FixedBuffer<detail::kSmallBuffer> Buffer;
public:
    template<typename T>
    void formatInteger(T);




    self &operator << (bool v)
    {
        //v为真放1
        buffer_.append(v ? "1" : "0",1);
        return *this;
    }

    self& operator<<(short);
    self& operator<<(unsigned short);
    self& operator<<(int);
    self& operator<<(unsigned int);
    self& operator<<(long);
    self& operator<<(unsigned long);
    self& operator<<(long long);
    self& operator<<(unsigned long long);

    self& operator<<(const void*);

    self& operator<<(float v)
    {
        *this<<static_cast<double >(v);
        return *this;
    }

    self& operator<<(double );

    self& operator<<(char v)
    {
        buffer_.append(&v,1);
        return *this;
    }

    self& operator<<(const char* v)
    {
        buffer_.append(v,strlen(v));
        return *this;
    }

    self& operator<<(const string& v)
    {
        buffer_.append(v.c_str(), v.size());
        return *this;
    }

    void append(const char* data,int len)
    {
        buffer_.append(data,len);
    }

    const Buffer & getBuf() const { return buffer_; }

    void resetBuffer() { buffer_.reset(); }

private:

    Buffer buffer_;

    static const int kMaxNumericSize=32;
};




/*
class Fmt
{
public:
    template<typename T>
    Fmt(const char* fmt,T val);

    const char* data() const { return buf_; }
    int length() const { return length_; }
private:
    char buf_[32];
    int length_;
};

LogStream& operator<<(LogStream& s,const Fmt& fmt)
{
    s.append(fmt.data(),fmt.length());
    return s;
}
 */
}
}


#endif //BOKKET_LOGSTREAM_H
