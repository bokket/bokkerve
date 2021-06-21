//
// Created by bokket on 2021/5/23.
//

#ifndef BOKKERVE_PLUS_PLUS_IOMANAGER_H
#define BOKKERVE_PLUS_PLUS_IOMANAGER_H

#include <memory>
#include <shared_mutex>

#include "iocontext.h"
#include "../time/timer.h"

namespace bokket
{

class IOManager: public Scheduler, public TimerManager
{
public:
    using ptr = std::shared_ptr<IOManager>;
public:
    IOManager(size_t threads=1,bool useCaller=true
             ,const std::string& name="IOManager");
    ~IOManager();

    int addEvent(int fd,IOContext::Event event,std::function<void()> cb= nullptr);
    bool delEvent(int fd,IOContext::Event event);

    bool delAll(int fd);

    bool cancelEvent(int fd,IOContext::Event event);

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
    std::vector<IOContext*> ioContexts_;


};

}


#endif //BOKKERVE_PLUS_PLUS_IOMANAGER_H
