//
// Created by bokket on 2021/5/16.
//

#include <mutex>
#include <vector>
#include "../bokket/Log/Log.h"
#include "../bokket/thread/thread.h"
#include "../bokket/thread/util.h"

//#include "../bokket/bokket.h"
bokket::Logger::ptr g_logger = BOKKET_LOG_ROOT();

int cnt=0;

std::mutex mutex_;

void fun1() {
    BOKKET_LOG_INFO(g_logger)<<"name:"<<bokket::Thread::getNowThreadName()
                             <<" this.name:"<<bokket::Thread::getThis()->getName()
                             <<" id:"<<bokket::getThreadId()
                             <<" this.id:"<<bokket::Thread::getThis()->getId();

    for(int i=0;i<100000;i++) {
        std::lock_guard<std::mutex> lockGuard(mutex_);
        ++cnt;
    }
}

void fun2() {
    while (true) {
        BOKKET_LOG_INFO(g_logger)<<"xxxxxxxxxxxxxxxxxxxxxxxxxxx";
    }
}

void fun3() {
    while(true) {
        BOKKET_LOG_INFO(g_logger)<<"===========================";
    }
}

int main() {
    BOKKET_LOG_INFO(g_logger)<<"thread test begin";

    std::vector<bokket::Thread::ptr> thread;
    for(int i=0;i<5;++i) {
        bokket::Thread::ptr thread1(new bokket::Thread(&fun1,"name_"+std::to_string(i*2)));
        //bokket::Thread::ptr thread2(new bokket::Thread(&fun2,"name_"+std::to_string(i*2+1)));
        thread.emplace_back(thread1);
        //thread.emplace_back(thread2);
    }
    for(auto i=0;i<thread.size();++i) {
        thread[i]->join();
    }

    BOKKET_LOG_INFO(g_logger)<<"thread test end";
    BOKKET_LOG_INFO(g_logger)<<"count="<<cnt;
}