#ifndef NTW_CLIENT_HPP
#define NTW_CLIENT_HPP

#include <Socket/SocketSerialized.hpp>
#include <Socket/SelectManager.hpp>
#include <string>

namespace ntw
{
    class SelectManager;
    class SocketSerialized;

    namespace cli
    {
        class Client
        {
            public:
                Client();
                Client(const Client&) = delete;
                Client& operator=(const Client&) = delete;

                ~Client();
                
                int connect(const std::string& host,int port=NTW_PORT_SERVER);

                template<typename Ret,typename ... Args>
                Ret call(Ret (*pf)(SocketSerialized&, Args ...),Args&& ... args);

            private:
                SocketSerialized request_sock;

        };
    }
}
#include <Socket/client/Client.tpl>
#endif
