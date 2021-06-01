//
// Created by bokket on 2020/12/30.
//

#include "address.h"

#include "../Log/Log.h"

namespace bokket
{

static bokket::Logger::ptr logger=BOKKET_LOG_NAME("system");


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

    int error=::getaddrinfo(address, nullptr,&hints,&res);
    if(error) {
        BOKKET_LOG_ERROR(logger)<<"Address::create("<<address<<","<<port<<"),error="
        <<error<<",errstr="<<gai_strerror(error);
        return nullptr;
    }

    try {
        Address::ptr result=create(res->ai_addr);
        if(result)
            result->setPort(port);

        ::freeaddrinfo(result);
    } catch (...) {
        ::freeaddrinfo(res);
        return nullptr;
    }
}

Address::ptr Address::getAddrInfo(const std::string &host
                                  , int family, int type, int protocol) {
    std::vector<Address::ptr> res;
    if(getAddrInfo(res,host,family,type,protocol)) {
        return res[0];
    }
    return nullptr;
}

bool Address::getAddrInfo(std::vector <Address::ptr> &result, const std::string &host
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
}


}
