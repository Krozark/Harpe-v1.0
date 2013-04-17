#include <stdlib.h>
#include "AA_tab.hpp"

using namespace std;
#define ERROR 0.01f

#define MH 1.00794
#define MO 15.99943
#define MH2O MO + 2*MH

#define PARAMS_ERROR(x) {cerr<<x<<endl<<"Les argmuments sont:\n -f MASSE (masse a chercher),\n -r CHAINE (donne la masse de la chaine),\n -e ERROR (specifie la marge d'erreur [0.01 par defaut]),\n -m (utiliser les valeur moyenne),\n -h2o (enleve la masse de h2o),\n-H2O (ajouts la masse de  h2o),\n -l LILMITE (limiter l'affichage),\n -d CODE (soustraire la masse de l'AA CODE(S) à MASSE),\n -D (soustraire la masse de touts les AA à MASSE individuelement),\n -a CODE (ajouter la masse de CODE(S) à MASSE),\n -A (ajouter la masse de touts les AA individuelement à MASSE),\n -delete CODE(S) (supprime toute les solutions qui contiennent les/les AA(s)),\n -u MASSE (enleve la MASSE a celle de la recherche),\n -U MASSE (ajoute la masse à celle de la recherche)"<<endl;return 0;}

enum param {
        REVERSE=1,
        MOYENNE=1<<2,
        DELETE_MASSE=1<<3,
        ADD_MASSE=1<<4,
        DELETE_LIST=1<<5
    };

int main(int argc, char* argv[])
{
    if (argc < 2)
        PARAMS_ERROR("Pas assez de parametres")

    AA_Tab tab;
    unsigned int params=0;
    int i=1;
    double to_find=0;
    float error = ERROR;
    int index_chaine=0;
    int limite = 0;
    while(i<argc)
    {
        //cout<<"<"<<argv[i]<<">"<<endl;
        if (string(argv[i])== "-f")
        {
            if(argc >= ++i)
                to_find += atof(argv[i++]);
            else
                PARAMS_ERROR("pas de masse trouvé")
        }
        else if (string(argv[i])== "-u")
        {
            if(argc >= ++i)
                to_find -= atof(argv[i++]);
            else
                PARAMS_ERROR("pas de masse trouvé")
        }
        else if (string(argv[i])== "-U")
        {
            if(argc >= ++i)
                to_find += atof(argv[i++]);
            else
                PARAMS_ERROR("pas de masse trouvé")
        }
        else if (string(argv[i]) == "-r")
        {
            if(argc >= ++i)
            {
                params |= REVERSE;
                index_chaine=i++;
            }
            else
                PARAMS_ERROR("pas de chaine trouvé")
        }
        else if (string(argv[i]) == "-e")
        {
            if(argc >= ++i)
                error = atof(argv[i++]);
            else
                PARAMS_ERROR("pas d'erreur trouvée");
        }
        else if (string(argv[i]) == "-l")
        {
            if(argc >= ++i)
                limite = atoi(argv[i++]);
            else
                PARAMS_ERROR("pas de limite trouvée");
        }
        else if (string(argv[i]) == "-m")
        {
            params |= MOYENNE;
            ++i;
        }
        else if (string(argv[i]) == "-h2o")
        {
            to_find -= MH2O;
            ++i;
        }
        else if (string(argv[i]) == "-H2O")
        {
            to_find += MH2O;
            ++i;
        }
        else if (string(argv[i]) == "-d")
        {
            if(argc >= ++i)
            {
                params |= DELETE_MASSE;
                index_chaine = i++;
            }
            else
                PARAMS_ERROR("pas de nom trouvé")
        }
        else if (string(argv[i]) == "-D")
        {
            params|=DELETE_MASSE;
            ++i;
        }
        else if (string(argv[i]) == "-a")
        {
            if(argc >= ++i)
            {
                params |= ADD_MASSE;
                index_chaine = i++;
            }
            else
                PARAMS_ERROR("pas de nom trouvé")
        }
        else if (string(argv[i]) == "-A")
        {
            params|=ADD_MASSE;
            ++i;
        }
        else if (string(argv[i]) == "-delete")
        {
            params|=DELETE_LIST;
            if(argc >= ++i)
                index_chaine = i++;
            else
                PARAMS_ERROR("pas de noms trouvé")
        }
        else
            PARAMS_ERROR("mauvais argument")
    }
        

    if (params & MOYENNE)
        tab.add_all_AA_moy();
    else
        tab.add_all_AA();

    cout.precision(15);

    if (params & REVERSE)
    {
        cout<<"masse trouvé: "<<tab.get_masse_for(argv[index_chaine],to_find)<<endl;
    }
    else if (to_find > 0 && params & DELETE_MASSE)
    {
        if (index_chaine)
            tab.delete_masse_of(to_find,argv[index_chaine]);
        else
            tab.delete_masse_all(to_find);
    }
    else if (to_find > 0 && params & ADD_MASSE)
    {
        if (index_chaine)
            tab.add_masse_of(to_find,argv[index_chaine]);
        else
            tab.add_masse_all(to_find);
    }
    else if (to_find > 0)
    {
        cout<<"a trouver:"<<to_find<<" avec erreur:"<<error<<endl;

        std::list< list_arg > res = tab.resolve(to_find,error);
        if (params & DELETE_LIST)
            tab.constrain_remove_all(res,argv[index_chaine]);
        res.sort();
        AA_Tab::print(res,limite);
        cout<<"nombre de solutions: "<<res.size()<<endl;
    }
    
    return 0;
};
