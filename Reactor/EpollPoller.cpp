//
// Created by bokket on 2020/12/9.
//

#include "EpollPoller.h"
#include "Handler.h"

EpollPoller::EpollPoller()
            :activeEventList_(KInitEventListSize)
{
    epfd_=epoll_create1(EPOLL_CLOEXEC);
}

EpollPoller::~EpollPoller()
{
    close(epfd_);
}

void EpollPoller::update(Handler *handler)
{
    epoll_event epollEvent;
    epollEvent.events=handler->ev
}