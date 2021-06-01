//
// Created by bokket on 2020/12/30.
//

#ifndef C_2_ADDRESS_H
#define C_2_ADDRESS_H

#include <map>
#include <string>
#include <vector>
#include <memory>
#include <iostream>


#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <arpa/inet.h>
#include <unistd.h>

using namespace std;

namespace bokket
{

class Address
{
public:
    using ptr=std::shared_ptr<Address>;
public:
    static Address::ptr create(const sockaddr* addr,socklen_t addrlen);

    static bool Lookup(vector<Address::ptr>& result,const string& host
                       ,int family=AF_INET,int type=0,int protocol=0);
    static Address::ptr LookupAny(const string& host
                                  ,int family=AF_INET,int type=0,int protocol=0);
    static shared_ptr<IPAddress> LookupAnyIPAddress(const string & host
                                                    ,int family=AF_INET,int type=0,int protocol=0);

    static bool GetInterfaceAddress()

    virtual ~Address();

    int getFamily() const;

    virtual const sockaddr* getAddr() const=0;
    virtual sockaddr* getAddr() =0;
    virtual socklen_t getAddrLen() const=0;

    virtual ostream & insert(ostream& os) const=0;

    virtual std::string toString() const=0;

    bool operator<(const Address& rhs) const;
    bool operator==(const Address& rhs) const;
    bool operator!=(const Address& rhs) const;
};

class IPAddress: public Address
{
public:
    using ptr=std::shared_ptr<IPAddress>;

    static IPAddress::ptr Create(const char* address,uint16_t port=0);

    virtual IPAddress::ptr broadcastAddress(uint32_t prefix_len)=0;
    virtual IPAddress::ptr networdAddress(uint32_t perfix_len)=0;
    virtual IPAddress::ptr subnetMask(uint32_t prefix_len)=0;

    virtual uint32_t getPort() const =0;
    virtual void setPort(uint16_t v) =0;
};

class IPv4Address: public IPAddress
{
public:
    typedef shared_ptr <IPv4Address> ptr;
public:
    IPv4Address(const sockaddr_in& address);
    IPv4Address(uint32_t address=INADDR_ANY,uint16_t port=0);

    static IPv4Address::ptr Create(const char* address,uint16_t port=0);

    const sockaddr* getAddr() const override;
    sockaddr* getAddr() override;
    socklen_t getAddrLen() override;

    ostream& insert(ostream& os) const override;

    IPAddress::ptr broadcastAddress(uint32_t prefix_len) override;
    IPAddress::ptr networdAddress(uint32_t perfix_len) override;
    IPAddress::ptr subnetMask(uint32_t prefix_len) override;

    uint32_t getPort() const override;
    void setPort(uint16_t v) override;

private:
    sockaddr_in m_addr;
};

class IPv6Address: public IPAddress
{
public:
    typedef shared_ptr <IPv6Address> ptr;
public:
    IPv6Address();
    IPv6Address(const sockaddr_in6& address);
    IPv6Address(const uint8_t address[16],uint16_t port=0);

    static IPv6Address::ptr Create(const char* address,uint16_t port=0);

    const sockaddr* getAddr() const override;
    sockaddr* getAddr() override;
    socklen_t getAddrLen() override;

    ostream& insert(ostream& os) const override;

    IPAddress::ptr broadcastAddress(uint32_t prefix_len) override;
    IPAddress::ptr networdAddress(uint32_t perfix_len) override;
    IPAddress::ptr subnetMask(uint32_t prefix_len) override;

    uint32_t getPort() const override;
    void setPort(uint16_t v) override;

private:
    sockaddr_in6 m_addr;
};

class UnixAddress: public Address
{
public:
    typedef shared_ptr <UnixAddress> ptr;
public:

    UnixAddress();
    UnixAddress(const string & path);
;

    const sockaddr *getAddr() const override;
    sockaddr *getAddr() override;
    socklen_t getAddrLen() override;

    ostream &insert(ostream &os) const override;

    void setAddrLen(uint32_t v);
private:
    sockaddr_un m_addr;
    socklen_t m_length;
}

class UnknownAddress : public Address
{
public:
        typedef std::shared_ptr<UnknownAddress> ptr;
public:
        UnknownAddress(int family);
        UnknownAddress(const sockaddr& addr);

        const sockaddr* getAddr() const override;
        sockaddr* getAddr() override;
        socklen_t getAddrLen() const override;

        std::ostream& insert(std::ostream& os) const override;
private:
        sockaddr m_addr;
    };
}

#endif //C_2_ADDRESS_H
