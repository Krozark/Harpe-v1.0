#ifndef PARSER_HPP
#define PARSER_HPP

#include <iostream>
#include <fstream>
#include <vector>
#include "defines.hpp"

extern int yylex();
extern char* yytext;
extern FILE* yyin;
extern FILE* yyout;

class Parser
{
    public:
        Parser(const char* filename);
        ~Parser();

        void parse_all();
        void __print__();
        static inline const double get_masse_for(const double mz,const unsigned int charge){return mz*charge - charge*MH;};

    private:
        class peptide 
        {
            class peak;
            public :
                peptide (const double m,const unsigned int i,const char c=0);

                double masse,mz;
                char charge;
                double intensitee;
                std::vector<peak> peaks;

                void __print__();
                void add(const double masse,const unsigned int intensitee,const char charge=0);
                inline void calc_masse(){masse = mz*charge - charge* MH;};
                void normalize_intensitee();
                void calc_masse_peaks();
                void normalize_and_calc_masse();
                
            private:

                class peak
                {
                    public :
                        peak(const double mz,const unsigned int intesitee,const char charge=0);

                        double mz, intensitee;
                        char charge;
                        std::vector<double> masse;

                        void __print__();
                        void calc_masse(const unsigned int parent_charge,const double parent_masse);
                        //TODO get_possibilitee => avoir toute les possibilitée pour une charge/masse max donne (celle de l'ion parent)
                };
        };

        std::vector<peptide*> peptides;
        enum states{OUT_PEPTIDE=0,/* commentaire, tous les truc qui sont hors des BEGIN IONS */
            IN_PEPTIDE /* entre BEGIN IONS et END IONS */
        } state;

        FILE* file;
        FILE* null_output;

        Parser::peptide* parse_next();
};

#endif
