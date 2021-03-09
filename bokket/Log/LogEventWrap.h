//
// Created by bokket on 2021/3/9.
//

#ifndef BOKKET_LOGEVENTWRAP_H
#define BOKKET_LOGEVENTWRAP_H

#include "LogEvent.h"
#include <sstream>

namespace bokket
{
class LogEventWrap
{
public:
    LogEventWrap(LogEvent::ptr event)
                :event_(event)
    {}
    ~LogEventWrap()
    {
        event_->getLogger()->append(event_->getLevel(),event_);
    }
    LogEvent::ptr getEvent() const { return event_; }

    std::stringstream& getStringStream() const { return event_->getStringStream(); }

private:
    LogEvent::ptr event_;
};
}

#endif //BOKKET_LOGEVENTWRAP_H
