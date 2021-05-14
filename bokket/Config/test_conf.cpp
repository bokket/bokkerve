//
// Created by bokket on 2021/5/14.
//

#include <yaml-cpp/yaml.h>
#include <iostream>
int main() {
    try {
        YAML::Node node= YAML::LoadFile("/home/bokket/bokkerve/conf/test.yml");

        std::cout<<node<<std::endl;
    } catch (...) {
        std::cout<<"error"<<std::endl;
    }
}