//
// Created by bokket on 2021/2/24.
//

#ifndef BOKKET_EPOLLER_H
#define BOKKET_EPOLLER_H




#include <map>
#include <unordered_map>
#include <vector>

#include <sys/epoll.h>

//#include "Channel.h"
#include "EventLoop.h"
#include "../base/noncopyable.h"



namespace bokket
{

class EventLoop;
class Channel;

class Epoller: public noncopyable
{
public:
    using ChannelList=std::vector<Channel*>;
public:
    Epoller(EventLoop* loop);

    ~Epoller();

    void poll(int timeoutMs,ChannelList* activeChannels);
    //void poll(ChannelList& activeChannels);
    void updateChannel(Channel* channel);
    void removeChannel(Channel* channel);
    bool hasChannel(Channel* channel) const;

    int getEventListSize() { return events_.size(); }
    // static void setEventListSize(int size) { kInitEventListSize=size; }

public:
    enum class Status {
        kNew=0,// 表示还没添加到 ChannelMap 中
        kAdded,// 已添加到 ChannelMap 中
        kDelete,// 无关心事件，已从 epoll 中删除相应文件描述符，但 ChannelMap 中有记录
    };

private:
    void fillActiveChannels(int numEvents,ChannelList* activeChannels) const;
    void update(int op,Channel* channel);
    void assertInLoopThread() const;

    static const int kInitEventListSize=108;

    using ChannelMap=std::unordered_map<int,Channel*>;
    ChannelMap channels_;

    EventLoop* loop_;

    using EventList=std::vector<struct epoll_event>;
    EventList events_;
    int epollfd_;
};

}


#endif //BOKKET_EPOLLER_H
