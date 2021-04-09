//
// Created by bokket on 2021/3/9.
//

#ifndef BOKKET_FILEWRITERTYPE_H
#define BOKKET_FILEWRITERTYPE_H

#include "LogAppender.h"

#include <stdint.h>
#include <unistd.h>
#include <string>
#include <stdio.h>
#include <memory>


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
    FileWriter()=default;
    virtual ~FileWriter();

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

std::string getHostName()
{
    char buf[256];
    //返回本地主机的标准主机名
    if(::gethostname(buf,sizeof(buf))==0)
    {
        buf[sizeof(buf)-1]='\0';
        return buf;
    }
    else
    {
        return "unknowhost";
    }
}

std::string getLogFileName(const std::string& basename,time_t* now)
{
    std::string filename;
    //字符串的容量设置为至少size. 如果size指定的数值要小于当前字符串中的字符数
    // (亦即size<this→size()), 容量将被设置为可以恰好容纳字符的数值.
    filename.reserve(basename.size()+64);

    filename=basename;

    char timebuf[32];
    std::tm tm;
    ::gmtime_r(now,&tm);

    strftime(timebuf,sizeof(timebuf),".%Y%m%d-%H%M%S.", &tm);
    filename+=timebuf;


    filename+=getHostName();

    /*char pidbuf[32];
    snprintf(pidbuf, sizeof pidbuf, ".%d", ::getpid());
    filename += pidbuf;*/

    filename+=".log";
    return filename;
}

}

#endif //BOKKET_FILEWRITERTYPE_H
