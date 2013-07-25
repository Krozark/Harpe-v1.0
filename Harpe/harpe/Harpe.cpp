//#include <cstdlib>
//#include <ctime>
#ifndef APPRENTISSAGE
#include "fonction_finds/408.hpp"
#endif

#include "parser_mgf/parser.hpp"
#include "analyseur/analyseur_peptide.hpp"

//#include "execution/serveur.hpp"
#include "execution/local.hpp"
//#include "execution/difference_masse_parent.hpp"

#if DEBUG & DEBUG_STATS
float calc_stats[20][STATS_SIZE];
#endif

using namespace std;

BDD_DECLARE();

AA_Tab aa_tab = AA_Tab();


int main(int argc, char* argv[])
{
    return local(argc,argv);
};


