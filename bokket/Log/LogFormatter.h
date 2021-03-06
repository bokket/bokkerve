//
// Created by bokket on 2021/3/8.
//

#ifndef BOKKET_LOGFORMATTER_H
#define BOKKET_LOGFORMATTER_H

#include <memory>
#include <chrono>
#include <iomanip>
#include "Logger.h"
#include "LogEvent.h"
#include "LogLevel.h"

namespace bokket
{

class LogFormatter
{
public:
    using ptr=std::shared_ptr<LogFormatter>;

public:
    class Impl
    {
    public:
        using ptr=std::shared_ptr<LogFormatter::Impl>;
    public:
        virtual ~Impl() {}
        virtual void format(std::ostream& os,LogEvent::ptr event)=0;
    };
public:
    void init();

    bool isError() const { return error_; }
private:
    bool error_=false;
};


}



#endif //BOKKET_LOGFORMATTER_H
