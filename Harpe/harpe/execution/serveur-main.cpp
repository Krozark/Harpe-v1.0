#include "Socket.hpp"
#include <iostream>

int main(int argc, char* argv[])
{
    Socket sock(Socket::Dommaine::IP,Socket::Type::TCP);
    Socket client = sock.Wait();
    char msg[32] = "hello word!";
    std::cout<<"Envoi du message: "<<msg<<std::endl;
    client.Send(msg,32);
    client.Shutdown();
    return 0;
};


