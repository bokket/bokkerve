//
// Created by bokket on 2021/5/23.
//

#include "timer.h"
#include "../thread/util.h"

namespace bokket
{

Timer::Timer(uint64_t ms, std::function<void()> cb, bool recurring, TimerManager *manager)
            :recurring_(recurring)
            ,ms_(ms)
            ,cb_(cb)
            ,manager_(manager)
{
    next_=getCurrentMs()+ms_;
}

Timer::Timer(uint64_t next)
            :next_(next)
{}

bool Timer::cancel() {
    std::unique_lock<std::shared_mutex> lk(manager_->mutex_);

    if(cb_) {
        cb_=nullptr;
        auto it=manager_->timers_.find(shared_from_this());
        manager_->timers_.erase(it);
        return true;
    }
    return false;

}

bool Timer::refresh() {
    std::unique_lock<std::shared_mutex> lk(manager_->mutex_);
    if(!cb_) {
        return false;
    }

    auto it=manager_->timers_.find(shared_from_this());
    if(it==manager_->timers_.end())
        return false;

    manager_->timers_.erase(it);
    next_=getCurrentMs()+ms_;

    manager_->timers_.insert(shared_from_this());
    return true;
}

bool Timer::reset(uint64_t ms, bool fromNow) {
    if(ms==ms_ && !fromNow) {
        return true;
    }

    std::unique_lock<std::shared_mutex> lk(manager_->mutex_);
    if(!cb_) {
        return false;
    }

    auto it=manager_->timers_.find(shared_from_this());
    if(it==manager_->timers_.end())
        return false;

    manager_->timers_.erase(it);

    uint64_t start=0;
    if(fromNow) {
        start=bokket::getCurrentMs();
    } else {
        start=next_-ms_;
    }

    ms_=ms;
    next_=start+ms_;

    manager_->addTimer(shared_from_this(),lk);
    return true;
}


bool Timer::Comparator::operator()(const Timer::ptr &lhs, const Timer::ptr &rhs) const {
    if(!lhs && !rhs)
        return false;
    if(!lhs)
        return true;
    if(!rhs)
        return false;
    if(lhs->next_<rhs->next_)
        return true;
    if(rhs->next_<lhs->next_)
        return false;
    return lhs.get()<rhs.get();
}

TimerManager::TimerManager() {

}

Timer::ptr TimerManager::addTimer(uint64_t ms, std::function<void()> cb, bool recurring) {

}

void TimerManager::addTimer(Timer::ptr val, std::unique_lock <std::shared_mutex> &lock) {

}

void TimerManager::timerTickle() {

}

static void OnTimer(std::weak_ptr<void> weak_cond,std::function<void()> cb) {
    std::shared_ptr<void> tmp=weak_cond.lock();
    if(tmp) {
        cb();
    }
}

Timer::ptr TimerManager::addConditionTimer(uint64_t ms, std::function<void()> cb, std::weak_ptr<void> weak_cond,
                                           bool recurring) {
    return addTimer(ms,std::bind(&OnTimer,weak_cond,cb),recurring);
}

uint64_t TimerManager::getNextTimer() {

}
void TimerManager::listExpiredCb(std::vector <std::function<void()>> &cbs) {

}


}