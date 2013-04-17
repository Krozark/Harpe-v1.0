#include "AA_tab.hpp"
#include "defines.hpp"
#include <stdarg.h>
#include <algorithm>

using namespace std;

//TODO  resolve_equation_with_constraings()
inline void resolve_equation_or(const double to_find,list<list_arg >* res,arg* list_val,const int size,const double error,const double tot_up)
{
    if(eq_error(tot_up,to_find,error)) // on stoque le résultat
    {
        list_arg l;
        l.erreur = to_find - tot_up;
        bool add=false;
        for(int i=0;i<size;++i)
        {
            if (list_val[i].coef > 0)
               l.list.push_back(list_val[i]);
            if (list_val[i].coef >= list_val[i].coef_min )
                add=true;
        }
        if (add)
            res->push_back(l);
    }
};

inline void resolve_equation_or(const double to_find,list<list_arg >* res,arg* list_val,const int size,const double error,const double tot_up,const int etage)
{
    list_val[etage].coef = 0;
    for(double i=0;i<=to_find + error;i+=list_val[etage].value)
    {
        const double tot = tot_up + i;
        if(tot <= to_find + error)
        {
            if (etage > 0)
                resolve_equation_or(to_find,
                                 res,
                                 list_val,
                                 size,
                                 error,
                                 tot,
                                 etage -1);
            else
                resolve_equation_or(to_find,
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



inline void resolve_equation(const double to_find,list<list_arg >* res,arg* list_val,const int size,const double error,const double tot_up)
{
    if(eq_error(tot_up,to_find,error)) // on stoque le résultat
    {
        list_arg l;
        l.erreur = to_find - tot_up;
        for(int i=0;i<size;++i)
            if (list_val[i].coef > 0)
            //{}         
               l.list.push_back(list_val[i]);
        res->push_back(l);
    }
};

inline void resolve_equation_and(const double to_find,list<list_arg >* res,arg* list_val,const int size,const double error,const double tot_up,const int etage)
{
    list_val[etage].coef = list_val[etage].coef_min;
    for(double i=list_val[etage].coef*list_val[etage].value;i<=to_find + error;i+=list_val[etage].value)
    {
        const double tot = tot_up + i;
        if(tot <= to_find + error)
        {
            if (etage > 0)
                resolve_equation_and(to_find,
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

inline void resolve_equation(const double to_find,list<list_arg >* res,arg* list_val,const int size,const double error,const double tot_up,const int etage)
{
    list_val[etage].coef = 0;
    for(double i=0;i<=to_find + error;i+=list_val[etage].value)
    {
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
    //AAs = NULL;
};

void AA_Tab::__print__()
{
    for(int i=0;i <length;++i)
        AAs[i].__print__();
};


void AA_Tab::add_all_AA()
{
    add("W",  186.079313);
    add("Y",  163.06332);
    add("R",  156.101111);
    //add("U",  150.95363);
    add("F",  147.068414);
    add("H",  137.058912);
    add("M",  131.040485);
    add("E",  129.042593);
    add("Q",  128.058578);
    add("K",  128.094963);
    add("D",  115.026943);
    add("N",  114.042927);
    add("I/L",113.084064);
    add("C",  103.009185);
    add("T",  101.047679);
    add("V",   99.068414);
    add("P",   97.052764);
    add("S",   87.032028);
    add("A",   71.03711);
    add("G",   57.021464);
};

void AA_Tab::add_all_AA_moy()
{
     add("W",   186.2099);
     add("Y",   163.1733);
     add("R",   156.1857);
     //add("U",   150.0379);
     add("F",   147.1739);
     add("H",   137.1393);
     add("M",   131.1961);
     add("E",   129.114);
     add("Q",   128.1292);
     add("K",   128.1723);
     add("D",   115.0874);
     add("N",   114.1026);
     add("I/L", 113.1576);
     add("C",   103.1429);
     add("T",   101.1039);
     add("V",   99.1311);
     add("P",   97.1152);
     add("S",   87.0773);
     add("A",   71.0779);
     add("G",   57.0513);
};


void AA_Tab::add_AA(AA aa)
{
    aa.id=length;
    AAs.push_back(aa);
    ++length;
};

arg * AA_Tab::to_args()
{
    arg* res = new arg[length];
    int cpt=0;
    for(unsigned int i=0;i < length;++i)
    {
        res[cpt].pt = &AAs[i];
        res[cpt++].value = AAs[i].masse;
    }
    return res;
};

std::list< list_arg > AA_Tab::resolve(const double to_find,const double error)
{
    list< list_arg > res;
    if (length > 0)
    {
        arg* kwargs =to_args();
        resolve_equation(to_find,
                     &res,
                     kwargs,
                     length,
                     error,
                     0,
                     length-1);
        delete kwargs;
    }
    return res;
};

std::list<list_arg > AA_Tab::resolve_and(const double to_find,const double error,int id, int coef_min,...)
{
    list< list_arg > res;
    if (length > 0)
    {
        va_list p_list;
        va_start(p_list,id);

        arg* kwargs =to_args();

        deb:

        for (unsigned int i=0; i< length;++i)
        {
            if(kwargs[i].pt->id == id)
                kwargs[i].coef_min = coef_min;
        }

        if((id=va_arg(p_list, int)) != NULL)
        {
            coef_min=va_arg(p_list,int);
            goto deb;
        }

        va_end(p_list);
        resolve_equation_and(to_find,
                         &res,
                         kwargs,
                         length,
                         error,
                         0,
                         length-1);
        delete kwargs;
    }
    return res;
};

std::list< list_arg > AA_Tab::resolve_or(const double to_find,const double error,int id, int coef_min,...)
{
    std::list< list_arg > res;
    if (length > 0)
    {
        va_list p_list;
        va_start(p_list,id);

        arg* kwargs =to_args();

        deb:

        for (unsigned int i=0; i< length;++i)
        {
            if(kwargs[i].pt->id == id)
                kwargs[i].coef_min = coef_min;
        }

        if((id=va_arg(p_list, int)) != NULL)
        {
            coef_min=va_arg(p_list,int);
            goto deb;
        }

        va_end(p_list);
        resolve_equation_or(to_find,
                         &res,
                         kwargs,
                         length,
                         error,
                         0,
                         length-1);
        delete kwargs;
    }

    return res;
};

void AA_Tab::print(list<list_arg > &res,int limite)
{
    cout<<VERT;
    for(list<list_arg >::iterator i=res.begin();i!=res.end() && limite > 0;++i)
    {
        --limite;
        print((*i));
    }
    cout<<BLANC;
};

void AA_Tab::print(list_arg &res)
{
    cout<<"erreur: "<<res.erreur<<" ";
    for(list<arg>::iterator i=res.list.begin();i!=res.list.end();++i)
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

void AA_Tab::constrain_with_and(std::list< list_arg > &l,int id,int coef_min,...)
{
    va_list p_list;
    va_start(p_list,coef_min);

    deb:

    for(list<list_arg >::iterator i=l.begin();i!=l.end();++i)
    {
        bool find=false;
        for(list<arg>::iterator j=(*i).list.begin();j!=(*i).list.end() && !find ;++j)
        {
            
            if((*j).pt->id == id && (*j).coef >= coef_min)
                find = true;
        }
        if(!find)
        {
            i= l.erase(i);
            --i;
        }
    }

    if((id=va_arg(p_list, int)) != NULL)
    { 
        coef_min=va_arg(p_list,int);
        goto deb;
    }

    va_end(p_list);
};

void AA_Tab::constrain_with_or(std::list< list_arg > &l,int id,int coef_min,...)
{
    va_list p_list;
    va_start(p_list,coef_min);
    
    list<int2> to_find;

    to_find.push_back(int2(id,coef_min));

    while((id=va_arg(p_list, int)) != NULL)
    {
        coef_min=va_arg(p_list,int);
        to_find.push_back(int2(id,coef_min));
    }

    va_end(p_list);

    for(list<list_arg >::iterator i=l.begin();i!=l.end();++i)
    {
        bool find=false;
        for(list<arg>::iterator j=(*i).list.begin();j!=(*i).list.end() && !find ;++j)
        { 
            for(list<int2>::iterator k=to_find.begin();k!=to_find.end() && !find ;++k)
            {
                if((*j).pt->id == (*k).id && (*j).coef >= (*k).coef)
                    find = true;
            }
        }
        if(!find)
        {
            i= l.erase(i);
            --i;
        }
    }

};

void AA_Tab::constrain_remove_all(std::list<list_arg> &l,char* codes)
{
    vector<int> to_find;
    int size = 0;

    while(codes[size]!= NULL)
    {
        to_find.push_back(get_id(codes[size]));
        ++size;
    }


    for(list<list_arg >::iterator i=l.begin();i!=l.end();++i)
    {
        bool find=false;
        for(list<arg>::iterator j=(*i).list.begin();j!=(*i).list.end() && !find ;++j)
        { 
            for(int k=0;k<size && !find;++k)           
            {
                if((*j).pt->id == to_find[k])
                    find = true;
            }
        }
        if(find)
        {
            i= l.erase(i);
            --i;
        }
    }

};

int AA_Tab::get_id(std::string name)
{
    for(int i=0;i<length;++i)
        if(AAs[i].name==name)
            return AAs[i].id;
    return -1;
};

int AA_Tab::get_id(char code)
{
    char name[2];
    name[0]=code;
    name[1]= '\0';
    return get_id(name);
}

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
            index=get_id("I/L");
        else
            index = get_id(names[i]);
        ++i;
        if (index >= 0)
        {
            res-= AAs[index].masse;
            cout<<AAs[index].name<<" ("<<AAs[index].masse<<")\ttotal: "<<res<<endl;;
        }
        else
            cerr<<names[i]<<" pas trouvé"<<endl;
    }
    return res;
};

void AA_Tab::delete_masse_all(double masse)
{
    for(int i=0;i<length;++i)
        cout<<AAs[i].name<<" "<<delete_masse_of(masse,i)<<endl;
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
            index=get_id("I/L");
        else
            index = get_id(names[i]);

        ++i;
        if (index >= 0)
        {
            res+= AAs[index].masse;
            cout<<AAs[index].name<<" ("<<AAs[index].masse<<")\ttotal: "<<res<<endl;;
        }
        else
            cerr<<names[i]<<" pas trouvé"<<endl;
    }
    return res;
};

void AA_Tab::add_masse_all(double masse)
{
    for(int i=0;i<length;++i)
        cout<<AAs[i].name<<" "<<(masse + AAs[i].masse)<<endl;
};


double AA_Tab::get_masse_for(const char* chaine,double res)
{
    int i=0;
    char code[2];
    while(chaine[i]!='\0')
    {
        int index=-1;

        if (chaine[i] == 'I' || chaine[i] == 'L')
            index=get_id("I/L");
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
