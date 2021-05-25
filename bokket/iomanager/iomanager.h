//
// Created by bokket on 2021/5/23.
//

#ifndef BOKKERVE_PLUS_PLUS_IOMANAGER_H
#define BOKKERVE_PLUS_PLUS_IOMANAGER_H

#include <memory>
#include <shared_mutex>

#include "../Scheduler/scheduler.h"
#include "../time/timer.h"

namespace bokket
{

class IOManager: public Scheduler, public TimerManager
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

private:
    struct FdContext
    {
    public:
        struct EventContext
        {
            Scheduler* scheduler = nullptr;
            Fiber::ptr fiber;
            std::function<void()> cb;
        };

    public:
        EventContext& getContext(Event event);

        void resetContext(EventContext& ctx);

        void triggerEvent(Event event);


    private:
        //std::mutex mutex_;
        std::shared_mutex mutex_;
        int fd_;
        EventContext context_[2];

        Event events_=Event::NONE;
    };

public:
    IOManager(size_t threads=-1,bool useCaller=true,const std::string& name="UNKNOW");
    ~IOManager();

    int addEvent(int fd,Event event,std::function<void()> cb= nullptr);
    bool delEvent(int fd,Event event);
    bool cancelEvent(int fd,Event event);

    bool cancelAll(int fd);

    static IOManager* getThis();

protected:

    void tickle() override;

    bool stopping() override;

    void idle() override;

    void timerTickle() override;


    void contextResize(size_t size);
    bool stopping(uint64_t& timeout);

private:
    int epfd_;
    int tickleFds_[2];
    std::atomic<size_t> pendingEventCount={0};
    //std::mutex mutex_;
    std::shared_mutex mutex_;
    std::vector<FdContext*> fdContexts_;


};

}


#endif //BOKKERVE_PLUS_PLUS_IOMANAGER_H
