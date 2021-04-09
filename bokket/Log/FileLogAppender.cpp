//
// Created by bokket on 2021/3/9.
//

#include "FileLogAppender.h"

namespace bokket
{

FileLogAppender::FileLogAppender(const std::string basename, size_t rollSize, int flushInterval,
                                 FileWriterType writerType,LogEvent::ptr event)
                                 :basename_(basename),rollSize_(rollSize),flushInterval_(flushInterval)
                                 ,count_(0),startOfPeriod_(0),lastRoll_(0),lastFlush_(0)
                                 ,mutex_(new std::mutex)
{
    /*auto now=std::chrono::system_clock::now();
    time_t time=std::chrono::system_clock::to_time_t(now);
    std::string filename=getLogFileName(basename_,&time);*/
    time_t time=static_cast<time_t>(event->getTime());
    std::string filename=getLogFileName(basename_,&time);

    if(writerType==FileWriterType::MMAPFILE)
        file_=std::make_unique<MmapFileWrite>(filename,rollSize_);
    else
        file_=std::make_unique<AppendFileWriter>(filename);

    writerType_=writerType;

    rollFile(event);
}


void FileLogAppender::append(const std::string &msg, int32_t len)
{
    if(mutex_)
    {
        std::unique_lock<std::mutex> lock(*mutex_);
        append_unlocked(msg,len);
    }
}

void FileLogAppender::flush()
{
    if(mutex_)
    {
        std::unique_lock<std::mutex> lock(*mutex_);
        file_->flush();
    }
}

bool FileLogAppender::rollFile(LogEvent::ptr event)
{

    time_t now=static_cast<time_t>(event->getTime());
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
    }
    return true;
    
}

}