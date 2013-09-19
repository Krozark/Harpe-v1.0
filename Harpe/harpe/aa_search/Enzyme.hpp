#ifndef ENZYME_HPP
#define ENZYME_HPP

#include "AA_tab.hpp"
#include <iostream>

#include "../sql/SqlObject.hpp"

extern AA_Tab aa_tab;

class AnalyseurPeptide;

/**
 * \break Classe permetant de gérer les enzyme depuis la Bdd
 **/
class Enzyme : public SqlObject<Enzyme>
{
    public:
        /**
         * \brief Construit une Enzyme depuis la bdd via un nom
         **/
        Enzyme(std::string name);

        /**
         * \brief Affiche l'enzyme pour le debug
         **/
        void __print__();

        /**
         * \brief Construit l'enzyme depuis une requete Mysql
         **/
        Enzyme(sql::ResultSet* query);

        /**
         * \brief Sauvgarde l'enzyme en Bdd
         **/
        void save(bool commit=false);

        static const std::string TableName;///< Table dasn la bdd = "website_aa";

    private:
        /**
         * \brief Coupure impossible en Bdd (pour eliminer des solutions si l'enzyme est spécifiée
         **/
        class ImpossibleCut : public SqlObject<ImpossibleCut>
        {
            public :
                /**
                 * \brief Construit l'objet depuis la bdd
                 **/
                ImpossibleCut(sql::ResultSet* query);
                /**
                 * \brief sauvgarde en bdd
                 **/
                void save(bool commit=true);

                static const std::string TableName;///< Table dans la bdd = "website_impossiblecut";

                int first; ///< pk de l'AA 
                int second;///< pk de l'AA 

                /**
                 * \brief Affiche pour le debug
                 **/
                void __print__();

        };

    protected:
        friend class AnalyseurPeptide;

        std::string name;///< Nom de l'enzyme
        std::vector<int> coupure_avant; ///< Peut couper avant ces AA (pk)
        std::vector<int> coupure_apres; //<< Peut couper apres ces AA (pk)
        std::vector<ImpossibleCut> coupure_impossibles; ///< Ne peut pas couper entre ces 2 AA (pk)
};

#endif
