#include "Socket.hpp"
#include <iostream>

int main(int argc, char* argv[])
{
    Socket sock(Socket::Dommaine::IP,Socket::Type::TCP);
    sock.Connect("127.0.0.1");
    char msg[32] = {0};
    sock.Receive(msg,32);
    std::cout<<"Recu : "<<msg<<std::endl;
    std::cout<<"char: "<<sizeof(char)
        <<" short: "<<sizeof(short)
        <<" int: "<<sizeof(int)
        <<" float: "<<sizeof(float)
        <<" double: "<<sizeof(double)
        <<" long: "<<sizeof(long)
        <<" long double:"<<sizeof(long double)
        <<std::endl;
    return 0;
};


