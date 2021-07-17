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

    ~Buffer()=default;

    void swap(Buffer& rhs) noexcept {

    }

    const char* peek() const {
        return begin()+head_;
    }

    const char* beginWrite() const {
        return begin()+tail_;
    }

    char* beginWrite() {
        return begin()+tail_;
    }

    uint8_t peekInt8() const {

        return *(static_cast<const uint8_t*>((void*)peek()));
    }


    const char* findCRLF() const {
        const char* crlf=std::search(peek(),beginWrite(),CRLF,CRLF+2);
        return crlf==beginWrite()? nullptr:crlf;
    }

    char &operator[](std::size_t offset) {
        return begin()[head_+offset];
    }



    int64_t peekInt64() const;
    int32_t peekInt32() const;
    int16_t peekInt16() const;
    int8_t peekInt8() const;

    int64_t readInt64();
    int32_t readInt32();
    int16_t readInt16();
    int8_t readInt8();

    void appendInt64(int64_t x);
    void appendInt32(int32_t x);
    void appendInt16(int16_t x);
    void appendInt8(int8_t x);

    ssize_t readFd(int fd,int& savedErrno);
private:
    std::size_t head_;
    std::size_t capacity_;
    std::vector<char> buffer_;
    std::size_t tail_;



    const char* begin() const {
        return &buffer_[0];
    }
    char* begin() {
        return &buffer_[0];
    }

    void makeSpace(std::size_t len);

    inline void swap(Buffer& one,Buffer& two) noexcept {
        one.swap(two);
    }
};

}




#endif //BOKKERVE_BUFFER_H
