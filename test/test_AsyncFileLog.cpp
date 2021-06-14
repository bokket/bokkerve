//
// Created by bokket on 2021/6/9.
//

#include <iostream>
#include "../bokket/Log/Log.h"
#include "../bokket/Log/AsyncFileLogAppender.h"


int main() {
    //bokket::LogAppenderAsyncFile file("test");

    auto m=bokket::LoggerMgr::GetInstance()->getLogger("test");
    bokket::LogAppenderAsyncFile::ptr file(new bokket::LogAppenderAsyncFile("./log-"));

    //file->start();
    //m->addAppender(bokket::LogAppender::ptr(new bokket::LogAppenderAsyncFile("./log-")));
    m->addAppender(file);
    BOKKET_LOG_INFO(m)<<"file test";

}