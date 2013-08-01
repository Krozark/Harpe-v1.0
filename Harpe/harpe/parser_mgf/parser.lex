
%{

#include <stdio.h>
#define YY_NO_INPUT
#include "defines.hpp"

extern int lex_line;


%}

%option nounput
%option noyywrap


%%

\n {++lex_line;}

#.* {}

^"BEGIN IONS" {
    return BEGIN_PEP;
}


^"END IONS" {
    return END_PEP;
}


ACCESSION|CLE|COM|CUTOUT|DB|DECOY|ERRORTOLERANT|FORMAT|FRAMES|MASS|ITOL|MODS|MULTI_SITE_MODS|PEP_ISOTOPE_ERROR|PFA|PRECURSOR|QUANTITATION|REPORT|REPTYPE|SEARCH|SEG|TAXONOMY|USER[0-9][0-9]|USEREMAIL|USERNAME {
    return OPTION_HEADER;
}

INSTRUMENT|IT_MODS|ITOLU|TOL|TOLU {
    return OPTION_ALL;
}

COMP|ETAG|LOCUS|RAWFILE|RAWSCANS|RTINSECONDS|SCANS|SEQ|TAG {
    return OPTION_LOCAL;
}

TITLE {
    return TITLE;
}


PEPMASS {
    return PEPMASS;
}

SOLUTION {
    return SOLUTION;
}

CHARGE {
    return CHARGE;
}

=[0-9]+\.[0-9]+[[:space:]][0-9]+[[:space:]]([1-9][+-])? {
    return AFF_PEAK;
}

=[^\n\r]+ {
    return AFF_VALUE;
}

[0-9]+\.[0-9]+[[:space:]][0-9]+[[:space:]][1-9][+-] {
    return PEAK_T;
}

[0-9]+\.[0-9]+[[:space:]][0-9]+[[:space:]] {
    return PEAK_D;
}


[0-9]+[+] {
    return CONST_CHARGE;
}

[0-9]+ {    // const int
    return CONST_INT;
}

[0-9]+[.][0-9]+ {
    return CONST_FLOAT;
}

[[:space:]] {}

%%

