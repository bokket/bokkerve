//
// Created by bokket on 2021/6/1.
//

#ifndef BOKKERVE_PLUS_PLUS_IOCONTEXT_H
#define BOKKERVE_PLUS_PLUS_IOCONTEXT_H


#include "../Log/Log.h"
#include "../thread/Assert.h"
#include "../time/timer.h"
#include "../Scheduler/scheduler.h"


namespace bokket
{

class IOContext
{

friend class IOManager;

public:
    enum Event
    {
        NONE    = 0x0,
        READ    = 0x1,
        WRITE   = 0x4,
    };

   struct Context
   {
       Scheduler *scheduler = nullptr;
       Fiber::ptr fiber;
       std::function<void()> cb;
   };


public:
    IOContext(int fd);

    ~IOContext()=default;

    Context& getContext(Event event);

    //void resetContext(Event event);
    void resetContext(Context& ctx);

    //void reset();

    void triggerEvent(Event event);


private:
    //std::mutex mutex_;
    std::shared_mutex mutex_;
    int fd_;
    //Context context_[2];
    Context read_;
    Context write_;

    Event events_=Event::NONE;
};


}



#endif //BOKKERVE_PLUS_PLUS_IOCONTEXT_H
