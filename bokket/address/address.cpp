//
// Created by bokket on 2020/12/30.
//

#include "address.h"

#include "../Log/Log.h"

namespace bokket
{

static bokket::Logger::ptr g_logger=BOKKET_LOG_NAME("system");


template<class T>
static T createMask(uint32_t bits) {
    return (1<<(sizeof(T)*8-bits))-1;
}

template<class T>
static uint32_t countBytes(T value) {
    uint32_t result=0;
    for(;value;++result) {
        value &= value-1;
    }
    return result;
}


Address::ptr Address::create(const sockaddr *address) {
    if(address== nullptr) {
        return nullptr;
    }
    Address::ptr addr;

    switch (address->sa_family) {
        case AF_INET:
            addr.reset(new IPv4Address(*(const sockaddr_in*)address));
            break;
        case AF_INET6:
            addr.reset(new IPv6Address(*(const sockaddr_in6*)address));
            break;
        default:
            addr.reset(new UnknownAddress(*address));
            break;
    }
    return addr;
}

Address::ptr Address::create(const std::string &address, uint16_t port) {
    ::addrinfo hints,*res;
    ::memset(&hints,0,sizeof(addrinfo));

    hints.ai_family=AF_UNSPEC;

    int error=::getaddrinfo(address.data(), nullptr,&hints,&res);
    if(error) {
        BOKKET_LOG_ERROR(g_logger)<<"Address::create("<<address<<","<<port<<"),error="
        <<error<<",errstr="<<gai_strerror(error);
        return nullptr;
    }

    try {
        Address::ptr result=create(res->ai_addr);
        if(result)
            result->setPort(port);

        ::freeaddrinfo(res);
    } catch (...) {
        ::freeaddrinfo(res);
        return nullptr;
    }
}

Address::ptr Address::getAddrInfo(const std::string &host
                                  , int family, int type, int protocol) {
    std::vector<Address::ptr> address;
    if(getAddrInfo(address,host,family,type,protocol)) {
        return res[0];
    }
    return nullptr;
}

bool Address::getAddrInfo(std::vector <Address::ptr> &address, const std::string &host
                          , int family, int type,int protocol) {
    ::addrinfo hits,*res,*next;
    hits.ai_flags=0;
    hits.ai_family=family;
    hits.ai_socktype=type;
    hits.ai_protocol=protocol;
    hits.ai_addrlen=0;
    hits.ai_canonname= nullptr;
    hits.ai_addr= nullptr;
    hits.ai_next= nullptr;

    std::string node;
    const char* service;


    //[3608::f0f0:3002:31:1]:80

    if(!host.empty() && host[0]=='[') {
        const char* endipv6=(const char*)memchr(host.c_str()+1,']',host.size()-1);
        if(endipv6) {
            if(*(endipv6+1)==':') {
                service=endipv6+2;
            }
            node=host.substr(1,endipv6-host.c_str()-1);
        }
    }

    if(node.empty()) {
        service=(const char*)memchr(host.c_str(),':',host.size());
        if(service) {
            if(!memchr(service+1,':',host.c_str()+host.size()-service-1)) {
                node=host.substr(0,service-host.c_str());
                ++service;
            }
        }
    }

    if(node.empty())
        node=host;

    int error=::getaddrinfo(node.c_str(),service,&hits,&res);


    if(error) {
        BOKKET_LOG_ERROR(g_logger)<<"Address::getAddressInfo("<<host<<","<<family<<","
        <<type<<")err="<<error<<" errstr="<<strerror(error);
        return false;
    }

    next=res;
    while(next) {
        address.emplace_back(create(next->ai_addr,static_cast<socklen_t>(next->ai_addrlen)));
        BOKKET_LOG_INFO(g_logger)<<((sockaddr_in*)next->ai_addr)->sin_addr.s_addr;
        next=next->ai_next;
    }

    ::freeaddrinfo(res);

    return !address.empty();
}

bool Address::getInterfaceAddress(std::multimap <std::string, std::pair<Address::ptr, uint32_t>> &address,
                                  int family) {
    struct ::ifaddrs *next,*res;
    if(::getifaddrs(&res)!=0) {
        BOKKET_LOG_ERROR(g_logger)<<"Address::getInterfaceAddress"
        <<" err="<<errno<<" errstr="<<strerror(errno);
        return false;
    }


    try {
        for(next=res;next;next=next->ifa_next) {
            Address::ptr addr;
            uint32_t prefix_len=~0u;
            if(family!=AF_UNSPEC && family!=next->ifa_addr->sa_family) {
                continue;
            }
            switch (next->ifa_addr->sa_family) {
                case AF_INET:
                {
                    addr=create(next->ifa_addr,sizeof(sockaddr_in));
                    uint32_t netmask=((sockaddr_in*)next->ifa_netmask)->sin_addr.s_addr;
                    prefix_len=
                }
                    break;
                case AF_INET6:
                {
                    addr=create(next->ifa_addr,sizeof(sockaddr_in6));
                    uint32_t netmask=((sockaddr_in6*)next->ifa_netmask)->sin6_addr;
                    prefix_len=0;
                    for(int i=0;i<16;++i) {
                        prefix_len+=
                    }
                }
                    break;
                default:
                    break;
            }
            if(addr) {
                address.insert(std::make_pair(next->ifa_name,std::make_pair(addr,prefix_len)));
            }
        }
    } catch (...) {
        BOKKET_LOG_ERROR(g_logger)<<"Address::getInterfaceAddress exception";
        ::freeifaddrs(res);
        return false;
    }
    ::freeifaddrs(res);
    return true;
}



bool Address::getInterfaceAddress(std::vector <std::pair<Address::ptr, uint32_t>> &address, const std::string &iface,
                                  int family) {
    if(iface.empty() || iface="*") {
        if(family==AF_INET || family==AF_UNSPEC ) {
            address.emplace_back(std::make_pair(Address::ptr(new IPv4Address()),0u));
        }
        if(family==AF_INET6 || family==AF_UNSPEC) {
            address.emplace_back(std::make_pair(Address::ptr(new IPv6Address()),0u));
        }
        return true;
    }

    std::multimap<std::string,
    std::pair<Address::ptr,uint32_t>> result;

    if(!getInterfaceAddress(result,family))
        return false;

    auto it=result.equal_range(iface);

    for(;it.first!=it.second;++it.first) {
        address.emplace_back(it.first->second);
    }
    return !address.empty();

}

bool Address::operator<(const Address &rhs) const {
    socklen_t minlen=std::min(getAddrLen(),rhs.getAddrLen());
    int result=memcmp(getAddr(),rhs.getAddr(),minlen);

    if(result<0)
        return true;
    else if(result>0)
        return false;
    else if(getAddrLen()<rhs.getAddrLen())
        return true;

    return false;
}

bool Address::operator==(const Address &rhs) const {
    return getAddrLen()==rhs.getAddrLen()
    && memcmp(getAddr(),rhs.getAddr(),getAddrLen())==0;
}

bool Address::operator!=(const Address &rhs) const {
    return !(*this==rhs);
}


IPv4Address::ptr IPv4Address::create(const std::string &address, uint16_t port) {
    IPv4Address::ptr IPv4(new IPv4Address);
    IPv4->addr_.sin_port=LittleEndian(port);

    int error=::inet_pton(AF_INET,address,&IPv4->addr_.sin_addr);

    if(error<=0) {
        BOKKET_LOG_ERROR(g_logger)<<"IPv4Address::create("<<address<<","<<port<<"),error="
        <<error<<",errstr="<<gai_strerror(error);
        return nullptr;
    }
    return IPv4;
}

Address::ptr IPv4Address::broadcastAddress(uint32_t prefix_len) {
    if(prefix_len>32)
        return nullptr;

    sockaddr_in sockaddr=addr_;
    sockaddr.sin_addr.s_addr |= LittleEndian(createMask<uint32_t>(prefix_len));

    return IPv4Address::ptr (new IPv4Address(sockaddr));
}

Address::ptr IPv4Address::networdAddress(uint32_t perfix_len) {
    if(prefix_len>32)
        return nullptr;

    sockaddr_in sockaddr=addr_;
    sockaddr.sin_addr.s_addr &= LittleEndian(createMask<uint32_t>(prefix_len));

    return IPv4Address::ptr (new IPv4Address(sockaddr));
}

Address::ptr IPv4Address::subnetMask(uint32_t prefix_len) {

    sockaddr_in sockaddr=addr_;
    memset(&addr_,0,sizeof(sockaddr));
    sockaddr.sin_addr.s_addr = ~LittleEndian(createMask<uint32_t>(prefix_len));

    return IPv4Address::ptr (new IPv4Address(sockaddr));
}

std::string IPv4Address::toString() const {
    std::stringstream ss;
    uint32_t addr=LittleEndian(addr_.sin_addr.s_addr);
    ss<<((addr>>24)&0xff) <<"."
      <<((addr>>16)&0xff) <<"."
      <<((addr>>8)&0xff) <<"."
      <<(addr&0xff);
    ss<"."<<LittleEndian(addr_.sin_port);
    return ss.str();
}


IPv6Address::ptr IPv6Address::create(const std::string &address, uint16_t port) {
    IPv6Address::ptr IPv6(new IPv6Address);
    IPv6->addr_.sin6_port=LittleEndian(port);
    int result=::inet_pton(AF_INET6,address,&IPv6->addr_.sin6_addr);
    if(result<=0) {
        BOKKET_LOG_ERROR(g_logger)<<"IPv6Address::create("<<address<<","<<port<<"),ret="
                                  <<result<<" errno="<<error<<" errstr="<<gai_strerror(error);
        return nullptr;
    }
    return IPv6;
}

std::string IPv6Address::toString() const {
    uint16_t *addr=(uint16_t*)addr_.sin6_addr
}

std::string UnknownAddress::toString() const {
    std::stringstream ss;
    ss<<"[UnknownAddress family="<<addr_.sa_family<<"]";
    return ss.str();
}

}
