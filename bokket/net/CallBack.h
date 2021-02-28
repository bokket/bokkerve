//
// Created by bokket on 2021/2/28.
//

#ifndef BOKKET_CALLBACK_H
#define BOKKET_CALLBACK_H


#include <functional>
#include <memory>
#include "Timestamp.h"


namespace bokket
{

using std::placeholders::_1;
using std::placeholders::_2;
using std::placeholders::_3;
using std::placeholders::_4;
using std::placeholders::_5;


using TimerCallback=function<void()>;

}




#endif //BOKKET_CALLBACK_H
