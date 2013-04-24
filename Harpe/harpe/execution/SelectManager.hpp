#ifndef SELECTMANAGER_HPP
#define SELECTMANAGER_HPP

#include <unordered_map>

#include "Socket.hpp"

namespace ntw {

class SelectManager
{
    public:
        SelectManager();
        
        void Add(Socket* s);
        void Remove(Socket* s);

        void(*OnSelect)(SelectManager& self,Socket& s);
        void SetArgs(bool read=false,bool write=false,bool except=false,float timeout_sec=0);
        void SetRead(bool read=false);
        void SetWrite(bool write=false);
        void SetExcept(bool except=false);
        void SetTimout(float timout_sec=0);

        SelectManager(const SelectManager& other) = delete;
        SelectManager& operator=(const SelectManager& other) = delete;
    private:
        fd_set* readfds;
        fd_set* writefds;
        fd_set* exceptfds;
        timeval* timeout;
        std::unordered_map<int,Socket*> datas;
};

};

#endif
