#ifndef SERVEUR_HPP
#define SERVEUR_HPP

#include "Socket/src/Socket.hpp"

#include "../parser_mgf/parser.hpp"
#include "../analyseur/analyseur_peptide.hpp"
//#include "../score_evo/Engine/src/random.hpp"

#ifdef APPRENTISSAGE
#include "../score_evo/IndividuTree.hpp"
#include "../score_evo/Engine/src/GeneticEngine.hpp"
#endif

#if DEBUG & DEBUG_STATS
extern float calc_stats[20][STATS_SIZE];
#endif

int serveur(int argc,char* argv[]);

#define SHOW_ARGS_SERVEUR(x) {std::cerr<<x<<std::endl<<"Les arguments sont:\n\
    -l nombre (de solution à lister),\n\
    -m masse max d'un trou à considérer\n\
    -i ignorer les charges des peaks dans le fichier, \n\
    -bdd base de donnée (Harpe par défaut), \n\
    -pass mot de passe de l'utilisateur de la basse de donnée (root par défaut), \n\
    -user utilisateur de la basse de donnée (root par défaut), -host adresse ip du serveur (127.0.0.1 par défaut), \n\
    -port port de connextion à la base donnée (3306 par défaut)"<<std::endl;return 1;}

#define INIT_ERROR 2

#define MAX_CLIENTS 	4


struct Client
{
    SOCKET sock;
    char name[BUF_SIZE];
};

class Serveur
{
    public:
        Serveur();
};


static void init(void);
static void end(void);
static void app(void);
static int init_connection(void);
static void end_connection(int sock);
//static int read_client(SOCKET sock, char *buffer);
//static void write_client(SOCKET sock, const char *buffer);
//static void send_message_to_all_clients(Client *clients, Client client, int actual, const char *buffer, char from_server);
//static void remove_client(Client *clients, int to_remove, int *actual);
//static void clear_clients(Client *clients, int actual);



#endif
