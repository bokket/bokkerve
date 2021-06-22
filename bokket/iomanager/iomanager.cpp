//
// Created by bokket on 2021/5/23.
//

#include "iomanager.h"


#include "iocontext.h"
#include "../Scheduler/scheduler.h"

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
    BOKKET_LOG_DEBUG(g_logger)<<"IOManager::tickle()";
    if(!hasIdleThreads()) {
        return;
    }
    int rt=write(tickleFds_[1],"T",1);
    ASSERT(rt==1);
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
    BOKKET_LOG_DEBUG(g_logger)<<"IOManager::idle()";
    epoll_event* events=new epoll_event[256]();
    std::shared_ptr<epoll_event> shared_events(events,[](epoll_event* ptr){
        delete[] ptr;
    });

    while(true) {
        uint64_t next_timeout=0;
        if(UNLIKELY(stopping(next_timeout))) {
            BOKKET_LOG_DEBUG(g_logger)<<"name="<<getName()
                                    <<" idle stopping exit";
            break;
        }

        int rt=0;
        do {
            static const int MAX_TIMEOUT=3000;
            if(next_timeout!=~0ull) {
                /*next_timeout=static_cast<int>(next_timeout>MAT_TIMEOUT
                                                ? MAT_TIMEOUT:next_timeout);*/
                next_timeout=std::min(static_cast<int>(next_timeout),MAX_TIMEOUT);
            } else {
                next_timeout=MAX_TIMEOUT;
            }
            rt=epoll_wait(epfd_,events,256,static_cast<int>(next_timeout));

            if(rt<0 && errno==EINTR) {
                continue;
            } else {
                break;
            }
        }while(true);


        std::vector<std::function<void()>> cbs;
        listExpiredCb(cbs);
        if(!cbs.empty()) {
            for(const auto & cb : cbs) {
                Scheduler::schedule(cb);
            }
            cbs.clear();
        }

    for(int i=0;i<rt;++i) {
        epoll_event& event=events[i];
        if(event.data.fd==tickleFds_[0]) {
            uint8_t dummy;
            while(read(tickleFds_[0],&dummy,sizeof(dummy))>0);
            continue;
        }

        IOContext* io_context= static_cast<IOContext*>(event.data.ptr);
        std::unique_lock<std::shared_mutex> uniqueLock(io_context->mutex_);

        if(event.events & (EPOLLERR | EPOLLHUP))
            event.events |= (EPOLLIN | EPOLLOUT)& io_context->events_;

        IOContext::Event real_events=IOContext::Event::NONE;

        if(event.events & EPOLLIN) 
            real_events =(IOContext::Event) (real_events | IOContext::Event::READ);

        if(event.events & EPOLLOUT) 
            real_events =(IOContext::Event) (real_events | IOContext::Event::WRITE);

        //if(io_context->events_ & IOContext::Event::NONE) 
        if((io_context->events_ &real_events)==IOContext::Event::NONE)
            continue;

        int left_events=(io_context->events_ & ~real_events);

        int op=left_events ? EPOLL_CTL_MOD : EPOLL_CTL_DEL;
        event.events=EPOLLET | left_events;

        int rt2=epoll_ctl(epfd_,op,io_context->fd_,&event);

        if(rt2) {
            BOKKET_LOG_ERROR(g_logger) << "epoll_ctl(" << epfd_ << ","
                                       << op << "," << io_context->fd_ << "," << event.events << "):"
                                       << rt << "(" << errno << ")(" << ::strerror(errno) << ")";
            continue;
        }

        if(real_events & IOContext::Event::READ) {
            io_context->triggerEvent(IOContext::Event::READ);
            --pendingEventCount;
        }
        if(real_events & IOContext::Event::WRITE) {
            io_context->triggerEvent(IOContext::Event::WRITE);
            --pendingEventCount;
        }
    }

    Fiber::ptr cur=Fiber::getThis();
    auto raw_ptr=cur.get();
    cur.reset();

    //raw_ptr->swapOut();
        raw_ptr->yield();

    }
}

