#include "SocketSerialized.hpp"
#include "SelectManager.hpp"

#include <iostream>
#include <chrono>

void reply(ntw::SelectManager& selector,ntw::Socket& sock)
{
    ntw::SocketSerialized& clientSock = *(ntw::SocketSerialized*)&sock;
    clientSock.Receive();
    //clientSock.Shutdown(ntw::Socket::Down::RECIVE);

    std::cout<<clientSock<<std::endl;

    char* c=0;
    clientSock>>c;
    std::cout<<"[client] recu char*: <"<<c<<">"<<std::endl;

    clientSock.Clear();
    clientSock<<"<message venu du client.>";
    clientSock.Send();
    
};

int main(int argc, char* argv[])
{
    /*
    ntw::Socket sock(ntw::Socket::Dommaine::IP,ntw::Socket::Type::TCP);
    sock.Connect("127.0.0.1");
    char msg[32] = {0};
    sock.Receive(msg,32);
    std::cout<<"Recu : "<<msg<<std::endl;
    */

    /*
    ntw::Serializer ser(0);
    char a[]="test de la mort";
    ser<<'t'
        <<65
        <<67
        <<a
        <<68;

    char c;
    ser>>c;
    std::cout<<c<<std::endl;

    int i;
    ser>>i;
    std::cout<<i<<std::endl;

    ser>>i;
    std::cout<<i<<std::endl;

    char* s = NULL;

    ser>>s;
    std::cout<<s<<std::endl;
    */
    
    ntw::SocketSerialized clientSock(ntw::Socket::Dommaine::IP,ntw::Socket::Type::TCP);
    clientSock.Connect("127.0.0.1");

    ntw::SelectManager clientSelector;
    clientSelector.SetRead(true);
    clientSelector.OnSelect = reply;
    clientSelector.Add(&clientSock);

    clientSelector.Start();
    //std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    //clientSelector.Stop();
    clientSelector.Wait();

    return 0;
};


