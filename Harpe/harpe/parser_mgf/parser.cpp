#include "parser.hpp"
#include <stdio.h>
#include <string.h>
#include <algorithm>

#include "../aa_search/AA_tab.hpp"

#ifdef APPRENTISSAGE
#include <string.h>
#endif



using namespace std;

int lex_line;

void find_and_replace(string& source, string const& find, string const& replace)
{
    for(std::string::size_type i = 0; (i = source.find(find, i)) != std::string::npos;)
    {
        source.replace(i, find.length(), replace);
        i += replace.length() - find.length() + 1;
    }
};


Parser::Parser(const char* filename)
{
    file = fopen(filename,"r");
    state= OUT_PEPTIDE;
    if(!file)
        cerr<<"impossible d'ouvrir le fichier "<<filename<<"."<<endl; 
    yyin = file;

    //null_output =0;
    null_output=fopen(NULL_OUTPUT,"w");
    if(null_output)
        yyout = null_output;
};

Parser::~Parser()
{
    const unsigned int size =peptides.size();
    for(unsigned int i=0;i<size;++i)
        delete peptides[i];

    if(file)
    {
        fclose(file);
    }
    if(null_output)
    {
        fclose(null_output);
    }
};

void Parser::parse_all(bool ignore)
{
    peptide* p;
    while((p=parse_next(ignore))!=NULL)
    {
        p->calc_masse_peaks();
        peptides.emplace_back(p);
    }
};

Parser::peptide* Parser::parse_next(bool ignore)
{
    #warning "traiter les cas d'erreur dans le parsage du mgf"
    #warning "traiter les variables mise dans le fichier mgf"

    peptide* pep=new peptide(0,0);

    int lex_st;
    while((lex_st=yylex())!= 0)
    {
        if(state == OUT_PEPTIDE) //TODO
        {
            if (lex_st == PASS)//TODO
            {
            }
            else if (lex_st == BEGIN_PEP) //TODO
            {
                state = IN_PEPTIDE;
            }
            else 
                PERROR("state "<<lex_st<<" "<<yytext);
            
        }
        else if (state == IN_PEPTIDE) //TODO
        {
            if (lex_st == END_PEP)
            {
                state = OUT_PEPTIDE;
                break;
            }
            else if (lex_st == PEAK_T) //DONE
            {
                float mz;
                int charge,intensitee;
                int res = sscanf(yytext,"%f %d %d+",&mz,&intensitee,&charge);
                if (not ignore)
                {
                    if (res == 3)
                        pep->add(mz,intensitee,charge);
                }
                else
                    pep->add(mz,intensitee);
            }
            else if (lex_st == PEAK_D) //DONE
            {
                float mz;
                int intensitee;
                int res = sscanf(yytext,"%f %d",&mz,&intensitee);
                if (res == 2)
                    pep->add(mz,intensitee);
            }
            else if (lex_st ==  OPTION_LOCAL || lex_st == OPTION_ALL) //TODO
            {
                lex_st=yylex();
                if (lex_st == AFF_VALUE)
                {
                }
                else
                    PERROR("state "<<lex_st<<" "<<yytext);

            }
            else if (lex_st == TITLE)
            {
                lex_st=yylex();
                if (lex_st == AFF_VALUE)
                {
                    pep->title = yytext;
                    pep->title.erase(0,1);
                }
                else
                    PERROR("state "<<lex_st<<" "<<yytext);
            }
            else if (lex_st == PEPMASS) //DONE
            {
                lex_st=yylex();
                if (lex_st == AFF_PEAK)
                {
                    float poids;
                    int intensitee;
                    int res = sscanf(yytext,"=%f %d+",&poids,&intensitee);
                    if (res ==2)
                    {
                        pep->mz= poids;
                        pep->intensitee = intensitee;
                    }
                }
                else if(lex_st == AFF_VALUE)
                {
                    float poids;
                    int res = sscanf(yytext,"=%f",&poids);
                    if (res ==1)
                    {
                        pep->mz= poids;
                        pep->intensitee = 0;
                    }
                }
                else
                    PERROR("state "<<lex_st<<" "<<yytext);
            }
            else if (lex_st == SOLUTION )
            {
                lex_st=yylex();
                if (lex_st == AFF_VALUE)
                {
                    int str_size = strlen(yytext);
                    string slug = "";
                    vector<string> aa_slugs;

                    for(int i=1;i<str_size;++i)
                    {
                        if(yytext[i] == '|')
                        {//fin du slug
                            if (slug == "I" or slug == "L")
                                slug = "I_L";
                            aa_slugs.emplace_back(slug);
                            slug = "";
                        }
                        else if(yytext[i] == ',' or i == str_size -1)
                        {//fin de la solution
                            if (slug == "I" or slug == "L")
                                slug = "I_L";
                            aa_slugs.emplace_back(slug);
                            slug = "";
                            string str;

                            int size_aa_slugs = aa_slugs.size();
                            //dans le sens de solution
                            //creer la str
                            for(int i=0; i< size_aa_slugs;++i)
                            {
                                if (i !=0)
                                    str += "|";
                                str += aa_slugs[i];

                            }
                            #if (DEBUG & DEBUG_STATS) | APPRENTISSAGE
                            pep->solutions->emplace_back(str);
                            #endif

                            #if DEBUG & DEBUG_MGF
                            cout<<"solution: "<<str<<endl;
                            #endif

                            //reset
                            str = "";

                            //dans le sens inverse
                            //creer la str
                            for(int i=size_aa_slugs -1; i>=0;--i)
                            {
                                if (i !=size_aa_slugs-1)
                                    str += "|";
                                str += aa_slugs[i];
                            }
                            #if (DEBUG & DEBUG_STATS) || APPRENTISSAGE
                            pep->solutions->emplace_back(str);
                            #endif

                            #if DEBUG & DEBUG_MGF
                            cout<<"solution reverse: "<<str<<endl;
                            #endif
                        }
                        else 
                        {//caractère du slug
                            slug += yytext[i];
                        }
                    }
                }
            }
            else if (lex_st == CHARGE) //DONE
            {
                lex_st=yylex();
                if (lex_st == AFF_VALUE )
                {
                    int charge,res;
                    res = sscanf(yytext,"= %d+",&charge);
                    if(res ==1)
                    {
                        pep->charge=charge;
                        pep->calc_masse();
                    }
                }
                else
                    PERROR("state "<<lex_st<<" "<<yytext);
            }
            else 
                PERROR("state "<<lex_st<<" "<<yytext);
        }
    }
    if (pep->mz ==0)
    {
        delete pep;
        PERROR("exit, state "<<lex_st<<" "<<yytext);
        return NULL;
    }
    pep->normalize_and_calc_masse();
    //ajouts du debut
    pep->add_firsts();
    //ajout de la fin
    pep->add_lasts();

    return pep;
};

