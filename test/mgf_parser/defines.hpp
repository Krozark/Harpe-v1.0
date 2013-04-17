#ifndef DEFINES_HPP
#define DEFINES_HPP

enum lex_states{
    OPTION_HEADER=0,
    OPTION_LOCAL,
    OPTION_ALL,
    BEGIN_PEP,
    END_PEP,
    PEAK_T,
    PEAK_D,
    AFF_PEAK,
    PASS,
    CHARGE,
    PEPMASS,
    AFF_VALUE,
    CONST_INT,
    CONST_FLOAT,
    CONST_CHARGE,
    UNKNOW
};

#define ROUGE  	"\033[31m"
#define VERT   	"\033[32m"
#define JAUNE  	"\033[33m"
#define BLEU   	"\033[34m"
#define MAGENTA "\033[35m"
#define BLEU2 "\033[36m"
#define COMMENTAIRE "\033[31m"
#define BLANC 	"\033[00m"

#define NULL_OUTPUT "/dev/null"

#define MH 1.00794f

#define PERROR std::cerr<<ROUGE<<"ERROR"<<BLANC<<std::endl;

#define MAX(x,y) (x>y?x:y)
#define MIN(x,y) (x<y?x:y)

#endif
