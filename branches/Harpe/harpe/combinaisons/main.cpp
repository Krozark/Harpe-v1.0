#include "Combinaison.hpp"
#include <stdlib.h>
#include <stdio.h>

int main(int argc,char * argv[])
{
    if (argc > 1)
    {
        Combinaison::returnType* res = Combinaison::getOrGenFor(atoi(argv[1]));
        Combinaison::__print__(*res);
        Combinaison::clear();
    }
    else
        printf("manque le nombre en param\n");

    return 0;
}
