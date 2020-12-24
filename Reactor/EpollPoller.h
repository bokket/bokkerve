//
// Created by bokket on 2020/12/9.
//

#ifndef C_2_EPOLLPOLLER_H
#define C_2_EPOLLPOLLER_H
#pragma once

#include <boost/noncopyable.hpp>
#include <vector>
#include <sys/epoll.h>
#include <memory>
#include <unistd.h>
#include <assert.h>

class Handler;
class EpollPoller:boost::noncopyable
{
    typedef vector<epoll_event> EventList;
public:
    EpollPoller();
    ~EpollPoller();
    void update(Handler*);
    void remove(Handler*);
    vector<Handler*> poll();

    static const int KInitEventListSize=16;
private:
    vector<Handler*> fillActiveChannels(int);

private:
    int epfd_;
    EventList activeEventList_;
};


#endif //C_2_EPOLLPOLLER_H
