//
// Created by bokket on 2021/2/24.
//

#include "Epoller.h"
#include <cerrno>
#include <cstring>

using namespace bokket;
using namespace bokket::net;



//使用epoll_create1的优点是它允许你指定标志,这些标志目前限于close-on-exec(因此在执行另一个进程时文件描述符会自动关闭).
/*
int epoll_create1(int flags);
功能：创建一个多路复用的实例 参数：flags
flags：
0:如果这个参数是0，这个函数等价于epoll_create（0）
EPOLL_CLOEXEC：这是这个参数唯一的有效值，如果这个参数设置为这个。那么当进程替换映像的时候会关闭这个文件描述符，这样新的映像中就无法对这个文件描述符操作，适用于多进程编程+映像替换的环境里
返回值：
成功：返回一个非0 的未使用过的最小的文件描述符
错误：-1 errno被设置
*/
Epoller::Epoller(EventLoop *loop)
                :loop_(loop)
                ,events_(kInitEventListSize)
                ,epollfd_(::epoll_create1(EPOLL_CLOEXEC))
{
    if(epollfd_==-1)
    {
    }
}

Epoller::~Epoller()
{
    ::close(epollfd_);
}

Timestamp Epoller::poll(int timeoutMs, ChannelList &activeChannels)
{
    loop_->assertInLoopThread();
    int maxEvents=static_cast<int>(events_.size());
    int numEvents=epoll_wait(epollfd_,events_.data(),maxEvents,timeoutMs);

    Timestamp now(Timestamp::now());

    if(numEvents!=-1)
    {
        if(errno!=EINTR)
        {}
    }
    else if(numEvents==0)
    {

    }
    else
    {
        fillActiveChannels(numEvents,activeChannels);

        if(static_cast<size_t>(numEvents)==maxEvents)
            events_.resize(events_.size()*2);
    }


    return now;
}


void Epoller::fillActiveChannels(int numEvents, ChannelList& activeChannels) const
{
    //ChannelMap::const_iterator it=channels_.find()
    assert(static_cast<size_t>(numEvents)<=events_.size());

    for(int i=0;i<numEvents;i++)
    {
        //Channel* channel=static_cast<Channel*>(events_[i].data.ptr);
        auto channel=static_cast<Channel*>(events_[i].data.ptr);

        channel->setRevents(events_[i].events);

        activeChannels.push_back(channel);
    }
}


void Epoller::update(int op, Channel *channel)
{
    struct epoll_event event;
    event.events=channel->getEvents();
    event.data.ptr=channel;
    auto fd=channel->getFd();

    if(::epoll_ctl(epollfd_,op,fd,&event) <0)
    {
        if(op==EPOLL_CTL_DEL)
        {}
        else
        {}
    }

}

void Epoller::updateChannel(Channel *channel)
{
    loop_->assertInLoopThread();
    int op=0;

    auto fd=channel->getFd();
    auto status=channel->getStatusInEpoll();


    if(status==net::Channel::kNew || status==net::Channel::kDelete )
    {
        if(status==net::Channel::kNew)
        {
            assert(channels_.find(fd)!=channels_.end());
            channels_[fd]=channel;
        }
        else
        {
            assert(channels_.find(fd)!=channels_.end());
            assert(channels_[fd]==channel);
        }
        channel->setStatusInEpoll(Channel::kAdded);
        update(EPOLL_CTL_ADD,channel);
    }
    else
    {
        assert(channels_.find(fd)!=channels_.end());
        assert(channels_[fd]==channel);
        assert(status==net::Channel::kAdded);


        if(channel->isNoneEvent())
        {
            update(EPOLL_CTL_DEL,channel);
            channel->setStatusInEpoll(Channel::kDelete);
        }
        else
        {
            update(EPOLL_CTL_MOD,channel);
        }
    }

}

void Epoller::removeChannel(Channel *channel)
{
    assertInLoopThread();
    auto fd=channel->getFd();

    channels_.erase(fd);

    if(channel->getStatusInEpoll()==net::Channel::kAdded)
    {
        update(EPOLL_CTL_DEL,channel);
    }
    channel->setStatusInEpoll(Channel::kNew);
}

bool Epoller::hasChannel(Channel *channel) const
{
    assertInLoopThread();
    auto item=channels_.find(channel->getFd());
    return item!=channels_.end() && item->second==channel;
}

void Epoller::assertInLoopThread() const
{
    loop_->assertInLoopThread();
}