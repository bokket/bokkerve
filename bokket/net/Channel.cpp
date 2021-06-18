//
// Created by bokket on 2021/2/23.
//

#include "Channel.h"

#include "EventLoop.h"

#include <unistd.h>
#include <cassert>
#include <sys/epoll.h>



namespace bokket 
{

const int Channel::kNoneEvent=0;
                                //0x001 | 0x002
const int Channel::kReadEvent = POLLIN | POLLPRI;//有紧迫数据可读
                               //0x004
const int Channel::kWriteEvent = POLLOUT;//写数据不会导致阻塞


Channel::Channel(EventLoop *loop, int fd)
                :loop_(loop)
                ,fd_(fd)
                ,events_(0)
                ,revents_(0)
                ,status_(Epoller::Status::kNew)
                ,tied_(false)
                ,eventHandling_(false)
{}

Channel::~Channel()
{
    //是否处于处理事件中
    assert(!eventHandling_);
    ::close(fd_);
}

void Channel::handleEvent()
{
    loop_->assertInLoopThread();
    //eventHandling_= true;

    if(tied_)
    {
        auto guard=tie_.lock();
        if(guard!= nullptr)
            handleEventsWithGuard();
    }
    else
        handleEventsWithGuard();
}

void Channel::handleEventsWithGuard()
{
    eventHandling_= true;
    if( (revents_& EPOLLHUP) && !(revents_&EPOLLIN) )
    {
        if(closeCallback_)
            closeCallback_();
    }
    if(revents_&EPOLLERR )
    {
        if(errorCallback_)
            errorCallback_();
    }
    if(revents_& (EPOLLIN | EPOLLPRI || EPOLLRDHUP) )
    {
        if(readCallback_)
            readCallback_();
    }
    if(revents_ & EPOLLOUT)
    {
        if(writeCallback_)
            writeCallback_();
    }
    eventHandling_= false;
}


void Channel::update()
{
    //通知所属EventLoop该通道读写权限已改变
    loop_->updateChannel(this);
}

// 调用这个函数之前确保调用disableAll
void Channel::remove()
{
    assert(isNoneEvent());
    loop_->removeChannel(this);
}

std::string Channel::eventsToString() const
{
    return eventsToString(fd_,events_);
}

std::string Channel::reventsToString() const
{
    return eventsToString(fd_,revents_);
}

std::string Channel::eventsToString(int fd, int event) const
{
    std::ostringstream oss;
    oss<<fd<<": ";
    oss << "IN ";
    //0x002
    if (event & EPOLLPRI)
        oss << "PRI ";
    //0x004
    if (event & EPOLLOUT)
        oss << "OUT ";
    //0x010
    if (event & EPOLLHUP)
        oss << "HUP ";
    // 0x2000
    if (event & EPOLLRDHUP)
        oss << "RDHUP ";
    //0x008
    if (event & EPOLLERR)
        oss << "ERR ";
    //0x001
    if (event & EPOLLIN)
        oss << "IN ";

    return oss.str().c_str();
}



}
