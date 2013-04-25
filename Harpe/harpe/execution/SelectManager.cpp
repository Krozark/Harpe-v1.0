#include "SelectManager.hpp"

#include <iostream>




namespace ntw {

SelectManager::SelectManager(): readfds(0), writefds(0), exceptfds(0), timeout(0), OnSelect(0), max_id(0), run(false)
{
};

SelectManager::~SelectManager()
{
    if(readfds)
        delete readfds;
    if(writefds)
        delete writefds;
    if(exceptfds)
        delete exceptfds;
    if(timeout)
        delete timeout;
}

void SelectManager::Add(Socket* s)
{
    int id = s->Id();
    datas[id]=s;
    max_id=(id>max_id)?id+1:max_id;
    if(readfds)
        FD_SET(id,readfds);
    if(writefds)
        FD_SET(id,writefds);
    if(exceptfds)
        FD_SET(id,exceptfds);
};

void SelectManager::Remove(Socket* s)
{
    int id = s->Id();
    auto it = datas.find(id);
    if(it != datas.end())
        datas.erase(it);
    if(readfds)
        FD_CLR(id,readfds);
    if(writefds)
        FD_CLR(id,writefds);
    if(exceptfds)
        FD_CLR(id,exceptfds);
};
void SelectManager::Clear()
{
    datas.clear();
    max_id=0;
    if(readfds)
        FD_ZERO(readfds);
    if(writefds)
        FD_ZERO(writefds);
    if(exceptfds)
        FD_ZERO(exceptfds);
};

void SelectManager::Reset()
{
    if(readfds)
        FD_ZERO(readfds);
    if(writefds)
        FD_ZERO(writefds);
    if(exceptfds)
        FD_ZERO(exceptfds);
    // add the connection socket
    auto end = datas.end();
    max_id = 0;
    for(auto it=datas.begin();it!=end;++it)
    {
        int id = it->second->Id();
        max_id=(id>max_id)?id+1:max_id;
        if(readfds)
            FD_SET(id,readfds);
        if(writefds)
            FD_SET(id,writefds);
        if(exceptfds)
            FD_SET(id,exceptfds);
    }
};

void SelectManager::SetArgs(bool read,bool write,bool except,float timeout_sec)
{
    SetRead(read);
    SetWrite(write);
    SetExcept(except);
    SetTimout(timeout_sec);
};

void SelectManager::SetRead(bool read)
{
    mutex.lock();
    if(read)
    {
        if(not readfds)
            readfds = new fd_set;
    }
    else
    {
        if(readfds)
        {
            delete readfds;
            readfds = 0;
        }
    }
    mutex.unlock();
};

void SelectManager::SetWrite(bool write)
{
    mutex.lock();
    if(write)
    {
        if(not writefds)
            writefds = new fd_set;
    }
    else
    {
        if(writefds)
        {
            delete writefds;
            writefds = 0;
        }
    }
    mutex.unlock();
};

void SelectManager::SetExcept(bool except)
{
    mutex.lock();
    if(except)
    {
        if(not exceptfds)
            exceptfds = new fd_set;
    }
    else
    {
        if(exceptfds)
        {
            delete exceptfds;
            exceptfds = 0;
        }
    }
    mutex.unlock();
};

void SelectManager::SetTimout(float timeout_sec)
{
    mutex.lock();
    if(timeout_sec > 0.f)
    {
        if(not timeout)
            timeout = new timeval;
        timeout->tv_sec = (int)timeout_sec;
        timeout->tv_usec = (int)timeout_sec*100000;//10⁻⁶
    }
    else
    {
        if(timeout)
        {
            delete timeout;
            timeout = 0;
        }
    }
    mutex.unlock();
};

template<class C,typename ... Args>
void thread_method(C* obj,void(C::*func)(Args ...),Args ... args)
{
    (obj->*func)(args ...);
};

void SelectManager::Start()
{
    mutex.lock();
    run = true;
    thread= std::thread(thread_method<SelectManager>,this,&SelectManager::Run);
    mutex.unlock();
};

void SelectManager::Run()
{
    //Reset();
    
    //can be stop withing Stop on other thread
    while(run)
    {
        int res;
        if(timeout)
        {
            timeval time = *timeout;
            res = select(max_id,readfds,writefds,exceptfds,&time);
        }
        else
        {
            res = select(max_id,readfds,writefds,exceptfds,NULL);
        }


        if(res <0)
        {
            perror("select()");
            return;
        }

        //loop sur les Socket pour savoir si c'est elle
        auto end = datas.end();
        for(auto it=datas.begin();it!=end and res > 0;++it)
        {
            auto& iit = *it;
            if(readfds and FD_ISSET(iit.first,readfds))
            {
                OnSelect(*this,*(iit.second));
                --res;
                continue;
            }
            if(writefds and FD_ISSET(iit.first,writefds))
            {
                OnSelect(*this,*(iit.second));
                --res;
                continue;
            }
            if(exceptfds and FD_ISSET(iit.first,exceptfds))
            {
                OnSelect(*this,*(iit.second));
                --res;
                continue;
            }
        }
    }
};

};
