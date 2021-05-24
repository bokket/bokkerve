//
// Created by bokket on 2021/5/23.
//

#ifndef BOKKERVE_PLUS_PLUS_IOMANAGER_H
#define BOKKERVE_PLUS_PLUS_IOMANAGER_H

#include "../Scheduler/scheduler.h"
#include "../time/timer.h"

namespace bokket
{

class IOManager: public Scheduler, public Timer
{
public:
    using ptr = std::shared_ptr<IOManager>;

public:
    enum class Event
    {
        NONE    = 0x0,
        READ    = 0x1,
        WRITE   = 0x4,
    };

public:
    IOManager(int fd);


protected:

    void tickle() override;

    void stopping() override;

    void idle() override;

    void timerTickle() override;

private:

private:
    struct FdContext
    {
    public:
        Scheduler* scheduler = nullptr;
        Fiber::ptr fiber;
        std::function<void()> cb;

    public:
        Context& getContext(Event event);

        void resetContext(Event event);

        void reset();

        void triggerEvent(Event event);


    private:
        std::mutex mutex_;
        int fd_;
        Context context_[2];

        int event_=Event::NONE;
    };
};

}


#endif //BOKKERVE_PLUS_PLUS_IOMANAGER_H
