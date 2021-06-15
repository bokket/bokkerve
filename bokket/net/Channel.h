//
// Created by bokket on 2021/2/23.
//

#ifndef BOKKET_CHANNEL_H
#define BOKKET_CHANNEL_H

#include <functional>
#include <memory>
#include <sstream>

#include "../base/noncopyable.h"


namespace bokket
{

class EventLoop;

//Channel通道 表示一个请求 或者是一个链接
class Channel: public std::enable_shared_from_this<Channel>
{
public:
    using EventCallback=std::function<void()>;
    using ptr=std::shared_ptr<Channel>;

public:
    enum Status
    {
        kNew=0,// 表示还没添加到 ChannelMap 中
        kAdded,// 已添加到 ChannelMap 中
        kDelete,// 无关心事件，已从 epoll 中删除相应文件描述符，但 ChannelMap 中有记录
    };
public:


    Channel(EventLoop* loop,int fd);
    ~Channel();


    EventLoop* ownerLoop();

    void setReadCallback(EventCallback cb) { readCallback_=std::move(cb); }
    void setWriteCallback(EventCallback cb) { writeCallback_=std::move(cb); }
    void setCloseCallback(EventCallback cb) { closeCallback_=std::move(cb); }
    void setErrorCallback(EventCallback cb) { errorCallback_=std::move(cb); }


    //防止主连接在handleEvent时被销毁
    void tie(const std::shared_ptr<void>&);


    void enableReading()
    {
        events_|=kReadEvent;
        update();
    }

    void enableWriting()
    {
        events_ |= kWriteEvent;
        update();
    }

    void disableReading()
    {
        events_ &= ~kReadEvent;
        update();
    }

    void disableWriting()
    {
        events_ &= ~kWriteEvent;
        update();
    }
    void disableAll()
    {
        //events_=0;
        events_ = kNoneEvent;
        update();
    }


    bool isReading() const
    {
        return events_ & kReadEvent;
    }

    bool isWriting() const
    {
        return events_ & kWriteEvent;
    }

    bool isNoneEvent() const
    {
        return events_ == kNoneEvent;
    }

    int getFd() const { return fd_; }
    int getEvents() const { return events_; }
    void setRevents(int revt) { revents_ = revt; } // used by epoller

    void remove();

    void handleEvent();

    int getStatusInEpoll() const { return status_; }
    void setStatusInEpoll(Status status) { status_=status; }

    // for debug
    std::string eventsToString(int fd,int event) const;
    std::string eventsToString() const;
    std::string reventsToString() const;

private:
    void update();

    void handleEventsWithGuard();

    Status status_;

    static const int kNoneEvent;
    static const int kReadEvent;
    static const int kWriteEvent;

    EventLoop* loop_;// 所属EventLoop
    const int fd_;// 文件描述符，但不负责关闭该文件描述符
    int events_;// 关注的事件
    int revents_;// epoll返回的事件

    //方便找到上层持有该Channel的对象 使用weak_ptr 不会像shared_ptr的use_count()改变，因此可以用来查看某个shared_ptr指向的对象
    /*
    当我们reset了shared_ptr之后，weak_ptr会自动“到期”（expired），不会产生悬空指针。
    同时，因为weak_ptr指向某个对象，并不会使该对象的shared_ptr的use_count()改变，
    因此weak_ptr用的比较多的场景就是查看某个shared_ptr指向的对象。
    */
    std::weak_ptr<void> tie_;
    EventCallback readCallback_;
    EventCallback writeCallback_;
    EventCallback closeCallback_;
    EventCallback errorCallback_;
};

}


#endif //BOKKET_CHANNEL_H
