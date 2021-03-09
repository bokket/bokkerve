//
// Created by bokket on 2021/2/19.
//

#include "LogStream.h"
#include "FixedBuffer.h"
#include <cmath>
#include <iostream>


int main()
{
    bokket::detail::LogStream stream;

    int t=1000;
    long long i=123456789;


    void* p= nullptr;
   int* d=&t;

    bokket::detail::FixedBuffer<1000> buffer;
    buffer.append("wxz",3);
    
    

    stream<<buffer.toString()<<2.123454678<<true<< false<<string("pow:")<<::pow(2,10)
            << "wxz\n"<<i<<'t'<<p;

    auto &e=stream.getBuf();
    
    e.toString();
    e.debugString();
}