//
// Created by bokket on 2021/3/9.
//

#ifndef BOKKET_FILEWRITERTYPE_H
#define BOKKET_FILEWRITERTYPE_H


#include <stdint.h>
#include <unistd.h>
#include <string>
#include <stdio.h>
#include <memory>
#include <sstream>
#include <iomanip>
#include <chrono>

namespace bokket
{
enum class FileWriterType:uint8_t
{
    MMAPFILE,
    APPENDFILE
};

class FileWriter
{
public:
    virtual ~FileWriter()=default;

    virtual void append(const std::string& msg,int32_t len)=0;

    virtual void flush()=0;

    virtual size_t getWrittenBytes() const=0;
};


class MmapFileWrite: public FileWriter
{
public:
    MmapFileWrite(const std::string& filename,size_t memSize);

    ~MmapFileWrite();

    void append(const std::string& msg,int32_t len) override;
    void flush() override;

    size_t getWrittenBytes() const override { return writen_; }
private:
    int fd_;
    char* buffer_;
    size_t memSize_;
    size_t writen_;
};

class AppendFileWriter: public FileWriter
{
public:
    AppendFileWriter(const std::string& filename);

    ~AppendFileWriter();

    void append(const std::string& msg,int32_t len) override;

    void flush() override;

    size_t getWrittenBytes() const override { return writen_; }

private:
    size_t write(const std::string& msg,int32_t len)
    {
        return ::fwrite_unlocked(msg.data(),1,len,fp_);
    }

    FILE* fp_;
    char buffer_[64*1024];
    size_t writen_;

};


class LogFile
{
public:
    LogFile(const std::string basename,size_t rollSize,
            int flushInterval,int check_freq_count,
            FileWriterType writerType);


    ~LogFile()=default;

    //void append(const std::string& msg,int32_t len);
    //void append(Logger::ptr logger,LogLevel level,LogEvent::ptr event) override;

    void flush();

    bool rollFile();

    //bool reopen();

    void append_unlocked(const std::string& msg,int32_t len);

private:
    const std::string basename_;

    //std::ofstream filestream_;
    //bokket::detail::LogStream stream_;


    const size_t rollSize_; // 日志文件达到rolSize_换一个新文件
    const int flushInterval_;
    const int checkFreqCount_;

    int count_;
    //std::unique_ptr<std::mutex> mutex_;
    time_t startOfPeriod_;	// 开始记录日志时间（调整至零点的时间）
    time_t lastRoll_;			// 上一次滚动日志文件时间
    time_t lastFlush_;		// 上一次日志写入文件时间

    std::unique_ptr<FileWriter> file_;

    FileWriterType writerType_;
    constexpr static int kRollPerSeconds=60*60*24;
};


}

#endif //BOKKET_FILEWRITERTYPE_H
