//
// Created by bokket on 2021/5/14.
//

#ifndef BOKKERVE_CONFIG_H
#define BOKKERVE_CONFIG_H


#include <memory>
#include <string>
#include <sstream>
#include <map>
#include <unordered_map>
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
    //using ptr=std::shared_ptr<ConfigVar>;
    typedef std::shared_ptr<ConfigVar> ptr;
public:
    ConfigVar(const std::string & name,const T & default_value, const std::string description="")
            :ConfigVarBase(name,description)
            ,val_(default_value)
    {}

    std::string toString() override {
        try {
            return boost::lexical_cast<std::string>(val_);
        } catch (std::exception & e) {
            BOKKET_LOG_ERROR(BOKKET_LOG_ROOT()) << "ConfigVar::toString error"
            <<e.what()<<"convert:"<< typeid(val_).name()<<"to string";
        }
        return "";
    }

    bool fromString(const std::string &val) override {
        try {
            val_ = boost::lexical_cast<T>(val);
        } catch (std::exception & e) {
            BOKKET_LOG_ERROR(BOKKET_LOG_ROOT()) << "ConfigVar::fromString error"
            <<e.what()<<"convert:"<< typeid(val_).name()<<"from string";
        }
        return false;
    }

    const T getValue() { return val_; }
    void setValue(const T& v) { val_=v; }

private:
    T val_;
};

class Config
{
public:
    using ConfigVarMap=std::unordered_map<std::string,ConfigVarBase::ptr>;
private:
    static ConfigVarMap s_datas;
public:
    template<class T>
    static typename ConfigVar<T>::ptr Lookup(const std::string& name,
                                             const T & default_value,
                                             const std::string & description="")
    {
        auto tmp = Lookup<T>(name);
        if (tmp==nullptr) {
            BOKKET_LOG_INFO(BOKKET_LOG_ROOT()) << "Lookup name=" << name << " exists";
            return tmp;
        }
        if (name.find_first_not_of("abcdefghikjlmnopqrstuvwxyz._012345678910") != std::string::npos)
        {
            BOKKET_LOG_ERROR(BOKKET_LOG_ROOT()) << "Lookup name invalid" << name;
            throw std::invalid_argument(name);
        }

        //typename ConfigVar<T>::ptr v(new ConfigVar<T>(name, default_value, description));
        typename ConfigVar<T>::ptr v(new ConfigVar<T>(name, default_value, description));
        s_datas[name] = v;
        return v;
    }

    template<class T>
    static typename ConfigVar<T>::ptr Lookup(const std::string & name)
    {
        auto it=s_datas.find(name);
        if(it==s_datas.end())
            return nullptr;
        return std::dynamic_pointer_cast< ConfigVar<T> >(it->second);
    }

    static void LoadFromYaml(const YAML::Node & root);
    static ConfigVarBase::ptr LookupBase(const std::string& name);
};
}


#endif //BOKKERVE_CONFIG_H
