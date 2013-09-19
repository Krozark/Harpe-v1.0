#ifndef AA_HPP
#define AA_HPP
//#include <stdio.h>
#include <iostream>
#include <algorithm>

#include "../sql/SqlObject.hpp"

/**
 * \breif Une classe permettant de représenter les acides aminées
 **/
class AA : public SqlObject<AA> {
    public:
        /**
         * \breif Construit un acide aminé à partir de sa masse, son nom et un identifiant unique
         *
         * \param name Le nom complet de l'AA
         * \param id L'identifiant de l'AA (entier)
         * \param masse La masse de l'AA en dalton
         **/
        AA(const char* name,const char id, double masse);

        /**
         * \breif Construit un acide aminé à partir de sa masse, son nom et un identifiant unique
         *
         * \param name Le nom complet de l'AA
         * \param slug Le nom court à afficher (souvent une seule lettre)
         * \param id L'identifiant de l'AA (entier)
         * \param masse La masse de l'AA en dalton
         **/
        AA(const char* name,const char* slug,const char id, double masse);


        /**
         * \breif Constructeur par défaut. Il n'est pas à utiliser, mais est présent pour permetre de crer des containeurs
         **/
        AA();
        ~AA();

        /**
         * \brief Affiche le slug de l'AA
         **/
        friend std::ostream& operator<<(std::ostream& output,const AA& p);

        /**
         * \brief Affiche le slug de l'AA
         **/
        inline void __print__() const {std::cout<<slug<<" "<<masse<<std::endl;};

        /**
         * \brief Compare simplement les masses de deux AA pour les trier par masse
         **/
        bool operator < (const AA& other) const {return this->masse < other.masse;};

        //friend class AA_Tab;
        char name[256]; ///< Le nom de l'AA
        char slug[16];  ///< Le nom court
        int id;         ///< son identifiant
        double masse;   ///< sa masse en dalton
        

        /**
         * \brief Permet de contruire un AA depuis une requête MySQL
         **/
        AA(sql::ResultSet* query);

        /**
         * \breif Ajoute ou modifie l'AA en bvase de donnée
         **/ 
        void save(bool commit=true);
        static const std::string TableName; ///< Nom de la table où est stock" l'objet en bdd  (website_aa)
};


#endif
