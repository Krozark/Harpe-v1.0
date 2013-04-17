#include "Enzyme.hpp"
#include "AA_tab.hpp"

using namespace std;

Enzyme::Enzyme(std::string n) : name(n)
{
    bdd_res = bdd->executeQuery("Select * from "+TableName+" where (`name` = \""+ n+"\") limit 1;");

    auto q = bdd_res;
    bdd_res->next();
    *this = Enzyme(bdd_res);
    delete q;
};

void Enzyme::__print__()
{

    cout<<name<<": Coupure avant"<<endl;
    for(int i: coupure_avant)
        aa_tab.AAs[i].__print__();
    cout<<name<<": Coupure apres"<<endl;
    for(int i: coupure_apres)
        aa_tab.AAs[i].__print__();

    cout<<endl<<"coupures impossibles"<<endl;
    for(ImpossibleCut& v:coupure_impossibles)
    {
        v.__print__();
    }
    cout<<endl;
};


/* ImpossibleCut */

void Enzyme::ImpossibleCut::__print__()
{
    cout<<"first: "<<aa_tab[aa_tab.get_pk(this->first)]<<" second: "<<aa_tab[aa_tab.get_pk(this->first)]<<endl;
};
