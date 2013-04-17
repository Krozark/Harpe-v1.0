#include <cstdlib>
#include <ctime>
//#include<list>

#include "parser_mgf/parser.hpp"
#include "analyseur_peptide.hpp"
#include "score_evo/random.hpp"

#ifdef APPRENTISSAGE
#include "score_evo/IndividuTree.hpp"
#include "score_evo/GenetiqueEngine.hpp"
#else
#include "fonction_finds/408.hpp"
#endif

using namespace std;

BDD_DECLARE();

AA_Tab aa_tab = AA_Tab();


int normal(int argc, char* argv[]);
void serveur(int argc,char* argv[]);

int main(int argc, char* argv[])
{
    return normal(argc,argv);
};

#if DEBUG & DEBUG_STATS
float calc_stats[20][STATS_SIZE];
#endif


///////////// FONCTIONS //////////////////////////
/**
 * @arg 
 * -f file name
 * -e erreur
 * -l nombre (de solutions à afficher)
 **/
#define SHOW_ARGS(x) {cerr<<x<<endl<<"Les arguments sont:\n\
    -f path (de fichier mgf),\n\
    -e erreur,\n\
    -l nombre (de solution à lister),\n\
    -m masse max d'un trou à considérer\n\
    -i ignorer les charges des peaks dans le fichier, \n\
    -bdd base de donnée (Harpe par défaut), \n\
    -pass mot de passe de l'utilisateur de la basse de donnée (root par défaut), \n\
    -user utilisateur de la basse de donnée (root par défaut), -host adresse ip du serveur (127.0.0.1 par défaut), \n\
    -port port de connextion à la base donnée (3306 par défaut)"<<endl;return 1;}

int normal(int argc, char* argv[])
{
    if (argc < 2)
        SHOW_ARGS("pas assez d'arguments")

    rand_init();

    char* filepath;
    double error=0.05;
    int nb_affiche = -1;
    bool ignore = false;
    float trou_max = 500;
    {//recuperation des arguments
        int i=1;
        while(i<argc)
        {
            string arg = string(argv[i]);

            if (arg =="-f")
            {
                if(++i <argc)
                    filepath = argv[i];
                else
                    SHOW_ARGS("Pas de nom de fichier")
            }
            else if (arg == "-e")
            {
                if(++i <argc)
                    error = atof(argv[i]);
                else
                    SHOW_ARGS("Pas de nombre")
            }
            else if (arg == "-l")
            {
                if(++i <argc)
                    nb_affiche = atoi(argv[i]);
                else
                    SHOW_ARGS("Pas de nombre")
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
                    SHOW_ARGS("Pas de nombre")
            }
            else if(arg == "-bdd")
            {
                if(++i <argc)
                    s_bdd = argv[i];
                else
                    SHOW_ARGS("Pas de base de donnée spécifiée")
            }
            else if(arg == "-pass")
            {
                if(++i <argc)
                    s_password = argv[i];
                else
                    SHOW_ARGS("Pas de mot de pass spécifiée")
            }
            else if(arg == "-user")
            {
                if(++i <argc)
                    s_username = argv[i];
                else
                    SHOW_ARGS("Pas de nom d'utilisateur spécifiée")
            }
            else if(arg == "-host")
            {
                if(++i <argc)
                    s_server = argv[i];
                else
                    SHOW_ARGS("Pas d'adresse ip spécifiée")
            }
            else if(arg == "-port")
            {
                if(++i <argc)
                    s_port = argv[i];
                else
                    SHOW_ARGS("Pas de port spécifié")
            }
            ++i;
        }
    }

    if(filepath == "")
        SHOW_ARGS("Un fichier mgf est requis");

    BDD_INIT();

    {//programme en temps que tel
        Parser parser(filepath);
        Parser::peptide* p;

        aa_tab.add_all_AA();

        //AnalyseurPeptide::aa_tab.__print__();
        #if DEBUG & DEBUG_STATS 
        int nb_peptide = 0;
        for(int i=0;i<20;++i)
            for(int j=0;j<STATS_SIZE;++j)
                calc_stats[i][j] = 0;
        #endif

        #ifdef APPRENTISSAGE
        vector<AnalyseurPeptide*> liste_AP;
        IndividuTree::Node::max_indice = AnalyseurPeptide::VALUES_SIZE;

        const int pop_size = 1500;
        GenetiqueEngine<IndividuTree> engine(pop_size,TREE_INIT_PROONDEUR);

        #endif

        //parse le fichier
        while((p=parser.parse_next(ignore))!=NULL)
        {
            cout.precision(10);

            AnalyseurPeptide* analiser_pep=new AnalyseurPeptide(p,nb_affiche,error, trou_max);
            analiser_pep->resolve();

            #ifdef APPRENTISSAGE
            liste_AP.emplace_back(analiser_pep);
            #else
            delete analiser_pep;
            #endif

            #if DEBUG & DEBUG_STATS 
            nb_peptide++;
            #endif
        }

        #if DEBUG & DEBUG_STATS 
        for(int i=0;i<20;++i)
        {
            calc_stats[i][NB_SOL_FIND] /= nb_peptide;
            calc_stats[i][NB_SOL_FIND_OK] /= nb_peptide;
            calc_stats[i][CALC_TIME] /= nb_peptide;
            cout<<"pour "<<nb_peptide<<" peptides => "<<i+1<<" NB_SOL : "<<calc_stats[i][NB_SOL_FIND_OK]<<"/"<<calc_stats[i][NB_SOL_FIND]<<" en: "<<calc_stats[i][CALC_TIME]<<" secondes"<<endl;
        }
        #endif
     
        //destruction des analiser_pep 

        #ifdef APPRENTISSAGE
        bool (* condition_arret)(int score,decltype(liste_AP) l) = [](int score,decltype(liste_AP) l){return false;};
        IndividuTree* best = engine.run_while(condition_arret,pop_size*0.75,liste_AP);
        cout<<"best: "<<*best<<endl;
        delete best;

        auto end=liste_AP.end();
        for(auto i=liste_AP.begin(); i!= end; ++i)
            delete (*i);
        #endif

    }
    
    BDD_DESTROY()

    return 0;
};

void serveur(int argc,char* argv[])
{
};
