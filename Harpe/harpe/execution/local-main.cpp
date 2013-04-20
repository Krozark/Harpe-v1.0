#include "Socket.hpp"
#include "Serializer.hpp"
#include <iostream>

int main(int argc, char* argv[])
{
    Socket sock(Socket::Dommaine::IP,Socket::Type::TCP);
    sock.Connect("127.0.0.1");
    char msg[32] = {0};
    sock.Receive(msg,32);
    std::cout<<"Recu : "<<msg<<std::endl;
    std::cout<<__BYTE_ORDER__
        <<" "<<__ORDER_LITTLE_ENDIAN__
        <<" "<<__ORDER_BIG_ENDIAN__<<std::endl;

    Serializer ser(0);
    char a[]="test de la mort";
    ser<<'t'
        <<65
        <<67
        <<a;

    std::cout<<ser.size()<<" ";
    for(int i=0; i< ser.size();++i)
        std::cout<<"<"<<ser.buffer()[i]<<">";
    std::cout<<std::endl;

    char c;
    ser>>c;
    std::cout<<c<<std::endl;

    std::cout<<ser.size()<<" ";
    for(int i=0; i< ser.size();++i)
        std::cout<<"<"<<ser.buffer()[i]<<">";
    std::cout<<std::endl;

    
    return 0;
};


