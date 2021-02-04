//
// Created by bokket on 2020/11/22.
//

#ifndef TIMER_TIMER_H
#define TIMER_TIMER_H
#include <time.h>
#define BUFFER_SIZE 64
class util_timer;
struct client_data
{
    sockaddr_in address;
    int sockfd;
    char buf[BUFFER_SIZE];
    util_timer* timer;
};
class util_timer
{
public:
    util_timer():prev(NULL),next(NULL){}

public:
    time_t expire;
    void (*cb_func)(client_data*);

    client_data* user_data;
    util_timer* prev;
    util_timer* next;
};

class sort_timer_lst
{
public:
    sort_timer_lst():head(NULL),tail(NULL){}
    ~sort_timer_lst();
    void add_timer(util_timer* timer);
    void adjust_timer(util_timer* timer);
    void del_timer(util_timer* timer);
    void tick();

private:
    void add_timer(util_timer* timer,util_timer* lst_head);
    util_timer* head;
    util_timer* tail;
};


#endif //TIMER_TIMER_H
