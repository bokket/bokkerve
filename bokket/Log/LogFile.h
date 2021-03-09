//
// Created by bokket on 2021/3/9.
//

#ifndef BOKKET_LOGFILE_H
#define BOKKET_LOGFILE_H

#include <memory>
#include <string>
#include <chrono>
#include <mutex>

namespace bokket
{
class LogFile
{
public:
    LogFile(const std::string basename,size_t rollSize,int flushInterval);

private:
    const std::string basename_;

    const size_t rollSize_; // 日志文件达到rolSize_换一个新文件
    const int flushInterval;

    int count_;
    std::unique_lock<std::mutex> mutex_;
    time_t startOfPeriod_;	// 开始记录日志时间（调整至零点的时间）
    time_t lastRoll_;			// 上一次滚动日志文件时间
    time_t lastFlush_;		// 上一次日志写入文件时间

    constexpr static int kRollPerSeconds=std::chrono::
};
}



#endif //BOKKET_LOGFILE_H
