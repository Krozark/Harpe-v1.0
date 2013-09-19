#ifndef AA_TAB_HPP
#define AA_TAB_HPP

#include "AA.hpp"
#include <list>
#include <vector>

/**
 * \breif Structure permetant de stocker un AA et le nombre d'occurence minimum de présence
 **/
struct arg {
    double value; ///< masse en dalton cherché
    int coef;     ///< nombre d'occurence trouvé
    int coef_min; ///< nombre minimum à trouver
    AA* pt;       ///< AA concerné
    /**
     * \breif Construit un arg depuis une masse et un nombre minimum
     *
     * \param v la masse cherché
     * \param c le nombre minimum de cette masse
     **/
    arg(double v,int c=0): value(v),coef(c),coef_min(0),pt(NULL){};

    /**
     * \breif Consrtuit un arg depuit un AA tout en précisant le nombre de fois qu'il doit être présent
     *
     * \param aa l'AA recherché
     * \param c le nombre minimum de présence de celui ci
     **/
    arg(AA* aa,int c=0):value(aa->masse),coef(c),coef_min(0),pt(aa){};

    /**
     * \breif Constructeur par défaut, à utiliser par les std::vector et autre containeurs
     **/
    arg(): value(0),coef(0),coef_min(0),pt(NULL){};

    /**
     * \breif Affiche l'arg pour le debug
     **/
    void __print__() const
    {
        std::cout<<pt->slug<<"*"<<coef;
    }
};
/**
 * \brief définie la fonction mathématique de la valeur absolue
 **/
#define ABS(x) (x<0?-x:x)

/**
 * \brief Permet simplement de stoquer une séquence d'arg
 *=*/
class list_arg {
    public:
        double erreur;///< erreur entre la demmande et la somme des args (value*coef)
        std::list<arg> list; ///< list des AA trouvé et de leur nombre

        /**
         * \brief compare simplement les erreurs
         **/
        const bool operator < (const list_arg& other) const
        {
            return ABS(erreur) < ABS(other.erreur);
        };
};

class AnalyseurPeptide;

/**
 * \brief Permet de stoquer les AA qui sont construit depuis la Bdd
 **/
class AA_Tab
{
    public:
        /**
         * \brief Constructeur
         **/
        AA_Tab();

        /**
         * \brief Ajoute tous les AA présent dans la base de donnée
         **/
        void add_all_AA();

        /**
         * \brief Ajoute un AA à ceux déja existant
         **/
        void add_AA(AA aa);

        /**
         * \brief Construit un AA depuit ces informations de bases
         *
         * \param name Le nom de l'AA
         * \param masse La masse de l'AA
         **/
        inline void add(const char* name, double masse){add_AA(AA(name,length,masse));};

        /***
         * \brief Permet d'acceder à un AA grace à son id
         *
         * \param index qui est stoqué dasn les AA
         *
         * \return l'acide animé à l'index indiqué
         **/
        inline const AA& operator [](const int index) const {return AAs[index];};

        /**
         * \brief Permet d'afficher l'AA pour le debug
         **/
        void __print__() const;

        /**
         * \brief Stoque tous les aa dasn un tableau de arg
         *
         * \return Le tableau de taille length
         **/
        arg * to_args();

        /**
         * \brief Permet de cherche une composition en AA pour une masse donnée
         *
         * \param to_find La masse à chercher en Da
         * \param error la marge d'erreur autorisée en Da
         *
         * \return Des liste de séquence d'AA  (AA + nombre)
         **/
        std::list< list_arg >* resolve(const double to_find,const double error);

        /**
         * \brief Permet d'afficher les résultats
         *
         * \param res Liste à afficher
         * \param limite Nombre maximum à afficher
         **/
        static void print(std::list<list_arg >& res, int limite);
        
        /**
         * \brief Permet d'afficher les résultats
         *
         * \param res Liste à afficher
         **/
        static void print(list_arg& res);

        /**
         * \brief Permet de trier la liste en utilisant l'operator >
         **/
        static void sort(std::list<list_arg >& res);

        /**
         * \brief Permet d'avoir d'id de l'AA de slug name
         *
         * \param name slug de l'AA
         *
         * \return l'id de l'AA (la case ou il est stoqué)
         **/
        int get_id(std::string name) const;
        /**
         * \brief Permet d'avoir d'id de l'AA de code code
         *
         * \param code Slug de l'AA de 1 char
         *
         * \return l'id de l'AA (la case ou il est stoqué)
         **/
        int get_id(char code) const;

        /**
         * \brief Permet d'avoir d'id de l'AA de pk pk
         *
         * \param pk PK en BDD de l'AA
         *
         * \return l'id de l'AA (la case ou il est stoqué)
         **/
        int get_pk(int pk);


        /**
         * \breif Permet d'avoir la masse de la chaine entrée en paramètre
         *
         * \param chaine La chaine où chaque char représente le sliug d'un AA
         * \param base_masse La masse en plus à ajouter à la masse recherché
         *
         * \return La masse total de la séquence
         **/
        double get_masse_for(const char* chaine,double base_masse=0) const;

        /**
         * \breif Retir à la masse passé en param, la masse de l'AA d'id index
         *
         * \param masse La masse de base
         * \param L'index de l'AA à retirer
         *
         * \return la masse <= masse
         **/
        double delete_masse_of(double masse,int index);
        /**
         * \breif Retir à la masse passé en param, la masse des AA spésifié dans names
         *
         * \param masse La masse de base
         * \param names Les slugs (de 1 char) des AA dont la masse est à soustrire
         *
         * \return la masse <= masse
         **/
        double delete_masse_of(double masse,char* names);

        /**
         * \breif Retir la masse de tous les AA à la masse spécifié et affiche la différence
         *
         * \param la masse de base
         **/

        void delete_masse_all(double masse);

        /**
         * \breif Ajout la masse de l'AA index à la masse masse
         *
         * \param masse La masse de base
         * \param index L'index de l'AA
         *
         * \return masse > masse
         **/
        double add_masse_of(double masse,int index);

        /**
         * \breif Ajout les masses des AA index dans names
         *
         * \param masse La masse de base
         * \param names Les slug des AA a ajouter (slug de A char)
         *
         * \return masse > masse
         **/
        double add_masse_of(double masse,char* names);

        /**
         * \breif Ajoute la masse de tous les AA à la masse de base qui est spécifié, et affiche le résultat
         *
         * \param masse la masse de base
         **/
        void add_masse_all(double masse);

        /**
         * \breif Renvoie la masse de plus lourd AA
         *
         * \return la masse du plus lourd AA
         **/
        inline const double get_max_masse(){return max_masse;};

        /**
         * \breif Trie les AA par masse.
         *
         * Note : Invalide tous les indexes précedement recus
         **/
        inline void sort(){std::sort(AAs.begin(),AAs.end());};

        /**
         * \return Le nombre d'AA
         **/
        inline const unsigned int size(){return length;}

        /**
         * \breif Calcule la masse moyenne des AA stoqués
         **/
        void calc_moyenne();


    private:
        friend class AnalyseurPeptide;
        friend class Enzyme;
        friend class ImpossibleCut;

        std::vector<AA> AAs; ///< Stocke les AA
        unsigned int length;///< LE nombre de AA
        double max_masse; ///< La plus grosse masse
        double moyenne; ///< La masse moyenne
};



#endif
