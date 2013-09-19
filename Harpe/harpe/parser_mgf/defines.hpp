#ifndef DEFINES_PARSER_HPP
#define DEFINES_PARSER_HPP

/**
 * \brief les différent états du parseur
 * Chaque opton corespond à une option du fichier .mgf
 *
 * \todo recoder le parseur avec yacc
 **/
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


extern int lex_lines;///< Pour compter les lignes du fichier

#ifdef COLORS

#define ROUGE  	"\033[31m" ///< couleur rouge pour le terminal
#define VERT   	"\033[32m" ///< couleur vert pour le terminal
#define JAUNE  	"\033[33m" ///< couleur jaune
#define BLEU   	"\033[34m" ///< couleur bleu
#define MAGENTA "\033[35m" ///< couleur magenta
#define LMAGENTA "\033[01;35m" ///<couleur magenta claire
#define BLEU2 "\033[36m" ///< couleur bleu
#define COMMENTAIRE "\033[31m" ///< couleur pour les commentaires
#define BLANC 	"\033[00m" ///< couleur par défaut du terminal

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

#define NULL_OUTPUT "/dev/null" ///< fichier de sortie pour la poubelle
#define MH 1.00794f ///< Masse d'un Hydrogène en Da
#define MO 15.99943f ///< masse d'un carbone en Da
#define MH2O (MO + 2*MH) ///< masse de H2O en Da

#define PERROR(x) std::cerr<<ROUGE<<__FILE__<<":"<<__LINE__<<" ERROR in line "<<lex_line<<": "<<x<<BLANC<<std::endl; ///< macro pour afficher où est l'erreur dans le fichier

#define MAX(x,y) (x>y?x:y) ///< fonction mathématique max 
#define MIN(x,y) (x<y?x:y) ///< fonction mathématique min

#endif
