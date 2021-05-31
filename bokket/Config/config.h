//
// Created by bokket on 2021/5/14.
//

#ifndef BOKKERVE_CONFIG_H
#define BOKKERVE_CONFIG_H


#include <memory>
#include <string>
#include <shared_mutex>
#include <sstream>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <boost/lexical_cast.hpp>
#include <yaml-cpp/yaml.h>

#include "../Log/Log.h"
#include "../Log/Sigleton.h"

namespace bokket
{



#define SEQUENCE(root) \
    std::stringstream ss; \
    ss<<root;          \
    return ss.str()


#define CONFIG() bokket::ConfigMgr::GetInstance()
#define CONFIG_LOAD(name) bokket::ConfigMgr::GetInstance()->LoadFromYaml(name);


template<class F,class T>
class LexicalCast
{
public:
    T operator()(const F& v) {
        return boost::lexical_cast<T>(v);
    }
};


template<class T>
class LexicalCast<std::string,std::vector<T>>
{
public:
    std::vector<T> operator()(const std::string& v) {
        YAML::Node node=YAML::Load(v);
        typename std::vector<T> vec;
        std::stringstream ss;
        for(auto i=0;i<node.size();++i) {
            ss.str("");
            ss<<node[i];
            vec.template emplace_back(LexicalCast<std::string,T>()(ss.str()));
        }
        return vec;
    }

};


template<class T>
struct LexicalCast<std::vector<T>,std::string>
{
public:
    std::string operator()(const std::vector<T>& v) {
        YAML::Node node;
        for(auto & i:v) {
            node.template push_back(YAML::Load(LexicalCast<T,std::string>()(i)));
        }

        SEQUENCE(node);
    }
};


template<class T>
class LexicalCast<std::string,std::list<T>>
{
public:
std::list<T> operator()(const std::string& v) {
    YAML::Node node=YAML::Load(v);
    typename std::list<T> vec;
    std::stringstream ss;
    for(auto i=0;i<node.size();++i) {
        ss.str("");
        ss<<node[i];
        vec.template emplace_back(LexicalCast<std::string,T>()(ss.str()));
    }
    return vec;
}

};


template<class T>
struct LexicalCast<std::list<T>,std::string>
{
public:
std::string operator()(const std::list<T>& v) {
    YAML::Node node;
    for(auto & i:v) {
        node.template push_back(YAML::Load(LexicalCast<T,std::string>()(i)));
    }

    SEQUENCE(node);
}
};


template<class T>
class LexicalCast<std::string,std::set<T>>
{
public:
std::set<T> operator()(const std::string& v) {
    YAML::Node node=YAML::Load(v);
    typename std::set<T> vec;
    std::stringstream ss;
    for(auto i=0;i<node.size();++i) {
        ss.str("");
        ss<<node[i];
        vec.insert(LexicalCast<std::string,T>()(ss.str()));
    }
    return vec;
}

};


template<class T>
struct LexicalCast<std::set<T>,std::string>
{
public:
std::string operator()(const std::set<T>& v) {
    YAML::Node node;
    for(auto & i:v) {
        node.template push_back(YAML::Load(LexicalCast<T,std::string>()(i)));
    }

    SEQUENCE(node);
}
};



template<class T>
class LexicalCast<std::string,std::unordered_set<T>>
{
public:
std::unordered_set<T> operator()(const std::string& v) {
    YAML::Node node=YAML::Load(v);
    typename std::unordered_set<T> vec;
    std::stringstream ss;
    for(auto i=0;i<node.size();++i) {
        ss.str("");
        ss<<node[i];
        vec.insert(LexicalCast<std::string,T>()(ss.str()));
    }
    return vec;
}

};


template<class T>
struct LexicalCast<std::unordered_set<T>,std::string>
{
public:
std::string operator()(const std::unordered_set<T>& v) {
    YAML::Node node;
    for(auto & i:v) {
        node.template push_back(YAML::Load(LexicalCast<T,std::string>()(i)));
    }

    SEQUENCE(node);
}
};

template<class T>
class LexicalCast<std::string,std::map<std::string,T>>
{
public:
std::map<std::string,T> operator()(const std::string& v) {
    YAML::Node node=YAML::Load(v);
    typename std::map<std::string,T> vec;
    std::stringstream ss;
    /*for(auto i=0;i<node.size();++i) {
        ss.str("");
        ss<<node[i];
        vec.insert(LexicalCast<std::string,T>()(ss.str()));
    }*/
    for(auto& [key,value ]:vec ) {
        ss.str("");
        ss<<value;
        vec.insert(std::make_pair(key.Scalar(),
        LexicalCast<std::string,T>()(ss.str())));
    }
    return vec;
}

};


template<class T>
struct LexicalCast<std::map<std::string,T>,std::string>
{
public:
std::string operator()(const std::map<std::string,T>& v) {
    YAML::Node node;
    /*for(auto & i:v) {
        //node.template push_back(YAML::Load(LexicalCast<T,std::string>()(i)));
    }*/

    for(auto& [key,value] : v ) {
        node[key]=YAML::node(LexicalCast<T,std::string>()(value));
    }

    SEQUENCE(node);
}
};


template<class T>
class LexicalCast<std::string,std::unordered_map<std::string,T>>
{
public:
std::unordered_map<std::string,T> operator()(const std::string& v) {
    YAML::Node node=YAML::Load(v);
    typename std::unordered_map<std::string,T> vec;
    std::stringstream ss;
    /*for(auto i=0;i<node.size();++i) {
        ss.str("");
        ss<<node[i];
        vec.insert(LexicalCast<std::string,T>()(ss.str()));
    }*/
    for(auto& [key,value ]:vec ) {
        ss.str("");
        ss<<value;
        vec.insert(std::make_pair(key.Scalar(),
        LexicalCast<std::string,T>()(ss.str())));
    }
    return vec;
}

};


template<class T>
struct LexicalCast<std::unordered_map<std::string,T>,std::string>
{
public:
std::string operator()(const std::unordered_map<std::string,T>& v) {
    YAML::Node node;
    /*for(auto & i:v) {
        //node.template push_back(YAML::Load(LexicalCast<T,std::string>()(i)));
    }*/

    for(auto& [key,value] : v ) {
        node[key]=YAML::node(LexicalCast<T,std::string>()(value));
    }

    SEQUENCE(node);
}
};



class ConfigBase
{
public:
    using ptr=std::shared_ptr<ConfigBase>;
public:
    ConfigBase(const std::string& name,const std::string & description="")
              :name_(name)
              ,description_(description)
    {
        std::transform(name_.begin(),name_.end(),name_.begin(),::tolower);
    }

