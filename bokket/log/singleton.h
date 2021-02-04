#include <memory>
#include <boost/noncopyable.hpp>
using namespace std;

namespace bokket {


    template<class T>
    class Singleton: public boost::noncopyable
    {
    public:
        static T* GetInstance()
        {
            static Singleton v;
            return &v;
        }
    };

    template<class T>
    class SingletonPtr: public boost::noncopyable
    {
    public:
        static shared_ptr<T> GetInstance()
        {
           static shared_ptr<T> v(new T);
           return v;
        }
    };
}