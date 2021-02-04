//
// Created by bokket on 2020/12/23.
//

#include "json_util.h"

namespace bokket{



bool JsonUtil::Escape(const string &v)
{
    for (auto &c : v) {
        switch (c) {
            case '\f':
            case '\t':
            case '\r':
            case '\n':
            case '\b':
            case '"':
            case '\\':
                return true;
            default:
                break;
        }
    }
    return false;
}
string JsonUtil::Escape(const string &v)
{
    size_t size=0;
    for (auto &c : v) {
        switch (c) {
            case '\f':
            case '\t':
            case '\r':
            case '\n':
            case '\b':
            case '"':
            case '\\':
                size+=2;
                break;
            default:
                size+=1;
                break;
        }
    }
    if(size==v.size())
        return v;

    string vt;
    vt.resize(size);

    for(auto& c:v)
    {
        switch (c) {
            case '\f':
                vt.append("\\f");
                break;
            case '\t':
                vt.append("\\t");
                break;
            case '\r':
                vt.append("\\r");
                break;
            case '\n':
                vt.append("\\n");
                break;
            case '\b':
                vt.append("\\b");
                break;
            case '"':
                vt.append("\\\"");
                break;
            case '\\':
                vt.append("\\\\");
                break;
            default:
                vt.append(1,c);
                break;
        }
    }
    return vt;
}
double JsonUtil::GetDouble(const Json::Value &json
                           , const string &name
                           , double default_v)
{
    if(!json.isMember(name))
        return default_v;
    auto & v=json[name];
    if(v.isDouble())
        return v.asDouble();
    else if(v.isString())
        return Type;

    return default_v;
}

int64_t JsonUtil::GetInt64(const Json::Value &json
                           , const string &name
                           , int64_t default_v)
{
    if(!json.isMember(name))
        return default_v;
    auto & v=json[name];
    if(v.isInt64())
        return v.asDouble();
    else if(v.isString())
        return Type;

    return default_v;
}

uint64_t JsonUtil::GetUint64(const Json::Value &json
                             , const string &name
                             , uint64_t default_v)
{
    if(!json.isMember(name))
        return default_v;
    auto & v=json[name];
    if(v.isUInt64())
        return v.asDouble();
    else if(v.isString())
        return Type;
    return default_v;
}
bool JsonUtil::FromString(Json::Value &json
                          , const string &v)
{
    Json::Reader reader;
    return reader.parse(v,json);
}
string JsonUtil::ToString(const Json::Value &json)
{
    Json::FastWriter writer;
    return writer.write(json);
}

}