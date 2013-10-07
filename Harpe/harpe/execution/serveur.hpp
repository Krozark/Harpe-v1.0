#ifndef SERVEUR_HPP
#define SERVEUR_HPP

#include "Socket/Socket/Socket.hpp"
#include "Socket/Socket/SelectManager.hpp"
#include "Socket/Socket/SocketSerialized.hpp"

/*#include "../parser_mgf/parser.hpp"
#include "../analyseur/analyseur_peptide.hpp"
//#include "../score_evo/Engine/src/random.hpp"

#ifdef APPRENTISSAGE
#include "../score_evo/IndividuTree.hpp"
#include "../score_evo/Engine/src/GeneticEngine.hpp"
#endif
*/
#if DEBUG & DEBUG_STATS
extern float calc_stats[20][STATS_SIZE];
#endif


#define SHOW_ARGS_SERVEUR(x) {std::cerr<<x<<std::endl<<"Les arguments sont:\n\
    -l nombre (de solution à lister),\n\
    -m masse max d'un trou à considérer\n\
    -i ignorer les charges des peaks dans le fichier, \n\
    -bdd base de donnée (Harpe par défaut), \n\
    -pass mot de passe de l'utilisateur de la basse de donnée (root par défaut), \n\
    -user utilisateur de la basse de donnée (root par défaut), -host adresse ip du serveur (127.0.0.1 par défaut), \n\
    -port port de connextion à la base donnée (3306 par défaut)"<<std::endl;return 1;}

class Serveur
{
    public:
        Serveur();
        Serveur(const Serveur&) = delete;
        Serveur& operator=(const Serveur&) = delete;

        ~Serveur();

        void start();
        void stop();

        static const int MAX_CLIENTS = 1;
        static const int PORT = 3987;

    //private :
        ntw::SocketSerialized main_sock;
        ntw::SelectManager new_client_selector;
        ntw::SelectManager client_selector;
        
        static void newClientRecived(ntw::SelectManager& new_client_selector, ntw::Socket& client_sock);
        static void clientMsgRecived(ntw::SelectManager& client_selector, ntw::Socket& client_sock);

};

int serveur(int argc,char* argv[]);

static void app(void);

#endif
