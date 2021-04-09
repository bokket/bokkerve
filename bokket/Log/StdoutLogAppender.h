//
// Created by bokket on 2021/3/9.
//

#ifndef BOKKET_STDOUTLOGAPPENDER_H
#define BOKKET_STDOUTLOGAPPENDER_H

#include "LogAppender.h"

#include <memory>
#include <string>

namespace bokket
{
class StdoutLogAppender: public LogAppender
{
public:
    using ptr=std::shared_ptr<StdoutLogAppender>;
public:
    StdoutLogAppender(const std::string filename);
private:

};
}



#endif //BOKKET_STDOUTLOGAPPENDER_H
