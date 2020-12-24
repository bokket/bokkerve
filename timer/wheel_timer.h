//
// Created by bokket on 2020/11/30.
//

#ifndef C_2_WHEEL_TIMER_H
#define C_2_WHEEL_TIMER_H


#include <time.h>
#include <netinet/in.h>
#include <stdio.h>

#define BUFFER_SIZE 64
class tw_timer;
struct client_data
{
    sockaddr_in address;
    int sockfd;
    char buf[BUFFER_SIZE];
    util_timer* timer;
};

class tw_timer
{
public:
    tw_timer(int rot,int ts):prev(NULL),next(NULL),rotation(rot),time_slot(ts){}

public:
    int rotation;//记录定时器在时间轮里多久生效
    int time_slot;//记录定时器属于时间轮上哪个槽

    void (*cb_func)(client_data*);
    client_data* user_data;
    tw_timer* prev;
    tw_timer* next;
};

class time_wheel
{
public:
    time_wheel();
    ~time_wheel();
    tw_timer* add_timer(int timeout);
    void del_timer(tw_timer* timer);
    void tick();

private:
    static const int N=60;//时间轮上的槽数目
    static const int SI=1;//每1秒转一次，槽间隔为1
    tw_timer* slots[N];//时间轮的槽，其中每一个元素指向一个定时器链表
    int cur_slot;//时间轮的当前槽
};

#endif //C_2_WHEEL_TIMER_H
