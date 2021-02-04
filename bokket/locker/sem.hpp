//
// Created by bokket on 2020/12/23.
//

#ifndef C_2_SEM_HPP
#define C_2_SEM_HPP
#include <pthread.h>
#include <semaphore.h>
#include <exception>
using namespace std;
class sem
{
public:
    sem();
    sem(int num);
    ~sem();
    bool wait();
    bool post();

private:
    sem_t m_sem;
};

sem::sem()
{
    if(sem_init(&m_sem,0,0)!=0)
        throw exception();
}
sem::sem(int num)
{
    if(sem_init(&m_sem,0,num)!=0)
        throw exception();
}
sem::~sem() { sem_destroy(&m_sem); }
bool sem::wait() { return sem_wait(&m_sem)==0; }
bool sem::post() { return sem_post(&m_sem)==0; }

#endif //C_2_SEM_HPP
