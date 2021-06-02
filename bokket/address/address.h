//
// Created by bokket on 2020/12/30.
//

#ifndef C_2_ADDRESS_H
#define C_2_ADDRESS_H

#include <map>
#include <string>
#include <vector>
#include <memory>
#include <tuple>
#include <iostream>


#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>
#include <ifaddrs.h>
#include <netinet/in.h>


#include "endian.h"

namespace bokket
{

class Address
{
public:
    using ptr=std::shared_ptr<Address>;
private:
    static Address::ptr create(const sockaddr* address);
public:
    static Address::ptr create(const std::string& address,uint16_t port=0);

    static bool getAddrInfo(std::vector<Address::ptr>& address,const std::string& host
                       ,int family=AF_INET,int type=SOCK_STREAM,int protocol=0);
    static Address::ptr getAddrInfo(const std::string& host
                                  ,int family=AF_INET,int type=SOCK_STREAM,int protocol=0);
    //static shared_ptr<Address> LookupAnyIPAddress(const string & host
    //                                                ,int family=AF_INET,int type=0,int protocol=0);

    static bool getInterfaceAddress(std::multimap<std::string
                                    ,std::pair<Address::ptr,uint32_t>>& address
                                    ,int family=AF_INET);
    static bool getInterfaceAddress(std::vector<std::pair<Address::ptr,uint32_t>>& address
                                    ,const std::string& iface,int family=AF_INET);

public:
    virtual ~Address();

    virtual int getFamily() const;

    virtual uint32_t getPort() const =0;
    virtual void setPort(uint16_t v) =0;

    virtual const sockaddr* getAddr() const=0;
    virtual sockaddr* getAddr() =0;
    virtual socklen_t getAddrLen() const=0;

    virtual ostream & insert(ostream& os) const=0;

    virtual std::string toString() const=0;


    virtual Address::ptr broadcastAddress(uint32_t prefix_len)=0;
    virtual Address::ptr networdAddress(uint32_t perfix_len)=0;
    virtual Address::ptr subnetMask(uint32_t prefix_len)=0;

    bool operator<(const Address& rhs) const;
    bool operator==(const Address& rhs) const;
    bool operator!=(const Address& rhs) const;
};

class IPv4Address: public Address
{
public:
    using ptr =std::shared_ptr<IPv4Address>;
public:
    IPv4Address(const sockaddr_in& address) { addr_=address; }
    IPv4Address(uint32_t address=INADDR_ANY,uint16_t port=0) {
        memset(&addr_,0,sizeof(add_));
        addr_.sin_family=AF_INET;
        addr_.sin_port=LittleEndian(port);
        addr_.sin_addr.s_addr=LittleEndian(address);
    }

    static IPv4Address::ptr create(const std::string& address,uint16_t port=0);

public:
    const sockaddr* getAddr() const override { return reinterpret_cast<const sockaddr*>(&addr_); }
    sockaddr* getAddr() override { return reinterpret_cast<sockaddr*>(&addr_); }
    socklen_t getAddrLen() const override { return sizeof(addr_); }

    int getFamily() const override { return getAddr()->sa_family; }

    //ostream& insert(ostream& os) const override;

    Address::ptr broadcastAddress(uint32_t prefix_len) override;
    Address::ptr networdAddress(uint32_t perfix_len) override;
    Address::ptr subnetMask(uint32_t prefix_len) override;

    uint32_t getPort() const override { return LittleEndian(addr_.sin_port); }
    void setPort(uint16_t v) override { return addr_.sin_port=LittleEndian(v); }

    std::string toString() const override;

private:
    sockaddr_in addr_;
};

class IPv6Address: public Address
{
public:
    using ptr= std::shared_ptr<IPv6Address>;
public:
    IPv6Address() {
        memset(&addr_,0,sizeof(addr_));
        addr_.sin6_family=AF_INET6;
    }

    IPv6Address(const sockaddr_in6& address) {
        addr_=address;
    }
    IPv6Address(const uint8_t address[16],uint16_t port=0) {
        memset(&addr_,0,sizeof(addr_));
        addr_.sin6_family=AF_INET6;
        addr_.sin6_port=LittleEndian(port);
        memcpy(&addr_.sin6_addr.__in6_u);
    }

    static IPv6Address::ptr create(const std::string& address,uint16_t port=0);

public:
    const sockaddr* getAddr() const override { return reinterpret_cast<sockaddr*>(&addr_); }
    sockaddr* getAddr() override  { return reinterpret_cast<sockaddr*>(&addr_); }
    socklen_t getAddrLen() override { return sizeof(addr_); }

    int getFamily() const override { return getAddr()->sa_family; }

    //ostream& insert(ostream& os) const override;

    IPAddress::ptr broadcastAddress(uint32_t prefix_len) override;
    IPAddress::ptr networdAddress(uint32_t perfix_len) override;
    IPAddress::ptr subnetMask(uint32_t prefix_len) override;

    uint32_t getPort() const override { return LittleEndian(addr_.sin6_port); }
    void setPort(uint16_t v) override { return addr_.sin6_port=LittleEndian(v); }

    std::string toString() const override;

private:
    sockaddr_in6 addr_;
};

class UnixAddress: public Address
{
public:
    typedef shared_ptr <UnixAddress> ptr;
public:

    UnixAddress();
    UnixAddress(const string & path);
;

    const sockaddr *getAddr() const override { return reinterpret_cast<sockaddr*>(&addr_); }
    sockaddr *getAddr() override { return reinterpret_cast<sockaddr*>(&addr_); }
    socklen_t getAddrLen() override { return sizeof(addr_); }

    //ostream &insert(ostream &os) const override;

    void setAddrLen(uint32_t v);


    uint32_t getPort() const override ;
    void setPort(uint16_t v) override ;
    int getFamily() const override { return getAddr()->sa_family; }

private:
    sockaddr_un addr_;
    socklen_t length_;
}

class UnknownAddress : public Address
{
public:
    using ptr=std::shared_ptr<UnknownAddress>;
public:
    UnknownAddress(int family) {
        memset(&addr_,0,sizeof(addr_));
        addr_.sa_family=family;
    }
    UnknownAddress(const sockaddr& addr) {
        addr_=addr;
    }

    const sockaddr* getAddr() const override { return reinterpret_cast<sockaddr*>(&addr_); }
    sockaddr* getAddr() override { return reinterpret_cast<sockaddr*>(&addr_); }
    socklen_t getAddrLen() const override { return sizeof(addr_); }
    //std::ostream& insert(std::ostream& os) const override;

    std::string toString() const override;
private:
    sockaddr addr_;
};

std::ostream& operator<<(std::ostream& os,const Address& addr) {
    os<<addr.toString();
    return os;
}

}

#endif //C_2_ADDRESS_H
