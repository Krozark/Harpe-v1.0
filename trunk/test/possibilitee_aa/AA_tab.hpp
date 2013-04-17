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
    void __print__()
    {
        std::cout<<pt->name<<"*"<<coef;
    }
};
#define ABS(x) (x<0?-x:x)
class list_arg {
    public:
        double erreur;
        std::list<arg> list;

        const bool operator < (const list_arg& other)
        {
            return ABS(erreur) < ABS(other.erreur);
        };
};


class AA_Tab
{
    public:
        AA_Tab();

        void add_all_AA();
        void add_all_AA_moy();

        void add_AA(AA aa);

        inline void add(const char* name, double masse){add_AA(AA(name,length,masse));};

        inline AA operator [](const int index){return AAs[index];};

        void __print__();

        arg * to_args();

        std::list< list_arg > resolve(const double to_find,const double error);
        std::list< list_arg > resolve_and(const double to_find,const double error,int id, int coef_min,...);
        std::list< list_arg > resolve_or(const double to_find,const double error,int id, int coef_min,...);
        void constrain_remove_all(std::list<list_arg> &l,char* codes);

        static void print(std::list<list_arg >& res, int limite);
        static void print(list_arg& res);
        static void constrain_with_and(std::list< list_arg > &l,int id,int coef_min,...);
        static void constrain_with_or(std::list< list_arg > &l,int id,int coef_min,...);

        //static void sort(std::list<list_arg >& res);

        int get_id(std::string name);
        int get_id(char code);
        double get_masse_for(const char* chaine,double base_masse=0);

        double delete_masse_of(double masse,int index);
        double delete_masse_of(double masse,char* names);
        void delete_masse_all(double masse);

        double add_masse_of(double masse,int index);
        double add_masse_of(double masse,char* names);
        void add_masse_all(double masse);

    private:
        std::vector<AA> AAs;
        unsigned int length;
};



#endif
