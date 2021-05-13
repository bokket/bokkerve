LogAppenderFile::LogAppenderFile(const std::string basename, size_t rollSize, int flushInterval, int check_freq_count,
                                 FileWriterType writerType)
                                 :basename_(basename),rollSize_(rollSize),flushInterval_(flushInterval),check_freq_count_(check_freq_count)
                                 ,count_(0),startOfPeriod_(0),lastRoll_(0),lastFlush_(0)
                                 //,stream_()
                                 //,mutex_(std::make_unique<std::mutex>())
{
    /*auto now=std::chrono::system_clock::now();
    time_t time=std::chrono::system_clock::to_time_t(now);
    std::string filename=getLogFileName(basename_,&time);*/
    //time_t time=static_cast<time_t>(event->getTime());
    time_t time=0;
    std::string filename=getLogFileName(basename_,&time);

    if(writerType==FileWriterType::MMAPFILE)
        file_=std::make_unique<MmapFileWrite>(filename,rollSize_);
    else
        file_=std::make_unique<AppendFileWriter>(filename);

    writerType_=writerType;

    rollFile();
}


void LogAppenderFile::append(Logger::ptr logger, LogLevel level, LogEvent::ptr event)
{
    /*if(mutex_)
    {
        //std::unique_lock<std::mutex> lock(*mutex_);
        append_unlocked(msg,len);
    }*/
    //LogAppender的锁
    std::lock_guard<std::mutex> uniqueLock(mutex_);
    //formatter_->format(filestream_,event);
    auto log=logger->getLogFormatter()->format(event);
    append_unlocked(log.data(),log.size());
    
}

void LogAppenderFile::append_unlocked(const std::string &msg, int32_t len)
{
    file_->append(msg,len);

    if(file_->getWrittenBytes() > rollSize_) {
        rollFile();
    } else {
        ++count_;
        //隔多久需要检查一下
        if(count_ >= check_freq_count_) {
            count_=0;
            time_t now = ::time(nullptr);
            time_t thisPeriod=now/kRollPerSeconds*kRollPerSeconds;
            if(thisPeriod!=startOfPeriod_) {
                rollFile();
            } else if(now-lastFlush_ > flushInterval_) {
                lastFlush_=now;
                file_->flush();
            }
        }
    }
}


void LogAppenderFile::flush()
{
    /*if(mutex_)
    {
        std::unique_lock<std::mutex> lock(*mutex_);
        file_->flush();
    }*/
    std::lock_guard<std::mutex> uniqueLock(mutex_);
    file_->flush();
}

bool LogAppenderFile::rollFile()
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
}
