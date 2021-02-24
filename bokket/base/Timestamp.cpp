//
// Created by bokket on 2021/2/20.
//

#include "Timestamp.h"


using namespace bokket;

Timestamp::Timestamp(int64_t ms)
                    :ms_(ms)
{}

string Timestamp::tostring() const
{
    char buf[32]={0};

    int64_t seconds=ms_/kMicroSeconds;
    int64_t microseconds=ms_%kMicroSeconds;//剩余微秒
    //# define PRId64		__PRI64_PREFIX "d"
    //int64_t在32位系统是long long int(%lld), 在64位系统是long int(%ld)
    snprintf(buf,sizeof(buf),"%" PRId64 ".%06" PRId64 "",seconds, microseconds);

    return buf;
}

string Timestamp::toFormtString() const
{
    char buf[64]={0};
    time_t seconds=static_cast<time_t>(ms_/kMicroSeconds);
    int microseconds=static_cast<int>(ms_%kMicroSeconds);

    struct tm tm;
    gmtime_r(&seconds,&tm);

    snprintf(buf,sizeof(buf),"%4d%02d%02d %02d:%02d:%02d.%06d",tm.tm_year+1900, tm.tm_mon+1,tm.tm_mday
                                                              ,tm.tm_hour, tm.tm_min, tm.tm_sec, microseconds);
    return buf;
}


Timestamp Timestamp::now()
{
    struct timeval tv;

    //可于tv指向缓冲区返回日历时间
    gettimeofday(&tv, nullptr);
    int64_t seconds=tv.tv_sec;//1970-1-1--00:00:00

    return Timestamp(seconds*kMicroSeconds+tv.tv_usec);

}

Timestamp Timestamp::invalid()
{
    return Timestamp();
}