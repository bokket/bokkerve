//
// Created by bokket on 2021/5/26.
//

#ifndef BOKKERVE_PLUS_PLUS_HOOK_H
#define BOKKERVE_PLUS_PLUS_HOOK_H


#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <sys/socket.h>


namespace bokket
{

bool is_hook_enable();

void hook_enable();

void hook_disable();

}

extern "C"
{

typedef unsigned int (*sleep_ptr)(unsigned int seconds);
extern sleep_ptr g_sys_sleep;


typedef int (*usleep_ptr)(useconds_t useconds);
extern usleep_ptr g_sys_sleep;


typedef int (*nanosleep_ptr)(const struct timespec* req,struct timespec* rem);
extern nanosleep_ptr g_sys_nanosleep;

typedef int (*socket_ptr)(int domain,int type,int protocol);
extern socket_ptr g_sys_socket;





}


#endif //BOKKERVE_PLUS_PLUS_HOOK_H
