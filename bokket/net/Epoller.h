//
// Created by bokket on 2021/2/24.
//

#ifndef BOKKET_EPOLLER_H
#define BOKKET_EPOLLER_H

#include "../base/Timestamp.h"
#include "Channel.h"
#include "EventLoop.h"
#include "../base/noncopyable.h"

#include <map>
#include <sys/epoll.h>
#include <vector>

using namespace std;

namespace bokket
{
namespace net
{
    class EventLoop;
    class Channel;

    class Epoller: public noncopyable
    {
    public:
        using ChannelList=vector<Channel*>;
    public:
        Epoller(EventLoop* loop);

        ~Epoller();

        Timestamp poll(int timeoutMs,ChannelList& activeChannels);
        void poll(ChannelList& activeChannels);
        void updateChannel(Channel* channel);
        void removeChannel(Channel* channel);
        bool hasChannel(Channel* channel) const;

        int getEventListSize() { return events_.size(); }
       // static void setEventListSize(int size) { kInitEventListSize=size; }

    private:
        void fillActiveChannels(int numEvents,ChannelList& activeChannels) const;
        void update(int op,Channel* channel);
        void assertInLoopThread() const;

        static const int kInitEventListSize;

        using ChannelMap=map<int,Channel*>;
        ChannelMap channels_;

        EventLoop* loop_;

        using EventList=vector<struct epoll_event>;
        EventList events_;
        int epollfd_;
    };
}
}


#endif //BOKKET_EPOLLER_H
