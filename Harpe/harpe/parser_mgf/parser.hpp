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

/*************** DEBUG VALUES ***********/
#define DEBUG_NO 0 ///<  pas de debug
#define DEBUG_FULL 0xFFFF ///< tous les debug

#define DEBUG_MGF 1<<1 ///< affiche le mgf et se qui est lié
#define DEBUG_SOLUTION 1<<2 ///< debug la construction de la solution
#define DEBUG_APPRENTISSAGE 1<<3 ///< debug l'algo génétique
#define DEBUG_CALC 1<<4 ///< débug le calcul des stats
#define DEBUG_STACK 1<<5 ///< débug la pile de recherche
#define DEBUG_COMP_SOL 1<<6 ///< débug sql
#define DEBUG_MERGE 1<<7 ///< débuge la fusion des solutions
#define DEBUG_FILTER 1<<8 ///< débug la réduction des solutions avec l'enzyme
#define DEBUG_VERIFICATION 1<<9 ///< débug la vérification automatique des résultats
#define DEBUG_STATS 1<<10 ///< la construction des stats des solutions
#define DEBUG_APPRENTISSAGE_STATS 1<<11 ///< affiche les proposition pour l'algo génétique
#define DEBUG_MGF_PEAKS_START 1<<12 ///< affiche les poinhts de dépad choisis
#define DEBUG_STATE_RUNNING 1<<13 ///< affiche ou en est l'execution
/***************** PRINT VALUES *******************/
#define PRINT_NO 0 ///< pas d'affichae particulier
#define PRINT_SCORE 1<<1 ///< affiche le score des header des solution
#define PRINT_FINDS 1<<2 ///< affiche les solution trouvées

/****************** VALUES *********************/
#define GENETIQUE_SAVE_RESULTS  1 ///< save les résultats de l'algo génétique à chaque meilleur individu
#define DEBUG_PEAKS 0 ///< \todo je sais plus se que ca fait
#define COMPLET_SOLUTION 0 ///< complete les trous
#define FILTER_SOLUTION 0 ///< filter avec l'enzyme

#define PRINT (PRINT_NO) ///< défini Print

#ifndef APPRENTISSAGE
//#define DEBUG (DEBUG_NO | DEBUG_STATE_RUNNING)
//#define DEBUG (DEBUG_NO | DEBUG_MGF)
#define DEBUG (DEBUG_NO ) ///< définis le niveau de débug
#else
//#define DEBUG (DEBUG_APPRENTISSAGE_STATS)
#define DEBUG (DEBUG_STATS |  DEBUG_MGF ) ///< défini le niveau de debug
#endif


class AA_Tab;
extern AA_Tab aa_tab;
//class AA;
/**
 * \brief Classe permetant de parser le fichier mgf et de récupérer les infos
 **/
class Parser
{
    public:
        class peptide;

        /**
         * \brief construit le parseur et analyse le fichier filname
         *
         * \param filname Le nom du fichier à analyser
         **/
        Parser(const char* filename);
        ~Parser();

        /**
         * \brief parse le fichier en entier
         **/
        void parse_all(bool ignore = false);

        /**
         * \brief Parse le prochain peptide
         *
         * \result le peptide parsé
         **/
        Parser::peptide* parse_next(bool ignore = false);

        /**
         * \brief Permet de debug
         **/
        void __print__() const;

        /**
         * \brief permet de comvertir un rapport m/z en dalton
         *
         * \param mz le rapport m/z
         * \param chague la charque (z)
         *
         * \result la masse calculée
         **/
        static inline const double get_masse_for(const double mz,const unsigned int charge){return mz*charge - charge*MH;};
        
        /**
         * \brief Classe permetant de stoquer les peptides
         **/
        class peptide 
        {
            public :
                class peak;
                /**
                 * \brief Constriut un peptide avec son m/z, intensitee et charge
                 *
                 * \param mz le rapport masse/charge
                 * \param intensitee l'itensitée du peptide
                 * \param charge La chage du peptide
                 **/
                peptide (const double mz,const unsigned int intensitee,const char charge=0);
                ~peptide();

                double masse, ///< la masse du peptide
                       mz; ///< le rappord masse / charge
                char charge; ///< la charge du peptide
                double intensitee;///< l'intensitée (inutilisé)
                std::string title;///< le nom du peptide (pour l'utilisateur uniquement)
                std::vector<peak*> peaks; ///< la liste des pics

