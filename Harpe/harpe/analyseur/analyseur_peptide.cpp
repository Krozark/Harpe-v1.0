#include "analyseur_peptide.hpp"
#include "../combinaisons/src/Combinaison.hpp"

#include <iomanip>
#include <algorithm>
#include <ctime>

#define eq_error(value,to_find,error) (value >= (to_find - error) && value <= (to_find + error))

using namespace std;


AnalyseurPeptide::AnalyseurPeptide (Parser::peptide* pep,int nb,const double er, double masse_max,int max_size) : erreur(er), masse_max_trou(masse_max)
{
    this->pep = pep;
    nb_affiche = nb;
    finds_max_size = max_size;

};

AnalyseurPeptide::~AnalyseurPeptide()
{
    for(int i = tokens_ptr.size()-1;i>=0;--i)
        delete tokens_ptr[i];
    delete pep;
}

const int AnalyseurPeptide::depiler(AnalyseurPeptide::pile_tokens_ptr& search,const int sens)
{

    int current_peak_index = -1;

    #if DEBUG & DEBUG_STACK
    cout<<"_______ depiler __________"<<endl;
    print_stack_all(search);
    #endif

    register int nb;
    stack_token* token;

    if (sens == Sens::RIGHT) // depiler depuis le back
    {
        list<AnalyseurPeptide::stack_token*>::reverse_iterator i;

        remove_1_peak_right:

        nb=0;
        i= search.rbegin();
        ++i;
        {
            auto end = search.rend();
            while(i!=end && (*i)->type != AnalyseurPeptide::stack_token::Type::PEAK_TOKEN && nb < 2)
            {
                ++i;
                ++nb;
            }
        }
        if (nb==0)
        {//la pile est vide
            current_peak_index = -1;
            goto end;
        }
        token = search.back();

        #if DEBUG & DEBUG_STACK
        cout<<"delete: ";
        search.back()->__print__(); // prochain peaks;
        #endif

        search.pop_back(); // delete Peak
        (*token->peak_token.pt_data->used)=false; //on ne l'utilise plus

        #if DEBUG & DEBUG_STACK
        cout<<"delete: ";
        search.back()->__print__(); // prochain peaks;
        #endif

        search.pop_back(); // delete AA

        if (nb ==1)
        {
            nb=0;
            goto remove_1_peak_right;
        }
        else
        {
            token = search.back()->get_peak_stack_NULL();
            #if DEBUG & DEBUG_STACK
            cout<<"add: ";
            token->__print__(); // prochain peaks;
            #endif
            search.emplace_back(token);
            current_peak_index = token->peak_token.index;
        }
    }
    else if (sens == Sens::LEFT) // depiler depuis le fron
    {
        list<AnalyseurPeptide::stack_token* >::iterator i;

        remove_1_peak_left:

        nb=0;
        token = search.front();
        (*token->peak_token.pt_data->used)=false; //on ne l'utilise plus

        #if DEBUG & DEBUG_STACK
        cout<<"delete: ";
        token->__print__(); // prochain peaks;
        #endif

        search.pop_front(); // delete Peak

        /// trouver le prochain peak
        i= search.begin();
        {
            auto end = search.end();
            while(i!= end && (*i)->type != AnalyseurPeptide::stack_token::Type::PEAK_TOKEN)
            {
                ++i;
                ++nb;
            }
        }

        if (nb==0)
        {//la pile est vide
            current_peak_index = -1;
            goto end;
        }

        //revenir au AA courament utilisé
        --i;
        #if DEBUG & DEBUG_STACK
        cout<<"delete: ";
        (*i)->__print__(); // prochain peaks;
        #endif

        i=search.erase(i);//delete le AA qui était utilisé

        //il n'y avait que le AA qui était pris en compt
        //on le suprime
        if (nb==1)
        {
            nb=0;
            //on recommence
            goto remove_1_peak_left;
        }
        // si il y avait d'autres solutions
        else
        {
            //on prend le AA juste avent
            --i;
            //on récupère son peak
            token = (*i)->get_peak_stack_NULL();
            #if DEBUG & DEBUG_STACK
            cout<<"add: ";
            token->__print__(); // prochain peaks;
            #endif
            //on l'ajoute
            search.push_front(token);
            current_peak_index = token->peak_token.index;
        }
    }
    end: 
    #if DEBUG & DEBUG_STACK
    cout<<"_______ end depiler __________"<<endl;
    #endif

    return current_peak_index;
};

bool solution_gt(const AnalyseurPeptide::v_tokens_ptr& _1, const AnalyseurPeptide::v_tokens_ptr& _2)
{
    return _1[0]->header_token.score > _2[0]->header_token.score;
};

void AnalyseurPeptide::save_stack(const AnalyseurPeptide::pile_tokens_ptr& search, std::list<AnalyseurPeptide::v_tokens_ptr>& res)
{
    v_tokens_ptr l;
    tokens_ptr.emplace_back(new stack_token(0.f));
    l.emplace_back(tokens_ptr.back());
    auto i=search.begin();
    auto end = search.end();

    int nb=0;
    double masse = 0;
    double intensitee = -pep->peaks[0]->intensitee;
    double errors = 0;
    double error_tot = 0;

    while(i!=end)
    {
        AnalyseurPeptide::stack_token& tmp_i= **i;

        if (tmp_i.type == AnalyseurPeptide::stack_token::Type::PEAK_TOKEN)
        {
            l.emplace_back(&tmp_i);
            Parser::peptide::peak* p = tmp_i.peak_token.pt_data;
            intensitee += p->intensitee;
            if ( pep->is_one_of_h2o(p))
                 masse += MH2O;
        }
        if (tmp_i.type == AnalyseurPeptide::stack_token::Type::AA_TOKEN and tmp_i.aa_token.pt_data==NULL)
        {
            l.emplace_back(&tmp_i);
            errors += ABS(tmp_i.aa_token.error);
            error_tot += tmp_i.aa_token.error;
            masse += aa_tab[tmp_i.aa_token.index].masse;
            ++nb;
        }
        ++i;
    }
    l.shrink_to_fit();
    //add the current
    res.emplace_back(l);
    //add all other possibilites tha can be (or not) complete
    --i;//peaks
    --i;//current head
    end = search.begin();//decement
    const int size =l.size();
    while(i!=end)
    {
        AnalyseurPeptide::stack_token& tmp_i= **i;

        if(tmp_i.type == AnalyseurPeptide::stack_token::Type::PEAK_TOKEN)
        {
            break;
        }
        else if (tmp_i.type == AnalyseurPeptide::stack_token::Type::AA_TOKEN and tmp_i.aa_token.pt_data!=NULL)
        {
            l[size-2] = &tmp_i;
            l[size-1] = tmp_i.aa_token.pt_data;
            res.emplace_back(l);
        }
        --i;
    }
};

