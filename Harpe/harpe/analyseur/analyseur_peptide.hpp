#ifndef ANALYSEUR_PEPTIDE_HPP
#define ANALYSEUR_PEPTIDE_HPP

#include <utility>
#include <vector>
#include <list>
#include <stddef.h>
#include <iostream>
#include <memory>
#include <mutex>
#include "../aa_search/Enzyme.hpp"

//#include <tuple>
#include "../aa_search/AA_tab.hpp"
#include "../parser_mgf/parser.hpp"

/*#ifdef APPRENTISSAGE
#include "score_evo/IndividuTree.hpp"
#endif
*/
extern AA_Tab aa_tab;
extern double calc_score(const double* const values);

#if DEBUG & DEBUG_STATS
/**
 * \brief enumeration pour les index le calcule des statistiques de l'algo
 **/
enum STATS_ENUM{NB_SOL_FIND=0,NB_SOL_FIND_OK,HAS_SOLUTION,CALC_TIME,MAX_LEN,STATS_SIZE};

/**
 * \brief Stats calculé pour une analyse statistique des résultats en fonction du nombre de point de départ
 *
 * Note: Le 20 corespond au nombre de points de depard à tester
 **/
extern float calc_stats[20][STATS_SIZE];
#endif


//static enum {PEAK_INDEX=0,AA_INDEX,IS_CROISEMENT,ERREUR_MASSE} res_get_near_index;

class stack_token;

/**
 * \brief Classe permetant d'effectuer l'analyse des données récupéré du fichier MGF
 **/
class AnalyseurPeptide
{
    public:
        /**
         * \brief Construit un AnalyseurPeptide pour un peptite spécifique
         *
         * \param pep Le peptide à analyser
         * \param nb_affiche Le nombre de solutions à afficher
         * \param er Marger d'erreur autorisée
         * \param masse_max Masse maximum à combler théoriquement si aucun AA est trouvé (surtout pour les bords)
         * \param finds_max_size nombre maximum de solution à stoquer en mémoire
         **/ 
        AnalyseurPeptide(Parser::peptide* pep,int nb_affiche,const double er,double masse_max,int finds_max_size=-1);
        ~AnalyseurPeptide();

        /*
         * \brief Tente de trouver les séquences possible du peptide en partant du pic debut
         *
         * \param debut index du pic de dépard. Si == -1, alors les points de dépard seront automatiquement calculés
         **/
        void resolve(int debut=-1);
        
        /*
         * \todo "une fois le noyeau calculé, mettre pour chaque AA sa suretée (nombre de fois présente dans les saloution, si les peaks pris en cmpt changent), et fusionner plusieurs solutions quand on peut : AA-AA-trou + trou-AA-AA = AA-AA-trou?-AA-AA"
         * \todo "faire un dif entre 2 analyse (pep en + ou -) / stats généraux / truc de comparaison"
         **/

        //typedef std::tuple<int,int,bool,double> res_get_near ; //peak index / index aa_tab pour le AA trouvé / is_croisement (true) / erreur entre trouvé et théorique

        /**
         * \brief Permet de nommer le sens d'analyse pour plus de claretée
         **/
        enum Sens{LEFT=-1,STOP=0,RIGHT=1};


        /**
         * \brief Enumeration définissant les index des différents stats calculable pour le trie
         **/
        static enum {
            ERROR_TOTAL=0, ///< les erreur + et - se compensent
            ERROR_AA_CUMUL, ///< les erreur + et - se compensnt pas
            INTENSITEE_TOTAL_PARENT,///< intensitée total des intensitées
            INTENSITEE_TOTAL, ///< intensitée totale qui est utilisée
            MASSE_TOTAL,///< somme des masse des AA
            PERCENT_COUVERTURE,///< MASSE_TOTAL /100 * MASSE_PARENT 
            //PERCENT_COUVERTURE_AA,///< MASSE_TOTAL / masse moyenne AA
            NB_AA, ///< nombre de AA dans la chaine
            NB_PEAKS, ///< nombre de peaks dasn la chaine
            MASSE_PARENT, ///< masse du peptide
            PERCENT_INTENSITEE_UTILISEE,///< INTENSITEE_TOTAL /100 * INTENSITEE_TOTAL_PARENT  
            VALUES_SIZE ///< Nombre de valeurs
        } enum_values;

        /**
         * \brief Struct permetant de stoquer un états de l'analyse dans la pile
         **/
        class stack_token {
            public:
                /**
                 * \brief Constructeur par défaut pour les containeurs seulement
                 **/
                stack_token();

                /**
                 * \brief Constructeur par move
                 **/ 
                stack_token(stack_token&& other);

                /**
                 * \brief Construit un stack_token par copie
                 **/
                stack_token(const stack_token& other);

                /**
                 * \brief Constuit un PEAK_TOKEN
                 *
                 * \param i index du pic
                 * \param data pic concerné
                 **/
                stack_token(const int i,Parser::peptide::peak* data); //peak

                /**
                 * \brief Construit un AA_TOKEN
                 *
                 * \param i index de l'aa dans aa_tab
                 * \param e Erreur entre la masse cheché et la masse trouvée
                 * \param peak Pic où l'AA est trouvé
                 **/
                stack_token(const int i,const double e,AnalyseurPeptide::stack_token* peak); //aa

                /**
                 * \brief Construi une AA_HOLD_TOKEN 
                 *
                 * \param i innutilisé
                 * \todo sert à rien pour le moment
                 **/
                stack_token(const int i); //aa pour boucher un trou

                /**
                 * \brief Construit un HEADER_TOKEN
                 *
                 * \param score score de la liste
                 **/
                stack_token(const double score); //HEADER

                /**
                 * Destructeur
                 */
                ~stack_token();

