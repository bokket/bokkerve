//
// Created by bokket on 2021/5/26.
//

#ifndef BOKKERVE_PLUS_PLUS_FD_MANAGER_H
#define BOKKERVE_PLUS_PLUS_FD_MANAGER_H


#include <memory>
#include <vector>
#include <shared_mutex>
#include "../thread/thread.h"
#include "../Log/Sigleton.h"


namespace bokket
{

class FdData: public std::enable_shared_from_this<FdData>
{
public:
    using ptr=std::shared_ptr<FdData>;
public:
private:

};


class FdManager
{
public:
    using ptr=std::shared_ptr<FdManager>;
public:
private:
    std::shared_mutex mutex_;
    std::vector<FdData::ptr> datas_;
};

using FdMgr=Sigleton<FdManager>;

}



#endif //BOKKERVE_PLUS_PLUS_FD_MANAGER_H
