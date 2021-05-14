//
// Created by bokket on 2021/5/14.
//

#ifndef BOKKERVE_CONFIG_H
#define BOKKERVE_CONFIG_H


#include <memory>
#include <string>
#include <sstream>
#include <map>
#include <boost/lexical_cast.hpp>
#include <yaml-cpp/yaml.h>
#include "../Log/Log.h"

namespace bokket
{


class ConfigVarBase
{
public:
    using ptr=std::shared_ptr<ConfigVarBase>;
public:
    ConfigVarBase(const std::string& name,const std::string & description="")
                :name_(name)
                ,description_(description)
    {
        std::transform(name_.begin(),name_.end(),name_.begin(),::tolower);
    }

    virtual ~ConfigVarBase()=default;

    const std::string & getName() const { return name_; }
    const std::string & getDescription() const { return description_; }

    virtual std::string toString() =0;
    virtual bool fromString(const std::string& val)=0;
protected:
    std::string name_;
    std::string description_;
};

template<class T>
class ConfigVar: public ConfigVarBase
{
public:
    using ptr=shared_ptr<ConfigVar>;
public:
    ConfigVar(const std::string & name,const T & default_value, const std::string description="")
            :ConfigVarBase(name,description)
            ,val_(default_value)
    {}

    std::string toString() override;

    bool fromString(const std::string &val) override;

    const T getValue() const { return val_; }
    void setValue(const T& v) { val_=v; }

private:
    T val_;
};

class Config
{
public:
    using ConfigVarMap=std::map<std::string,ConfigVarBase::ptr>;
private:
    static ConfigVarMap s_datas;
public:
    template<class T>
    static typename ConfigVar<T>::ptr Lookup(const std::string& name,const T & default_value,const std::string & description="")
    {
        auto tmp = Lookup<T>(name);
        if (tmp) {
            BOKKET_LOG_INFO(BOKKET_LOG_ROOT()) << "Lookup name=" << name << "exists";
            return tmp;
        }
        if (name.find_first_not_of("abcdefghikjlmnopqrstuvwxyz") != string::npos)
        {
            BOKKET_LOG_INFO(BOKKET_LOG_ROOT()) << "Lookup name invalid" << name;
            throw invalid_argument(name);
        }

        typename ConfigVar<T>::ptr v(new ConfigVar<T>(name, default_value, description));
        s_dadas[name] = v;
        return v;
    }

    template<class T>
    static typename ConfigVar<T>::ptr Lookup(const std::string & name)
    {
        auto it=s_datas.find(name);
        if(it==s_datas.end())
            return nullptr;
        return dynamic_pointer_cast<ConfigVar<T> >(it->second);
    }

    static void LoadFromYaml(const YAML::Node & root);
    static ConfigVarBase::ptr LookupBase(const std::string& name);
};
}


#endif //BOKKERVE_CONFIG_H
