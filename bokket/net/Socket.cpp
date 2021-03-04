//
// Created by bokket on 2021/3/2.
//

#include "Socket.h"
#include "../base/Types.h"
#include <sys/socket.h>
#include <fcntl.h>
#include <errno.h>
#include<sstream>


using namespace bokket;
using namespace bokket::net;

Socket::~Socket()
{
    bokket::net::socks::close(sockfd_);
}



void Socket::bindAddress(const InetAddress &localaddr)
{}

void Socket::listen()
{}


int Socket::accpet(InetAddress *peeraddr)
{}

void Socket::shutdownWrite()
{}

void Socket::setTcpNoDelay(bool on)
{}

void Socket::setKeepAlive(bool on)
{}

void Socket::setReusePort(bool on)
{}

std::string Socket::getTcpInfoString() const
{
    struct tcp_info tcpInfo;
    socklen_t len=static_cast<socklen_t>(sizeof(tcpInfo));
    memset(&tcpInfo,0,sizeof(len));

    if( ::getsockopt(tcpInfo,SOL_TCP,TCP_INFO,&tcpInfo,&len)<0 )
    {
    }

    std::ostringstream oss;
    oss<<"tcpInfo"<<": ";
    oss<<tcpi.tcpi_retransmits<<  //重传数，表示当前待重传的包数，这个值在重传完毕后清零
            tcpi.tcpi_rto<<          //重传超时时间，这个和RTT有关系，RTT越大，rto越大
            tcpi.tcpi_ato<<          /*用来延时确认的估值，单位为微秒.
                                       在收到TCP报文时，会根据本次与上次接收的时间间隔来调整改制，在设置延迟确认定时器也会根据
                                       条件修改该值*/
            tcpi.tcpi_snd_mss<<     // 本端的MSS
            tcpi.tcpi_rcv_mss<<     // 对端的MSS
            tcpi.tcpi_lost<<        //本端在发送出去被丢失的报文数。重传完成后清零
            tcpi.tcpi_retrans<<     // 重传且未确认的数据段数
            tcpi.tcpi_rtt<<         //smoothed round trip time,微妙
            tcpi.tcpi_rttvar<<       //描述RTT的平均偏差，该值越大，说明RTT抖动越大
            tcpi.tcpi_snd_ssthresh<<  //拥塞控制慢开始阈值
            tcpi.tcpi_snd_cwnd<<        //拥塞控制窗口大小
            tcpi.tcpi_total_retrans;  //统计总重传的包数，持续增长。
    return oss.str().c_str();
}


int net::socks::createNonblockingOrDie()
{
    int sockfd=::socket(AF_INET,SOCK_STREAM | SOCK_NONBLOCK | SOCK_CLOEXEC,IPPROTO_TCP);
    if(sockfd<0)
    {}

    return sockfd;
}

void net::socks::setNonBlockAndCloseOnExec(int sockfd)
{
    //#define F_GETFL		3
    /* Get file status flags.  */
    int flag=::fcntl(sockfd,F_GETFL,0);

    flag|=O_NONBLOCK;

    int ret=::fcntl(sockfd,F_SETFL,flag);
    if(ret<0)
    {}

    flag=::fcntl(sockfd,F_GETFD,0);

    flag|=O_EXCL;

    ret=::fcntl(sockfd,F_SETFD,flag);
    if(ret<0)
    {}

    return;
}

void net::socks::listenOrDie(int sockfd)
{
    /* Maximum queue length specifiable by listen.  */
    //#define SOMAXCONN	128
    int ret=::listen(sockfd,SOMAXCONN);
    if(ret<0)
    {}
}

int net::socks::accpetOrDie(int sockfd, struct sockaddr_in *addr)
{
    int ret=::accept4(sockfd,static_cast<struct sockaddr*>(implict_cast<void*>(addr)),sizeof(addr),SOCK_NONBLOCK | SOCK_CLOEXEC);



}

int net::socks::bindOrDie(int sockfd, const struct sockaddr_in &addr)
{
    int ret=::bind(sockfd,static_cast<const struct sockaddr*>(implict_cast<const void*>(addr)),sizeof(addr));
    if(ret<0)
    {}
}

int net::socks::connect(int sockfd, const struct sockaddr_in &addr)
{
    return ::connect(sockfd,static_cast<const struct sockaddr*>(implict_cast<const void*>(addr)),sizeof(addr));
}

void net::socks::close(int sockfd)
{
    int ret=::close(sockfd);
    if(ret<0)
    {}
}

ssize_t net::socks::read(int sockfd, void *buf, size_t count)
{
    return ::read(sockfd,buf,count);
}

ssize_t net::socks::write(int sockfd, void *buf, size_t count)
{
    return ::write(sockfd,buf,count);
}

void net::socks::shutdownWrite(int sockfd)
{
    int ret=::shutdown(sockfd,SHUT_WR);

    if(ret<0)
    {}
}

int net::socks::getSockError(int sockfd)
{
    int optval;
    socklen_t optlen=static_cast<socklen_t>(sizeof(optlen));

    if(::getsockopt(sockfd,SOL_SOCKET,SO_ERROR,&optval,&optlen)<0)
    {
        return errno;
    }
    else
    {
        return optval;
    }

}

struct sockaddr_in net::socks::getLoaclAddr(int sockfd)
{
    struct sockaddr_in localaddr;
    memset(&localaddr,0,sizeof(localaddr));
    socklen_t addrlen=static_cast<socklen_t>(sizeof(localaddr));

    if(::getsockname(sockfd,static_cast<struct sockaddr*>(implict_cast<void*>(&localaddr)),&addrlen)<0)
    {

    }
    return localaddr;
}

struct sockaddr_in net::socks::getPeerAddr(int sockfd)
{
    struct sockaddr_in peeraddr;
    memset(&peeraddr,0,sizeof(peeraddr));
    socklen_t addrlen=static_cast<socklen_t>(sizeof(peeraddr));

    if(::getpeername(sockfd,static_cast<struct sockaddr*>(implict_cast<void*>(&peeraddr)),&addrlen)<0)
    {

    }
    return localaddr;
}


// 自连接是指(sourceIP, sourcePort) = (destIP, destPort)
// 自连接发生的原因:
// 客户端在发起connect的时候，没有bind(2)
// 客户端与服务器端在同一台机器，即sourceIP = destIP，
// 服务器尚未开启，即服务器还没有在destPort端口上处于监听
// 就有可能出现自连接，这样，服务器也无法启动了
bool net::socks::isSelfConnecnt(int sockfd)
{
    struct sockaddr_in localaddr=getLoaclAddr(sockfd);
    struct sockaddr_in peeraddr=getPeerAddr(sockfd);

    return localaddr.sin_port==peeraddr.sin_port
    &&     localaddr.sin_addr.s_addr=peeraddr.sin_addr.s_addr;
}