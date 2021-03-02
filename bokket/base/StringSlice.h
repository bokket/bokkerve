//
// Created by bokket on 2021/3/2.
//

#ifndef BOKKET_STRINGSLICE_H
#define BOKKET_STRINGSLICE_H

#include "copyable.h"
#include <cassert>
#include <cstddef>
#include <cstring>
#include <string>
#include <iostream>

namespace bokket
{

class StringSlice: public copyable
{
public:
    StringSlice()
               :data_("")
               ,size_(0)
    {}
    StringSlice(const char* s,size_t n)
               :data_(s)
               ,size_(n)
    {}
    StringSlice(const std::string& s)
               :data_(s.data())
               ,size_(s.size())
    {}
    StringSlice(const char* s)
               :data_(s)
               ,size_(strlen(s))
    {}

public:
    const char* data() const { return data_; }

    size_t size() const { return size_; }

    bool empty() const { return size_==0; }

    char operator[](size_t n) const
    {
        assert(n<size());
        return data_[n];
    }

    void clear()
    {
        data_="";
        size_=0;
    }


    void remove_prefix(size_t n)
    {
        assert(n<=size());
        data_+=n;
        size_-=n;
    }

    void remove_suffix(size_t n)
    {
        assert(n <= size());
        size_ -= n;
    }


    std::string ToString() const
    {
        return std::string(data_,size_);
    }


    int compare(const StringSlice& b) const;


    bool starts_with(const StringSlice& x) const
    {
        return  ( (size_>=x.size_) && (memcmp(data_,x.data_,x.size_)==0) );
    }
private:
    const char* data_;
    size_t size_;
};

inline bool operator==(const StringSlice& x,const StringSlice& y)
{
    return ( (x.size()==y.size()) && (memcmp(x.data(),y.data(),x.size())==0) );
}

inline bool operator!=(const StringSlice& x,const StringSlice& y)
{
    return !(x==y);
}

inline int StringSlice::compare(const StringSlice &b) const
{
    const size_t min_len=(size_<b.size_) ? size_:b.size_;

    int r=memcmp(data_,b.data_,min_len);

    if(r==0)
    {
        if(size_<b.size_)
            r=-1;
        else if(size_>b.size_)
            r+=1;
    }
    return r;
}

    std::ostream& operator<<(std::ostream& o, const StringSlice& slice)
    {
        return o<<slice.data();
    }

}

#endif //BOKKET_STRINGSLICE_H
