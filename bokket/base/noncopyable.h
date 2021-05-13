//
// Created by bokket on 2021/2/19.
//

#ifndef BOKKERVE_NONCOPYABLE_H
#define BOKKERVE_NONCOPYABLE_H


namespace bokket {

class noncopyable
{
protected:
    noncopyable() = default;
    ~noncopyable() = default;

private:
    noncopyable(const noncopyable&) = delete;
    noncopyable& operator=(const noncopyable&) = delete;
};

}
#endif //BOKKERVE_NONCOPYABLE_H
