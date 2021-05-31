//
// Created by bokket on 2021/5/26.
//

#ifndef BOKKERVE_PLUS_PLUS_FD_MANAGER_H
#define BOKKERVE_PLUS_PLUS_FD_MANAGER_H


#include <memory>
#include <vector>
#include <shared_mutex>


#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "../Log/Sigleton.h"


namespace bokket
{

class FdData: public std::enable_shared_from_this<FdData>
{
public:
    using ptr=std::shared_ptr<FdData>;
public:
    FdData(int fd);

    int64_t getTimeOut(int type);

    void setTimerOut(int type,int64_t v);
private:
    int fd_;
    int64_t recvTimeOut_;
    int64_t sendTimeOut_;

};


class FdManager
{
public:
    using ptr=std::shared_ptr<FdManager>;
public:
    FdManager();

    FdData::ptr get(int fd,bool auto_create=false);
    void del(int fd);
private:
    std::shared_mutex mutex_;
    std::vector<FdData::ptr> datas_;
};

using FdMgr=Sigleton<FdManager>;

}



#endif //BOKKERVE_PLUS_PLUS_FD_MANAGER_H
