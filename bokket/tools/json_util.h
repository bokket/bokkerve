//
// Created by bokket on 2020/12/23.
//

#ifndef C_2_JSON_UTIL_H
#define C_2_JSON_UTIL_H

#include <string>
#include <jsoncpp/json/json.h>
using namespace std;
namespace bokket {
    class JsonUtil {
    public:
        static bool Escape(const string &v);

        static string Escape(const string &v);

        static string GetString(const Json::Value &json
                                , const string &name
                                , const string &default_v = "");

        static double GetDouble(const Json::Value &json
                                , const string &name
                                , double default_v = 0);

        static int64_t GetInt64(const Json::Value &json
                                , const string &name
                                , int64_t default_v = 0);

        static uint64_t GetUint64(const Json::Value &json
                                  , const string &name
                                  , uint64_t default_v = 0);

        static bool FromString(Json::Value &json
                               , const string &v);

        static string ToString(const Json::Value &json);
    };
}


#endif //C_2_JSON_UTIL_H
