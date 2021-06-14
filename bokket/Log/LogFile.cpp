//
// Created by bokket on 2021/3/10.
//

#include "LogFile.h"

#include <fcntl.h>
#include <sys/mman.h>
#include <cstring>
#include <string>

namespace bokket 
{

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


std::string getLogFileName(const std::string& basename,std::time_t* now)
{
    std::string filename;
    //字符串的容量设置为至少size. 如果size指定的数值要小于当前字符串中的字符数
    // (亦即size<this→size()), 容量将被设置为可以恰好容纳字符的数值.
    filename.reserve(basename.size()+64);

    filename=basename;

    //char timebuf[32];
    std::stringstream ss;
    std::tm tm;
    //gmtime_r转出来的是0时区的标准时间
    //::gmtime_r(now,&tm);


    ss<<filename;
    ss<<getHostName();
    //ss<<std::put_time(::localtime_r(now,&tm),".%04d-%02d-%02d-%02d-%02d-%02d-%06d");
    ss<<std::put_time(::gmtime_r(now,&tm),"-  %Y-%m-%d %H:%M:%S");
    ss<<".log";

    return ss.str();
    //strftime(timebuf,sizeof(timebuf),".%Y%m%d-%H%M%S.", &tm);
    //filename+=timebuf;


    //filename+=getHostName();

    /*char pidbuf[32];
    snprintf(pidbuf, sizeof pidbuf, ".%d", ::getpid());
    filename += pidbuf;*/

    //filename+=".log";
    //return filename;
}    

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


//MmapFileWrite::MmapFileWrite(const std::string &filename,size_t memSize)
MmapFileWrite::MmapFileWrite(const std::string &filename)
{
    //memSize_=memSize;
    memSize_=0;

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
        //int n=::ftruncate(fd_,memSize);
        int n=::ftruncate(fd_,memSize_);
        

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
    if(len>static_cast<int32_t>(memSize_-writen_))
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

/*LogFile::LogFile(const std::string basename, size_t rollSize, int flushInterval, int check_freq_count,
                                 FileWriterType writerType)*/
LogFile::LogFile(const std::string basename, FileWriterType writerType)
                :basename_(basename)
                                 /*:basename_(basename),rollSize_(rollSize),flushInterval_(flushInterval),checkFreqCount_(check_freq_count)
                                 ,count_(0),startOfPeriod_(0),lastRoll_(0),lastFlush_(0)*/
                                 //,stream_()
                                 //,mutex_(std::make_unique<std::mutex>())
{
    /*auto now=std::chrono::system_clock::now();
    time_t time=std::chrono::system_clock::to_time_t(now);
    std::string filename=getLogFileName(basename_,&time);*/
    //time_t time=static_cast<time_t>(event->getTime());
    std::time_t time=std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::string filename=getLogFileName(basename_,&time);

    if(writerType==FileWriterType::MMAPFILE)
        file_=std::make_unique<MmapFileWrite>(filename);
        //file_=std::make_unique<MmapFileWrite>(filename,rollSize_);
    else
        file_=std::make_unique<AppendFileWriter>(filename);

    writerType_=writerType;

    //rollFile();
}


//void LogFile::append(Logger::ptr logger, LogLevel level, LogEvent::ptr event)
/*void LogFile::append(const std::string &msg, int32_t len) 
{*/
       /*if(mutex_)
    {
        //std::unique_lock<std::mutex> lock(*mutex_);
        append_unlocked(msg,len);
    }*/
    //LogAppender的锁
    //std::lock_guard<std::mutex> uniqueLock(mutex_);
    //formatter_->format(filestream_,event);
    //auto log=logger->getLogEvent()->format(event);
    //auto log=logger->getLogFormatter()->format(event);
    //std::cout<<log.data()<<std::endl;
    //append_unlocked(log.data(),log.size());
    
//}

void LogFile::append_unlocked(const std::string &msg, int32_t len)
{
    file_->append(msg,len);

    /*if(file_->getWrittenBytes() > rollSize_) {
        rollFile();
    } else {
        ++count_;
        //隔多久需要检查一下
        if(count_ >= checkFreqCount_) {
            count_=0;
            //time_t now = ::time(nullptr);
            std::time_t now=std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
            time_t thisPeriod=now/kRollPerSeconds*kRollPerSeconds;
            if(thisPeriod!=startOfPeriod_) {
                rollFile();
            } else if(now-lastFlush_ > flushInterval_) {
                lastFlush_=now;
                file_->flush();
            }
        }
    }*/
}


void LogFile::flush()
{
    /*if(mutex_)
    {
        std::unique_lock<std::mutex> lock(*mutex_);
        file_->flush();
    }*/
    //std::lock_guard<std::mutex> uniqueLock(mutex_);
    file_->flush();
}

/*
bool LogFile::rollFile()
{

    //time_t now=static_cast<time_t>(event->getTime());
    time_t now=0;
    std::string filename=getLogFileName(basename_,&now);

    time_t start=now/kRollPerSeconds*kRollPerSeconds;

    if(now>lastRoll_)
    {
        lastRoll_=now;
        lastFlush_=now;
        startOfPeriod_=start;

        if(writerType_==FileWriterType::APPENDFILE)
            file_.reset(new AppendFileWriter(filename));
        else
            file_.reset(new MmapFileWrite(filename,rollSize_));
        return true;
    }
    return false;
}*/




}