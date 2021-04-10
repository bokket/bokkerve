//
// Created by bokket on 2021/3/10.
//

#include "FileWriterType.h"
#include "StdoutLogAppender.h"

#include <fcntl.h>
#include <sys/mman.h>
#include <cstring>
#include <string>

using namespace bokket;

// 'e' :O_CLOEXEC
AppendFileWriter::AppendFileWriter(const std::string &filename)
                                  :fp_(::fopen(filename.data(),"ae"))
                                  ,writen_(0)
{
    ::setbuffer(fp_,buffer_,sizeof(buffer_));
}

AppendFileWriter::~AppendFileWriter()
{
    flush();
    if(fp_)
        ::fclose(fp_);
}

void AppendFileWriter::append(const std::string &msg, int32_t len)
{
    size_t n=write(msg,len);

    size_t remain=len-n;

    while(remain >0 )
    {
        size_t x=write(msg.data()+n,remain);

        if(x==0)
        {
            int err=ferror(fp_);
            if(err)
            {
                fprintf(stderr,"AppendFileWriter::append() failed %s\n",strerror(err));
            }
            break;
        }
        n+=x;
        remain = len-n;
    }

    writen_+=len;
}

void AppendFileWriter::flush()
{
    ::fflush(fp_);
}


MmapFileWrite::MmapFileWrite(const std::string &filename,size_t memSize)
{
    memSize_=memSize;

    writen_=0;

    if(fd_>=0)
        ::close(fd_);

    //PROT_READ	 内存段可读
    //PROT_WRITE 内存段可写
    //PROT_EXEC	 内存段可执行
    //PROT_NONE	 内存段不能被访问
    fd_=::open(filename.data(),O_RDONLY | O_CREAT | S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH );

    if (fd_<0)
        fprintf(stderr,"open new file failed , errno=%d",errno);
    else
    {
        int n=::ftruncate(fd_,memSize);
        

        buffer_=static_cast<char*>(mmap(nullptr,memSize_,PROT_READ | PROT_WRITE ,MAP_SHARED,fd_,0));
        if(buffer_==MAP_FAILED)
        {
            fprintf(stderr,"mmap file failed,errno=%d",errno);
        }
    }
}

MmapFileWrite::~MmapFileWrite()
{
    if(fd_>=0)
    {
        ::close(fd_);
        fd_=-1;
        buffer_=nullptr;
    }
    if(buffer_!=MAP_FAILED)
        munmap(buffer_,memSize_);
}

void MmapFileWrite::append(const std::string &msg, int32_t len)
{
    if(len>memSize_-writen_)
    {
        fprintf(stderr,"mmap memory overflow, errno=%d",errno);
        return;
    }
    memcpy(buffer_+writen_,msg.data(),len);
    writen_+=len;
}

void MmapFileWrite::flush()
{
    if(buffer_!=MAP_FAILED)
        msync(buffer_,memSize_,MS_ASYNC);
}