#ifndef ENZYME_HPP
#define ENZYME_HPP

#include "AA_tab.hpp"
#include <iostream>

#include "../sql/SqlObject.hpp"

extern AA_Tab aa_tab;

class AnalyseurPeptide;

class Enzyme : public SqlObject<Enzyme>
{
    public:
        Enzyme(std::string name);
        void __print__();

        Enzyme(sql::ResultSet* query);
        void save(bool commit=false);
        static const std::string TableName;//= "website_aa";

    private:
        class ImpossibleCut : public SqlObject<ImpossibleCut>
        {
            public :
                ImpossibleCut(sql::ResultSet* query);
                void save(bool commit=true);
                static const std::string TableName;//= "website_aa";

                int first;
                int second;

                void __print__();

        };

    protected:
        friend class AnalyseurPeptide;
        std::string name;
        std::vector<int> coupure_avant; //AA id
        std::vector<int> coupure_apres; //AA id
        std::vector<ImpossibleCut> coupure_impossibles; //AA id
};

#endif
