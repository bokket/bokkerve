//
// Created by bokket on 2021/5/15.
//

#include <iostream>
#include <yaml-cpp/yaml.h>

#include "../bokket/Config/config.h"
#include "../bokket/Log/Log.h"



bokket::ConfigVar<int>::ptr g_int_config=
        bokket::Config::Lookup("system.port",static_cast<int>(8080),"system port");


bokket::ConfigVar<float>::ptr g_float_config=
        bokket::Config::Lookup("system.value",static_cast<float >(10.2f),"system value");


void print_yaml(const YAML::Node& node,int level) {
    if(node.IsScalar()) {
        BOKKET_LOG_INFO(BOKKET_LOG_ROOT())<<std::string(level*4,' ')
        <<node.Scalar()<<"-"<<node.Type()<<"-"<<level;
    } else if(node.IsNull()) {
        BOKKET_LOG_INFO(BOKKET_LOG_ROOT())<<std::string(level*4,' ')
        <<"NULL "<<"-"<<node.Type()<<"-"<<level;
    } else if(node.IsMap()) {
        for(auto it=node.begin();it!=node.end();++it) {
            BOKKET_LOG_INFO(BOKKET_LOG_ROOT())<<std::string(level*4,' ')
            <<it->first<<"-"<<it->second.Type()<<"-"<<level;

            print_yaml(it->second,level+1);
        }
    } else if(node.IsSequence()) {
        for(size_t i=0;i<node.size();++i) {
            BOKKET_LOG_INFO(BOKKET_LOG_ROOT())<<std::string(level*4,' ')
            <<i<<"-"<<node[i].Type()<<"-"<<level;

            print_yaml(node[i],level+1);
        }
    }
}

void test_yaml() {
    YAML::Node root=YAML::LoadFile("/home/bokket/bokkerve-plus-plus/conf/test.yml");

    print_yaml(root,0);

    BOKKET_LOG_INFO(BOKKET_LOG_ROOT())<<root.Scalar();
}

void test_config() {
    std::cout<<"???"<<std::endl;
    BOKKET_LOG_INFO(BOKKET_LOG_ROOT())<<"before:"<<g_int_config->getValue()
    <<"  "<<g_int_config->getDescription()<<" "<<g_int_config->getName()
    <<" "<<g_int_config.get()<<" "<<" "<<g_int_config->getValue()<<" "<<g_int_config->toString();
    //g_int_config->setValue((int)11)
    BOKKET_LOG_INFO(BOKKET_LOG_ROOT())<<"before:"<<g_float_config->toString();
    

    YAML::Node root=YAML::LoadFile("/home/bokket/bokkerve/conf/test.yml");
    bokket::Config::LoadFromYaml(root);

    BOKKET_LOG_INFO(BOKKET_LOG_ROOT())<<"after:"<<g_int_config->getValue();
    BOKKET_LOG_INFO(BOKKET_LOG_ROOT())<<"after:"<<g_float_config->toString();
}

int main() {
    test_config();
}