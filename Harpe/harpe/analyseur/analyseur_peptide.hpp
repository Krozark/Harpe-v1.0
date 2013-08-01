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
enum STATS_ENUM{NB_SOL_FIND=0,NB_SOL_FIND_OK,HAS_SOLUTION,CALC_TIME,MAX_LEN,STATS_SIZE};
extern float calc_stats[20][STATS_SIZE];
#endif


//static enum {PEAK_INDEX=0,AA_INDEX,IS_CROISEMENT,ERREUR_MASSE} res_get_near_index;

class stack_token;

class AnalyseurPeptide
{
    public:
        AnalyseurPeptide(Parser::peptide* pep,int nb_affiche,const double er,double masse_max,int finds_max_size=-1);
        ~AnalyseurPeptide();
        void resolve(int debut=-1);
        #warning "une fois le noyeau calculé, mettre pour chaque AA sa suretée (nombre de fois présente dans les saloution, si les peaks pris en cmpt changent), et fusionner plusieurs solutions quand on peut : AA-AA-trou + trou-AA-AA = AA-AA-trou?-AA-AA"
        #warning "faire un dif entre 2 analyse (pep en + ou -) / stats généraux / truc de comparaison"
        //typedef std::tuple<int,int,bool,double> res_get_near ; //peak index / index aa_tab pour le AA trouvé / is_croisement (true) / erreur entre trouvé et théorique
        enum Sens{LEFT=-1,STOP=0,RIGHT=1};

        static enum {
            ERROR_TOTAL=0, //les erreur + et - se compensent
            ERROR_AA_CUMUL, //les erreur + et - se compensnt pas
            INTENSITEE_TOTAL_PARENT,//intensitée total des intensitées
            INTENSITEE_TOTAL, //intensitée totale qui est utilisée
            MASSE_TOTAL,//somme des masse des AA
            PERCENT_COUVERTURE,//MASSE_TOTAL /100 * MASSE_PARENT 
            //PERCENT_COUVERTURE_AA,//MASSE_TOTAL / masse moyenne AA
            NB_AA, //nombre de AA dans la chaine
            NB_PEAKS, //nombre de peaks dasn la chaine
            MASSE_PARENT, //masse du peptide
            PERCENT_INTENSITEE_UTILISEE,//INTENSITEE_TOTAL /100 * INTENSITEE_TOTAL_PARENT  
            VALUES_SIZE
        } enum_values;

        class stack_token {
            public:
                stack_token();
                stack_token(stack_token&& other);
                stack_token(const stack_token& other);
                stack_token(const int i,Parser::peptide::peak* data); //peak
                stack_token(const int i,const double e,AnalyseurPeptide::stack_token* peak); //aa
                stack_token(const int i); //aa pour boucher un trou
                stack_token(const double score); //HEADER

                ~stack_token();

                stack_token& operator=(const stack_token& other);
                stack_token& operator=(stack_token&& other);

                inline stack_token* get_peak_stack_NULL() {
                    stack_token* res = aa_token.pt_data;
                    aa_token.pt_data=NULL;
                    return res;
                };

                void __print__() const;
                

                enum Type {UNKNOW=-1,AA_TOKEN=0,AA_HOLD_TOKEN,/*AA_LIST,*/PEAK_TOKEN, HEADER_TOKEN} type;

                union {
                    struct  {
                        int index;
                        double error;
                        stack_token* pt_data;
                    } aa_token;

                    struct{
                        double error;
                        std::vector<int> aa_index;
                    } aa_hold_token;

                    struct {
                        int index;
                        Parser::peptide::peak* pt_data;
                    } peak_token;

                    struct {
                        double score;
                        struct _holds {
                            std::shared_ptr<std::list<list_arg> > link;
                            double to_find;
                        } holds [Parser::peptide::SIZE];
                        //double values[VALUES_SIZE];
                    } header_token;
                };
        };

    typedef std::vector<AnalyseurPeptide::stack_token*> v_tokens_ptr;
    typedef std::list<AnalyseurPeptide::stack_token*> pile_tokens_ptr;

    #ifdef APPRENTISSAGE
        friend class IndividuTree;
        class ApprentissageSolution
        {
            public:

                ApprentissageSolution(const AnalyseurPeptide::v_tokens_ptr& s,const Parser::peptide* const pep);

                bool operator>(AnalyseurPeptide::ApprentissageSolution other) const {return this->score > other.score;};

                void __print__();

            protected:
                friend class IndividuTree;
                friend class AnalyseurPeptide;

                void check_validity();//cheque si la solution trouvé fait parti d'une possible

                std::string solution;
                double values[VALUES_SIZE];
                double score;
                int real_score;
                int validity;// nombre de AA de la solution (0 si non valide au COMPLET !!) Juste pour les vérifications
                std::shared_ptr<std::vector<std::string> > solutions_reals;

        };
        std::vector<ApprentissageSolution> propositions;

        std::mutex mutex;
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
