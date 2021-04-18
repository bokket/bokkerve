//
// Created by bokket on 2021/4/11.
//

#ifndef BOKKERVE_UTIL_H
#define BOKKERVE_UTIL_H


#include <filesystem>


namespace bokket
{

class FSUtil
{
public:
    static bool Mkdir(const std::string& dirname);
};
}

#endif //BOKKERVE_UTIL_H
