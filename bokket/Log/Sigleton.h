//
// Created by bokket on 2021/3/6.
//

#ifndef BOKKET_SIGLETON_H
#define BOKKET_SIGLETON_H

#include "../base/noncopyable.h"
#include <memory>
namespace bokket
{

template <class T>
class Sigleton: public noncopyable
{
public:
    static T& GetInstance()
    {
        static Sigleton v;
        return v;
    }
};


template <class T>
class SigletonPtr: public noncopyable
{
public:
    static std::shared_ptr<T> GetInstance()
    {
        static std::shared_ptr<T> v(new T);
        return v;
    }
};


}

#endif //BOKKET_SIGLETON_H
