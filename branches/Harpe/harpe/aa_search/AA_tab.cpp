#include "AA_tab.hpp"
#include "defines.hpp"
#include <stdarg.h>
#include <algorithm>

using namespace std;



inline void resolve_equation(const double to_find,list<list_arg >* res,arg* list_val,const int size,const double error,const double tot_up)
{
    if(eq_error(tot_up,to_find,error)) // on stoque le résultat
    {
        list_arg l;
        l.erreur = to_find - tot_up;
        for(int i=0;i<size;++i)
            if (list_val[i].coef > 0)
            //{}         
               l.list.emplace_back(list_val[i]);
        res->emplace_back(l);
    }
};

inline void resolve_equation(const double to_find,list<list_arg >* res,arg* list_val,const int size,const double error,const double tot_up,const int etage)
{
    list_val[etage].coef = 0;
    for(double i=0;i<=to_find + error;i+=list_val[etage].value)
    {
        //tab_int[etage]++;
        const double tot = tot_up + i;
        if(tot <= to_find + error)
        {
            if (etage > 0)
                resolve_equation(to_find,
                                 res,
                                 list_val,
                                 size,
                                 error,
                                 tot,
                                 etage -1);
            else
                resolve_equation(to_find,
                                 res,
                                 list_val,
                                 size,
                                 error,
                                 tot);
        }
        else
            break;
        ++(list_val[etage].coef);
    }
};

AA_Tab::AA_Tab()
{
    length=0;
    max_masse = 0;
    //AAs = NULL;
};

void AA_Tab::__print__() const
{
    for(int i=0;i <length;++i)
        AAs[i].__print__();
};


void AA_Tab::add_all_AA()
{
    vector<AA>  AA_all = AA::all();
    for (AA& aa : AA_all)
        add_AA(aa);
    
    //calc_moyenne();

};

void AA_Tab::add_AA(AA aa)
{
    aa.id=length;
    AAs.emplace_back(aa);
    if (aa.masse > max_masse)
        max_masse = aa.masse;
    ++length;
};

arg * AA_Tab::to_args()
{
    arg* res = new arg[length];
    for(unsigned int i=0;i < length;++i)
    {
        res[i].pt = &AAs[i];
        res[i].value = AAs[i].masse;
    }
    return res;
};

std::list< list_arg >* AA_Tab::resolve(const double to_find,const double error)
{
    list< list_arg >* res= new list<list_arg>;
    if (length > 0)
    {
        arg* kwargs =to_args();
        resolve_equation(to_find,
                     res,
                     kwargs,
                     length,
                     error,
                     0,
                     length-1);
        delete[] kwargs;
    }
    return res;
};
void AA_Tab::print(list<list_arg > &res,int limite) 
{
    cout<<VERT;
    for(auto i=res.begin();i!=res.end() && limite > 0;++i)
    {
        --limite;
        print((*i));
    }
    cout<<BLANC;
};

void AA_Tab::print(list_arg &res) 
{
    cout<<"erreur: "<<res.erreur<<" ";
    for(auto i=res.list.begin();i!=res.list.end();++i)
    {
        (*i).__print__();
        cout<<" ";
    }
    cout<<endl;
};

struct int2 {
    const int id;
    const int coef;
    int2(const int i1,const int i2):id(i1),coef(i2){};
};


bool lt(const list_arg& _1 ,const list_arg& _2)
{
    return ABS(_1.erreur) < ABS(_2.erreur);
};


void AA_Tab::sort(std::list<list_arg>& arg)
{
    arg.sort(lt);
};

int AA_Tab::get_id(std::string name) const
{
    for(int i=0;i<length;++i)
        if(AAs[i].slug==name)
            return AAs[i].id;
    return -1;
};

int AA_Tab::get_id(char code) const
{
    char name[2];
    name[0]=code;
    name[1]= '\0';
    return get_id(name);
};

int AA_Tab::get_pk(int pk)
{
    for(int i=0;i<length;++i)
        if(AAs[i].get_pk()==pk)
            return AAs[i].id;
    return -1;
};

double AA_Tab::delete_masse_of(double masse,int index)
{
    return masse - AAs[index].masse;
};

double AA_Tab::delete_masse_of(double masse,char* names)
{
    double res = masse;
    int i=0;
    while (names[i] != '\0')
    {
        int index;
        if (names[i] == 'I' || names[i] == 'L')
            index=get_id("I_L");
        else
            index = get_id(names[i]);
        ++i;
        if (index >= 0)
        {
            res-= AAs[index].masse;
            cout<<AAs[index].slug<<" ("<<AAs[index].masse<<")\ttotal: "<<res<<endl;;
        }
        else
            cerr<<names[i]<<" pas trouvé"<<endl;
    }
    return res;
};

void AA_Tab::delete_masse_all(double masse)
{
    for(int i=0;i<length;++i)
        cout<<AAs[i].slug<<" "<<delete_masse_of(masse,i)<<endl;
};

double AA_Tab::add_masse_of(double masse,int index)
{
    return masse + AAs[index].masse;
};

double AA_Tab::add_masse_of(double masse,char* names)
{
    double res = masse;
    int i=0;
    while (names[i] != '\0')
    {
        int index;
        if (names[i] == 'I' || names[i] == 'L')
            index=get_id("I_L");
        else
            index = get_id(names[i]);

        ++i;
        if (index >= 0)
        {
            res+= AAs[index].masse;
            cout<<AAs[index].slug<<" ("<<AAs[index].masse<<")\ttotal: "<<res<<endl;;
        }
        else
            cerr<<names[i]<<" pas trouvé"<<endl;
    }
    return res;
};

void AA_Tab::add_masse_all(double masse)
{
    for(int i=0;i<length;++i)
        cout<<AAs[i].slug<<" "<<(masse + AAs[i].masse)<<endl;
};


double AA_Tab::get_masse_for(const char* chaine,double res) const
{
    int i=0;
    char code[2];
    while(chaine[i]!='\0')
    {
        int index=-1;

        if (chaine[i] == 'I' || chaine[i] == 'L')
            index=get_id("I_L");
        else
            index=get_id(chaine[i]);

        if (index!= -1)
        {
            res+=AAs[index].masse;
            cout<<chaine[i]<<" ("<<AAs[index].masse<<")\ttotal: "<<res<<endl;;
        }
        else
            cerr<<chaine[i]<<" pas trouvé"<<endl;
        ++i;
    }
    return res;
};

void AA_Tab::calc_moyenne()
{
    moyenne =0;
    const unsigned int size = AAs.size();
    for (unsigned int i=0;i<size;++i)
        moyenne += AAs[i].masse;
    moyenne/=size;
};
