//
// Created by bokket on 2021/5/23.
//

#ifndef BOKKERVE_PLUS_PLUS_TIMER_H
#define BOKKERVE_PLUS_PLUS_TIMER_H

#include <set>
#include <vector>
#include <memory>

#include "../thread/thread.h"


namespace bokket
{

class TimerManager;

class Timer: public std::enable_shared_from_this<Timer>
{
friend class TimerManager;


public:
    using ptr=std::shared_ptr<Timer>;
public:
    bool cancel();
    bool refresh();
    bool reset(uint64_t ms,bool fromNow);

private:
    Timer(uint64_t ms,std::function<void()> cb,
          bool recurring,TimerManager* manager);
    Timer(uint64_t next);

private:
    struct Comparator
    {
        bool operator()(const Timer::ptr& lhs,const Timer::ptr& rhs) const;
    };

private:
    bool recurring_=false;

    uint64_t ms_=0;

    uint64_t next_=0;

    std::function<void()> cb_;

    TimerManager* manager_= nullptr;
};


class TimerManager
{
friend class Timer;

public:
    

};

}



#endif //BOKKERVE_PLUS_PLUS_TIMER_H
