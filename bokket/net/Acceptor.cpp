//
// Created by bokket on 2021/3/6.
//

#include "Acceptor.h"
#include <errno.h>
#include <fcntl.h>

using namespace bokket;
using namespace net;

Acceptor::Acceptor(EventLoop *loop, const InetAddress &listenAddr)
                  :loop_(loop)
                  ,accpetSocket_(net::socks::createNonblockingOrDie())
                  ,accpetChannel_(loop,accpetSocket_.getFd())
                  ,localaddr_(listenAddr)
                  ,listening_(false)
                  ,clinetCount_(0)
{
    accpetSocket_.setReuseAddr(true);
    accpetSocket_.setReusePort(true);
    accpetSocket_.bindAddress(localaddr_);
    accpetChannel_.setReadCallback(std::bind(&Acceptor::handleRead,this));
    clinetCount_++;
}

Acceptor::~Acceptor()
{
    accpetChannel_.disableAll();
    accpetChannel_.remove();

    net::socks::close();
}

void Acceptor::listen()
{
    loop_->assertInLoopThread();

    listening_=true;
    accpetSocket_.listen();

    accpetChannel_.enableReading();
}

void Acceptor::handleRead()
{
    loop_->assertInLoopThread();
    InetAddress peerAddr(0);

    int connfd=accpetSocket_.accpet(&peerAddr);
    if(newConnectionCallback_)
        newConnectionCallback_(connfd,peerAddr);
    else
        net::socks::close(connfd);

}