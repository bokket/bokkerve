#include <filesystem>
#include <iostream>
using namespace std;

namespace fs=std::filesystem;

int main()
{
    fs::path s("./test");
    fs::create_directory(s);
    cout<<s.filename()<<endl;
    cout<<s.c_str()<<endl;
    
}