void AnalyseurPeptide::print_results(const std::list<AnalyseurPeptide::v_tokens_ptr>& res)
{
    cout<<" +++++++++++++++++ RESULTS ("<<res.size()<<") +++++++++++++++++"<<endl;
    int nb = 1;
    for(auto i=res.begin();i!=res.end();++i)
    {
        if (nb_affiche >0 && nb++> nb_affiche)
            break;
        cout<<" ---- solution "<<nb<<"----"<<endl;
        const AnalyseurPeptide::v_tokens_ptr& tmp_i = *i;
        for(auto j=tmp_i.begin();j!=tmp_i.end();++j)
        {
            (*j)->__print__();
        }
        cout<<" ---- end solution ----"<<endl;
    }
    cout<<" +++++++++++++++ END RESULTS +++++++++++++++"<<endl;
};

void AnalyseurPeptide::print_stack_all(const AnalyseurPeptide::pile_tokens_ptr& search)
{
    cout<<"-----stack all -----"<<endl;
    auto i=search.begin();
    while(i!=search.end())
    {
        (*i)->__print__();
        ++i;
    }
    cout<<"-----end stack-----"<<endl;
};

void AnalyseurPeptide::print_stack_used(const AnalyseurPeptide::pile_tokens_ptr& search)
{
    cout<<" +++++ stack used +++++"<<endl;
    auto i=search.begin();
    while(i!=search.end())
    {
        auto& tmp_i = **i;
        if (tmp_i.type == AnalyseurPeptide::stack_token::Type::PEAK_TOKEN)
        {
            tmp_i.__print__();
        }
        if (tmp_i.type == AnalyseurPeptide::stack_token::Type::AA_TOKEN and tmp_i.aa_token.pt_data==NULL)
        {
            tmp_i.__print__();
        }
        ++i;
    }
    cout<<" +++++ end stack +++++"<<endl;
};

void AnalyseurPeptide::resolve(int debut)
{
    //pep->set_seuil();
    #ifndef APPRENTISSAGE && (DEBUG & DEBUG_STATE_RUNNING) ||(DEBUG & DEBUG_MGF)
    //visualisation MGF
    std::cout<<"Spectre reconstitué et complété"<<std::endl;
    pep->__print__();
    #endif

    #if DEBUG & DEBUG_STATS 
    for( int k=1;k<=20;++k)
    {
    #endif
        clock_t t = clock();
        std::vector<int> peak_index;
        if (debut==-1)
        {
            #if DEBUG & DEBUG_STATS 
            peak_index = get_index_max_intensitee_vector(k);
            #else
            peak_index = get_index_max_intensitee_vector(10);
            #endif
        }
        else
            peak_index.emplace_back(debut);


        const int size = peak_index.size();

        #if DEBUG & DEBUG_MGF_PEAKS_START
        cout<<"peaks les plus intenses (points de depart): "<<std::endl;
        for(auto i:peak_index)
            pep->peaks[i]->__print__();
        #endif

       
        // instantiation
        for(int i=0;i<size;++i)
        {
            register int current_peak_index = peak_index[i];
            #if (DEBUG & DEBUG_SOLUTION) || (DEBUG & DEBUG_STATE_RUNNING)
            std::cout<<ROUGE<<"Pic #"<<i<<"("<<current_peak_index<<")"<<BLANC;
            pep->peaks[current_peak_index]->__print__();
            std::cout<<std::endl;
            #endif


            pile_tokens_ptr search;
            std::list<v_tokens_ptr > results_right;
            std::list<v_tokens_ptr > results_left;
            /* Chaque résultata est sous la forme: (vecteur<stack_token>)
             * HEADER_TOKEN, [PEAK_TOKEN, AA_TOKEN]* PEAK_TOKEN
             */
            AnalyseurPeptide::Sens sens= Sens::RIGHT;

            //initialisation
            tokens_ptr.emplace_back(new stack_token(current_peak_index,pep->peaks[current_peak_index]));
            search.emplace_back(tokens_ptr.back());

            while (sens)
            {
                Parser::peptide::peak* current_peak = pep->peaks[current_peak_index];
                *(current_peak->used) = true;

                #if DEBUG & DEBUG_SOLUTION
                cout<<endl<<" ==================================================== "<<endl<<endl;
                cout<<"current peak: ";
                current_peak->__print__();
               #endif

                const v_tokens_ptr& find=*get_near(current_peak_index,sens);
                int size = find.size();

                if (size <= 0) // rien de trouvé
                {

                    current_peak_index = depiler(search,sens);
                    if (current_peak_index == -1)
                    {
                        if (sens == Sens::RIGHT )
                        {
                            #if DEBUG & DEBUG_SOLUTION
                            cout << "Changement de sens vers la gauche"<<endl;
                            #endif
                            sens = Sens::LEFT;
                            current_peak_index = search.front()->peak_token.index;
                        }
                        else if (sens == Sens::LEFT)
                        {
                            #if DEBUG & DEBUG_SOLUTION
                            cout << "Changement de sens à stop"<<endl;
                            #endif
                            sens = Sens::STOP;
                        }
                        //continue;
                        //break;
                    }
                }
                else
                {
                    if (sens == Sens::RIGHT)
                    {
                        for (int i=0;i<size-1;++i)
                        {

                            #if DEBUG & DEBUG_SOLUTION
                            cout<<"trouvé: ";
                            find[i]->__print__();
                            #endif
                            search.emplace_back(find[i]);
                        }

                        auto& tmp_find_last = *(find[size-1]);
                        stack_token* current_stack_peak = tmp_find_last.get_peak_stack_NULL();

                        current_peak_index = (current_stack_peak->peak_token.index);
                        search.emplace_back(&tmp_find_last); //AA
                        search.emplace_back(current_stack_peak); //PEAK

                        #if DEBUG & DEBUG_SOLUTION
                        cout<<"trouvé: ";
                        tmp_find_last.__print__();
                        cout<<"prochain peak: ";
                        current_stack_peak->__print__();
                        #endif

                        save_stack(search,results_right);
                        #if COMPLET_SOLUTION == 1
                        complet_solution(results_right.back(),Sens::RIGHT);
                        #endif
                    }
                    else if (sens == Sens::LEFT)
                    {

                        auto& tmp_find_last = *(find[size-1]);

                        stack_token* current_stack_peak = tmp_find_last.get_peak_stack_NULL();

                        current_peak_index = (current_stack_peak->peak_token.index);
                        search.emplace_front(&tmp_find_last); //AA


                        #if DEBUG & DEBUG_SOLUTION
                        cout<<"trouvé: ";
                        tmp_find_last__print__();
                        cout<<"prochain peak: ";
                        current_stack_peak->__print__();
                        #endif

                        for (int i=size-2;i>=0;--i)
                        {
                            #if DEBUG & DEBUG_SOLUTION
                            cout<<"trouvé: ";
                            find[i]->__print__();
                            #endif
                            search.emplace_front(find[i]);
                        }

                        search.emplace_front(current_stack_peak); //PEAK

                        save_stack(search,results_left);
                        #if COMPLET_SOLUTION == 1
                        complet_solution(results_left.back(),Sens::LEFT);
                        #endif
                    }
                }
                delete &find;
            }
            

            #if DEBUG & DEBUG_SOLUTION
            cout<<endl<<" ---------------- RESOLVE ---------------------------"<<endl;
            cout<<" ////////////// resultats gauches //////////////////"<<endl;
            print_results(results_left);
            cout<<" ////////////// end resultats gauches //////////////////"<<endl;
            cout<<" ////////////// resultats droits //////////////////"<<endl;
            print_results(results_right);
            cout<<" ////////////// end resultats droits //////////////////"<<endl;
            cout<<" ------------------------- END RESOLVE ----------------------"<<endl;
            #endif

            std::cout<<"merge ("<<results_left.size()<<" | "<<results_right.size()<<" = "<<results_right.size()*results_left.size()+results_left.size()+results_right.size()<<")"<<std::endl;

            #if DEBUG & DEBUG_STATS 
            merge_solution(results_left,results_right,k);
            #else
            merge_solution(results_left,results_right);
            #endif


            results_left.clear();
            results_right.clear();

            #warning "TODO ajouter l'enzyme corectement, et non en dure"
            #if FILTER_SOLUTION == 1
            //TODO
            Enzyme tryp("Trypsine");
            filter_enzyme(finds,tryp);
            #endif


            #ifdef APPRENTISSAGE
                #if DEBUG & DEBUG_STATS
                verifier_resultats_complet(finds,k-1);
                #else
                verifier_resultats_complet(finds);
                #endif

                for(auto current_sol= finds.begin();current_sol != finds.end();++current_sol)
                {
                    propositions.emplace_back(ApprentissageSolution(*current_sol,pep));
                    #if DEBUG & DEBUG_APPRENTISSAGE_STATS
                    //propositions.back().__print__();
                    #endif
                }
            #endif

        }

    #if DEBUG & DEBUG_STATS 
        calc_stats[k-1][CALC_TIME] += float(clock()-t)/CLOCKS_PER_SEC;
    }
    #endif

    #ifndef APPRENTISSAGE
    {
        int _size = finds.size();
        int f_size = _size;
        if(nb_affiche >=0 and  nb_affiche<_size)
            f_size = nb_affiche;

        f_size = (f_size>_size)?_size:f_size;
        
        if(f_size > 0)
        {
            int i=0;
            for(v_tokens_ptr& ii:finds)
            {
                print_AA(ii);
                if (++i>f_size)
                    break;
            }
            std::cout<<std::endl;
        }
    }
    #else

    //mise des score reels
    {
        const int _size =propositions.size();
        for(int i=0;i<_size;++i)
        {
            propositions[i].real_score = propositions[i].validity;
        }
    }
    #endif
    
};

