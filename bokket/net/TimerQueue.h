//
// Created by bokket on 2021/2/28.
//

#ifndef BOKKET_TIMERQUEUE_H
#define BOKKET_TIMERQUEUE_H

#include <set>
#include <vector>
#include "../base/noncopyable.h"
#include "Timestamp.h"
#include "CallBack.h"
#include "Channel.h"

namespace bokket
{
    namespace net
    {

        class EventLoop;
        class Timer;
        class TimerId;

        class TimerQueue: public noncopyable
        {
        public:
            explicit TimerQueue(EventLoop* loop);
            ~TimerQueue();

            Timer* add


        private:
            //unique_ptr
            // 无法得到指向同一对象的两个unique_ptr指针
            // 但可以进行移动构造与移动赋值操作，即所有权可以移动到另一个对象（而非拷贝构造）
            using Entry=pair<Timestamp,Timer*>;
            using TimerList=set<Entry>;
        private:
            EventLoop* loop_;
            const int timerfd_;
            Channel timerfdChannel_;


        };
    }
}


#endif //BOKKET_TIMERQUEUE_H
