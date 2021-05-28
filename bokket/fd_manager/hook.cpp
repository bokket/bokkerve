//
// Created by bokket on 2021/5/26.
//

#include "hook.h"

#include "fd_manager.h"
#include "../iomanager/iomanager.h"
#include "../thread/Assert.h"


namespace bokket
{



}


extern "C"
{
#define SYS_HOOK(XX) \
    XX(sleep)        \
    XX(usleep)       \
    XX(nanosleep)    \
    XX(socket)       \
    XX(accept)       \
    XX(read)         \
    XX(readv)        \
    XX(recv)         \
    XX(recvfrom)     \
    XX(recvmsg)      \
    XX(write)        \
    XX(send)         \
    XX(sendto)       \
    XX(sendmsg)      \
    XX(close)        \
    XX(fcntl)        \
    XX(setsockopt)


#define XX(name) name ## _ptr g_sys_ ## name=nullptr;
    SYS_HOOK(XX);
#undef XX

static int hook_init()
{
#define XX(name) g_sys_## name=(name ## _ptr)dlsym(RTLD_NEXT,#name);
    SYS_HOOK(XX);
#undef XX
    return 0;
}

unsigned int sleep(unsigned int seconds) {
    if(UNLIKELY(!g_sys_sleep))
        hook_init();

    if(UNLIKELY(!bokket::t_hook_ena))
        return g_sys_sleep(seconds);

    bokket::Fiber::ptr fiber=bokket::Fiber::getThis();

    bokket::IOManager* ioManager=bokket::IOManager::getThis();

    ioManager->addTimer(seconds*1000,std::bind( dynamic_cast<void>((bokket::Scheduler::*)(bokket::Fiber::ptr,int thread)&bokket::IOManager::schedule,ioManager,fiber,-1)) );

   //dynamic_cast<void>((bokket::Scheduler::*)(bokket::Fiber::ptr,int thread)&bokket::IOManager::schedule,ioManager,fiber,-1);
    //std::bind((void(sylar::Scheduler::*)(sylar::Fiber::ptr, int thread))&sylar::IOManager::schedule,iom, fiber, -1)


    bokket::Fiber::yieldToHold();
    return 0;
}

int fcntl(int fd,int cmd,...) {
    if(UNLIKELY(!g_sys_fcntl))
        hook_init();

    ::va_list va;
    ::va_start(va,cmd);
    int ret=-1;

    switch (cmd) {
        case F_SETFL:
        {
            int arg=va_arg(va,int);
            va_end(va);
            bokket::FdData::ptr data=bokket::FdMgr::GetInstance().get(fd);

            if(data && bokket::t_hook_enable)
                arg |=O_NONBLOCK;

            ret=g_sys_fcntl(fd,cmd,arg);
            break;
        }

        case

    }
}

}