void AnalyseurPeptide::calc_values(double* const values,const AnalyseurPeptide::v_tokens_ptr& s,const Parser::peptide* pep)
{
    for(int i=0;i<VALUES_SIZE;++i)
        values[i] = 0;
    //reset
    values[MASSE_PARENT]=pep->masse;
    {
        auto i=pep->peaks.begin() + 2;
        auto end =pep->peaks.end() - 2;
        while (i != end)
        {
            values[INTENSITEE_TOTAL_PARENT] += (*i)->intensitee;
            ++i;
        }
    }
    /* > ERROR_TOTAL
     * > ERROR_AA_CUMUL
     * > INTENSITEE_TOTAL_PARENT
     * > INTENSITEE_TOTAL
     * > MASSE_TOTAL
     * > PERCENT_COUVERTURE
     //* PERCENT_COUVERTURE_AA
     * > NB_AA
     * > NB_PEAKS
     * > MASSE_PARENT
     * > PERCENT_INTENSITEE_UTILISEE
     */
    auto i=s.begin();
    auto end = s.end();

    while(i!=end)
    {
        const stack_token& ii = **i;

        if (ii.type == AnalyseurPeptide::stack_token::Type::AA_TOKEN)
        {
            ++values[NB_AA];
            values[MASSE_TOTAL] += aa_tab[ii.aa_token.index].masse;

            values[ERROR_AA_CUMUL] += ABS(ii.aa_token.error);
            values[ERROR_TOTAL] += ii.aa_token.error;
        }
        if (ii.type == AnalyseurPeptide::stack_token::Type::PEAK_TOKEN)
        {
            ++values[NB_PEAKS];
            Parser::peptide::peak* p = ii.peak_token.pt_data;
            values[INTENSITEE_TOTAL] += p->intensitee;
            if ( pep->is_one_of_h2o(p))
                 values[MASSE_TOTAL] += MH2O;
        }
        ++i;
    }
    values[PERCENT_COUVERTURE] = values[MASSE_TOTAL] / values[MASSE_PARENT];
    values[PERCENT_INTENSITEE_UTILISEE] = values[INTENSITEE_TOTAL] /100.0 * values[INTENSITEE_TOTAL_PARENT];

};

const int AnalyseurPeptide::get_index_max_intensitee()
{
    int res=-1;
    const int size_pep = pep->peaks.size() - 2; //virrer les special fin
    double max_find = 0;

    for (int i=2; i<size_pep;++i) //virer les special debuts
    //for (int i=size_pep -1; i>= 2;--i) //virer les special debuts
    {
        const double in = pep->peaks[i]->intensitee;
        if (in > max_find)
        {
           res = i; 
           max_find = in;
        }
    }
   return res; 
};


bool gt_intensitee(const Parser::peptide::peak* _1,const Parser::peptide::peak* _2)
{
    return _1->intensitee > _2->intensitee;
};

const std::vector<int> AnalyseurPeptide::get_index_max_intensitee_vector(const int nb)
{
    vector<int> res;
    vector<Parser::peptide::peak*> res_p;

    std::vector<Parser::peptide::peak*>& pep_peaks = pep->peaks;
    
    //on prend les nb plus intenses
    const int _size = pep_peaks.size();
    
    int size_nb = nb*pep->charge+2;
    if (size_nb >= _size)
    {
        size_nb = _size-1;
    }

    partial_sort(pep_peaks.begin()+2,pep_peaks.begin()+size_nb,pep_peaks.end()-2,gt_intensitee); //au cas ou tous les peaks n'ont pas eu de charge spécifiées
    
    //on les saves
    #if DEBUG & DEBUG_CALC
    cout<<"MAXI :";
    #endif
    for(int i=2;i<size_nb && i < _size; ++i)
    {
        #if DEBUG & DEBUG_CALC
        pep_peaks[i]->__print__();
        #endif
        res_p.emplace_back(pep_peaks[i]);
    }

    pep->sort(); //on remet comme c'était

    const int size2 = res_p.size();
    //pour tous les peaks
    int finds=0;
    #if DEBUG & DEBUG_CALC
    cout<<"Finds :";
    #endif
    for(int j=0;j<size2;++j)
    {
        //pour tous les peaks
        for(int i=0;i<_size;++i)
            if(pep_peaks[i] == res_p[j]) // si c'est un des peaks
            {
                res.emplace_back(i); //on l'ajoute
                #if DEBUG & DEBUG_CALC
                pep_peaks[i]->__print__(); //on l'ajoute
                #endif
                if(pep_peaks[i]->originale) // si on a récupéré nb peaks différents
                    if(++finds >= nb)
                        goto exit_loop;
            }
    }
    exit_loop: //exit loop
    return res;
};

