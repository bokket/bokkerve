//
// Created by bokket on 2021/3/9.
//
#include "AsyncFileLogAppender.h"
namespace bokket 
{
LogAppenderAsyncFile::LogAppenderAsyncFile(const std::string &filename,std::time_t persistPeriod)
                                          :started_(false),running_(false)
                                          ,persistPeriod_(persistPeriod)
                                          ,filename_(filename)
                                          ,latch_(1)
                                          ,persitThread_(std::bind(&LogAppenderAsyncFile::threadFunc,this),"AsyncLogging")
                                          ,curBuffer_(new LogBuffer())
{
    //::mkdir(filename_.c_str(),0755);
    start();
}

LogAppenderAsyncFile::~LogAppenderAsyncFile() noexcept {
    if(started_) {
        stop();
    }
}

void LogAppenderAsyncFile::append(Logger::ptr logger, LogLevel level, LogEvent::ptr event) {
    std::lock_guard<std::mutex> lockGuard(mutex_);


    //auto log=logger->getLogEvent()->format(event);
    auto log=logger->getLogFormatter()->format(event);
    if(curBuffer_->available() >= log.size()) {
        curBuffer_->append(log.data(),log.size());
    } else {
        buffers_.emplace_back(std::move(curBuffer_));

        curBuffer_.reset(new LogBuffer());
        curBuffer_->append(log.data(),log.size());

        cv_.notify_one();
    }
}


void LogAppenderAsyncFile::start() {
    started_= true;
    running_= true;
    persitThread_.start();
    latch_.wait();
}


void LogAppenderAsyncFile::stop() {
    started_= false;
    cv_.notify_one();
    persitThread_.join();
}

void LogAppenderAsyncFile::threadFunc() {
    std::unique_ptr<LogBuffer> buffer(new LogBuffer());
    std::vector<std::unique_ptr<LogBuffer>> persistBuffers_;
    persistBuffers_.reserve(2);
    //LogFile::LogFile(const std::string basename, size_t rollSize, int flushInterval, int check_freq_count,
    //        FileWriterType writerType)
    /*LogFile log_file(filename_,1024,persistPeriod_,0,
                     FileWriterType::APPENDFILE);*/


    LogFile log_file(filename_,FileWriterType::APPENDFILE);


    latch_.countLatch();

    while (running_) {

        std::cout<<buffer->toString()<<std::endl;

        assert(buffer);
        assert(buffer->length()==0);
        assert(persistBuffers_.empty());

        std::unique_lock<std::mutex> uniqueLock(mutex_);
        std::chrono::seconds time_out(persistPeriod_);
        if(buffers_.empty()) {
            cv_.wait_for(uniqueLock,time_out);
        }
        if(buffers_.empty() && curBuffer_->length()==0) {
            continue;
        }

        buffers_.emplace_back(std::move(curBuffer_));

        persistBuffers_.swap(buffers_);
        curBuffer_=std::move(buffer);
        curBuffer_->clear();

        assert(buffers_.empty());
        assert(curBuffer_->length()==0);
        assert(curBuffer_);
    }

    //if(persistBuffers_.size() > bokket::detail::kLargeBuffer ) {
    if(persistBuffers_.size() > 1 ) {
        std::cerr<<"log is too large,drop some "<<std::endl;

        persistBuffers_.erase(persistBuffers_.begin()+1,persistBuffers_.end());
    }

    for(const auto& buffer: persistBuffers_) {
        log_file.append_unlocked(buffer->data(),buffer->length());
    }


    assert(persistBuffers_.size()==1);
    buffer=std::move(persistBuffers_[0]);
    buffer->clear();
    persistBuffers_.clear();

    log_file.flush();

    if(!started_) {
        std::unique_lock<std::mutex> uniqueLock(mutex_);
        if(curBuffer_->length()==0) {
            running_= false;
        }
    }
    log_file.flush();

    std::cerr << "AsyncFileLogAppender flush complete" << std::endl;
}

}