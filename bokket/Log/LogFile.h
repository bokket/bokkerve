//
// Created by bokket on 2021/3/9.
//

#ifndef BOKKET_LOGFILE_H
#define BOKKET_LOGFILE_H

#include <memory>
#include <string>
#include <chrono>

namespace bokket
{
class LogFile
{
public:
    LogFile(const std::string filename,off_t rollSize,int flushInterval);

private:

};
}



#endif //BOKKET_LOGFILE_H
