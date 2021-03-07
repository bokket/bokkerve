//
// Created by bokket on 2021/3/6.
//

#ifndef BOKKET_ACCEPTOR_H
#define BOKKET_ACCEPTOR_H

#include "../base/noncopyable.h"
#include "Channel.h"
#include "Socket.h"
#include "InetAddress.h"
#include "EventLoop.h"
#include <functional>
#include <atomic>
namespace bokket
{
namespace net
{
class EventLoop;
class InetAddress;

class Acceptor: public noncopyable
{
public:
    using NewConnectionCallback=function<void (int sockfd,const InetAddress &) >;
public:
    Acceptor(EventLoop* loop,const InetAddress& listenAddr);
    ~Acceptor();

    bool listening() const
    { return listening_; }

    void listen();

    //设置新连接到来的回调函数
    void setNewConnectionCallback(NewConnectionCallback cb) { newConnectionCallback_=std::move(cb); }

    InetAddress getLocalAddr() const
    {
        return localaddr_;
    }

    int getClinetCount() const
    {
        return clinetCount_;
    }

    bool isListening() const
    {
        return listening_;
    }

private:
    void handleRead();

    bool listening_;
    EventLoop* loop_;
    Socket accpetSocket_;
    Channel accpetChannel_;

    InetAddress localaddr_;
    NewConnectionCallback newConnectionCallback_;

    std::atomic<int> clinetCount_;
};

}
}



#endif //BOKKET_ACCEPTOR_H
