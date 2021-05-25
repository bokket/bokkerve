//
// Created by bokket on 2021/5/23.
//

#include "iomanager.h"


#include <sys/epoll.h>
#include <sys/fcntl.h>

namespace bokket
{

static bokket::Logger::ptr g_logger= BOKKET_LOG_NAME("system");


IOManager * IOManager::getThis() {
    return dynamic_cast<IOManager*>(Scheduler::getThis());
}

void IOManager::tickle() {

}

bool IOManager::stopping(uint64_t &timeout) {
    timeout=getNextTimer();
    return timeout=~0ull
            && pendingEventCount==0
            && Scheduler::stopping();
}

bool IOManager::stopping() {
    uint64_t timeout=0;
    return stopping(timeout);
}

void IOManager::idle() {

}

void IOManager::timerTickle() {
    tickle();
}


EventContext & IOManager::FdContext::getContext(Event event)
{
    switch (event) {
        case Event::READ:
            return read;
        case Event::WRITE:
            return write;
        default:
            return ;
    }
}


void IOManager::FdContext::resetContext(EventContext& ctx) {
    ctx.scheduler= nullptr;
    ctx.fiber.reset();
    ctx.cb= nullptr;
}


void IOManager::FdContext::triggerEvent(Event event) {

    events_=events_& ~ event;
    EventContext& ctx=getContext(event);

    if(ctx.cb) {
        ctx.scheduler->schedule(&ctx.cb);
    } else {
        ctx.scheduler->schedule(&ctx.fiber);
    }

    ctx.scheduler= nullptr;
    return;
}

IOManager::IOManager(size_t threads, bool useCaller, const std::string &name)
                    :Scheduler(threads,useCaller,name)
{
    epfd_=epoll_create(5000);

    inr rt=::pipe(tickleFds_);

    epoll_event event;
    memset(&event,0,sizeof(epoll_event));
    event.events=EPOLLIN | EPOLLET;
    event.data.fd=tickleFds_[0];

    rt=fcntl(tickleFds_[0],F_SETFL,O_NONBLOCK);

    rt=epoll_ctl(epfd_,EPOLL_CTL_ADD,tickleFds_[0],&event);

    contextResize(32);

    start();
}

IOManager::~IOManager() noexcept {
    stop();

    close(epfd_);
    close(tickleFds_[0]);
    close(tickleFds_[1]);


    for(auto i=0;i<fdContexts_.size();++i) {
        id(fdContexts_[i]) {
            delete fdContexts_[i];
        }
    }
}

void IOManager::contextResize(size_t size) {
    fdContexts_.resize(size);

    for(auto i=0;i<fdContexts_.size();++i) {
        if(!fdContexts_[i]) {
            fdContexts_[i] = new FdContext;
            fdContexts_[i]->fd_ = i;
        }
    }
}

int IOManager::addEvent(int fd, Event event, std::function<void()> cb) {

}


bool IOManager::delEvent(int fd, Event event) {

}

bool IOManager::cancelEvent(int fd, Event event) {

}

bool IOManager::cancelAll(int fd) {

}

}