void AnalyseurPeptide::complet_solution(AnalyseurPeptide::v_tokens_ptr& sol,int sens)
{
    #if DEBUG & DEBUG_COMP_SOL
    cout<<" ------------------ COMPLET SOLUTION --------------------"<<endl;
    print_AA(sol,sens);
    #endif

    int size = sol.size();
    if (size > 1) // seulement le peak de debut
    {
        AnalyseurPeptide::stack_token& header = *sol[0];

        if (sens == Sens::LEFT) //debut
        {
            AnalyseurPeptide::stack_token& first_peak = *sol[1];
            //trouver les AA qui correspondent à la masse

            double to_find = first_peak.peak_token.pt_data->masse - pep->special_peaks[Parser::peptide::DEBUT]->masse;
            header.header_token.holds[Parser::peptide::DEBUT].to_find = to_find;
            if (to_find >0 and (masse_max_trou <0 or to_find < masse_max_trou))
            {
                std::list<list_arg>& debut = *aa_tab.resolve(to_find,erreur);
                //touver les ordres possibles

                #if DEBUG & DEBUG_COMP_SOL
                cout<<"Left"<<endl<<"debut: "<<to_find<<" ("<<debut.size()<<")"<<endl;
                AA_Tab::sort(debut);
                aa_tab.print(debut,nb_affiche>=0?nb_affiche:debut.size());
                #endif
                //ajouter a la solution acctuel

                header.header_token.holds[Parser::peptide::DEBUT].link.reset(&debut);
                //delete &debut;
            }

            to_find = first_peak.peak_token.pt_data->masse - pep->special_peaks[Parser::peptide::DEBUT_H2O]->masse;
            header.header_token.holds[Parser::peptide::DEBUT_H2O].to_find = to_find;
            if (to_find >0 and (masse_max_trou <0 or to_find < masse_max_trou))
            {
                std::list<list_arg>& debut_h2o = *aa_tab.resolve(to_find,erreur);

                #if DEBUG & DEBUG_COMP_SOL
                cout<<"debut_h2o: "<<to_find<<" ("<<debut_h2o.size()<<")"<<endl;
                AA_Tab::sort(debut_h2o);
                aa_tab.print(debut_h2o,nb_affiche>=0?nb_affiche:debut_h2o.size());
                #endif
                
                header.header_token.holds[Parser::peptide::DEBUT_H2O].link.reset(&debut_h2o);

                //delete &debut_h2o;
            }
        }
        else if (sens == Sens::RIGHT)//fin
        {
            AnalyseurPeptide::stack_token& last_peak = *sol[size - 1];

            double to_find = pep->special_peaks[Parser::peptide::FIN_H2O]->masse - last_peak.peak_token.pt_data->masse;
            header.header_token.holds[Parser::peptide::FIN_H2O].to_find = to_find;
            if (to_find >0 and (masse_max_trou <0 or to_find < masse_max_trou))
            {
                std::list<list_arg>& fin_h2o = *aa_tab.resolve(to_find,erreur);

                #if DEBUG & DEBUG_COMP_SOL
                cout<<"right"<<endl<<"fin_h2o: "<<to_find<<" ("<<fin_h2o.size()<<")"<<endl;
                AA_Tab::sort(fin_h2o);
                aa_tab.print(fin_h2o,nb_affiche>=0?nb_affiche:fin_h2o.size());
                #endif

                header.header_token.holds[Parser::peptide::FIN_H2O].link.reset(&fin_h2o);
                //delete &fin_h2o;
            }

            to_find = pep->special_peaks[Parser::peptide::FIN]->masse - last_peak.peak_token.pt_data->masse;
            header.header_token.holds[Parser::peptide::FIN].to_find = to_find;
            if(to_find>0 and (masse_max_trou <0 or to_find < masse_max_trou))
            {
                std::list<list_arg>& fin = *aa_tab.resolve(to_find,erreur);

                #if DEBUG & DEBUG_COMP_SOL
                cout<<"fin: "<<to_find<<" ("<<fin.size()<<")"<<endl;
                AA_Tab::sort(fin);
                aa_tab.print(fin,nb_affiche>=0?nb_affiche:fin.size());
                #endif

                header.header_token.holds[Parser::peptide::FIN].link.reset(&fin);

                //delete &fin;
            }

        }
    }
    #warning "ajouter tous les ordes possibles"
    #if DEBUG & DEBUG_COMP_SOL
    cout<<" ------------------ END COMPLET SOLUTION --------------------"<<endl;
    #endif
};

