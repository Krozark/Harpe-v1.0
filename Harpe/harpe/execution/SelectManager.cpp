#include "SelectManager.hpp"

#include <iostream>

#include <errno.h>
#include <signal.h>
#include <string.h>

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
            //timeout = new timespec;
            timeout = new timeval;
        timeout->tv_sec = (int)timeout_sec;
        //timeout->tv_nsec = (int)timeout_sec*100000000;//10⁻⁹
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

// Signal handler to catch SIGTERM.
void handler(int sig) {
    std::cout<<"Signal chopé: "<<sig<<std::endl;
}

void SelectManager::Run()
{
    sigset_t originalSignals;     
    sigset_t blockedSignals;
    sigemptyset(&blockedSignals);
    sigaddset(&blockedSignals, SIGUSR1);
    if(sigprocmask(SIG_BLOCK, &blockedSignals, &originalSignals) != 0)
    {
        perror("Failed to block signals");
        return;
    }
    struct sigaction signalAction;
    memset(&signalAction, 0, sizeof(struct sigaction));

    signalAction.sa_mask = blockedSignals;

    signalAction.sa_handler = handler;

    if(sigaction(SIGUSR1, &signalAction, NULL) == -1)
    {
        perror("Could not set signal handler");
        return;
    }
    while(run)
    {

        Reset();//TODO
        int res;

        sigset_t omask;
        if (sigprocmask(SIG_SETMASK, &originalSignals, &omask) < 0) {
            perror("sigprocmask");
            break;
        }

        if(timeout)
        {
            auto time = *timeout;
            std::cout<<"start select"<<std::endl;
            res = select(max_id,readfds,writefds,exceptfds,&time);
            std::cout<<"endselect"<<std::endl;
        }
        else
        {
            std::cout<<"start select"<<std::endl;
            res = select(max_id,readfds,writefds,exceptfds,NULL);
            std::cout<<"endselect"<<std::endl;
        }
        if (sigprocmask(SIG_SETMASK, &omask, NULL) < 0) {
            perror("sigprocmask");
            break;
        }

        if(res <0)
        {
            if (errno == EINTR)
            {
                std::cout<<"Signal chopé"<<std::endl;
                continue;
            }
            else
            {
                perror("pselect()");
                return;
            }
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
