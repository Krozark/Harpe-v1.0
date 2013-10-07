#include "serveur.hpp"

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <stddef.h>

Serveur::Serveur() : main_sock(ntw::Socket::Dommaine::IP,ntw::Socket::Type::TCP)
{
    main_sock.ServeurMode(MAX_CLIENTS,"",PORT);

    new_client_selector.SetRead(true);
    new_client_selector.OnSelect = Serveur::newClientRecived;
    new_client_selector.Add(&main_sock);

    client_selector.SetRead(true);
    client_selector.OnSelect = Serveur::clientMsgRecived;

};


Serveur::~Serveur()
{
    stop();
}

void Serveur::start()
{
    new_client_selector.Start();
    client_selector.Start();

    new_client_selector.Wait();
    client_selector.Wait();
}

void Serveur::stop()
{
    new_client_selector.Stop();
    client_selector.Stop();
}


void Serveur::newClientRecived(ntw::SelectManager& new_client_selector, ntw::Socket& client_sock)
{
    ntw::SocketSerialized* clientSock = new ntw::SocketSerialized(client_sock.Accept());
    //gros hack pour choper l'instance de serveur
    Serveur* serveur = (Serveur*)((int)&new_client_selector- (int)offsetof(class Serveur,new_client_selector));
    serveur->client_selector.Add(clientSock);

    *clientSock<<"hello!";
    clientSock->Send();
    
}

void Serveur::clientMsgRecived(ntw::SelectManager& client_selector, ntw::Socket& client_sock)
{
    ntw::SocketSerialized& clientSock = *(ntw::SocketSerialized*)&client_sock;
    if(clientSock.Receive() >0)
    {
        char* c=0;
        clientSock>>c;
        std::cout<<"[serveur] recu char*: <"<<c<<">"<<std::endl;
        clientSock.Clear();
        clientSock<<"message du serveur";
        clientSock.Send();
    }
    else
    {
        std::cerr<<"Client connection lost <id:"<<clientSock.Id()<<">"<<std::endl; 
        client_selector.Remove(&clientSock);
        delete &clientSock;
    }
}

int serveur(int argc,char* argv[])
{
    /*int nb_affiche = -1;
    bool ignore = false;
    float trou_max = 500;
    {//recuperation des arguments
        int i=1;
        while(i<argc)
        {
            std::string arg(argv[i]);

            if (arg == "-l")
            {
                if(++i <argc)
                    nb_affiche = atoi(argv[i]);
                else
                    SHOW_ARGS_SERVEUR("Pas de nombre")
            }
            else if (arg == "-i")
            {
                ignore=true;
            }
            else if (arg == "-m")
            {
                if(++i <argc)
                    trou_max = atof(argv[i]);
                else
                    SHOW_ARGS_SERVEUR("Pas de nombre")
            }
            else if(arg == "-bdd")
            {
                if(++i <argc)
                    s_bdd = argv[i];
                else
                    SHOW_ARGS_SERVEUR("Pas de base de donnée spécifiée")
            }
            else if(arg == "-pass")
            {
                if(++i <argc)
                    s_password = argv[i];
                else
                    SHOW_ARGS_SERVEUR("Pas de mot de pass spécifiée")
            }
            else if(arg == "-user")
            {
                if(++i <argc)
                    s_username = argv[i];
                else
                    SHOW_ARGS_SERVEUR("Pas de nom d'utilisateur spécifiée")
            }
            else if(arg == "-host")
            {
                if(++i <argc)
                    s_server = argv[i];
                else
                    SHOW_ARGS_SERVEUR("Pas d'adresse ip spécifiée")
            }
            else if(arg == "-port")
            {
                if(++i <argc)
                    s_port = argv[i];
                else
                    SHOW_ARGS_SERVEUR("Pas de port spécifié")
            }
            ++i;
        }
    }*/

    //BDD_INIT();

    app();

    //BDD_DESTROY()
        return 0;
};

static void app(void)
{
    Serveur serveur;
    serveur.start();
};
