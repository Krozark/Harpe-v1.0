#ifndef AA_TAB_HPP
#define AA_TAB_HPP

#include "AA.hpp"
#include <list>
#include <vector>

struct arg {
    double value;
    int coef;
    int coef_min;
    AA* pt;
    arg(double v,int c=0): value(v),coef(c),coef_min(0),pt(NULL){};
    arg(AA* aa,int c=0):value(aa->masse),coef(c),coef_min(0),pt(aa){};
    arg(): value(0),coef(0),coef_min(0),pt(NULL){};
    void __print__() const
    {
        std::cout<<pt->slug<<"*"<<coef;
    }
};
#define ABS(x) (x<0?-x:x)
class list_arg {
    public:
        double erreur;
        std::list<arg> list;

        const bool operator < (const list_arg& other) const
        {
            return ABS(erreur) < ABS(other.erreur);
        };
};

class AnalyseurPeptide;
class AA_Tab
{
    public:
        AA_Tab();

        void add_all_AA();

        void add_AA(AA aa);

        inline void add(const char* name, double masse){add_AA(AA(name,length,masse));};

        inline const AA& operator [](const int index) const {return AAs[index];};

        void __print__() const;

        arg * to_args();

        std::list< list_arg >* resolve(const double to_find,const double error);

        static void print(std::list<list_arg >& res, int limite);
        static void print(list_arg& res);

        static void sort(std::list<list_arg >& res);

        int get_id(std::string name) const;
        int get_id(char code) const;
        int get_pk(int pk);
        double get_masse_for(const char* chaine,double base_masse=0) const;

        double delete_masse_of(double masse,int index);
        double delete_masse_of(double masse,char* names);
        void delete_masse_all(double masse);

        double add_masse_of(double masse,int index);
        double add_masse_of(double masse,char* names);
        void add_masse_all(double masse);
        inline const double get_max_masse(){return max_masse;};
        inline void sort(){std::sort(AAs.begin(),AAs.end());};
        inline const unsigned int size(){return length;}
        void calc_moyenne();


    private:
        friend class AnalyseurPeptide;
        friend class Enzyme;
        friend class ImpossibleCut;
        std::vector<AA> AAs;
        unsigned int length;
        double max_masse;
        double moyenne;
};



#endif
