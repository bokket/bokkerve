#include "noncopyable.h"
#include <memory>

namespace bokket 
{

template<class T>
class Singleton: public noncopyable
{
public:
    static T* GetInstance()
    {
        static T v;
        return &v;
    }
};

template<class T>
class SingletonPtr: public noncopyable
{
public:
    static std::shared_ptr<T> GetInstance()
    {
        static std::shared_ptr<T> v(std::make_shared<T>());
        return v;
    }
};
    
}