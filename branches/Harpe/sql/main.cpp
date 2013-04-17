#include "config.hpp"

using namespace std;

int main()
{
    BDD_DECLARE();
    BDD_INIT();

    bdd_res = bdd->executeQuery("show tables");

	while (bdd_res->next())
	{
		cout << bdd_res->getString(1) << endl;                
	}
 
	// Clean up after ourselves
    BDD_DESTROY();
 
	return 0;
}
