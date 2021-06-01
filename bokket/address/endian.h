//
// Created by bokket on 2021/6/1.
//

#ifndef BOKKERVE_PLUS_PLUS_ENDIAN_H
#define BOKKERVE_PLUS_PLUS_ENDIAN_H


#include <byteswap.h>
#include <stdint.h>

#include <type_traits>
namespace bokket
{

template<class T>
typename std::enable_if<sizeof(T)==sizeof(uint64_t),T>::type
byteswap(T value) {
    return (T)bswap_64(static_cast<uint64_t>(value));
}

template<class T>
typename std::enable_if<sizeof(T)==sizeof(uint32_t),T>::type
byteswap(T value) {
        return (T)bswap_32(static_cast<uint32_t>(value));
}

template<class T>
typename std::enable_if<sizeof(T)==sizeof(uint16_t),T>::type
byteswap(T value) {
return (T)bswap_16(static_cast<uint16_t>(value));
}

#if BIG_ENDIAN
template<class T>
T LittleEndian(T t) {
    return t;
}

template<class T>
T BigEndian(T t) {
    return byteswap(t);
}

#else

template<class T>
T LittleEndian(T t) {
    return byteswap(t);
}

template<class T>
T BigEndian(T t) {
    return t;
}

#endif


}

#endif //BOKKERVE_PLUS_PLUS_ENDIAN_H
