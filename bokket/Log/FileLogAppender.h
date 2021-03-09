//
// Created by bokket on 2021/3/9.
//

#ifndef BOKKET_FILELOGAPPENDER_H
#define BOKKET_FILELOGAPPENDER_H


#include "LogAppender.h"
#include "LogEvent.h"
#include "FileWriterType.h"


#include <memory>
#include <string>
#include <chrono>
#include <mutex>

namespace bokket
{


class FileLogAppender: public LogAppender
{
public:
    using ptr=std::shared_ptr<FileLogAppender>;
public:
    FileLogAppender(const std::string basename,size_t rollSize,int flushInterval,FileWriterType writerType,LogEvent::ptr event);

    ~FileLogAppender();

    void append(const std::string& msg,int32_t line) override;

private:
    const std::string basename_;

    const size_t rollSize_; // 日志文件达到rolSize_换一个新文件
    const int flushInterval_;

    int count_;
    std::unique_lock<std::mutex> mutex_;
    time_t startOfPeriod_;	// 开始记录日志时间（调整至零点的时间）
    time_t lastRoll_;			// 上一次滚动日志文件时间
    time_t lastFlush_;		// 上一次日志写入文件时间

    std::unique_ptr<FileWriter> file_;

    FileWriterType writerType_;
    constexpr static int kRollPerSeconds=60*60*24;
};
}




#endif //BOKKET_FILELOGAPPENDER_H
