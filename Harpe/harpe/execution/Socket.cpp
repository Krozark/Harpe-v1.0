#include "Socket.hpp"

unsigned int Socket::Max_clients = 5;

Socket::Socket(Socket::Dommaine dommaine,Socket::Type type,int protocole)
{
    //déclaration de la socket
    sock = socket(dommaine,type,protocole);

    //sin_addr.s_addr = adresse IP à utiliser
    sock_cfg.sin_addr.s_addr = htonl(INADDR_ANY); //IP automatiquement chopée utiliser inet_addr("127.0.0.1") pour présiser
    //sin_family = Dommaine
    sock_cfg.sin_family = dommaine;
    //sin_port = port à utiliser
    sock_cfg.sin_port = htons(PORT);

    if(bind(sock,(SOCKADDR*)&sock_cfg,sizeof(sock_cfg)) == SOCKET_ERROR)
    {
        perror("bind()");
        throw "Ennable to bind soket";
    }

    if(listen(sock,Socket::Max_clients) == SOCKET_ERROR)
    {
        perror("listen()");
        throw "Ennable to listen";
    }

};

Socket::~Socket()
{
    closesocket(sock);
};

