//
// Created by bokket on 2021/5/23.
//

#include "iomanager.h"

#include <cerrno>
#include <cstdint>
#include <sys/epoll.h>
#include <sys/fcntl.h>


namespace bokket
{

static bokket::Logger::ptr g_logger= BOKKET_LOG_NAME("system");


IOManager * IOManager::getThis() {
    return dynamic_cast<IOManager*>(Scheduler::getThis());
}

void IOManager::tickle() {
    if(hasIdleThreads()) {
        return;
    }
    int rt=write(tickleFds_[1],"T",1);
    //
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
    epoll_event* events=new epoll_event[64]();
    std::shared_ptr<epoll_event> shared_events(events,[](epoll_event* ptr){
        delete[] ptr;
    });

    while(true) {
        uint64_t next_timeout=0;
        if(stopping(next_timeout)) {
            BOKKET_LOG_INFO(g_logger)<<"name="<<getName()
                                    <<" idle stopping exit";
            break;
        }

        int rt=0;
        do {
            static const int MAT_TIMEOUT=3000;
            if(next_timeout!=~0ull) {
                next_timeout=static_cast<int>(next_timeout>MAT_TIMEOUT
                                                ? MAT_TIMEOUT:next_timeout);
            } else {
                next_timeout=MAT_TIMEOUT;
            }
            rt=epoll_wait(epfd_,events,64,static_cast<int>(next_timeout));

            if(rt<0 && errno==EINTR) {

            } else {
                break;
            }
        } while (true);

    std::vector<std::function<void()>> cbs;
    listExpiredCb(cbs);
    if(!cbs.empty()) {
        schedule(cbs.begin(),cbs.end());
        cbs.clear();
    }

    for(int i=0;i<rt;++i) {
        epoll_event& event=events[i];
        if(event.data.fd==tickleFds_[0]) {
            uint8_t dummy;
            while(read(tickleFds_[0],&dummy,1)==1);
            continue;
        }

        FdContext* fd_context=dynamic_cast<FdContext*>(event.data.ptr);
        std::unique_lock<std::shared_mutex> uniqueLock(fd_context->mutex_);
        if(event.events & (EPOLLERR | EPOLLHUP)) {
            event.events | = EPOLLIN | EPOLLOUT;
        }
        int real_events=Event::NONE;
        if(event.events & EPOLLIN) {
            real_events | = Event::READ;
        }
        if(event.events & EPOLLOUT) {
            real_events | = Event::WRITE;
        }

        if(fd_context->events_ & Event::NONE) {
            continue;
        }

        int left_events=(fd_context->events_ & ~real_events);

        int op=left_events ? EPOLL_CTL_MOD : EPOLL_CTL_DEL;
        event.events=EPOLLET | left_events;

        int rt2=epoll_ctl(epfd_,op,fd_context->fd_,&event);

        if(rt2) {
            BOKKET_LOG_ERROR(g_logger) << "epoll_ctl(" << epfd_ << ","
                                       << op << "," << fd << "," << epevent.events << "):"
                                       << rt << "(" << errno << ")(" << ::strerror(errno) << ")";
            continue;
        }
        if(real_events&Event::READ) {
            fd_context->triggerEvent(Event::READ);
            --pendingEventCount;
        }
        if(real_events&Event::WRITE) {
            fd_context->triggerEvent(Event::WRITE);
            --pendingEventCount;
        }
    }

    Fiber::ptr cur=Fiber::getThis();
    auto raw_ptr=cur.get();
    cur.reset();

    raw_ptr->swapOut();


    }
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
    FdContext* fd_context= nullptr;

    std::unique_lock<std::shared_mutex> uniqueLock(mutex_);

    if(static_cast<int>(fdContexts_.size()) > fd ) {
        fd_context=fdContexts_[fd];
        uniqueLock.unlock();
    } else {
        uniqueLock.unlock();
        std::unique_lock<std::shared_mutex> uniqueLock1(mutex_);
        contextResize(fd*1.5);
        fd_context=fdContexts_[fd];
    }

    std::unique_lock<std::shared_mutex> uniqueLock1(fd_context->mutex_);

    if(fd_context->events_ & event) {
        BOKKET_LOG_ERROR(g_logger)<<"addEvent assert fd="<<fd
                                <<" event="<<event
                                <<" fd_context.event="<<fd_context->events_;

    }

    int op=fd_context->events_ ? EPOLL_CTL_MOD:EPOLL_CTL_ADD;
    epoll_event epevent;
    //memset(&event,0,sizeof(epoll_event));
    epevent.events=EPOLLET | fd_context->events_ | event;
    epevent.data.ptr=fd_context;

    int rt=epoll_ctl(epfd_,op,fd,&epevent);

    if(rt) {
        BOKKET_LOG_ERROR(g_logger)<<"epoll_ctl("<<epfd_<<","
                                <<op<<","<<fd<<","<<epevent.events<<"):"
                                <<rt<<"("<<errno<<")("<<::strerror(errno)<<")";
        return -1;
    }

    ++pendingEventCount;
    fd_context->events_=fd_context->events_ | event;

    FdContext::EventContext& event_context=fd_context->getContext(event);


    event_context.scheduler=Scheduler::getThis();

    if(cb) {
        event_context.cb.swap(cb);
    } else {
        event_context.fiber=Fiber::getThis();

    }
    return 0;

}


bool IOManager::delEvent(int fd, Event event) {
    std::unique_lock<std::shared_mutex> uniqueLock(mutex_);
    if(static_cast<int>(fdContexts_.size()) <= fd ) {
        return false;
    }

    FdContext* fd_context=fdContexts_[fd];
    uniqueLock.unlock();


    std::unique_lock<std::shared_mutex> uniqueLock1(fd_context->mutex_);


    if(!(fd_context->events_ & event)) {
        return false;
    }


    Event new_event=(fd_context->events_ & ~event);
    int op=new_event ? EPOLL_CTL_MOD:EPOLL_CTL_DEL;
    epoll_event epevent;
    epevent.events=EPOLLET | new_event;
    epevent.data.ptr=fd_context;

    int rt=epoll_ctl(epfd_,op,fd,&epevent);

    if(rt) {
        BOKKET_LOG_ERROR(g_logger)<<"epoll_ctl("<<epfd_<<","
                                  <<op<<","<<fd<<","<<epevent.events<<"):"
                                  <<rt<<"("<<errno<<")("<<::strerror(errno)<<")";
        return false;
    }

    --pendingEventCount;
    fd_context->events_=new_event;

    FdContext::EventContext& event_context=fd_context->getContext(event);

    fd_context->resetContext(event_context);
    return true;
}

bool IOManager::cancelEvent(int fd, Event event) {
    std::unique_lock<std::shared_mutex> uniqueLock(mutex_);
    if(static_cast<int>(fdContexts_.size()) <= fd ) {
        return false;
    }

    FdContext* fd_context=fdContexts_[fd];
    uniqueLock.unlock();

    std::unique_lock<std::shared_mutex> uniqueLock1(fd_context->mutex_);
    if(!(fd_context->events_ & event)) {
        return false;
    }

    Event new_event=(fd_context->events_ & ~event);
    int op=new_event ? EPOLL_CTL_MOD:EPOLL_CTL_DEL;
    epoll_event epevent;
    epevent.events=EPOLLET | new_event;
    epevent.data.ptr=fd_context;

    int rt=epoll_ctl(epfd_,op,fd,&epevent);

    if(rt) {
        BOKKET_LOG_ERROR(g_logger)<<"epoll_ctl("<<epfd_<<","
                                  <<op<<","<<fd<<","<<epevent.events<<"):"
                                  <<rt<<"("<<errno<<")("<<::strerror(errno)<<")";
        return false;
    }

    fd_context->triggerEvent(event);
    --pendingEventCount;
    return true;
}

bool IOManager::cancelAll(int fd) {
    std::unique_lock<std::shared_mutex> uniqueLock(mutex_);
    if(static_cast<int>(fdContexts_.size()) <= fd ) {
        return false;
    }

    FdContext* fd_context=fdContexts_[fd];
    uniqueLock.unlock();

    std::unique_lock<std::shared_mutex> uniqueLock1(fd_context->mutex_);
    if(!(fd_context->events_ )) {
        return false;
    }

    int op=EPOLL_CTL_DEL;
    epoll_event epevent;
    epevent.events=0;
    epevent.data.ptr=fd_context;

    int rt=epoll_ctl(epfd_,op,fd,&epevent);

    if(rt) {
        BOKKET_LOG_ERROR(g_logger)<<"epoll_ctl("<<epfd_<<","
                                  <<op<<","<<fd<<","<<epevent.events<<"):"
                                  <<rt<<"("<<errno<<")("<<::strerror(errno)<<")";
        return false;
    }

    if(fd_context->events_ & Event::READ) {
        fd_context->triggerEvent(Event::READ);
        --pendingEventCount;
    }
    if(fd_context->events_ & Event::WRITE) {
        fd_context->triggerEvent(Event::WRITE);
        --pendingEventCount;
    }

    //
    return true;

}

}