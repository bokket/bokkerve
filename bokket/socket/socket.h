//
// Created by bokket on 2021/6/5.
//

#ifndef BOKKERVE_PLUS_PLUS_SOCKET_H
#define BOKKERVE_PLUS_PLUS_SOCKET_H

#include <memory>
#include "../address/address.h"
#include "../base/noncopyable.h"

namespace bokket
{
class Socket: public std::enable_shared_from_this<Socket> ,public noncopyable
{
public:
    using ptr=std::shared_ptr<Socket>;
    using weak_ptr=std::weak_ptr<Socket>;

public:
    enum class Type {
        TCP=SOCK_STREAM,
        UDP=SOCK_DGRAM,
    };

    enum class Family {
        IPv4=AF_INET,
        IPv6=AF_INET6,
        UNIX=AF_UNIX,
    };


    static Socket::ptr createTCP(bokket::Address::ptr address);
    static Socket::ptr createUDP(bokket::Address::ptr address);

    static Socket::ptr createTCPSocket();
    static Socket::ptr createUDPSocket();

    static Socket::ptr createTCPSocket6();
    static Socket::ptr createUDPSocket6();


public:
    Socket(int family,int type,int protocol=0);

    ~Socket();

    int64_t getSendTimeout();
    void setSendTimeout(int64_t v);

    int64_t getRecvTimeout();
    void setRecvTimeout(int64_t v);

    bool getOption(int level,int option,void* result,size_t* len);

    template<class T>bool getOption(int level,int option,T& result) {
        size_t length=sizeof(T);
        return getOption(level,option,&result,&length);
            }

            bool setOption(int level,int option,const void* result,size_t len);
    template<class T>
            bool setOption(int level,int option,const T& result) {
        return setOption(level,option,&result,sizeof(result));
            }

public:
    Socket::ptr accpet();

    bool bind(const Address::ptr addr);
    bool connect(const Address::ptr addr,uint64_t timeout_ms=-1);
    bool listen(int backlog=SOMAXCONN);
    bool close();

public:
    int send(const void* buffer,size_t length,int flags=0);
    int send(const iovec* buffers,size_t length,int flags=0);
    int sendTo(const void* buffer,size_t length,const Address::ptr to,int flags=0);
    int sendTo(const iovec* buffers,size_t length,const Address::ptr to,int flags=0);

    int recv(void* buffer,size_t length,int flags=0);
    int recv(iovec* buffers,size_t length,int flags=0);
    int recvFrom(void* buffer,size_t length,Address::ptr from,int flags=0);
    int recvFrom(iovec* buffers,size_t length,Address::ptr from,int flags=0);

public:
    Address::ptr getRemoteAddress();
    Address::ptr getLocalAddress();

public:
    int getFamily() const { return family_; }
    int getType() const { return type_; }
    int getProtocol() const { return protocol_; }

public:
    bool isConnected() const { return isConnected_; }
    bool isValid() const { return }
    int getError();

    int getSocket() const { return sock_; }

    std::string toString();

public:
    bool cancelRead();
    bool cancelWrite();
    bool cancelAccept();
    bool cancelAll();

private:
    void initSock();
    void newSock();
    bool init(int sock);

private:
    int sock_;
    int family_;
    int type_;
    int protocol_;
    bool isConnected_;

    Address::ptr localAddress_;
    Address::ptr remoteAddress_;
};


std::ostream & operator<<(std::ostream& os,const Socket& addr);

}


#endif //BOKKERVE_PLUS_PLUS_SOCKET_H
