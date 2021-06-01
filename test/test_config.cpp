//
// Created by bokket on 2021/5/15.
//

#include <iostream>
#include <yaml-cpp/yaml.h>

#include "../bokket/Config/config.h"
#include "../bokket/Log/Log.h"



bokket::Config<int>::ptr g_int_config=
        CONFIG()->Lookup("system.port",static_cast<int>(8080),"system port");
        //bokket::ConfigMgr::GetInstance()->Lookup("system.port",static_cast<int>(8080),"system port");

bokket::Config<float>::ptr g_float_config=
        CONFIG()->Lookup("system.value",static_cast<float >(10.2f),"system value");
        //bokket::ConfigMgr::GetInstance()->Lookup("system.value",static_cast<float >(10.2f),"system value");
/*
bokket::Config<vector<int>>::ptr g_vector_int_config=
        CONFIG()->Lookup<>();
*/

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

    BOKKET_LOG_INFO(BOKKET_LOG_ROOT())<<"before:"<<g_int_config->getValue()
    <<"  "<<g_int_config->getDescription()<<" "<<g_int_config->getName()
    <<" "<<g_int_config.get()<<" "<<" "<<g_int_config->getValue()<<" "<<g_int_config->toString()
    <<" type name="<<g_int_config->getTypeName();
    //g_int_config->setValue((int)11)
    BOKKET_LOG_INFO(BOKKET_LOG_ROOT())<<"before:"<<g_float_config->toString();
    

    YAML::Node root=YAML::LoadFile("/home/bokket/bokkerve/conf/test.yml");
    //bokket::ConfigMgr::GetInstance()->LoadFromYaml(root);
    CONFIG_LOAD(root);


    BOKKET_LOG_INFO(BOKKET_LOG_ROOT())<<"after:"<<g_int_config->getValue()
    <<" description="<<g_int_config->getDescription()<<" toString="<<g_int_config->toString()<<" getName="<<g_int_config->getName()
    <<" "<<g_int_config->getValue()<<" type name="<<g_int_config->getTypeName();
    BOKKET_LOG_INFO(BOKKET_LOG_ROOT())<<"after:"<<g_float_config->toString();
}

int main() {
    test_config();

    test_yaml();
}