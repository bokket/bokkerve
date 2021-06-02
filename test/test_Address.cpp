//
// Created by bokket on 2021/6/2.
//


#include <iostream>

#include "../bokket/Log/Log.h"
#include "../bokket/address/address.h"
#include "../bokket/address/endian.h"

static bokket::Logger::ptr g_logger = BOKKET_LOG_ROOT();



void test() {
    std::vector<bokket::Address::ptr> address;

    BOKKET_LOG_INFO(g_logger)<<"begin";
    bool v=bokket::Address::getAddrInfo(address,"www.baidu.com",AF_INET);
    //bool v1=bokket::Address::getAddrInfo(address,"https://xiyoulinux.org",AF_INET);

    BOKKET_LOG_INFO(g_logger)<<"end";

    if(!v) {
        BOKKET_LOG_ERROR(g_logger)<<"getAddress Info fail";
        return;
    }

    /*if(!v1) {
        BOKKET_LOG_ERROR(g_logger)<<"getAddress Info fail";
        return;
    }*/

    for(auto i=0;i<address.size();++i) {
        BOKKET_LOG_INFO(g_logger)<<"i"<<'-'<<address[i]->getAddr()<<" "<<address[i]->getPort()<<address[i]->toString();
    }
}

void test_iface() {
    std::multimap<std::string,std::pair<bokket::Address::ptr,uint32_t>> res;

    bool v=bokket::Address::getInterfaceAddress(res);

    if(!v) {
        BOKKET_LOG_ERROR(g_logger)<<"getAddress Info fail";
        return;
    }
    for(auto&i:res) {
        BOKKET_LOG_INFO(g_logger)<<i.first<<"-"<<i.second.first->toString()<<"-"
        <<i.second.second;
    }
}

void test_ipv4() {
    auto addr=bokket::Address::create("127.0.0.1");
    if(addr) {
        BOKKET_LOG_INFO(g_logger)<<addr->toString();
    }
}

int main() {
    test();
    test_iface();
    test_ipv4();
}