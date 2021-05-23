//
// Created by bokket on 2021/5/12.
//

#ifndef BOKKERVE_SCHEDULER_H
#define BOKKERVE_SCHEDULER_H

#include <memory>
#include <vector>
#include <list>
#include <mutex>
#include "../Fiber/fiber.h"
#include "../thread/thread.h"
namespace bokket
{

class Scheduler
{
public:
    using ptr= std::shared_ptr<Scheduler>;

public:
    Scheduler(size_t threads=1,bool useCaller=true,const std::string& name="");

    virtual ~Scheduler();

private:
    std::mutex mutex_;
    std::vector<Thread::ptr> threads_;
    std::list<Fiber>

    Fiber::ptr rootFiber_;

    std::string name_;
protected:
    std::vector<int> threadIds_;
    size_t threadCount_=0;
    std::atomic<size_t> activeThreadCount={0};
    std::atomic<size_t> idleThreadCount={0};

    bool stopping_= true;
    bool autoStop_= false;

    int rootThread_ =0;

};

}


#endif //BOKKERVE_SCHEDULER_H
