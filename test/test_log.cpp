//
// Created by bokket on 2021/4/11.
//



#include <chrono>
#include <string>
#include <iostream>

#include "gtest/gtest.h"
#include "../bokket/Log/Log.h"
//#include "../bokket/Log/AsyncFileLogAppender.h"
//#include "../bokket/Log/AsyncFileLogAppender.cpp"
#include "../bokket/thread/thread.h"
#include "../bokket/thread/util.h"

/*TEST(FileBenchmarks,LogFileappend) {
    auto l=bokket::LoggerMgr::GetInstance()->getLogger("test");
    l->addAppender(bokket::LogAppender::ptr(new bokket::LogAppenderAsyncFile("./log-",20)));

    const int32_t count=100000;
    auto start=std::chrono::system_clock::now();

    for(int32_t i=0;i<count;++i) {
        BOKKET_LOG_INFO(l);
    }
    auto end=std::chrono::system_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << count << "次运行耗时" << duration.count() << "us" << std::endl;
}*/

TEST(StdoutBenchmarks,LogStdoutAppender) {
    bokket::Logger::ptr logger(new bokket::Logger);

    bokket::LogAppender::ptr Stdout(new bokket::LogAppenderStdout);
    logger->addAppender(Stdout);

    Stdout->setLevel(bokket::LogLevel::ERROR);

    auto start=std::chrono::system_clock::now();

    const int32_t count=100000;
    for(int32_t i=0;i<count;++i) {
        BOKKET_LOG_ERROR(logger)<<"test";
    }
    auto end=std::chrono::system_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << count << "次运行耗时" << duration.count() << "us" << std::endl;

    BOKKET_LOG_INFO(logger)<<"test";

    BOKKET_LOG_ERROR(logger)<<"test macro error";

    BOKKET_LOG_FMT_ERROR(logger, "test macro fmt error %s", "wxz");

    auto l=bokket::LoggerMgr::GetInstance()->getLogger("xx");

    BOKKET_LOG_DEBUG(l)<<"xxxxxxx";
    BOKKET_LOG_INFO(l)<<"xxxxxx";
    BOKKET_LOG_ERROR(l)<<"error";
    BOKKET_LOG_FATAL(l)<<"fatal";

    /*auto m=bokket::LoggerMgr::GetInstance()->getLogger("test");
    m->addAppender(bokket::LogAppender::ptr(new bokket::LogAppenderAsyncFile("./log-",20)));
    BOKKET_LOG_INFO(m)<<"file test";*/
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

/*
int main()
{
    bokket::Logger::ptr logger(new bokket::Logger);

    bokket::LogAppender::ptr Stdout(new bokket::LogAppenderStdout);
    logger->addAppender(Stdout);

    Stdout->setLevel(bokket::LogLevel::ERROR);

    BOKKET_LOG_INFO(logger)<<"test";
    BOKKET_LOG_ERROR(logger)<<"test macro error";

    BOKKET_LOG_FMT_ERROR(logger, "test macro fmt error %s", "wxz");

    auto l=bokket::LoggerMgr::GetInstance()->getLogger("xx");
    BOKKET_LOG_INFO(l)<<"xxx";
}*/
/*
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
}*/