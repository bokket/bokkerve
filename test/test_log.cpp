//
// Created by bokket on 2021/4/11.
//



#include <chrono>
#include <iostream>

#include "gtest/gtest.h"
#include "../bokket/Log/Log.h"
#include "../bokket/thread/util.h"


int main() {
   bokket::Logger::ptr logger(new bokket::Logger);
   //bokket::Logger::ptr logger=std::make_shared<bokket::Logger>();

    logger->addAppender(bokket::LogAppender::ptr(new bokket::LogAppenderStdout));
    std::cout<<">>>"<<std::endl;
    //logger->append();

    bokket::LogAppenderFile::ptr file_appender(new bokket::LogAppenderFile("./log-",0,0,0,bokket::FileWriterType::APPENDFILE));
    std::cout<<">>>1"<<std::endl;
    //bokket::LogFormatter::ptr fmt(new bokket::LogFormatter("%d%T%p%T%m%n"));
    //bokket::LogFormatter::ptr fmt(new bokket::LogFormatter());
    //bokket::LogFormatter::ptr fmt(new bokket::LogFormatter("%04d-%02d-%02d-%02d-%02d-%02d-%06d"));
    //file_appender->setFormatter(fmt);
    std::cout<<">>>1"<<std::endl;
    file_appender->setLevel(bokket::LogLevel::ERROR);
    std::cout<<">>>1"<<std::endl;
   logger->addAppender(file_appender);
    std::cout<<">>>1"<<std::endl;
   BOKKET_LOG_INFO(logger)<<"test";
   std::cout<<">>>1"<<std::endl;
    std::cout<<">>>2"<<std::endl;

   //auto l=bokket::LoggerMgr::GetInstance()->getLogger("xx");
   //BOKKET_LOG_INFO(l)<<"xxx";
    std::cout<<">>>3"<<std::endl;
}