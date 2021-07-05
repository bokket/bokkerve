//
// Created by bokket on 2021/7/6.
//

#ifndef BOKKERVE_BUFFER_H
#define BOKKERVE_BUFFER_H

#include <vector>
#include <string>
#include <algorithm>

#include "../base/noncopyable.h"

namespace bokket
{

static constexpr std::size_t kBufferDefaultLength{2048};
static constexpr char CRLF[]{"\r\n"};

class Buffer: public noncopyable
{
public:
    Buffer(std::size_t len=kBufferDefaultLength);

    const char* peek() const;

    const char* beginWrite() const;

    char* beginWrite();


private:
    std::size_t head_;
    std::size_t capacity_;
    std::vector<char> buffer_;
    std::size_t tail_;


};

}




#endif //BOKKERVE_BUFFER_H