void Parser::__print__() const
{
    const unsigned int size=peptides.size();
    for(unsigned int i=0;i<size;++i)
    {
        peptides[i]->__print__();
        cout<<endl;
    }
};

//////////// peptides ////////////////////////

Parser::peptide::peptide(const double m,const unsigned int i,const char c):charge(c),intensitee(i),mz(m)
{
    #if (DEBUG & DEBUG_STATS) || APPRENTISSAGE
    solutions = make_shared<vector<string> >();
    #endif
    for (short int i=0;i<SIZE;++i)
        special_peaks[i] = NULL;
    //peak_h2o = NULL;
    //seuil = 0;
    calc_masse();
};


Parser::peptide::~peptide()
{

    for(vector<peak*>::iterator i=peaks.begin();i != peaks.end();++i)
        delete (*i);
}

void Parser::peptide::add_firsts() //ajouts de faux peaks de debut
{

    peak* p = new peak(0,1,-1); 
    p->masse = MH2O; 
    peaks.insert(peaks.begin(),p);
    special_peaks[DEBUT_H2O] = p;

    p = new peak(*(p));
    p->masse = 0; 
    special_peaks[DEBUT] = p;
    peaks.insert(peaks.begin(),p);

};

void Parser::peptide::add_lasts()
{
    peak* p = new peak(0,1,-1);
    p->masse = this->masse - MH2O;
    peaks.emplace_back(p);
    special_peaks[FIN_H2O] = p;

    p = new peak(*(p));
    p->masse = this->masse; 
    peaks.emplace_back(p);
    special_peaks[FIN] = p;
};

void Parser::peptide::add(const double m,const unsigned int i,const char c)
{
    peaks.emplace_back(new peak(m,i,c));
};

void Parser::peptide::__print__() const {
    cout<<"titre:"<<title<<endl;
    cout<<"masse:"<<masse<<" charge:"<<(int)charge<<" mz:"<<mz<<" intensitee:"<<intensitee<<endl;

    const unsigned int size = peaks.size();
    for(unsigned int i=0;i<size;++i)
    {
        cout<<i<<" ";
        peaks[i]->__print__();
    }
};

void Parser::peptide::normalize_intensitee()
{
    const unsigned int size = peaks.size();
    double max = 0;
    for(unsigned int i=0;i<size;++i)
        max = MAX(max,peaks[i]->intensitee);

    for(unsigned int i=0;i<size;++i)
        peaks[i]->intensitee/=max;
};
void Parser::peptide::calc_masse_peaks()
{
    vector<peak*>::iterator i=peaks.begin();
    while(i != peaks.end())
    {
        (*i)->calc_masse(this);
        if ((*i)->masse > masse) //supression des truc impossible à etre en lien avec le peptide
        {
            delete (*i);
            i = peaks.erase(i);
        }
        else
            ++i;
    }
};

