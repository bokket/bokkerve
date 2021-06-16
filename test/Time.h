//
// Created by bokket on 2021/6/16.
//

#ifndef BOKKERVE_PLUS_PLUS_TIME_H
#define BOKKERVE_PLUS_PLUS_TIME_H


#include <chrono>

namespace bokket
{

class Time
{
public:
    Time()
        :begin_(std::chrono::high_resolution_clock::now())
    {}
    ~Time()=default;

public:
    int64_t elapsed_seconds() const {
        auto seconds=std::chrono::duration_cast<std::chrono::duration<std::chrono::seconds>>(std::chrono::high_resolution_clock::now()-begin_).count();
        return seconds.count();
    }

    int64_t elapsed() const {
        return std::chrono::duration_cast<std::chrono::duration<std::chrono::milliseconds>>(std::chrono::high_resolution_clock::now()-begin_).count();
    }

    int64_t elapsed_micro() const {
        return std::chrono::duration_cast<std::chrono::duration<std::chrono::microseconds>>(std::chrono::high_resolution_clock::now()-begin_).count();
    }

    int64_t elapsed_nano() const {
        return std::chrono::duration_cast<std::chrono::duration<std::chrono::nanoseconds>>(std::chrono::high_resolution_clock::now()-begin_).count();
    }

    int64_t elapsed_minutes() const {
        return std::chrono::duration_cast<std::chrono::duration<std::chrono::minutes>>(std::chrono::high_resolution_clock::now()-begin_).count();
    }

    int64_t elapsed_hours() const {
        return std::chrono::duration_cast<std::chrono::duration<std::chrono::hours>>(std::chrono::high_resolution_clock::now()-begin_).count();
    }


private:
    std::chrono::time_point<std::chrono::high_resolution_clock> begin_;
};

}

#endif //BOKKERVE_PLUS_PLUS_TIME_H
