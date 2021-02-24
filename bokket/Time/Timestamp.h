//
// Created by bokket on 2021/2/20.
//

#ifndef BOKKERVE_TIMESTAMP_H
#define BOKKERVE_TIMESTAMP_H

#include "../base/copyable.h"
#include <boost/operators.hpp>

using namespace std;
namespace bokket
{
class Timestamp: public bokket::copyable,
                 public boost::less_than_comparable<Timestamp>,
                 public boost::equality_comparable<Timestamp>
{
public:
    Timestamp()
            :ms_(0)
    {}

    explicit Timestamp(int64_t ms);

    void swap(Timestamp& that)
    {
        std::swap(ms_,that.ms_);
    }

    string tostring() const;
    string toFormtString() const;

    bool valid() const { return ms_>0; }
    int64_t getMs() const { return ms_; }

    //从微秒转换成秒
    //1秒等于100万微秒
    time_t seconds() const { return static_cast<time_t>(ms_/kMicroSeconds); }


    //
    static Timestamp now();

    static Timestamp invalid();

    string getLocalTime()
    {
        time_t t=time(nullptr);
        return getLocalTime(&t);
    }
    string getLocalTime(time_t *t)
    {
        int microseconds=static_cast<int>( ms_ %kMicroSeconds);
        struct tm tm;
        localtime_r(t, &tm);

        char buf[32];
        snprintf(buf,sizeof(buf),"%04d-%02d-%02d-%02d-%02d-%02d-%06d", 1900 + tm.tm_year,
                1 + tm.tm_mon, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec, microseconds);

        return buf;
    }

    string getLocalTimeByStrf()
    {
        time_t t=time(nullptr);
        return getLocalTimeByStrf(&t);
    }

    string getLocalTimeByStrf(time_t *t)
    {
        //%Y-%m-%d
        //带世纪部分的十进制年份-十进制的月份-十进制表示的每月的第几天

        //%H:%M:%S
        //24小时制的小时:十时制表示的分钟:十进制的秒数
        string m_string="%Y-%m-%d %H:%M:%S";
        struct tm tm;
        localtime_r(t,&tm);
        char buf[128];
        strftime(buf,sizeof(buf),m_string.c_str(),&tm);

        return buf;
    }

    // 从秒变成微秒
    static Timestamp getUnixTime(time_t t)
    {
        return getUnixTime(t,0);
    }

    static Timestamp getUnixTime(time_t t,int microseconds)
    {
        return Timestamp(static_cast<int64_t>(t)* kMicroSeconds + microseconds);
    }

    //微秒
    //1秒等于100万微秒
    static const int kMicroSeconds=1000* 1000;

private:
    //微秒
    int64_t ms_;
};

inline bool operator<(Timestamp lhs,Timestamp rhs)
{
    return lhs.getMs()<rhs.getMs();
}

inline bool operator==(Timestamp lhs,Timestamp rhs)
{
    return lhs.getMs()==rhs.getMs();
}

//秒的差距
inline double timeDifference(Timestamp high,Timestamp low)
{
    int64_t diff=high.getMs()-low.getMs();
    return static_cast<double>(diff) / Timestamp::kMicroSeconds;
}

//微秒的相加
inline Timestamp addTime(Timestamp timestamp,double seconds)
{
    int64_t delta=static_cast<int64_t>(seconds*Timestamp::kMicroSeconds);
    return Timestamp(timestamp.getMs() + delta);
}

}


#endif //BOKKERVE_TIMESTAMP_H
