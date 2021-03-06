//
// Created by bokket on 2021/3/2.
//

#include "InetAddress.h"
#include <cstring>
#include <arpa/inet.h>

using namespace bokket;
using namespace bokket::net;

InetAddress::InetAddress(uint16_t port)
{
    memset(&addr_,0,sizeof(addr_));
    addr_.sin_family=AF_INET;
    addr_.sin_addr.s_addr=::htonl(INADDR_ANY);
    addr_.sin_port=::htons(port);

    //addr_.sin_port=htobe16(port);
}


InetAddress::InetAddress(const std::string &ip, uint16_t port)
{
    memset(&addr_,0,sizeof(addr_));
    addr_.sin_family=AF_INET;
    int ret=::inet_pton(AF_INET,ip.c_str(),&addr_.sin_addr);
    if(ret!=1)
    {}
    addr_.sin_port=htons(port);
}

std::string InetAddress::toIp() const
{
    char buf[INET_ADDRSTRLEN];
    const char* ret=::inet_ntop(AF_INET,&addr_.sin_addr,buf,sizeof(buf));
    if(ret== nullptr)
    {
        buf[0]='\0';
    }
    return std::string(buf);
}

std::string InetAddress::toPort() const
{
    uint16_t port=be16toh(addr_.sin_port);
    return std::to_string(port);
}

uint16_t InetAddress::getPort() const
{
    return ntohs(addr_.sin_port);
}

uint32_t InetAddress::getIp() const
{
    return ntohl(addr_.sin_addr.s_addr);
}

std::string InetAddress::toIpPort() const
{
    std::string ret=toIp();
    ret.push_back(':');


    return ret.append(toPort());
}