bool lt(const Parser::peptide::peak* _1 ,const Parser::peptide::peak* _2)
{
    return _1->masse < _2->masse;
};

void Parser::peptide::sort()
{
    std::sort(this->peaks.begin(),this->peaks.end(),lt);
};

void Parser::peptide::normalize_and_calc_masse()
{
    {
        int i= 0;
        while(i < peaks.size())
        {
            peaks[i]->calc_masse(this);
            if (peaks[i]->masse > masse) //supression des truc impossible à etre en lien avec le peptide
            {
                delete peaks[i];
                peaks.erase(peaks.begin()+i);
            }
            else
            {
                //peaks[i]->__print__();
                ++i;
            }
        }
    }

    //on est assuré de ne pas suprimer le furtu peak à 1
    this->sort();

    const unsigned int size = peaks.size();
    double max = 0;
    for(unsigned int i=0;i<size;++i)
        //if (peaks[i].originale) // pour ne pas compter les intesitée 2 fois
            max = MAX(max,peaks[i]->intensitee);
    for(unsigned int i=0;i<size;++i)
    {
        //peaks[i]->index = i;
        peaks[i]->intensitee/=max;
    }

};

/*void Parser::peptide::calc_seuil()
{
    seuil = 0;
    int nb = 0;
    const unsigned int size = peaks.size();
    for(unsigned int i=0;i<size;++i)
    {
        nb++;
        seuil += peaks[i]->intensitee;
    }
    if (nb > 0)
    {
        seuil /= nb;
        //double med = peaks[nb/2]->intensitee;
        //#ifdef DEBUG
        //cout<<"moyenne: "<<seuil<<endl;
        //cout<<"médiane: "<<med<<endl;
        //#endif
        
        //seuil= (seuil*2 + med)/3.0;
    }    

    #if DEBUG & DEBUG_CALC
    cout<<"seuil (moyenne): "<<seuil<<endl;
    #endif
};*/

/*void Parser::peptide::set_seuil(const double s)
{
    const unsigned int size = peaks.size();
    for(unsigned int i=0;i<size;++i)
        peaks[i]->bruit = (peaks[i]->intensitee >= s);
};*/

/*void Parser::peptide::set_seuil()
{
    calc_seuil();
    set_seuil(seuil);
}*/

bool Parser::peptide::is_one_of_special(Parser::peptide::peak* p) const
{
    return (p == special_peaks[DEBUT] 
            or p == special_peaks[DEBUT_H2O] 
            or p == special_peaks[FIN_H2O] 
            or p == special_peaks[FIN]);
};

bool Parser::peptide::is_one_of_firsts(Parser::peptide::peak* p) const
{
    return (p == special_peaks[DEBUT] 
            or p == special_peaks[DEBUT_H2O]);
};

bool Parser::peptide::is_one_of_lasts(Parser::peptide::peak* p) const
{
    return (p == special_peaks[FIN_H2O] 
            or p == special_peaks[FIN]);
};

bool Parser::peptide::is_one_of_h2o(Parser::peptide::peak* p) const
{
    return (p == special_peaks[DEBUT_H2O] 
            or p == special_peaks[FIN_H2O]);
};
/////////////////// peaks //////////////////////////////
Parser::peptide::peak::peak(const double m,const unsigned int i,const char c): mz(m),intensitee(i),charge(c), originale(true)
{
    //bruit = false;
    used = new bool;
    *used = false;
};

Parser::peptide::peak::peak(const Parser::peptide::peak& other) : originale(false)
{
    mz = other.mz;
    intensitee = other.intensitee;
    masse = other.masse;
    //bruit = other.bruit;
    used = other.used;
    charge = other.charge;
}


Parser::peptide::peak::~peak()
{
    if (originale) //c'est crassous, mais au mois je libère que quand il faut
        delete used;
}

void Parser::peptide::peak::__print__() const
{
    cout<</*ROUGE<<(bruit?"+":"-")<<*/VERT<<" <"<<originale<<"> "<<JAUNE<<masse<<BLEU<<" ("<<mz<<") "<<ROUGE<<intensitee<<" "<<VERT<<(int)charge<<"+"<<BLANC<<endl;
};

void Parser::peptide::peak::calc_masse(Parser::peptide* parent)
{
    if (charge == 0)
    {
        charge = 1;
        masse = mz*charge - charge* MH;
        const int max = parent->charge;
        for (unsigned int i=2; i<=max;++i)
        {
            int size = parent->peaks.size();
            peak* p = new peak(*this);
            parent->peaks.emplace_back(p);
            p->charge = i;
            p->calc_masse();
        }
    }
    else if (charge >= 0)
    {
        calc_masse();
    }
};

