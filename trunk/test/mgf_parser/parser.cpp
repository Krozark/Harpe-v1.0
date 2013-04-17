#include "parser.hpp"
#include <stdio.h>
#include <string.h>

using namespace std;

Parser::Parser(const char* filename)
{
    file = fopen(filename,"r");
    state= OUT_PEPTIDE;
    if(!file)
        cerr<<"impossible d'ouvrir le fichier "<<filename<<"."<<endl; 
    yyin = file;
    fseek(file,0,SEEK_SET);

    null_output =0;
    /*null_output=fopen(NULL_OUTPUT,"w");
    if(null_output)
        yyout = null_output;
        */
};

Parser::~Parser()
{
    const unsigned int size =peptides.size();
    for(unsigned int i=0;i<size;++i)
        delete peptides[i];

    if(file)
        fclose(file);
    if(null_output)
        fclose(null_output);
};

void Parser::parse_all()
{
    peptide* p;
    //while((p=parse_next())!=NULL)
    {
        p=parse_next();
        p->calc_masse_peaks();
        peptides.push_back(p);

    }
};

Parser::peptide* Parser::parse_next()
{
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
                PERROR
            
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
                if (res == 3)
                    pep->add(mz,intensitee,charge);
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
                    PERROR

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
                else
                    PERROR
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
                    PERROR
            }
            else 
                PERROR
        }
    }
    if (pep->mz ==0)
    {
        delete pep;
        return NULL;
    }
    pep->normalize_intensitee();
    return pep;
};

void Parser::__print__()
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
    calc_masse();
};

void Parser::peptide::add(const double m,const unsigned int i,const char c)
{
    peaks.push_back(peak(m,i,c));
};

void Parser::peptide::__print__(){
    cout<<"masse:"<<masse<<" charge:"<<(int)charge<<" mz:"<<mz<<" intensitee:"<<intensitee<<endl;

    const unsigned int size = peaks.size();
    for(unsigned int i=0;i<size;++i)
        peaks[i].__print__();
};

void Parser::peptide::normalize_intensitee()
{
    const unsigned int size = peaks.size();
    double max = 0;
    for(unsigned int i=0;i<size;++i)
        max = MAX(max,peaks[i].intensitee);

    for(unsigned int i=0;i<size;++i)
        peaks[i].intensitee/=max;
};
void Parser::peptide::calc_masse_peaks()
{
    unsigned int size = peaks.size();
    unsigned int i=0;
    while(i<size)
    {
        peaks[i].calc_masse(this->charge,this->masse);
        if (peaks[i].masse.size() == 0) //supression des truc impossible à etre en lien avec le peptide
        {
            peaks.erase(peaks.begin()+i);
            --size;
        }
        else
            ++i;
    }
};

void Parser::peptide::normalize_and_calc_masse()
{
    const unsigned int size = peaks.size();
    double max = 0;
    for(unsigned int i=0;i<size;++i)
        max = MAX(max,peaks[i].intensitee);

    for(unsigned int i=0;i<size;++i)
    {
        peaks[i].intensitee/=max;
        peaks[i].calc_masse(this->charge,this->masse);
    }
};

/////////////////// peaks //////////////////////////////
Parser::peptide::peak::peak(const double m,const unsigned int i,const char c): mz(m),intensitee(i),charge(c)
{
};


void Parser::peptide::peak::__print__(){
    cout<<mz<<" (";
    const unsigned int size = masse.size();
    for (unsigned int i=0;i<size;++i)
    {
        if (i>0)
            cout<<" ";
        cout<<masse[i];
    }
    cout<<") "<<intensitee;
    if (charge)
        cout<<" "<<(int)charge<<"+";
    cout<<endl;
};

void Parser::peptide::peak::calc_masse(const unsigned int parent_charge,const double parent_masse)
{
    masse.clear();
    if (charge>0)
        masse.push_back(mz*charge - charge* MH);
    else
    {
        for (unsigned int i=1; i<parent_charge;++i)
        {
            const double m = Parser::get_masse_for(mz,i);
            if (m <= parent_masse)
                masse.push_back(m);
        }
    }
};
