//
// Created by bokket on 2021/6/5.
//

#include "socket.h"

#include <limits>

#include "../Log/Log.h"
#include "../fd_manager/fd_manager.h"
#include "../iomanager/iomanager.h"
#include "../fd_manager/hook.h"



namespace bokket
{

static bokket::Logger::ptr g_logger=BOKKET_LOG_NAME("system");


Socket::Socket(int family, int type, int protocol)
              :sock_(-1)
              ,family_(family)
              ,protocol_(protocol)
              ,isConnected_(false)
{}

Socket::~Socket() {
    close();
}


int64_t Socket::getSendTimeout() {
    FdData::ptr data=FdMgr::GetInstance()->get(sock_);
    if(data)
        return data->getTimeOut(SO_SEND);
    return -1;
}

void Socket::setSendTimeout(int64_t v) {
    setOption();
}

}
