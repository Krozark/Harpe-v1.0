#include "AA.hpp"
#include <string.h>


AA::AA(const char* name,const char id, double masse)
{
    this->id=id;
    this->masse=masse;
    strncpy(this->name,name,255);
    strncpy(this->slug,name,16);
};


AA::AA(const char* name,const char* slug,const char id, double masse)
{
    this->id=id;
    this->masse=masse;
    strncpy(this->name,name,255);
    strncpy(this->slug,slug,16);
};

AA::AA()
{
    this->id= 0;
    this->masse=0;
};

AA::~AA()
{
    //  delete name;
};

std::ostream& operator<<(std::ostream& output,const AA& p)
{
    output<<p.slug;
    return output;
};

