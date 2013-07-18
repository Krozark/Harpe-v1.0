#ifndef PARSER_HPP
#define PARSER_HPP

#include <iostream>
#include <fstream>
#include <vector>
#include <memory>

#include <algorithm>
#include "defines.hpp"

extern int yylex();
extern char* yytext;
extern FILE* yyin;
extern FILE* yyout;

#define DEBUG_NO 0
#define DEBUG_FULL 0xFFFF

#define DEBUG_MGF 1<<1
#define DEBUG_SOLUTION 1<<2
#define DEBUG_APPRENTISSAGE 1<<3
#define DEBUG_CALC 1<<4
#define DEBUG_STACK 1<<5
#define DEBUG_COMP_SOL 1<<6
#define DEBUG_MERGE 1<<7
#define DEBUG_FILTER 1<<8
#define DEBUG_VERIFICATION 1<<9
#define DEBUG_STATS 1<<10
#define DEBUG_APPRENTISSAGE_STATS 1<<11
#define DEBUG_MGF_PEAKS_START 1<<12
#define DEBUG_STATE_RUNNING 1<<13

#define DEBUG_PEAKS 0

#define GENETIQUE_SAVE_RESULTS  1

#ifndef APPRENTISSAGE
#define DEBUG (DEBUG_NO | DEBUG_STATE_RUNNING)
#else
#define DEBUG (DEBUG_APPRENTISSAGE_STATS)
#endif

#define COMPLET_SOLUTION 0
#define FILTER_SOLUTION 0

class AA_Tab;
extern AA_Tab aa_tab;
//class AA;
class Parser
{
    public:
        class peptide;

        Parser(const char* filename);
        ~Parser();

        void parse_all(bool ignore = false);
        Parser::peptide* parse_next(bool ignore = false);
        void __print__() const;
        static inline const double get_masse_for(const double mz,const unsigned int charge){return mz*charge - charge*MH;};
        
        class peptide 
        {
            public :
                class peak;
                peptide (const double mz,const unsigned int intensitee,const char charge=0);
                ~peptide();

                double masse,mz;
                char charge;
                double intensitee;//,seuil;
                std::string title;
                std::vector<peak*> peaks;
                #ifdef APPRENTISSAGE
                std::shared_ptr<std::vector<std::string> > solutions;
                #endif

                enum {DEBUT=0,DEBUT_H2O,FIN_H2O,FIN,SIZE} special_peaks_keys;
                peak* special_peaks[SIZE];
                //peak* peak_h2o;

                void __print__() const;
                void add(const double masse,const unsigned int intensitee,const char charge=0);
                void add_firsts();
                void add_lasts();
                inline void calc_masse() {masse = mz*charge - charge* MH;};
                void normalize_intensitee();
                void calc_masse_peaks();
                void normalize_and_calc_masse();
                //void calc_seuil();
                //void set_seuil(const double s);
                //void set_seuil();

                bool is_one_of_special(peak* p) const;
                bool is_one_of_firsts(peak* p) const;
                bool is_one_of_lasts(peak* p) const;
                bool is_one_of_h2o(peak* p) const;

                void sort();
                
            //private:

                class peak
                {
                    public :
                        peak(const double mz,const unsigned int intesitee,const char charge=0);
                        peak(const Parser::peptide::peak& other);
                        ~peak(); //pour affecter original à false

                        double mz, intensitee, masse;
                        bool /*bruit,*/
                             *used;
                        char charge;
                        //int index; // pour pouvoir acceder à son index depuis son pointeur

                        void __print__() const;
                        void calc_masse(peptide* parent);
                        inline void calc_masse(){masse = mz*charge - charge* MH;};
                        bool operator < (const peak& other) const {return this->masse < other.masse;};
                    //private :
                        bool originale; //juste pour la libération complette de la mémoire
                };
        };

    private:
        std::vector<peptide*> peptides;
        enum states{OUT_PEPTIDE=0,/* commentaire, tous les truc qui sont hors des BEGIN IONS */
            IN_PEPTIDE /* entre BEGIN IONS et END IONS */
        } state;

        FILE* file;
        FILE* null_output;



};

#endif
