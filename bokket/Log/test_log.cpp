//
// Created by bokket on 2021/4/11.
//

#include "Log.h"
#include "LogLevel.h"
#include "FileWriterType.h"
#include "gtest/gtest.h"
#include <chrono>
#include <iostream>

int main() {
    bokket::Logger::ptr logger=std::make_shared<bokket::Logger>();

    bokket::LogAppender::ptr appender=std::make_shared<bokket::LogAppenderStdout>();
    logger->addAppender("xx",appender);


    bokket::LogAppenderFile::ptr file_appender=std::make_shared<bokket::LogAppenderFile>("./log.txt",0,0,0,bokket::FileWriterType::APPENDFILE);

    bokket::LogFormatter::ptr fmt=std::make_shared<bokket::LogFormatter>("%d%T%p%T%m%n");
    file_appender->setFormatter(fmt);
    file_appender->setLevel(bokket::LogLevel::ERROR);

    logger->addAppender("log",file_appender);

    BOKKET_LOG_INFO(logger)<<"test";
}