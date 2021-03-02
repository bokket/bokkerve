//
// Created by bokket on 2021/3/2.
//

#ifndef BOKKET_INETADDRESS_H
#define BOKKET_INETADDRESS_H

#include "../base/copyable.h"
#include <netinet/in.h>
#include <string_view>


namespace bokket
{
namespace net
{

class InetAddress: public copyable
{
public:

    explicit InetAddress(uint16_t port);

    ~InetAddress();


    InetAddress(std::string_view ip,uint16_t port);

    InetAddress(const struct sockaddr_in& addr)
               :addr_(addr)
    {}
    
    void setAddress(const struct sockaddr_in& addr)
    {
        addr_=addr;
    }
    
    const struct sockaddr_in* getSockaddr() const
    {
        return reinterpret_cast<const struct sockaddr*>(&addr_);
    }

    socklen_t getSocklen() const
    {
        return sizeof(addr_);
    }
     
    string toIp() const;

    string toIpPort() const;
private:
    struct sockaddr_in addr_;
};

}
}


#endif //BOKKET_INETADDRESS_H
