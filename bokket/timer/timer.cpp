//
// Created by bokket on 2020/11/22.
//

#include <timer.h>
#include <iostream>
using namespace std;

sort_timer_lst::~sort_timer_lst()
{
    util_timer* tmp=head;
    while(tmp)
    {
        head=tmp->next;
        delete tmp;
        tmp=head;
    }
}
void sort_timer_lst::add_timer(util_timer* timer)
{
    //如果为空就返回空
    if(!timer)
    {
        return ;
    }
    if(!head)
    {
        head=tail=timer;
        return ;
    }

    //如果目标定时器的超时时间小于当前链表所有定时器的超时事件，就把该定时器插入链表头部
    //否则插入合适的位置保证链表的升序特性
    if(timer->expire < head->expire)
    {
        timer->next=head;
        head->prev=timer;
        head=timer;
        return ;
    }
    //重载函数
    add_timer(timer,head);
}
void sort_timer_lst::adjust_timer(util_timer* timer)
{
    if(!timer)
    {
        return ;
    }
    util_timer* tmp=timer->next;

    if(!tmp || (timer->expire < tmp->expire))
    {
        return ;
    }


    if(timer==head)
    {
        head=head->next;
        head->prev=NULL;
        timer->next=NULL;
        add_timer(timer,head);
    }
    //如果不是头节点，将该点从链表中取出插入到其原来所在位置之后的部分链表中
    else
    {
        timer->prev->next=timer->next;
        timer->next->prev=timer->prev;
        add_timer(timer,head);
    }

}
void sort_timer_lst::del_timer(util_timer* timer)
{
    if(!timer)
        return;
    if(timer==head && timer==tail)
    {
        delete timer;
        head=NULL;
        tail=NULL;
        return;
    }
    if(timer==head)
    {
        head=head->next;
        head->prev=NULL;
        delete timer;
        return;
    }
    if(timer==tail)
    {
        tail=tail->prev;
        tail->next=NULL;
        delete timer;
        return;
    }
    timer->prev->next=timer->next;
    timer->next->prev=timer->prev;
    delete timer;
}
void sort_timer_lst::tick()
{
    if(!head)
        return;
    cout<<"time tick\n"<<endl;

    time_t cur=time(NULL);

    util_timer* tmp=head;
    while(tmp)
    {
        if(cur<tmp->expire)
            break;
        tmp->cb_func(tmp->user_data);

        head=tmp->next;

        if(head)
        {
            head->prev=NULL;
        }
        delet tmp;
        tmp=head;
    }
}
void sort_timer_lst::add_timer(util_timer *timer,util_timer* lst_head)
{
    util_timer* prev=lst_head;
    util_timer* tmp=prev->next;

    while(tmp)
    {
        if(timer->expire < tmp->expire)
        {
            prev->next=timer;
            timer->next=tmp;
            tmp->prev=timer;
            timer->prev=prev;
            break;
        }
        prev=tmp;
        tmp=tmp->next;
    }
    if(!tmp)
    {
        prev->next=timer;
        timer->prev=prev;
        timer->next=NULL;
        tail=timer;
    }
}