void AnalyseurPeptide::print_AA(const AnalyseurPeptide::v_tokens_ptr& v,int sens,bool p)
{
    int size = v.size();
    const stack_token& v_0 = *v[0];
    #if DEBUG & DEBUG_FILTER
    v_0.__print__();
    #endif
    cout<<"score: ["<<v_0.header_token.score<<"] ";
    if (p)
    {
        bool pr=false;
        const stack_token& v_1 = *v[1];

        if(v_1.peak_token.pt_data != pep->special_peaks[Parser::peptide::DEBUT])
            cout<<BLEU<<"<"<<pep->special_peaks[Parser::peptide::DEBUT]->masse<<">"<<BLANC;
        else
        {
            pr == true;
            cout<<(sens==Sens::RIGHT?MAGENTA:ROUGE)<<"<"<<v_1.peak_token.pt_data->masse<<">"<<BLANC;
        }

        if(v_1.peak_token.pt_data != pep->special_peaks[Parser::peptide::DEBUT_H2O])
            cout<<BLEU<<"-[H2O]<"<<pep->special_peaks[Parser::peptide::DEBUT_H2O]->masse<<">"<<BLANC;
        else
        {
            pr == true;
            cout<<(sens==Sens::RIGHT?MAGENTA:ROUGE)<<"-<"<<v_1.peak_token.pt_data->masse<<">"<<BLANC;
        }
        
        if (not pr)
            cout<<VERT<<"-?-"<<(sens==Sens::RIGHT?MAGENTA:ROUGE)<<"<"<<v_1.peak_token.pt_data->masse<<">"<<BLANC;
    }
    
    #if ! DEBUG_PEAKS
    cout<<JAUNE;
    #endif
    for(int i=2;i<size;++i)
    {
        const stack_token& v_i = *v[i];

        if(v_i.type == AnalyseurPeptide::stack_token::AA_TOKEN)
        {
            #if DEBUG_PEAKS
            cout<<JAUNE;
            #endif
            cout<<"-"<<aa_tab[v_i.aa_token.index].slug;
        }
        #if DEBUG_PEAKS
        else if (i < size-1 and v_i.type == AnalyseurPeptide::stack_token::PEAK_TOKEN)
            cout<<VERT<<"-<"<<v_i.peak_token.pt_data->masse<<">";
        #endif
    }
    cout<<BLANC;

    if (p)
    {
        bool pr = false;
        const stack_token& v_size_1 = *v[size-1];

        if(v_size_1.peak_token.pt_data != pep->special_peaks[Parser::peptide::FIN_H2O] and v_size_1.peak_token.pt_data != pep->special_peaks[Parser::peptide::FIN])
            cout<<(sens==Sens::LEFT?MAGENTA:ROUGE)<<"-<"<<v_size_1.peak_token.pt_data->masse<<">-"<<VERT<<"?"<<BLANC;
        else
        {
            if(v_size_1.peak_token.pt_data == pep->special_peaks[Parser::peptide::FIN_H2O])
            {
                pr = true;
                cout<<(sens==Sens::RIGHT?ROUGE:BLEU)<<"-<"<<pep->special_peaks[Parser::peptide::FIN_H2O]->masse<<">[H2O]"<<BLANC;
            }

            if(not pr and v_size_1.peak_token.pt_data == pep->special_peaks[Parser::peptide::FIN])
            {
                pr = true;
                cout<<(sens==Sens::RIGHT?ROUGE:BLEU)<<"-<"<<pep->special_peaks[Parser::peptide::FIN]->masse<<">"<<BLANC;
            }
        }
        
        if(not pr and v_size_1.peak_token.pt_data != pep->special_peaks[Parser::peptide::FIN_H2O])
            cout<<BLEU<<"-<"<<pep->special_peaks[Parser::peptide::FIN_H2O]->masse<<">[H2O]"<<BLANC;

        if(not pr and v_size_1.peak_token.pt_data != pep->special_peaks[Parser::peptide::FIN])
            cout<<BLEU<<"-<"<<pep->special_peaks[Parser::peptide::FIN]->masse<<">"<<BLANC;
    }
    cout<<endl;
};

std::string AnalyseurPeptide::to_string(const AnalyseurPeptide::v_tokens_ptr& v)
{
    const int size = v.size();
    string res = "";
    for(int i=2;i<size;++i)
    {
        const stack_token& v_i = *v[i];
        if(v_i.type == AnalyseurPeptide::stack_token::AA_TOKEN)
        {
            if (i > 2)
                res+="|";
            res += aa_tab[v_i.aa_token.index].slug;
        }
    }
    return res;
};



const AnalyseurPeptide::v_tokens_ptr* AnalyseurPeptide::get_near(const int index, const short int inc)//[peaks,AA]*
{
    AnalyseurPeptide::v_tokens_ptr* res = new AnalyseurPeptide::v_tokens_ptr;
    
    const unsigned int size_pep = pep->peaks.size();
    const unsigned int size_aa = aa_tab.length;
    const double initial_masse = pep->peaks[index]->masse;
    const static double max_masse = aa_tab.get_max_masse();

    if (inc >= 0) //++i Sens::RIGHT
    {
        for (unsigned int i=index+1;i<size_pep;++i) // loop peaks
        {
            if (*(pep->peaks[i]->used) /*or not pep->peaks[i]->bruit*/) // si il est déja pris
                continue;

            const double current_masse = pep->peaks[i]->masse;
            if (initial_masse + max_masse + erreur < current_masse) // si la masse est <= que la masse du plus gros AA (on cherche ici que les peak corespondant à 1 AA)
                break;

            for(unsigned register int j=0;j<size_aa;++j) //on cherche l'AA qui corespond à cette différence de masse
            {
                const double aa_masse = initial_masse + aa_tab[j].masse;
                if(eq_error(current_masse,aa_masse,erreur)) // avec une marge d'erreur
                {
                    stack_token* tmp = new stack_token(i,pep->peaks[i]);
                    tokens_ptr.emplace_back(tmp);

                    tmp = new stack_token(j,current_masse - aa_masse,tmp);
                    tokens_ptr.emplace_back(tmp);
                    res->emplace_back(tokens_ptr.back());
                }
            }
        }
    }
    else //--i Sens::LEFT
    {
        for (int i=index-1;i>=0;--i) // loop peaks
        {
            if (*(pep->peaks[i]->used) /*or not pep->peaks[i]->bruit*/) // si il est déja pris
                continue;

            const double current_masse = pep->peaks[i]->masse;
            if (initial_masse - max_masse -erreur > current_masse) // si la masse est >= que la masse du plus gros AA (on cherche ici que les peak corespondant à 1 AA)
                break;

            for(unsigned int j=0;j<size_aa;++j) //on cherche l'AA qui corespond à cette différence de masse
            {
                const double aa_masse = initial_masse - aa_tab[j].masse;
                if(eq_error(current_masse,aa_masse,erreur)) // avec une marge d'erreur
                {
                    tokens_ptr.emplace_back(new stack_token(i,pep->peaks[i]));
                    tokens_ptr.emplace_back(new stack_token(j,current_masse - aa_masse,tokens_ptr.back()));
                    res->emplace_back(tokens_ptr.back());
                }
            }
        }
    }
    return res;
};


