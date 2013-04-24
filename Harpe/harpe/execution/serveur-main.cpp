#include "Socket.hpp"
#include "SocketSerialized.hpp"

#include <iostream>

int main(int argc, char* argv[])
{
    /*
    ntw::Socket sock(ntw::Socket::Dommaine::IP,ntw::Socket::Type::TCP);
    ntw::Socket client = sock.Wait();
    char msg[32] = "hello word!";
    std::cout<<"Envoi du message: "<<msg<<std::endl;
    client.Send(msg,32);
    client.Shutdown();
    */
    ntw::SocketSerialized sockSer(ntw::Socket::Dommaine::IP,ntw::Socket::Type::TCP);
    ntw::SocketSerialized clientSer(sockSer.Wait());
    std::cout<<"Envoi du message: hello word!"<<std::endl;
    clientSer<<"hello word!";
    std::cout<<clientSer<<std::endl;
    clientSer.Send();


    return 0;
};


