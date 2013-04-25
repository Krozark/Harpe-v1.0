#include "SocketSerialized.hpp"
#include "SelectManager.hpp"

#include <iostream>
#include <chrono>

void reply(ntw::SelectManager& selector,ntw::Socket& sock)
{
    ntw::SocketSerialized& sockSer = *(ntw::SocketSerialized*)&sock;
    std::cout<<sockSer<<std::endl;
    sockSer.Receive();
    sockSer.Shutdown(ntw::Socket::Down::RECIVE);

    std::cout<<sockSer<<std::endl;

    char* c=0;
    sockSer>>c;
    std::cout<<"char* "<<c<<std::endl;

    int i;
    sockSer>>i;
    std::cout<<"i1 "<<i<<std::endl;

    sockSer>>i;
    std::cout<<"i2 "<<i<<std::endl;

    double d;
    sockSer>>d;
    std::cout<<"d1 "<<d<<std::endl;
    std::cout<<sockSer<<std::endl;
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
    
    ntw::SocketSerialized sockSer(ntw::Socket::Dommaine::IP,ntw::Socket::Type::TCP);
    sockSer.Connect("127.0.0.1");

    ntw::SelectManager clientSelector;
    clientSelector.SetRead(true);
    clientSelector.OnSelect = reply;
    clientSelector.Add(&sockSer);

    clientSelector.Start();
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    clientSelector.Stop();

    return 0;
};


