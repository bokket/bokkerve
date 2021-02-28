//
// Created by bokket on 2021/2/23.
//

#include "EventLoop.h"
#include <signal.h>
#include <sys/eventfd.h>

using namespace bokket;
using namespace bokket::net;

namespace
{
    thread_local EventLoop* t_EventLoop= nullptr;


    int CreateEventFd()
    {
        int evfd=::eventfd(0,EFD_NONBLOCK | EFD_CLOEXEC);
        if(evfd<0)
        {}
        return evfd;
    }

    class IgnoreSigPipe
    {
    public:
        IgnoreSigPipe()
        {
            ::signal(SIGPIPE,SIG_IGN);
        }
        IgnoreSigPipe ignore;
    };

EventLoop::EventLoop()
           :threadId_(bokket::CurrentThread::tid())
           ,looping_(false)
           ,quit_(false)
           ,callingPendingFunctors_(false)
           ,eventHandling_(false)
           ,epoller_(new Epoller(this))
           ,wakeupFd_(CreateEventFd())
           ,wakeupChannel_(new Channel(this,wakeupFd_))
           ,currentActiveChannel_(nullptr)
{
    wakeupChannel_->
}


}