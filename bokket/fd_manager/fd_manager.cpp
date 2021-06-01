//
// Created by bokket on 2021/5/26.
//

#include "fd_manager.h"
#include "hook.h"

#include "../Log/Log.h"
#include "../thread/Assert.h"

namespace bokket
{

SYSTEM_LOG();

FdData::FdData(int fd)
              :fd_(fd)
              ,recvTimeOut_(-1)
              ,sendTimeOut_(-1)
{
    int flags=g_sys_fcntl(fd_,F_GETFL,0);
    if(flags==-1) {
        ASSERT_MSG(false,"fcntl error,errno="+std::to_string(errno)
        +",errstr="+::strerror(errno));
    }

    if(!(flags& O_NONBLOCK)) {
        g_sys_fcntl(fd_,F_SETFL,flags|O_NONBLOCK);
    }
}

int64_t FdData::getTimeOut(int type) {
    if(type==SO_RCVTIMEO)
        return recvTimeOut_;
    else
        return sendTimeOut_;
}

void FdData::setTimerOut(int type, int64_t v) {
    if(type==SO_RCVTIMEO)
        recvTimeOut_=v;
    else
        sendTimeOut_=v;
}

FdManager::FdManager() {
    datas_.resize(64);
}


FdData::ptr FdManager::get(int fd, bool auto_create) {
    /*if(fd<0)
        return nullptr;*/

    std::shared_lock<std::shared_mutex> lk(mutex_);
    if(fd>= static_cast<int>(datas_.size())) {
        if(auto_create== false)
            return nullptr;
        else {
            if(datas_[fd] || !auto_create)
                return datas_[fd];
        }
    }
    lk.unlock();

    std::unique_lock<std::shared_mutex> lk1(mutex_);
    FdData::ptr data(new FdData(fd));
    return datas_[fd]=data;
}

void FdManager::del(int fd) {
    std::unique_lock<std::shared_mutex> lk(mutex_);
    if(fd>= static_cast<int>(datas_.size()))
        return ;

    datas_[fd].reset();

}

}