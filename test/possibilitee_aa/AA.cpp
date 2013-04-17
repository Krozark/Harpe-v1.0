#include "AA.hpp"
#include <string.h>

AA::AA(const char* name,const char id, double masse)
{
    this->id=id;
    this->masse=masse;
    strncpy(this->name,name,255);
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


