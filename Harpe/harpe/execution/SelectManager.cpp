#include "SelectManager.hpp"

namespace ntw {

SelectManager::SelectManager(): readfds(0), writefds(0), exceptfds(0), timeout(0), OnSelect(0)
{
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
};

void SelectManager::SetWrite(bool write)
{
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
};

void SelectManager::SetExcept(bool except)
{
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
};

void SelectManager::SetTimout(float timeout_sec)
{
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
};

};
