#include <stdlib.h>
#include "parser.hpp"

using namespace std;

int main(int args, char* argv[])
{
    Parser p("file_test.mgf");
    p.parse_all();
    cout.precision(10);
    p.__print__();
    return 0;
};
