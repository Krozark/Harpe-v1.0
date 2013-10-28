#include <Socket/client/Client.hpp>
#include <Socket/FuncWrapper.hpp>

namespace ntw
{
    namespace cli
    {
        Client::Client() :  request_sock(Socket::Dommaine::IP,Socket::Type::TCP)
        {
        }

        Client::~Client()
        {
            request_sock.shutdown();
        }

        int Client::connect(const std::string& host,int port)
        {   
            //init request sock
            request_sock.connect(host,port);
            std::cout<<"Verify connexion"<<std::endl;
            if (ntw::FuncWrapper::verifyIsConnected(request_sock) != NTW_ERROR_NO)
            {
                return NTW_ERROR_CONNEXION;
            }
            std::cout<<"Ready"<<std::endl;
            return NTW_ERROR_NO;
        }

    }
}
