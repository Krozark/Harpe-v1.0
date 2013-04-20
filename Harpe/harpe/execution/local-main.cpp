#include "Socket.hpp"
#include "Serializer.hpp"
#include <iostream>

int main(int argc, char* argv[])
{
    ntw::Socket sock(ntw::Socket::Dommaine::IP,ntw::Socket::Type::TCP);
    sock.Connect("127.0.0.1");
    char msg[32] = {0};
    sock.Receive(msg,32);
    std::cout<<"Recu : "<<msg<<std::endl;
    std::cout<<__BYTE_ORDER__
        <<" "<<__ORDER_LITTLE_ENDIAN__
        <<" "<<__ORDER_BIG_ENDIAN__<<std::endl;

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

    return 0;
};