    virtual ~ConfigBase()=default;

    const std::string & getName() const { return name_; }
    const std::string & getDescription() const { return description_; }

    virtual std::string getTypeName() const=0;

    virtual std::string toString() =0;
    virtual bool fromString(const std::string& val)=0;
protected:
    std::string name_;
    std::string description_;
};

template<class T,class FromStr=LexicalCast<std::string,T>
        ,class ToStr=LexicalCast<T,std::string> >
class Config: public ConfigBase
{
public:
    using ptr=std::shared_ptr<Config>;
    //typedef std::shared_ptr<Config> ptr;

    using OnChangeCb=std::function<void(const T&old_value,const T& new_value)>;
public:
    Config(const std::string & name
           ,const T & default_value
           ,const std::string& description="")
           :ConfigBase(name,description)
           ,val_(default_value)
    {}

    std::string toString() override {
        try {
            std::shared_lock<std::shared_mutex> lk(mutex_);
            //return boost::lexical_cast<std::string>(val_);
            return ToStr()(val_);
        } catch (std::exception & e) {
            BOKKET_LOG_ERROR(BOKKET_LOG_ROOT()) << "ConfigVar::toString error"
            <<e.what()<<"convert:"<< typeid(val_).name()<<"to string";
        }
        return "";
    }

    bool fromString(const std::string &val) override {
        try {
            //val_ = boost::lexical_cast<T>(val);
            setValue(FromStr()(val));
        } catch (std::exception & e) {
            BOKKET_LOG_ERROR(BOKKET_LOG_ROOT()) << "ConfigVar::fromString error"
            <<e.what()<<"convert:"<< typeid(val_).name()<<"from string";
        }
        return false;
    }

