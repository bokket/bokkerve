//
// Created by bokket on 2021/3/4.
//

#ifndef BOKKET_TYPES_H
#define BOKKET_TYPES_H

namespace bokket
{

template<typename To,typename From>
inline To implict_cast(From const& f)
{
    return f;
}



}


#endif //BOKKET_TYPES_H
