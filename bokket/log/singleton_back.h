#include <memory>
#include <boost/noncopyable.hpp>
using namespace std;

namespace bokket {


    class Singleton: public boost::noncopyable
    {
    public:
        static Singleton* GetInstance()
        {
            if(!ptr.get())
            {
                ptr=shared_ptr<Singleton>(new Singleton);
            }
            return ptr.get();
        }
    private:
        static shared_ptr<Singleton> ptr;
    };

    class SingletonPtr: public boost::noncopyable
    {
    public:
        static SingletonPtr* GetInstance()
        {
            if(!ptr.get())
            {
                ptr=shared_ptr<SingletonPtr>(new SingletonPtr);
            }
            return ptr.get();
        }
    private:
        static shared_ptr<SingletonPtr> ptr;
    };
}