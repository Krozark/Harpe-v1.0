#ifndef DEFINES_PARSER_HPP
#define DEFINES_PARSER_HPP

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
    TITLE,
    CHARGE,
    PEPMASS,
    AFF_VALUE,
    CONST_INT,
    CONST_FLOAT,
    CONST_CHARGE,
    SOLUTION,
    UNKNOW
};

extern int lex_lines;

#ifdef COLORS

#define ROUGE  	"\033[31m"
#define VERT   	"\033[32m"
#define JAUNE  	"\033[33m"
#define BLEU   	"\033[34m"
#define MAGENTA "\033[35m"
#define LMAGENTA "\033[01;35m"
#define BLEU2 "\033[36m"
#define COMMENTAIRE "\033[31m"
#define BLANC 	"\033[00m"

#else

#define ROUGE  	""
#define VERT   	""
#define JAUNE  	""
#define BLEU   	""
#define MAGENTA ""
#define LMAGENTA ""
#define BLEU2 ""
#define COMMENTAIRE ""
#define BLANC 	""

#endif

#define NULL_OUTPUT "/dev/null"
#define MH 1.00794f
#define MO 15.99943f
#define MH2O (MO + 2*MH)

#define PERROR(x) std::cerr<<ROUGE<<__FILE__<<":"<<__LINE__<<" ERROR in line "<<lex_line<<": "<<x<<BLANC<<std::endl;

#define MAX(x,y) (x>y?x:y)
#define MIN(x,y) (x<y?x:y)

#endif
