#ifndef INDIVIDUTREE_HPP
#define INDIVIDUTREE_HPP

#include <cstdlib>
#include <iostream>
#include <math.h>
#include <vector>
#include <stack>

#include "../analyseur/analyseur_peptide.hpp"

#define TREE_INIT_PROONDEUR 7
#define TREE_SUB_INIT_PROFONDEUR 5
#define TO_BE_BEST 3


class IndividuTree
{
    public:
        IndividuTree(): genome(0),score(0)/*, seuil(1.f)*/{}; //pour les vector, list ...
        IndividuTree(const int& profondeur_init);

        IndividuTree* clone() const;

        ~IndividuTree();

        void mutate(); //fait mutter un coef en ajoutant [-1,1] ou remplacer sous arbre
        IndividuTree* crossOver(const IndividuTree& other) const;

        void eval(); //fonction d'évaluation, affect le score
        static std::vector<AnalyseurPeptide*>* pep_to_test;
        bool need_eval(){return not evaluate;};

        bool operator>(const IndividuTree& other)const {return (this->score == other.score)?this->size()<other.size():this->score > other.score;}

        //void __print__();
        friend std::ostream& operator<<(std::ostream& output,const IndividuTree& individu);

        class Node
        {
            public:
                enum Type {CONSTANTE=0,VALS_INDICE,UNAIRE,BINAIRE};

                Node(float cst);
                Node(int indice);
                Node(float (*funaire)(float),Node* _1);
                Node(float (*funaire)(float,float),Node* _1,Node* _2);
                ~Node();/* recursif */

                Node* clone() const;/* recursif */
                float eval(const double * const vals) const;

                static Node* CreateRandTree(const int max_profondeur);

                /* FONCTIONS */
                //unaire
                /*0*/static float sin_f(float _1) {return sin(_1);};
                //*6*/static float moins_u(float _1) {return -_1;};
                //binaire
                /*1*/static float mul(float _1,float _2) {return _1*_2;};
                /*2*/static float div(float _1,float _2) {return _1/_2;};
                /*3*/static float add(float _1,float _2) {return _1+_2;};
                /*4*/static float moins_b(float _1,float _2) {return _1-_2;};
                /*5 sous programme?*/



                //croisement et mutation qui s'adapte au fur et à mesure du temps?
                //nombre de croisement proportionele au nombre de nodes
                //changer le nombre d'args à une fonctions auto définie
                //petite mutation (un noeu de meme arité)
                
                static int max_indice;

                friend std::ostream& operator<<(std::ostream& output,const Node* root);

            private:
                friend class IndividuTree;
                Node(): fils1(0),fils2(0){}; //interne use only
                inline void maj(){nb_sub_nodes = (fils1?fils1->nb_sub_nodes:0) + (fils2?fils2->nb_sub_nodes:0) + 1;}

                Node* fils1;
                Node* fils2;

                Type type;
                union
                {
                    float value;
                    int indice;
                    float (*funaire)(float _1);
                    float (*fbinaire)(float _1,float _2);
                };

                int nb_sub_nodes; /*1 => feuilles*/
        };

        static float tauxMutation;

        const int get_score()const {return score;};
        const int size()const {return genome->nb_sub_nodes;};

        void minimize(){minimize(genome);};

        //float seuil;//< TODO ??

    private:
        inline std::stack<Node*> get(int numero);/* entre ]1,genome->nb_sub_nodes[ , genome->nb_sub_nodes est le numero de la racine*/
        inline Node* get_node(int numero) const;/* entre ]1,genome->nb_sub_nodes[ , genome->nb_sub_nodes est le numero de la racine*/

        int score;
        Node* genome;

        void minimize(Node* root);

        bool evaluate;
};

#endif
