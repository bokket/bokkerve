//
// Created by bokket on 2021/2/24.
//

#include "Epoller.h"
#include <cerrno>

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
                ,events_(128)
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