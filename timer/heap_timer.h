//
// Created by bokket on 2020/12/1.
//

#ifndef C_2_HEAP_TIMER_H
#define C_2_HEAP_TIMER_H

#include <iostream>
#include <netinet/in.h>
#include <time.h>

using std::exception;
#define BUFFER_SIZE 64
class heap_timer;

struct client_data
{
    sockaddr_in address;
    int sockfd;
    char buf[BUFFER_SIZE];
    util_timer* timer;
};


#endif //C_2_HEAP_TIMER_H
