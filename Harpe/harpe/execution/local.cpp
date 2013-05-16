#include "../parser_mgf/parser.hpp"
#include "../analyseur/analyseur_peptide.hpp"
#include "../score_evo/Engine/src/random.hpp"
#include "../sql/config.hpp"
#include "local.hpp"

#ifdef APPRENTISSAGE
#include "../score_evo/IndividuTree.hpp"
#include "../score_evo/Engine/src/GeneticEngine.hpp"
#endif

using namespace std;

///////////// FONCTIONS //////////////////////////
int local(int argc, char* argv[])
{
    if (argc < 2)
        SHOW_ARGS_NOMAL("pas assez d'arguments")

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
                    SHOW_ARGS_NOMAL("Pas de nom de fichier")
            }
            else if (arg == "-e")
            {
                if(++i <argc)
                    error = atof(argv[i]);
                else
                    SHOW_ARGS_NOMAL("Pas de nombre")
            }
            else if (arg == "-l")
            {
                if(++i <argc)
                    nb_affiche = atoi(argv[i]);
                else
                    SHOW_ARGS_NOMAL("Pas de nombre")
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
                    SHOW_ARGS_NOMAL("Pas de nombre")
            }
            else if(arg == "-bdd")
            {
                if(++i <argc)
                    s_bdd = argv[i];
                else
                    SHOW_ARGS_NOMAL("Pas de base de donnée spécifiée")
            }
            else if(arg == "-pass")
            {
                if(++i <argc)
                    s_password = argv[i];
                else
                    SHOW_ARGS_NOMAL("Pas de mot de pass spécifiée")
            }
            else if(arg == "-user")
            {
                if(++i <argc)
                    s_username = argv[i];
                else
                    SHOW_ARGS_NOMAL("Pas de nom d'utilisateur spécifiée")
            }
            else if(arg == "-host")
            {
                if(++i <argc)
                    s_server = argv[i];
                else
                    SHOW_ARGS_NOMAL("Pas d'adresse ip spécifiée")
            }
            else if(arg == "-port")
            {
                if(++i <argc)
                    s_port = argv[i];
                else
                    SHOW_ARGS_NOMAL("Pas de port spécifié")
            }
            ++i;
        }
    }

    if(filepath == "")
        SHOW_ARGS_NOMAL("Un fichier mgf est requis");

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

        int pop_size = 10000;
        float mutation_taux = 0.01;
        int nb_threads = -1;
        int pop_child = 1000;
        int profondeur_init = TREE_INIT_PROONDEUR;
        GeneticEngine<IndividuTree> engine(nb_threads,mutation_taux,"res",pop_size,pop_child,profondeur_init);
        engine.setTimeout(10000);

        //engine.setCreationMode(GeneticEngine<IndividuTree>::CreationMode::STUPIDE);
        //engine.setReductionMode(GeneticEngine<IndividuTree>::ReductionMode::STUPIDE);


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
        bool (* condition_arret)(const IndividuTree& score,const int generation) = [](const IndividuTree& best,const int generation)
        {
            return false;
        };
        IndividuTree::pep_to_test = &liste_AP;
        IndividuTree* best = engine.run_while(condition_arret);
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

