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



    if(!v) {
        BOKKET_LOG_ERROR(g_logger)<<"getAddress Info fail";
        return;
    }

    /*if(!v1) {
        BOKKET_LOG_ERROR(g_logger)<<"getAddress Info fail";
        return;
    }*/

    for(auto i=0;i<address.size();++i) {
        BOKKET_LOG_INFO(g_logger)<<"i="<<i<<'-'<<"addr="<<address[i]->getAddr()<<" port="<<address[i]->getPort()<<" debug string="<<address[i]->toString();
    }
    address.clear();


    BOKKET_LOG_INFO(g_logger)<<"test www.baidu.com";
    v=bokket::Address::getAddrInfo(address,"www.baidu.com",AF_INET);
     if(!v) {
        BOKKET_LOG_ERROR(g_logger)<<"get baidu.com Address Info fail";
        return;
    }

    for(auto i=0;i<address.size();++i) {
        BOKKET_LOG_INFO(g_logger)<<"i="<<i<<'-'<<"addr="<<address[i]->getAddr()<<" port="<<address[i]->getPort()<<" debug string="<<address[i]->toString();
    }

    BOKKET_LOG_INFO(g_logger)<<"test localhost:4444";
    for(auto i=0;i<7;i++) {
        auto addr=bokket::Address::getAddrInfo("localhost:4444",AF_INET);
        if(addr) 
            BOKKET_LOG_INFO(g_logger)<<"i="<<i<<'-'<<"addr="<<addr->getAddr()<<" port="<<addr->getPort()<<" debug string="<<addr->toString();
        else 
             BOKKET_LOG_ERROR(g_logger)<<"get localhost:4444 Address Info fail";
    }
    BOKKET_LOG_INFO(g_logger)<<"end";

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

    BOKKET_LOG_INFO(g_logger)<<"IPv4 begin";

    auto addr=bokket::IPv4Address::create("127.0.2.22",1111);
    //auto addr=bokket::Address::create("127.0.2.22");
    if(addr) {
        BOKKET_LOG_INFO(g_logger)<<addr->toString();
        BOKKET_LOG_INFO(g_logger)<<addr->broadcastAddress(24)->toString();
        BOKKET_LOG_INFO(g_logger)<<addr->subnetMask(24)->toString();
        BOKKET_LOG_INFO(g_logger)<<addr->networdAddress(24)->toString();
    } else 
        BOKKET_LOG_ERROR(g_logger)<<"get IPv4 Address Info fail";

    addr=bokket::IPv4Address::create("www.baidu.com");
    
    if(addr) {
        BOKKET_LOG_INFO(g_logger)<<addr->toString();
    } else 
        BOKKET_LOG_ERROR(g_logger)<<"get IPv4 Address Info fail";


    BOKKET_LOG_INFO(g_logger)<<"IPv4 end";
}

void test_ipv6() {

    BOKKET_LOG_INFO(g_logger)<<"IPv6 begin";
    auto addr=bokket::IPv6Address::create("fe80:4c00:4c00:5741:4c00:4c00:4c00:824b");
    if(addr) {
        BOKKET_LOG_INFO(g_logger)<<addr->toString();
    }
    else 
        BOKKET_LOG_ERROR(g_logger)<<"get IPv6: Address Info fail";

    BOKKET_LOG_INFO(g_logger)<<"IPv6 end";

}

int main() {
    test();
    test_iface();
    test_ipv4();
    test_ipv6();
}