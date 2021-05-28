//
// Created by bokket on 2021/5/26.
//

#ifndef BOKKERVE_PLUS_PLUS_HOOK_H
#define BOKKERVE_PLUS_PLUS_HOOK_H


#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <sys/socket.h>
#include <stdint.h>
#include <unistd.h>
#include <dlfcn.h>


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

typedef int (*connect_ptr)(int sockfd,const struct sockaddr* addr,socklen_t addrlen);
extern connect_ptr g_sys_connect;

typedef int (*accept_ptr)(int sockfd,struct sockaddr* addr,socklen_t* addrlen);
extern accept_ptr g_sys_accept;


typedef ssize_t (*read_ptr)(int fd,void* buf,size_t count);
extern read_ptr g_sys_read;

typedef ssize_t (*readv_ptr)(int fd,const struct iovec* iov,int iovcnt);
extern readv_ptr g_sys_readv;

typedef ssize_t (*recv_ptr)(int sockfd,void* buf,size_t len,int flags);
extern recv_ptr g_sys_recv;

typedef ssize_t (*recvfrom_ptr)(int sockfd,void* buf,size_t len,int flags,struct sockaddr* src_addr,socklen_t* addrlen);
extern recvfrom_ptr g_sys_recvfrom;

typedef ssize_t (*recvmsg_ptr)(int sockfd,struct msghdr* msg,int flags);
extern recvfrom_ptr g_sys_recvmsg;

typedef ssize_t (*write_ptr)(int fd,const void* buf,size_t count);
extern write_ptr g_sys_write;

typedef ssize_t (*writev_ptr)(int fd,const struct iovec* iov,int iovcnt);
extern writev_ptr g_sys_write;

typedef ssize_t (*send_ptr)(int s,const void* msg,size_t len,int flags);
extern send_ptr g_sys_send;

typedef ssize_t (*sendto_ptr)(int s,const void* msg,size_t len,int flags,const struct sockaddr* to,socklen_t tolen);
extern sendto_ptr g_sys_sendto;

typedef ssize_t (*sendmsg_ptr)(int s, const struct msghdr *msg, int flags);
extern sendmsg_ptr g_sys_sendmsg;

typedef int (*close_ptr)(int fd);
extern close_ptr g_sys_close;

typedef int (*fcntl_ptr)(int fd, int cmd, ...);
extern fcntl_ptr g_sys_fcntl;

typedef int (*setsockopt_ptr)(int sockfd, int level, int optname, const void *optval, socklen_t optlen);
extern setsockopt_ptr g_sys_setsockopt;

// 自定义超时连接函数
extern int connect_with_timeout(int fd, const struct sockaddr* addr, socklen_t addrlen, int64_t timeout_ms);

}


#endif //BOKKERVE_PLUS_PLUS_HOOK_H
