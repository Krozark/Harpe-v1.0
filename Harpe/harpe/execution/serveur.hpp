#include "../parser_mgf/parser.hpp"
#include "../analyseur/analyseur_peptide.hpp"
#include "../score_evo/random.hpp"

#ifdef APPRENTISSAGE
#include "../score_evo/IndividuTree.hpp"
#include "../score_evo/GenetiqueEngine.hpp"
#endif

#if DEBUG & DEBUG_STATS
extern float calc_stats[20][STATS_SIZE];
#endif

int serveur(int argc,char* argv[]);

#define SHOW_ARGS_SERVEUR(x) {cerr<<x<<endl<<"Les arguments sont:\n\
    -l nombre (de solution à lister),\n\
    -m masse max d'un trou à considérer\n\
    -i ignorer les charges des peaks dans le fichier, \n\
    -bdd base de donnée (Harpe par défaut), \n\
    -pass mot de passe de l'utilisateur de la basse de donnée (root par défaut), \n\
    -user utilisateur de la basse de donnée (root par défaut), -host adresse ip du serveur (127.0.0.1 par défaut), \n\
    -port port de connextion à la base donnée (3306 par défaut)"<<endl;return 1;}
