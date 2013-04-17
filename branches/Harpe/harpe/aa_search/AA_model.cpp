#include "../sql/SqlObject.hpp"

#include "AA.hpp"
#include <string.h>

using namespace std;

const std::string AA::TableName("website_aa");

AA::AA(sql::ResultSet* query)
{
    this->pk = query->getInt("id");
    this->id=0;
    this->masse= query->getDouble("mass");
    strncpy(this->name,query->getString("name").c_str(),255);
    strncpy(this->slug,query->getString("slug").c_str(),16);
};

void AA::save(bool commit)
{
    if (commit)
    {//insert
        insert("mass, name, slug",masse,name,slug);
    }
    else
    {//update
        update(string("mass=")+to_string(masse), string("name=")+name, string("slug=")+slug);
    }
};

