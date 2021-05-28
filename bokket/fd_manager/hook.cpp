//
// Created by bokket on 2021/5/26.
//

#include "hook.h"


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

}

