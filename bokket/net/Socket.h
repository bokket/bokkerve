//
// Created by bokket on 2021/3/2.
//

#ifndef BOKKET_SOCKET_H
#define BOKKET_SOCKET_H

#include "../base/noncopyable.h"
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <string>
#include <ostream>


namespace bokket
{
namespace net
{

    class InetAddress;
class Socket: public noncopyable
{
public:
    explicit Socket(int sockfd)
                   :sockfd_(sockfd)
    {}
    ~Socket();

    std::string getTcpInfoString() const;

    int getFd() const { return sockfd_; }
    void bindAddress(const InetAddress& localaddr);

    void listen();

    int accpet(InetAddress* peeraddr);

    void shutdownWrite();

    void setTcpNoDelay(bool on);

    void setReusePort(bool on);

    void setKeepAlive(bool on);
private:
    const int sockfd_;
};

}
}

namespace bokket::net::socks
{
    int createNonblockingOrDie();
    void setNonBlockAndCloseOnExec(int sockfd);


    int connect(int sockfd,const struct sockaddr_in& addr);

    int bindOrDie(int sockfd,const struct sockaddr_in& addr);

    void listenOrDie(int sockfd);

    int accpetOrDie(int sockfd,struct sockaddr_in* addr);

    //size_t 反映内存中对象的大小（以字节为单位）
    //ssize_t 供返回字节计数或错误提示的函数使用

    ssize_t read(int sockfd,void *buf,size_t count);
    ssize_t write(int sockfd,void *buf,size_t count);

    void close(int sockfd);

    void shutdownWrite(int sockfd);

    int getSockError(int sockfd);

    struct sockaddr_in getLoaclAddr(int sockfd);

    struct sockaddr_in getPeerAddr(int sockfd);

    bool isSelfConnecnt(int sockfd);
}



#endif //BOKKET_SOCKET_H
