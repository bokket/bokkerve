//
// Created by bokket on 2021/3/8.
//

#include "LogFormatter.h"


class MessageFormatImpl: public LogFormatter::Impl
{
public:
    MessageFormatImpl(const std::string& str="")
    {}

    void format(LogStream& stream,std::shared_ptr<Logger> logger,LogLevel level,LogEvent::ptr event) override
    {
        stream<<event->
    }
};