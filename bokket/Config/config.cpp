//
// Created by bokket on 2021/5/14.
//

#include "config.h"
#include <tuple>
#include <list>
namespace bokket
{
Config::ConfigVarMap Config::s_datas;


ConfigVarBase::ptr Config::LookupBase(const std::string &name)
{
    auto it=s_datas.find(name);

    return it==s_datas.end() ? nullptr:it->second;
}


static void ListAllMember(const std::string& prefix,const YAML::Node & node
                          ,std::list<std::pair<std::string,const YAML::Node> >& output)
{
    if(prefix.find_first_not_of("abcdefghikjlmnopqrstuvwxyz")!=std::string::npos)
    {
        BOKKET_LOG_ERROR(BOKKET_LOG_ROOT()) << "Config invalid name:" << prefix<<":"<<node;
        return ;
    }
    output.push_back(make_pair(prefix,node));

    if(node.IsMap())
    {
        for(auto it=node.begin();it!=node.end();it++)
        {
            ListAllMember(prefix.empty() ? it->first.Scalar() : prefix + "."+it->first.Scalar()
                          , it->second ,output);

        }
    }
}

void Config::LoadFromYaml(const YAML::Node &root)
{
    std::list<std::pair<std::string,const YAML::Node> > all_nodes;
    ListAllMember("",root,all_nodes);



}
}