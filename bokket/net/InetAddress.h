//
// Created by bokket on 2021/3/2.
//

#ifndef BOKKET_INETADDRESS_H
#define BOKKET_INETADDRESS_H

#include "../base/copyable.h"
#include <netinet/in.h>
#include <string>

namespace bokket
{
namespace net
{

class InetAddress: public copyable
{
public:

    explicit InetAddress(uint16_t port);

    ~InetAddress();


    InetAddress(const std::string & ip,uint16_t port);

    InetAddress(const struct sockaddr_in& addr)
               :addr_(addr)
    {}
    
    void setSockAddress(const struct sockaddr_in& addr)
    {
        addr_=addr;
    }
    
    const struct sockaddr_in& getSockAddress() const
    {
        //return reinterpret_cast<const struct sockaddr*>(&addr_);
        return addr_;
    }



    socklen_t getSocklen() const
    {
        return sizeof(addr_);
    }
     
    std::string toIp() const;
    std::string toPort() const;

    std::string toIpPort() const;
    uint16_t getPort() const;
    uint32_t getIp() const;
private:
    struct sockaddr_in addr_;
};

}
}


#endif //BOKKET_INETADDRESS_H