                /**
                 * \brief Affectation
                 **/
                stack_token& operator=(const stack_token& other);
                /**
                 * \brief Affectation
                 * Note : Other as un status indéfinie apres cet appel
                 **/
                stack_token& operator=(stack_token&& other);


                /**
                 * \brief permet de récupéré de pic et de l'enlever du token
                 * Note : cette méthode est a appeler que sur les AA_TOKEN
                 *
                 * \return stack_token du pic associé
                 **/
                inline stack_token* get_peak_stack_NULL() {
                    stack_token* res = aa_token.pt_data;
                    aa_token.pt_data=NULL;
                    return res;
                };

                /**
                 * \brief Affiche le token pour du debug
                 **/
                void __print__() const;
                

                /**
                 * \brief Les différents types de token
                 **/
                enum Type {UNKNOW=-1, ///< Inconnus / ERREUR
                    AA_TOKEN=0, ///< acide aminé
                    AA_HOLD_TOKEN, ///< acide aminé théorique
                    /*AA_LIST,*/
                    PEAK_TOKEN, ///< Pic
                    HEADER_TOKEN ///< Stocke le score / liste des AA
                } type;

                /**
                 * \brief Permet de stoquer les informations des différent type de token
                 **/ 
                union {
                    struct  {
                        int index;
                        double error;
                        stack_token* pt_data;
                    } aa_token ///< AA_TOKEN;

                    struct{
                        double error;
                        std::vector<int> aa_index;
                    } aa_hold_token ///<AA_HOLD_TOKEN;

                    struct {
                        int index;
                        Parser::peptide::peak* pt_data;
                    } peak_token ///< PEAK_TOKEN;

                    struct {
                        double score;
                        struct _holds {
                            std::shared_ptr<std::list<list_arg> > link;
                            double to_find;
                        } holds [Parser::peptide::SIZE];
                        //double values[VALUES_SIZE];
                    } header_token ///< HEADER_TOKEN;
                };
        };

    typedef std::vector<AnalyseurPeptide::stack_token*> v_tokens_ptr; ///< Type utilisé pour stoqué les résultats
    typedef std::list<AnalyseurPeptide::stack_token*> pile_tokens_ptr; ///< Type utilisé pour la pile lors de l'analyse (pile double face)

    #ifdef APPRENTISSAGE
        friend class IndividuTree;
        /**
         * \brief Classe utilisé par l'algorithme génétique pour calculer les stats
         **/
        class ApprentissageSolution
        {
            public:

                /**
                 * \brief Constructeur
                 *
                 * \param s séquence d'AA trouvé
                 * \param pep Peptide conserné par la séquance
                 **/
                ApprentissageSolution(const AnalyseurPeptide::v_tokens_ptr& s,const Parser::peptide* const pep);

                /**
                 * \brief compare les score
                 **/
                bool operator>(AnalyseurPeptide::ApprentissageSolution other) const {return this->score > other.score;};

                /**
                 * \brief Affiche pour le debug
                 **/
                void __print__();

            protected:
                friend class IndividuTree;
                friend class AnalyseurPeptide;

                void check_validity(); ///< Regarde si la solution trouvée fait parti d'une possible

                std::string solution; ///< String stoquand la valeur de la ligne SOLUTION= dans le .mgf
                double values[VALUES_SIZE]; ///< tableau de stats pour le calcule du score
                double score; ///< score de la solution
                int real_score; ///< score attendu (pas la même echelle, mais permet de trie dasn l'ordre attendu)
                int validity;//< nombre de AA de la solution (0 si non valide au COMPLET !!) Juste pour les vérifications
                std::shared_ptr<std::vector<std::string> > solutions_reals; ///< solution attendu

        };
        std::vector<ApprentissageSolution> propositions; ///< proposition trouvées par l'algo

        std::mutex mutex; ///< Permet de bloquer les acces concurents
    #endif

    #if DEBUG & DEBUG_STATS
        void verifier_resultats_complet(std::vector<v_tokens_ptr>& s,int boucle); 
    #else
    void verifier_resultats_complet(std::vector<v_tokens_ptr>& s);
    #endif

        const Parser::peptide* get_peptide(){return pep;};
    private:


        static void calc_values(double* const values,const v_tokens_ptr& s,const Parser::peptide* pep);
        
        Parser::peptide* pep;

        std::vector<v_tokens_ptr> finds;
        std::vector<stack_token*> tokens_ptr;

        int nb_affiche;
        int finds_max_size;
        const double erreur, masse_max_trou;

        const int get_index_max_intensitee();
        //const int get_index_max_intensitee(const double max);
        const std::vector<int> get_index_max_intensitee_vector(const int nb);
        const v_tokens_ptr* get_near(const int index, const short int inc=1); //[<peak,AA]*
        const int depiler(pile_tokens_ptr& search,const int sens);

        void save_stack(const pile_tokens_ptr& search, std::list<v_tokens_ptr>& res);

        void print_stack_all(const pile_tokens_ptr& search);
        void print_stack_used(const pile_tokens_ptr& search);
        void print_results(const std::list<v_tokens_ptr>& res);
        void complet_solution(v_tokens_ptr& sol,int sens);

        #if DEBUG & DEBUG_STATS 
        void merge_solution(std::list<v_tokens_ptr >& left_part,const std::list<v_tokens_ptr>& right_part,int boucle); //met tout dasn left
        #else 
        void merge_solution(std::list<v_tokens_ptr >& left_part,const std::list<v_tokens_ptr>& right_part); //met tout dasn left
        #endif
        
        void filter_enzyme(std::list<v_tokens_ptr>& solutions,Enzyme& enz);

        void print_AA(const v_tokens_ptr& v,int sens=Sens::STOP,bool p=true);
        static std::string to_string(const v_tokens_ptr& v);
};

#endif