void AnalyseurPeptide::merge_solution(std::list<AnalyseurPeptide::v_tokens_ptr>& left_part,const std::list<AnalyseurPeptide::v_tokens_ptr>& right_part)
{

    auto l_end = left_part.end();
    auto r_end = right_part.end();
    auto l_begin = left_part.begin();
    auto r_begin = right_part.begin();

    #ifndef APPRENTISSAGE
    double tmp_values[VALUES_SIZE];
    #endif
    int _size = 0;

    for(auto i=l_begin; i != l_end; ++i)
    {
        if(i->size() > 2) //il y a au moins 1 AA
        {
            for(auto j=r_begin; j!= r_end; ++j)
            {
                if(j->size() > 2) //il y a au moins 1 AA
                {
                    v_tokens_ptr tmp= (*i);
                    stack_token* tmp_head = new stack_token(*tmp[0]);
                    tokens_ptr.emplace_back(tmp_head);//copy it
                    tmp[0] = tmp_head;
                    stack_token& i_0 = *tmp[0];
                    stack_token& j_0 = *(*j)[0];
                    //fusion des header
                    i_0.header_token.holds[Parser::peptide::FIN_H2O].link = j_0.header_token.holds[Parser::peptide::FIN_H2O].link;
                    i_0.header_token.holds[Parser::peptide::FIN_H2O].to_find = j_0.header_token.holds[Parser::peptide::FIN_H2O].to_find;

                    i_0.header_token.holds[Parser::peptide::FIN].link = j_0.header_token.holds[Parser::peptide::FIN].link;
                    i_0.header_token.holds[Parser::peptide::FIN].to_find = j_0.header_token.holds[Parser::peptide::FIN].to_find;
                    //ajout du noyeau (header-peak(en commun)-[AA -peak]* )
                    copy((*j).begin()+2,(*j).end(),back_inserter(tmp)); 
                    //ajout du nouveau
                    
                    //TODO
                    #if DEBUG & DEBUG_STATS 
                    //++calc_stats[k-1][NB_SOL_FIND];
                    #endif

                    //SCORE
                    #ifndef APPRENTISSAGE
                    //calc_values(tmp_values,tmp,pep);
                    //tmp[0]->header_token.score = calc_score(tmp_values);
                    #endif

                    /*finds.emplace_back(move(tmp));

                    #ifndef APPRENTISSAGE
                    if(finds_max_size > 0 and ++_size > finds_max_size*5)
                    {
                        const auto& _begin = finds.begin();
                        partial_sort(_begin,_begin+finds_max_size,finds.end(),solution_gt);
                        finds.resize(finds_max_size);
                        _size = finds_max_size;
                    }
                    #endif
                    */
                    /*
                    v_tokens_ptr tmp= (*i);
                    stack_token* tmp_head = new stack_token(*tmp[0]);
                    stack_token& i_0 = *tmp_head;
                    stack_token& j_0 = *(*j)[0];
                    //fusion des header
                    i_0.header_token.holds[Parser::peptide::FIN_H2O].link = j_0.header_token.holds[Parser::peptide::FIN_H2O].link;
                    i_0.header_token.holds[Parser::peptide::FIN_H2O].to_find = j_0.header_token.holds[Parser::peptide::FIN_H2O].to_find;

                    i_0.header_token.holds[Parser::peptide::FIN].link = j_0.header_token.holds[Parser::peptide::FIN].link;
                    i_0.header_token.holds[Parser::peptide::FIN].to_find = j_0.header_token.holds[Parser::peptide::FIN].to_find;
                    //ajout du noyeau (header-peak(en commun)-[AA -peak]* )
                    copy((*j).begin()+2,(*j).end(),back_inserter(tmp)); 
                    //ajout du nouveau
                    
                    //TODO
                    #if DEBUG & DEBUG_STATS 
                    ++calc_stats[k-1][NB_SOL_FIND];
                    #endif

                    //SCORE
                    #ifndef APPRENTISSAGE
                    calc_values(tmp_values,tmp,pep);
                    i_0.header_token.score = calc_score(tmp_values);
                    #endif

                    if(_size < finds_max_size or i_0.header_token.score > finds[_size-1][0]->header_token.score)
                    {
                        tokens_ptr.emplace_back(tmp_head);//save to be delete at the end
                        finds.emplace_back(move(tmp));
                        ++_size;
                    }

                    #ifndef APPRENTISSAGE
                    if(finds_max_size > 0 and _size > finds_max_size*5)
                    {
                        const auto& _begin = finds.begin();
                        partial_sort(_begin,_begin+finds_max_size,finds.end(),solution_gt);
                        finds.resize(finds_max_size);
                        _size = finds_max_size;
                    }
                    #endif
                    */
                }
            }
        }
    }
    

    #ifndef APPRENTISSAGE
    /*const auto& _begin = finds.begin();
    partial_sort(_begin,_begin+finds_max_size,finds.end(),solution_gt); //au cas ou tous les peaks n'ont pas eu de charge spécifiées
    if(_size > finds_max_size)
    {
        finds.resize(finds_max_size);
    }*/
    #endif

    #if DEBUG & DEBUG_MERGE
    for(auto i=finds.begin();i != finds.end();++i)
    {
        cout<<" ++++++++++++++++ MERGE ++++++++++++++++++++++++++"<<endl;
        print_AA((*i));
        cout<<" +++++++++++++++++++ END MERGE ++++++++++++++++++++ "<<endl;
    }
    #endif
};


inline void filter_AA(std::list<list_arg>& link,std::vector<int>& aa_ids)
{
    #if DEBUG & DEBUG_FILTER 
    int nb = 0;
    #endif

    if (aa_ids.size()>0)
    {
        auto ez_d_end = aa_ids.end();
        for(auto sols = link.begin(); sols != link.end();) //loop sur les solutions
        {
            bool find = false;
            //pour toute les coupure possible (des fois vide)
            for(auto ez_aa = aa_ids.begin();ez_aa != ez_d_end && not find ;++ez_aa)
            {
                // est se que la solution contien un des AA
                for( auto aa=sols->list.begin(); aa != sols->list.end() && not find;++aa)
                    find = ((*aa).pt->get_pk() == aa_tab[(*ez_aa)].get_pk());
            }

            if (not find)
            {
                sols = link.erase(sols);
            }
            else
            {
                #if DEBUG & DEBUG_FILTER 
                for( auto aa=sols->list.begin(); aa != sols->list.end();++aa)
                    (*aa).__print__();
                cout<<endl;
                nb++;
                #endif

                ++sols;
            }
        }
    }
    #if DEBUG & DEBUG_FILTER 
    cout<<"solution valides: "<<nb<<endl;
    #endif
}

void AnalyseurPeptide::filter_enzyme(std::list<AnalyseurPeptide::v_tokens_ptr>& solutions,Enzyme& enz)
{
    #warning "faire les combinaisons"

    #if DEBUG & DEBUG_FILTER 
    cout<<"********************* FILTER ENZYME **************************************"<<endl;
    #endif

    for(auto i = solutions.begin(); i != solutions.end();++i) //pour chaque solution possible
    {
        #if DEBUG & DEBUG_FILTER 
        print_AA(*i);
        #endif

        //vérifier que un AA du debut est un de ceux qui sont autorisés "coupure_avant"
        #if DEBUG & DEBUG_FILTER 
        cout<<" DEBUT "<<endl;
        #endif
        std::shared_ptr<std::list<list_arg> > link= (*i)[0]->header_token.holds[Parser::peptide::DEBUT].link;
        if (link.get())
            filter_AA(*link,enz.coupure_apres);
        #if DEBUG & DEBUG_FILTER 
        if (link.get())
        {
            aa_tab.print(*link,nb_affiche>=0?nb_affiche:link->size());
        }
        #endif
       

        //vérifier que un AA du debut est un de ceux qui sont autorisés "coupure_avant"
        #if DEBUG & DEBUG_FILTER 
        cout<<"DEBU_H2O ";
        #endif
        link= (*i)[0]->header_token.holds[Parser::peptide::DEBUT_H2O].link;
        if ( link.get())
            filter_AA(*link,enz.coupure_apres);

        #if DEBUG & DEBUG_FILTER 
        if (link.get())
        {
            aa_tab.print(*link,nb_affiche>=0?nb_affiche:link->size());
        }
        #endif

        //vérifier que un AA de la fin est un de ceux qui sont autorisés "coupure_apres"
        #if DEBUG & DEBUG_FILTER 
        cout<<"FIN_H2O ";
        #endif
        link= (*i)[0]->header_token.holds[Parser::peptide::FIN_H2O].link;
        if (link.get())
            filter_AA(*link,enz.coupure_avant);

        #if DEBUG & DEBUG_FILTER 
        if (link.get())
        {
            aa_tab.print(*link,nb_affiche>=0?nb_affiche:link->size());
        }
        #endif

        //vérifier que un AA de la fin est un de ceux qui sont autorisés "coupure_apres"
        #if DEBUG & DEBUG_FILTER 
        cout<<"FIN ";
        #endif
        link= (*i)[0]->header_token.holds[Parser::peptide::FIN].link;
        if (link.get())
            filter_AA(*link,enz.coupure_avant);

        #if DEBUG & DEBUG_FILTER 
        if (link.get())
        {
            aa_tab.print(*link,nb_affiche>=0?nb_affiche:link->size());
        }
        #endif
    }
    #if DEBUG & DEBUG_FILTER 
    cout<<"********************* END FILTER ENZYME **************************************"<<endl;
    #endif
};

