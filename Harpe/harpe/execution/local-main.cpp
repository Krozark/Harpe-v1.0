#include <cstdlib>
#include <ctime>

#include "local.hpp"

using namespace std;

BDD_DECLARE();
AA_Tab aa_tab = AA_Tab();

#if DEBUG & DEBUG_STATS
float calc_stats[20][STATS_SIZE];
#endif

int main(int argc, char* argv[])
{
    return normal(argc,argv);
};
