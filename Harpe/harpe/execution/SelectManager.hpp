#ifndef SELECTMANAGER_HPP
#define SELECTMANAGER_HPP

#include <unordered_map>
#include <thread>
#include <mutex>

#include "Socket.hpp"

namespace ntw {

class SelectManager
{
    public:
        explicit SelectManager();
        ~SelectManager();
        
        void Add(Socket* s);
        void Remove(Socket* s);
        void Clear();

        void(*OnSelect)(SelectManager& self,Socket& s);
        void SetArgs(bool read=false,bool write=false,bool except=false,float timeout_sec=0);
        void SetRead(bool read=false);
        void SetWrite(bool write=false);
        void SetExcept(bool except=false);
        void SetTimout(float timout_sec=0);

        void Start(); //create a thread and lunch Run() a loop while(run); ie while Stop() is not called
        inline void Stop(bool detach=false){
            mutex.lock();
            run=false;
            mutex.unlock();
            if(detach)
                Detach();
            else
                Wait();
        };
        inline void Wait(){thread.join();};
        inline void Detach(){thread.detach();};

        SelectManager(const SelectManager& other) = delete;
        SelectManager& operator=(const SelectManager& other) = delete;

    private:
        void Run(); //Use Start to run it
        void Reset();

        fd_set* readfds;
        fd_set* writefds;
        fd_set* exceptfds;
        timeval* timeout;
        std::unordered_map<int,Socket*> datas;
        int max_id;
        bool run;
        std::thread thread;
        std::mutex mutex;
};

};

#endif