void IOManager::timerTickle() {
    tickle();
}



IOManager::IOManager(size_t threads, bool useCaller, const std::string &name)
                    :Scheduler(threads,useCaller,name)
{
    epfd_=epoll_create(5000);
    ASSERT(epfd_>0);

    int rt=::pipe(tickleFds_);
    ASSERT(!rt);

    epoll_event event;
    memset(&event,0,sizeof(epoll_event));
    event.events=EPOLLIN | EPOLLET;
    event.data.fd=tickleFds_[0];

    rt=fcntl(tickleFds_[0],F_SETFL,O_NONBLOCK);
    ASSERT(!rt);

    rt=epoll_ctl(epfd_,EPOLL_CTL_ADD,tickleFds_[0],&event);
    ASSERT(!rt);

    contextResize(32);

    start();
}

IOManager::~IOManager() noexcept {
    stop();

    close(epfd_);
    close(tickleFds_[0]);
    close(tickleFds_[1]);


    /*for(auto i=0;i<fdContexts_.size();++i) {
        id(fdContexts_[i]) {
            delete fdContexts_[i];
        }
    }*/

    for(auto& ioContext: ioContexts_) {
        if(ioContext)
            delete ioContext;
    }
}

void IOManager::contextResize(size_t size) {
    ioContexts_.resize(size);

    /*for(auto i=0;i<ioContexts_.size();++i) {
        if(!fdContexts_[i]) {
            fdContexts_[i] = new ioContext;
            fdContexts_[i]->fd_ = i;
        }
    }*/
    for(auto i=0;i<ioContexts_.size();++i) {
        ioContexts_[i]=new IOContext(i);
    }
}

int IOManager::addEvent(int fd, IOContext::Event event, std::function<void()> cb) {
    IOContext* io_context= nullptr;

    std::unique_lock<std::shared_mutex> uniqueLock(mutex_);

    if(static_cast<int>(ioContexts_.size()) > fd ) {
        io_context=ioContexts_[fd];
        uniqueLock.unlock();
    } else {
        uniqueLock.unlock();
        std::unique_lock<std::shared_mutex> uniqueLock1(mutex_);
        contextResize(fd*1.5);
        io_context=ioContexts_[fd];
    }

    std::unique_lock<std::shared_mutex> uniqueLock1(io_context->mutex_);

    if(UNLIKELY(io_context->events_ & event)) {
        BOKKET_LOG_ERROR(g_logger)<<"addEvent assert fd="<<fd
                                <<" event="<<event
                                <<" io_context.event="<<io_context->events_;
        ASSERT(!(io_context->events_ & event));
    }

    int op=io_context->events_ ? EPOLL_CTL_MOD:EPOLL_CTL_ADD;
    epoll_event epevent;
    //memset(&event,0,sizeof(epoll_event));
    epevent.events=EPOLLET | io_context->events_ | event;
    epevent.data.ptr=io_context;

    int rt=epoll_ctl(epfd_,op,fd,&epevent);

    if(rt) {
        BOKKET_LOG_ERROR(g_logger)<<"epoll_ctl("<<epfd_<<","
                                <<op<<","<<fd<<","<<epevent.events<<"):"
                                <<rt<<"("<<errno<<")("<<::strerror(errno)<<") io_context->events_="
                                <<io_context->events_;
        return -1;
    }

    ++pendingEventCount;


    io_context->events_=(IOContext::Event)(io_context->events_ | event);

    IOContext::Context& event_context=io_context->getContext(event);
    ASSERT(!event_context.scheduler&& !event_context.fiber && !event_context.cb);

    event_context.scheduler=Scheduler::getThis();

    if(cb) {
        event_context.cb.swap(cb);
    } else {
        event_context.fiber=Fiber::getThis();
        ASSERT(event_context.fiber->getStatus()==Fiber::Status::EXEC);

    }
    return 0;

}


