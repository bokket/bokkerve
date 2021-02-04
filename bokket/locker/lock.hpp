//
// Created by bokket on 2020/12/23.
//

#ifndef C_2_LOCK_HPP
#define C_2_LOCK_HPP

#include <pthread.h>
#include <exception>
using namespace std;
class lock
{
public:
    lock();
    ~lock();
    bool lock();
    bool unlock();
    pthread_mutex_t* get();

private:
    pthread_mutex_t m_mutex;
};

lock::lock()
{
    if(pthread_mutex_lock(&m_mutex,NULL)!=0)
        throw exception;
}
lock::~lock() { pthread_mutex_destroy(&m_mutex); }
bool lock::unlock() { pthread_mutex_unlock(&m_mutex)==0; }
pthread_mutex_t * lock::get() { return &m_mutex; }

#endif //C_2_LOCK_HPP
