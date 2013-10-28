#include <Socket/server/Server.hpp>

#if NTW_MODE == NTW_SERVER
namespace ntw
{
namespace srv
{
    Server::Server(unsigned int max_client,unsigned int min_client,float timeout) :
        new_connexion_sock(ntw::Socket::Dommaine::IP,ntw::Socket::Type::TCP),
        new_connexion_recv(timeout),
        request_recv(true,false,false,onRequestRecv,this,min_client,max_client,0,timeout)
    {
        //request_recv.setDelete(false);
        //new_connexion_recv.setDelete(false);
        //init sock
        new_connexion_sock.serverMode(NTW_PORT_SERVER);
        //init selector
        new_connexion_recv.setRead(true);
        new_connexion_recv.onSelect = onNewClientRecv;
        new_connexion_recv.data = this;
        //add sock
        new_connexion_recv.add(&new_connexion_sock);
    }

    Server::~Server()
    {
        new_connexion_sock.shutdown();
    }

    void Server::start()
    {
        //start
        new_connexion_recv.start();
        request_recv.start();

        //join
        new_connexion_recv.wait();
        request_recv.wait();
    }

    void Server::stop()
    {
        new_connexion_recv.stop();
        request_recv.stop();

    }

    void Server::wait()
    {
        new_connexion_recv.wait();
        request_recv.wait();
    }
    void Server::onNewClientRecv(ntw::SelectManager& new_connexion_recv,void* data, ntw::SocketSerialized& sock)
    {
        Server& self = *(ntw::srv::Server*)data;

        self.client_mutex.lock();
        self.clients.emplace_back();
        Client& client = self.clients.back();
        self.client_mutex.unlock();

        sock.accept(client.request_sock);
        bool ok = true;
        if(not (self.request_recv.add(&client.request_sock)))
        {
            ok = false;
            ntw::FuncWrapper::msg(client.request_sock,NTW_ERROR_REQUEST_ADD_MSG,NTW_ERROR_REQUEST_ADD);
        }

        if(not ok)
        {
            client.request_sock.shutdown();
        }
        else
        {
            ntw::FuncWrapper::msg(client.request_sock,NTW_WELCOM_MSG,NTW_ERROR_NO);
        }            
    }

    void Server::onRequestRecv(ntw::SelectManager& request_recv,void* data, ntw::SocketSerialized& sock)
    {
        if(sock.receive() >0)
        {
            ntw::FuncWrapper::dispatch(sock);
        }
        else
        {
            std::cerr<<"[SERVER] onRequest connexion lost <id:"<<sock.id()<<">"<<std::endl; 
            Server& self = *(Server*)data;
            Client* client = ((ntw::srv::Client*)((long int)(&sock) - (long int)(&((ntw::srv::Client*)NULL)->request_sock)));
            
            self.remove (client);
        }
    }

    bool Server::remove(Client* client_ptr)
    {
        client_mutex.lock();
        auto begin = clients.begin();
        while(begin != clients.end())
        {
            if(&(*begin) == client_ptr)
            {
                std::cout<<"delete client"<<std::endl;
                Client& client = *client_ptr;

                request_recv.remove(&client.request_sock);
                client.request_sock.shutdown();

                begin = clients.erase(begin);
            }
            else
                ++begin;
        }
        client_mutex.unlock();
    }
}
}
#endif
