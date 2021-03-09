//
// Created by bokket on 2021/3/9.
//

#include "FileLogAppender.h"

namespace bokket
{

FileLogAppender::FileLogAppender(const std::string basename, int rollSize, int flushInterval,
                                 FileWriterType writerType,LogEvent::ptr event)
                                 :basename_(basename),rollSize_(rollSize),flushInterval_(flushInterval)
                                 ,count_(0),startOfPeriod_(0),lastRoll_(0),lastFlush_(0)
{
    /*auto now=std::chrono::system_clock::now();
    time_t time=std::chrono::system_clock::to_time_t(now);
    std::string filename=getLogFileName(basename_,&time);*/
    time_t time=static_cast<time_t>(event->getTime());
    std::string filename=getLogFileName(basename_,&time);

    if(writerType==FileWriterType::MMAPFILE)
        file_=std::make_unique<MmapFileWrite>(filename,rollSize_);
    else
        file_=std::make_unique<AppendFileWriter>(filename,rollSize_);

}

}