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

}

#endif //BOKKET_FILEWRITERTYPE_H