//////////////////////// stack token ///////////////////////////////////

AnalyseurPeptide::stack_token::~stack_token()
{
    switch(type)
    {
        case AA_TOKEN:
        {
            //aa_token.pt_data.~shared_ptr<stack_token>();
        }break;

        case AA_HOLD_TOKEN :
        {
            aa_hold_token.aa_index.~vector<int>();
        }break;

        case PEAK_TOKEN : 
        {
        }break;

        case HEADER_TOKEN : 
        {
            for (int i=0;i<Parser::peptide::SIZE;++i)
                header_token.holds[i].link.~shared_ptr<list<list_arg> >();
        }break;

        default : break;
    }
};

AnalyseurPeptide::stack_token::stack_token(AnalyseurPeptide::stack_token&& other)
{
    type = other.type;
    switch (type)
    {
        case AA_TOKEN:
        {
            //new(&aa_token.pt_data) shared_ptr<stack_token>();
            swap(aa_token,other.aa_token);
            /*
            swap(aa_token.index,other.aa_token.index);
            swap(aa_token.error,other.aa_token.error);
            swap(aa_token.pt_data,other.aa_token.pt_data);
            */
        }break;

        case AA_HOLD_TOKEN :
        {
            //new(&aa_hold_token.aa_index) vector<int>();
            swap(aa_hold_token,other.aa_hold_token);
            /*
            swap(aa_hold_token.error,other.aa_hold_token.error);
            swap(aa_hold_token.aa_index,other.aa_hold_token.aa_index);
            */

        }break;

        case PEAK_TOKEN : 
        {
            swap(peak_token,other.peak_token);
            /*
            swap(peak_token.index,other.peak_token.index);
            swap(peak_token.pt_data,other.peak_token.pt_data);
            */
        }break;

        case HEADER_TOKEN : 
        {
            swap(header_token,other.header_token);
            /*
            swap(header_token.score,other.header_token.score);
            for(int i=0;i<Parser::peptide::SIZE;++i)
            {
                new (&header_token.holds[i].link) shared_ptr<list<list_arg> >;
                swap(header_token.holds[i],other.header_token.holds[i]);

                //swap(header_token.holds[i].link,other.header_token.holds[i].link);
                //swap(header_token.holds[i].to_find,other.header_token.holds[i].to_find);
            }*/
        }break;

        default : break;
    }
    other.type = UNKNOW;
};

AnalyseurPeptide::stack_token::stack_token()
{
    this->type = UNKNOW;
};

AnalyseurPeptide::stack_token::stack_token(const int i,Parser::peptide::peak* data) //peak
{
    this->type=PEAK_TOKEN;
    peak_token.index = i;
    peak_token.pt_data = data;
};

AnalyseurPeptide::stack_token::stack_token(const int i,const double e,AnalyseurPeptide::stack_token* peak) //aa
{
    this->type=AA_TOKEN;
    aa_token.error =e;
    aa_token.index = i;
    aa_token.pt_data = peak;
};

AnalyseurPeptide::stack_token::stack_token(const int i) //aa pour boucher un trou
{
    this->type=AA_HOLD_TOKEN;
    aa_hold_token.error = 0;
    //aa_hold_token.pt_data = this;
    new(&aa_hold_token.aa_index) vector<int>();
};


AnalyseurPeptide::stack_token::stack_token(const double score) //HEADER
{
    this->type = HEADER_TOKEN;
    header_token.score = score;
    for (int i=0;i<Parser::peptide::SIZE;++i)
    {
        new (&header_token.holds[i].link) shared_ptr<list<list_arg> >;
    }
};

AnalyseurPeptide::stack_token::stack_token(const AnalyseurPeptide::stack_token& other)
{
    type = other.type;
    switch(type)
    {
        case AA_TOKEN:
            //new(&aa_token.pt_data) shared_ptr<stack_token>();
            aa_token = other.aa_token;
        break;

        case AA_HOLD_TOKEN :
            new(&aa_hold_token.aa_index) vector<int>();
            aa_hold_token = other.aa_hold_token;
        break;

        case PEAK_TOKEN :
            peak_token = other.peak_token;
        break;

        case HEADER_TOKEN : 
            this->type = HEADER_TOKEN;
            for (int i=0;i<Parser::peptide::SIZE;++i)
                new (&header_token.holds[i].link) shared_ptr<list<list_arg> >;

            header_token.score = other.header_token.score;

            for (int i=0; i< Parser::peptide::SIZE;++i)
            {
                header_token.holds[i].link = other.header_token.holds[i].link;
                header_token.holds[i].to_find = other.header_token.holds[i].to_find;
            }
        break;

        default : break;
    }

}

AnalyseurPeptide::stack_token& AnalyseurPeptide::stack_token::operator=(const AnalyseurPeptide::stack_token& other)
{
    this->~stack_token();

    type = other.type;
    switch(type)
    {
        case AA_TOKEN:
            aa_token = other.aa_token;
        break;

        case AA_HOLD_TOKEN :
            aa_hold_token = other.aa_hold_token;
        break;

        case PEAK_TOKEN :
            peak_token = other.peak_token;
        break;

        case HEADER_TOKEN : 
            header_token.score = other.header_token.score;
            for (int i=0; i< Parser::peptide::SIZE;++i)
            {
                header_token.holds[i].link = other.header_token.holds[i].link;
                header_token.holds[i].to_find = other.header_token.holds[i].to_find;
            }
        break;

        default : break;
    }
    return *this;
};