                //#if (DEBUG & DEBUG_STATS) || APPRENTISSAGE
                std::shared_ptr<std::vector<std::string> > solutions;///< solutions (SOLUTION=X|Y|Z,Y|E|K)
                //#endif

                ///< les pic artificiels spéciaux (début / fin ...)
                enum {DEBUT=0, ///< masse = 0
                    DEBUT_H2O, ///< masse = MH2O
                    FIN_H2O, ///< masse = fin - MH2O
                    FIN, ///< mass = tout
                    SIZE} special_peaks_keys;
                peak* special_peaks[SIZE]; ///< les pics artificiels
                //peak* peak_h2o;

                /**
                 * \brief Pour le debug Affiche le peptide (mgf)
                 **/
                void __print__() const;

                /**
                 * \brief Ajoute un pic au peptide
                 **/
                void add(const double masse,const unsigned int intensitee,const char charge=0);

                /**
                 * \brief ajoute les pic de synthèse de debut des fichier (DEBUT_XX)
                 **/
                void add_firsts();

                /**
                 * \brief ajoute les pics de synthèse de fin des fichier (FIN_XXX)
                 **/
                void add_lasts();

                /**
                 *  \brief calcule ma masse du peptide ap partir de son m/z et charge
                 **/
                inline void calc_masse() {masse = mz*charge - charge* MH;};

                /**
                 * \brief Normalize les intensitée entre 0 et 1.
                 * 1 sera l'intensité du pic le plus intense
                 **/
                void normalize_intensitee();

                /**
                 * \brief calcule la masse des pic
                 **/
                void calc_masse_peaks();

                /**
                 * \brief normalize les pic et malcul leur masse
                 **/
                void normalize_and_calc_masse();
                //void calc_seuil();
                //void set_seuil(const double s);
                //void set_seuil();

                bool is_one_of_special(peak* p) const; ///< true si p fait partie de special_peaks
                bool is_one_of_firsts(peak* p) const;///< true si p est un DEBUT_XXX
                bool is_one_of_lasts(peak* p) const;///< true si p est un FIN_XXX
                bool is_one_of_h2o(peak* p) const;///< true si p est XXX_H2O

                void sort(); ///<trie les pic par masse croisante
                
            //private:

                /**
                 * \brief Classe permetant de stocker les pic
                 **/
                class peak
                {
                    public :
                        /**
                         * \brief Construit un pic
                         *
                         * \param mz son rapport m/z
                         * \param intensitee son intensitee
                         * \param charge ca charge
                         **/
                        peak(const double mz,const unsigned int intesitee,const char charge=0);

                        /**
                         * \brief Constructeur par copie
                         **/
                        peak(const Parser::peptide::peak& other);
                        ~peak(); //pour affecter original à false

                        double mz, ///< rapport mase / charge
                               intensitee,///< intensitee
                               masse; ///< masse
                        bool /*bruit,*/
                        /**
                         * \brief si le pic est utilisé.
                         * Note : le pointeur set à utiliser le pic qu'un seul fois pour toutes les instance (différent taux de charge) de celui ci
                         **/
                             *used;
                        char charge; ///< la charge du pic
                        //int index; // pour pouvoir acceder à son index depuis son pointeur

                        /**
                         * \brief Permet d'affichier le pic
                         **/
                        void __print__() const;

                        /**
                         * \brief Calcule la masse du pic
                         * Note : Si la charge est de 0, toutes les charge possible sont considéré et ajouté au peptide
                         **/
                        void calc_masse(peptide* parent);

                        /**
                         * \brief Calcule la masse du pic
                         **/
                        inline void calc_masse(){masse = mz*charge - charge* MH;};

                        /**
                         * \brief Permet de trier par masse
                         **/
                        bool operator < (const peak& other) const {return this->masse < other.masse;};
                    //private :
                        bool originale; ///< juste pour la libération complette de la mémoire (pour *used)
                };
        };

    private:
        std::vector<peptide*> peptides; ///< liste des peptides associés

        /**
         * \brief définie les états du parseur
         **/
        enum states{
            OUT_PEPTIDE=0, ///< commentaire, tous les truc qui sont hors des BEGIN IONS
            IN_PEPTIDE ///< entre BEGIN IONS et END IONS
        } state;

        FILE* file; ///< fichier à analyser
        FILE* null_output; ///< fichier poubelle

};

#endif
