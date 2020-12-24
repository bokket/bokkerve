//
// Created by bokket on 2020/12/23.
//

#ifndef C_2_COND_HPP
#define C_2_COND_HPP
#include <pthread.h>
#include <exception>
using namespace std;

class cond
{
public:
    cond();
    ~cond();
    bool wait(pthread_cond_t* m_mutex);
    bool timewait(pthread_mutex_t* m_mutex,struct timespec t);
    bool signal();
    bool broadcast();

private:
    pthread_cond_t m_cond;
};

cond::cond()
{
    if(pthread_cond_init(&m))
}
cond::~cond() {}
bool cond::wait(pthread_cond_t *m_mutex) {}
bool cond::timewait(pthread_mutex_t *m_mutex, struct timespec t)
bool cond::signal() {}
bool cond::broadcast() {}


#endif //C_2_COND_HPP
