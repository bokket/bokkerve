//
// Created by bokket on 2021/6/23.
//

#include "../bokket/bokket.h"

static bokket::Logger::ptr g_logger=BOKKET_LOG_NAME("test");


static int timeout=1000;
static bokket::Timer::ptr t_timer;

void timer_callback() {
    BOKKET_LOG_INFO(g_logger)<<"timer_callback(),timeout="<<timeout;
    timeout+=1000;
    if(timeout<5000) {
        t_timer->reset(timeout, true);
    } else {
        t_timer->cancel();
    }
}


void test_timer() {
    bokket::IOManager iom;

    t_timer=iom.addTimer(1000,timer_callback,true);

    iom.addTimer(500, []{
        BOKKET_LOG_INFO(g_logger)<<"500 ms timeout";
    });

    iom.addTimer(5000, []{
        BOKKET_LOG_INFO(g_logger)<<"5000 ms timeout";
    });
}

int main() {
    test_timer();
    BOKKET_LOG_INFO(g_logger)<<"end";
}