    const T getValue() {
        std::shared_lock<std::shared_mutex> lk(mutex_);
        return val_;
    }
    void setValue(const T& v) {
        //val_=v;
        {
            std::shared_lock <std::shared_mutex> lk(mutex_);
            if (v == val_)
                return;
            for (auto&[i, cb]:cbs_) {
                cb(val_, v);
            }
        }
        std::unique_lock<std::shared_mutex> lk(mutex_);
        val_=v;
    }
    std::string getTypeName() const override { return typeid(T).name(); }


public:
    void addListener(uint64_t key,OnChangeCb cb) {
        std::unique_lock<std::shared_mutex> lk(mutex_);
        cbs_[key]=cb;
    }

    void delListener(uint64_t key) {
        std::unique_lock<std::shared_mutex> lk(mutex_);
        cbs_.erase(key);
    }

    OnChangeCb getListener(uint64_t key) {
        std::shared_lock<std::shared_mutex> lk(mutex_);
        auto it = cbs_.find(key);
        return it==cbs_.end()? nullptr : it->second;
    }

    void clearListener() {
        std::unique_lock<std::shared_mutex> lk(mutex_);
        cbs_.clear();
    }

private:
    T val_;
    std::shared_mutex mutex_;
    std::unordered_map<uint64_t,OnChangeCb> cbs_;
};

class ConfigManager
{
public:
    using ConfigMap=std::unordered_map<std::string,ConfigBase::ptr>;
/*private:
    static ConfigMap s_datas;*/
public:
    template<class T>
    typename Config<T>::ptr Lookup(const std::string& name
                                  ,const T & default_value
                                  ,const std::string & description="")
    {
        std::unique_lock<std::shared_mutex> lk(mutex_);
        /*auto tmp = Lookup<T>(name);
        if (tmp) {
            BOKKET_LOG_INFO(BOKKET_LOG_ROOT()) << "Lookup name=" << name << " exists";
            return tmp;
        }
        if (name.find_first_not_of("abcdefghikjlmnopqrstuvwxyz._012345678910") != std::string::npos)
        {
            BOKKET_LOG_ERROR(BOKKET_LOG_ROOT()) << "Lookup name invalid" << name;
            throw std::invalid_argument(name);
        }*/
        auto it=datas_.find(name);
        if(it!=datas_.end()) {
            auto tmp=std::dynamic_pointer_cast<Config<T>>(it->second);
            if(tmp) {
                BOKKET_LOG_INFO(BOKKET_LOG_ROOT()) << "Lookup name=" << name << " exists ";
                return tmp;
            } else {
                BOKKET_LOG_ERROR(BOKKET_LOG_ROOT()) << "Lookup name=" << name << " exists but type not "
                                                   <<typeid(T).name()<<", real_type="<<it->second->getTypeName()
                                                   <<""<<it->second->toString();
                return nullptr;
            }
        }

        if (name.find_first_not_of("abcdefghikjlmnopqrstuvwxyz._012345678910") != std::string::npos)
        {
            BOKKET_LOG_ERROR(BOKKET_LOG_ROOT()) << "Lookup name invalid" << name;
            throw std::invalid_argument(name);
        }

        //typename ConfigVar<T>::ptr v(new ConfigVar<T>(name, default_value, description));
        typename Config<T>::ptr v(new Config<T>(name, default_value, description));
        datas_[v->getName()] = v;
        return v;
    }

    template<class T>
    typename Config<T>::ptr Lookup(const std::string & name)
    {
        auto it=datas_.find(name);
        /*if(it==s_datas.end())
            return nullptr;*/
        return it==datas_.end() ? nullptr : std::dynamic_pointer_cast< Config<T> >(it->second);
    }

    void LoadFromYaml(const YAML::Node & root);
    ConfigBase::ptr LookupBase(const std::string& name);

private:
    std::shared_mutex mutex_;

    ConfigMap datas_;
};


using ConfigMgr=bokket::Sigleton<ConfigManager>;

}


#endif //BOKKERVE_CONFIG_H