bool IOManager::delEvent(int fd, IOContext::Event event) {
    std::unique_lock<std::shared_mutex> uniqueLock(mutex_);
    if(static_cast<int>(ioContexts_.size()) <= fd ) {
        return false;
    }

    IOContext* io_context=ioContexts_[fd];
    uniqueLock.unlock();


    std::unique_lock<std::shared_mutex> uniqueLock1(io_context->mutex_);


    if(UNLIKELY(!(io_context->events_ & event))) {
        return false;
    }


    IOContext::Event new_event=(IOContext::Event)(io_context->events_ & ~event);
    int op=new_event ? EPOLL_CTL_MOD:EPOLL_CTL_DEL;

    epoll_event epevent;
    epevent.events=EPOLLET | new_event;
    epevent.data.ptr=io_context;

    int rt=epoll_ctl(epfd_,op,fd,&epevent);

    if(rt) {
        BOKKET_LOG_ERROR(g_logger)<<"epoll_ctl("<<epfd_<<","
                                  <<op<<","<<fd<<","<<epevent.events<<"):"
                                  <<rt<<"("<<errno<<")("<<::strerror(errno)<<")";
        return false;
    }

    --pendingEventCount;

    io_context->events_=new_event;

    IOContext::Context& event_context=io_context->getContext(event);


    io_context->resetContext(event_context);
    return true;
}

bool IOManager::cancelEvent(int fd, IOContext::Event event) {
    std::unique_lock<std::shared_mutex> uniqueLock(mutex_);
    if(static_cast<int>(ioContexts_.size()) <= fd ) {
        return false;
    }

    IOContext* io_context=ioContexts_[fd];
    uniqueLock.unlock();

    std::unique_lock<std::shared_mutex> uniqueLock1(io_context->mutex_);
    if(UNLIKELY(!(io_context->events_ & event))) {
        return false;
    }

    IOContext::Event new_event=(IOContext::Event)(io_context->events_ & ~event);
    int op=new_event ? EPOLL_CTL_MOD:EPOLL_CTL_DEL;

    epoll_event epevent;
    epevent.events=EPOLLET | new_event;
    epevent.data.ptr=io_context;

    int rt=epoll_ctl(epfd_,op,fd,&epevent);

    if(rt) {
        BOKKET_LOG_ERROR(g_logger)<<"epoll_ctl("<<epfd_<<","
                                  <<op<<","<<fd<<","<<epevent.events<<"):"
                                  <<rt<<"("<<errno<<")("<<::strerror(errno)<<")";
        return false;
    }

    io_context->triggerEvent(event);
    --pendingEventCount;
    return true;
}

bool IOManager::cancelAll(int fd) {
    std::unique_lock<std::shared_mutex> uniqueLock(mutex_);
    if(static_cast<int>(ioContexts_.size()) <= fd ) {
        return false;
    }

    IOContext* io_context=ioContexts_[fd];
    uniqueLock.unlock();

    std::unique_lock<std::shared_mutex> uniqueLock1(io_context->mutex_);
    if(!(io_context->events_ )) {
        return false;
    }

    int op=EPOLL_CTL_DEL;

    epoll_event epevent;
    epevent.events=0;
    epevent.data.ptr=io_context;

    int rt=epoll_ctl(epfd_,op,fd,&epevent);

    if(rt) {
        BOKKET_LOG_ERROR(g_logger)<<"epoll_ctl("<<epfd_<<","
                                  <<op<<","<<fd<<","<<epevent.events<<"):"
                                  <<rt<<"("<<errno<<")("<<::strerror(errno)<<")";
        return false;
    }

    if(io_context->events_ & IOContext::Event::READ) {
        io_context->triggerEvent(IOContext::Event::READ);
        --pendingEventCount;
    }
    if(io_context->events_ & IOContext::Event::WRITE) {
        io_context->triggerEvent(IOContext::Event::WRITE);
        --pendingEventCount;
    }

    ASSERT(io_context->events_==0);
    return true;

}

}