//
// Created by bokket on 2021/3/9.
//

#ifndef BOKKET_FILELOGAPPENDER_H
#define BOKKET_FILELOGAPPENDER_H


#include "LogAppender.h"
#include <memory>
#include <string>

namespace bokket
{
class FileLogAppender: public LogAppender
{
public:
    using ptr=std::shared_ptr<FileLogAppender>;
public:
    FileLogAppender(const std::string& filename)

};

}


#endif //BOKKET_FILELOGAPPENDER_H
