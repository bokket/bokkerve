//
// Created by bokket on 2021/3/2.
//

#include "Socket.h"


using namespace bokket;
using namespace bokket::net;

Socket::~Socket()
{
    bokket::net::socks::close(sockfd_);
}

bool Socket::getTcpInfoString(char *buf, int len) const
{

}