AnalyseurPeptide::stack_token& AnalyseurPeptide::stack_token::operator=(AnalyseurPeptide::stack_token&& other)
{
    swap(type,other.type);
    switch (type)
    {
        case AA_TOKEN:
        {
            swap(aa_token.index,other.aa_token.index);
            swap(aa_token.error,other.aa_token.error);
            swap(aa_token.pt_data,other.aa_token.pt_data);
        }break;

        case AA_HOLD_TOKEN :
        {
            swap(aa_hold_token.error,other.aa_hold_token.error);
            //swap(aa_hold_token.pt_data,other.aa_hold_token.pt_data);
            swap(aa_hold_token.aa_index,other.aa_hold_token.aa_index);

        }break;

        case PEAK_TOKEN : 
        {
            swap(peak_token.index,other.peak_token.index);
            swap(peak_token.pt_data,other.peak_token.pt_data);
        }break;

        case HEADER_TOKEN : 
        {
            swap(header_token.score,other.header_token.score);
            for(int i=0;i<Parser::peptide::SIZE;++i)
            {
                swap(header_token.holds[i].link,other.header_token.holds[i].link);
                swap(header_token.holds[i].to_find,other.header_token.holds[i].to_find);
            }
        }break;

        default : break;
    }
    return *this;
};

void AnalyseurPeptide::stack_token::__print__() const
{
    if (type==AA_TOKEN)
    {
        std::cout<<"AA_TOKEN error:"<<aa_token.error<<" "<<aa_tab[aa_token.index].slug<<" ";
        if (aa_token.pt_data != NULL)
            aa_token.pt_data->peak_token.pt_data->__print__();
        else
            std::cout<<std::endl;
    }
    else if (type == PEAK_TOKEN)
    {
        std::cout<<"PEAKS_TOKEN ";
        peak_token.pt_data->__print__();
    }
    else if (type == HEADER_TOKEN)
    {
        std::cout<<"HEADER_TOKEN score: "<<header_token.score<<std::endl;
        for(int i=0;i<Parser::peptide::SIZE;++i)
        {
            cout<<i<<" to find : "<<header_token.holds[i].to_find<<"("<<(header_token.holds[i].link.get()?header_token.holds[i].link->size():0)<<")"<<endl;
            if (header_token.holds[i].link.get())
                aa_tab.print(*header_token.holds[i].link,header_token.holds[i].link->size());
        }
    }
};

#ifdef APPRENTISSAGE
//////////////////////////////////////// ApprentissageSolution ////////////////////////////////
AnalyseurPeptide::ApprentissageSolution::ApprentissageSolution(const AnalyseurPeptide::v_tokens_ptr& s,const Parser::peptide* const pep)
{

    score = 0;
    real_score = 0;
    //reset
    calc_values(values,s,pep);

    solutions_reals = pep->solutions;

    /* > ERROR_TOTAL
     * > ERROR_AA_CUMUL
     * > INTENSITEE_TOTAL_PARENT
     * > INTENSITEE_TOTAL
     * > MASSE_TOTAL
     * > PERCENT_COUVERTURE
     //* PERCENT_COUVERTURE_AA
     * > NB_AA
     * > NB_PEAKS
     * > MASSE_PARENT
     * > PERCENT_INTENSITEE_UTILISEE
     */
    auto i=s.begin();
    auto end = s.end();

    while(i!=end)
    {
        const stack_token& ii = **i;
        if (ii.type == AnalyseurPeptide::stack_token::Type::AA_TOKEN)
        {
            if(values[NB_AA])
                solution+="|";

            solution+=aa_tab[ii.aa_token.index].slug;
        }
        ++i;
    }
    check_validity();
};

void AnalyseurPeptide::ApprentissageSolution::__print__()
{
    cout<<COMMENTAIRE<<"["<<score<<"]";
    if(validity)
        cout<<LMAGENTA<<"["<<validity<<"]";
    else
        cout<<ROUGE<<"[0]";

    cout<<BLEU<<" "<<solution
    <<JAUNE<<" (";
    for (string& s : *solutions_reals)
        cout<<s<<",";

    cout<<") "<<BLANC
        <<"{ERROR_TOTAL:"<<values[ERROR_TOTAL]
        <<", ERROR_AA_CUMUL:"<<values[ERROR_AA_CUMUL]
        <<", INTENSITEE_TOTAL_PARENT:"<<values[INTENSITEE_TOTAL_PARENT]
        <<", INTENSITEE_TOTAL:"<<values[INTENSITEE_TOTAL]
        <<", MASSE_TOTAL:"<<values[MASSE_TOTAL]
        <<", PERCENT_COUVERTURE:"<<values[PERCENT_COUVERTURE]
        <<", NB_AA:"<<values[NB_AA]
        <<", NB_PEAKS:"<<values[NB_PEAKS]
        <<", MASSE_PARENT:"<<values[MASSE_PARENT]
        <<", PERCENT_INTENSITEE_UTILISEE:"<<values[PERCENT_INTENSITEE_UTILISEE]<<"}"<<endl;
};

void AnalyseurPeptide::ApprentissageSolution::check_validity()
{
    validity = 0;
    if(not solution.empty())
    {
        for(auto s : *solutions_reals)
        {
            if(s.find(solution) != std::string::npos)//la solution est valide
            {
                int tmp = count(solution.begin(),solution.end(),'|') +1;//nombre de AA
                validity=MAX(validity,tmp);
            }
        }
    }
};


#if DEBUG & DEBUG_STATS
void AnalyseurPeptide::verifier_resultats_complet(list<AnalyseurPeptide::v_tokens_ptr>& s,int boucle)
#else
void AnalyseurPeptide::verifier_resultats_complet(list<AnalyseurPeptide::v_tokens_ptr>& s)
#endif
{
    #warning "le code acctuel ne donne pas tout, PE regarder les plus longue véquense commune?"
    #if DEBUG & DEBUG_VERIFICATION
    cout<<" ***************** VERIFICATION RESULTAT COMPLET ***********"<<endl;
    #endif
    //loop sur les solutions trouvées
    for(auto current_sol= s.begin();current_sol != s.end();++current_sol)
    {
        string value = to_string(*current_sol);
        //loop sur tout les solution théoriques du mgf  (SOLUTION=xxx,yyy...)
        for(vector<string>::iterator i_str = pep->solutions->begin(); i_str != pep->solutions->end(); ++i_str)
        {
            //si une solution théorique contient le noyeau (value)
            int find = (*i_str).find(value);
            if(find != string::npos)
            {
                //on l'ajoute parmis les solutions possibles
                #warning "TODO ajouter le résultat dans un tableau ou autre"
                #if DEBUG & DEBUG_VERIFICATION
                cout<<"solution: "<<(*i_str)<<" trouvé: "<<value<<" nb AA: "<<find<<endl;
                #endif
                //et on regarde pour les bords (mais n'influe pas sur le score)
                #if DEBUG & DEBUG_STATS
                ++calc_stats[boucle][NB_SOL_FIND_OK];
                #endif
            }
        }
    }

    #if DEBUG & DEBUG_VERIFICATION
    cout<<" *************** END VERIFICATION RESULTAT COMPLET *********"<<endl;
    #endif

};

#endif
