//
// Created by bokket on 2021/3/7.
//

#ifndef BOKKET_FILEUTIL_H
#define BOKKET_FILEUTIL_H


#include "../base/noncopyable.h"
#include <string>
#include <fstream>
#include <sstream>

namespace bokket
{

class FileUtil: public noncopyable
{
//public:
    //static const int kBufferSize=65536;
public:
    explicit FileUtil(const FileUtil& )
    {}
    ~FileUtil()
    {}

    static bool readFileContent(const std::string &filepath,std::string& content)
    {
        std::ifstream fileContent(filepath);
        if(!fileContent)
            return false;
        std::ostringstream oss;

        oss<<fileContent.rdbuf();
        content=oss.str();

        fileContent.close();
        return true;
    }
    //void append(const std::string& msg,int32_t line);

    //void flush();



private:


    int fd_;
    char buffer[kBufferSize];
};
}
#endif //BOKKET_FILEUTIL_H
