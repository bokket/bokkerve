//
// Created by bokket on 2021/5/14.
//

#include <list>
#include <tuple>
#include <algorithm>

#include <ctype.h>

#include "config.h"

namespace bokket
{
//Config::ConfigVarMap Config::s_datas;


static bokket::Logger::ptr g_logger = BOKKET_LOG_NAME("system");

ConfigBase::ptr ConfigManager::LookupBase(const std::string &name)
{
    auto it=datas_.find(name);

    return it==datas_.end() ? nullptr:it->second;
}


static void ListAllMember(const std::string& prefix,const YAML::Node & node,
                          std::list<std::pair<std::string,const YAML::Node> >& output)
{
    if(prefix.find_first_not_of("abcdefghikjlmnopqrstuvwxyz._012345678910")!=std::string::npos)
    {
        BOKKET_LOG_ERROR(BOKKET_LOG_ROOT()) << "Config invalid name:" << prefix<<":"<<node;
        return ;
    }
    output.push_back(std::make_pair(prefix,node));

    if(node.IsMap())
    {
        for(auto it=node.begin();it!=node.end();it++)
        {
            ListAllMember(prefix.empty() ? it->first.Scalar() : prefix + "."+it->first.Scalar()
                          , it->second ,output);

        }
    }
}

void ConfigManager::LoadFromYaml(const YAML::Node &root)
{
    std::list<std::pair<std::string,const YAML::Node> > all_nodes;
    ListAllMember("",root,all_nodes);

    for(auto & [key,value] : all_nodes) {
        std::string key_t = key;
        if(key_t.empty()) {
            continue;
        }
    

        std::transform(key_t.begin(),key_t.end(),key_t.begin(),::tolower);

        ConfigBase::ptr var = LookupBase(key_t);

        if(var) {
            if(value.IsScalar()) {
                var->fromString(value.Scalar());
            } else {
                std::stringstream ss;
                ss<< value;
                var->fromString(ss.str());
            }
        }

    }

}


}