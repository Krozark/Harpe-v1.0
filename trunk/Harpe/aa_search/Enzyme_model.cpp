#include "../sql/SqlObject.hpp"

#include "Enzyme.hpp"
#include <string.h>

/* Enzyme */
const std::string Enzyme::TableName("website_enzyme");

Enzyme::Enzyme(sql::ResultSet* query)
{
    //base
    this->pk = query->getInt("id");
    this->name = query->getString("name");

    //coupure_avant
    bdd_res = bdd->executeQuery("select * from website_enzyme_cut_before where enzyme_id = "+std::to_string(pk)+";");
    while(bdd_res->next())
    {
        int id;
        if((id = aa_tab.get_pk(bdd_res->getInt("aa_id"))) != -1)
            coupure_avant.emplace_back(id);
    }
     delete bdd_res;
    //coupure_apres
    bdd_res = bdd->executeQuery("select * from website_enzyme_cut_after where enzyme_id = "+std::to_string(pk)+";");
    while(bdd_res->next())
    {
        int id;
        if((id = aa_tab.get_pk(bdd_res->getInt("aa_id"))) != -1)
            coupure_apres.emplace_back(id);
    }
     delete bdd_res;
    //coupure_impossibles
    bdd_res = bdd->executeQuery("select ic.id, ic.first_id, ic.second_id from  website_enzyme ez\
                                 inner join website_enzyme_cut_imposible ez_ic\
                                 on (ez.id = ez_ic.enzyme_id)\
                                 inner join website_impossiblecut ic\
                                 on (ic.id = ez_ic.impossiblecut_id)\
                                 where ez.id = "+std::to_string(pk)+";"
                                 );

    while(bdd_res->next())
    {
        coupure_impossibles.emplace_back(ImpossibleCut(bdd_res));
    }
     delete bdd_res;
};

void Enzyme::save(bool commit)
{
#warning "TODO"
    if (commit)
    {//insert
    }
    else
    {//update
    }
};

/* ImpossibleCut */

const std::string Enzyme::ImpossibleCut::TableName("website_impossiblecut");

Enzyme::ImpossibleCut::ImpossibleCut(sql::ResultSet* query)
{
    this->pk = query->getInt("id");
    this->first = aa_tab.get_pk(query->getInt("first_id"));
    this->second= aa_tab.get_pk(query->getInt("second_id"));
};

void Enzyme::ImpossibleCut::save(bool commit)
{
    if(commit)
    {
       insert("first_id,second_id",this->first,this->second);
    }
    else
    {
        update("first_id="+std::to_string(first),"second_id="+std::to_string(second));
    }    
};

