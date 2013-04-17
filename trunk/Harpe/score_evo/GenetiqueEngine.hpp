#ifndef GENETIQUEENGINE_HPP
#define GENETIQUEENGINE_HPP

#include <utility>
#include <functional>
#include <algorithm>
#include <fstream>
#include <ctime>
#include "IndividuTree.hpp"
#include "random.hpp"



template <class T>
class GenetiqueEngine
{
    public:
        template <typename ... Args>
        GenetiqueEngine(int pop_size,Args ... args) : size(pop_size)
        {
            individus = new T*[pop_size];
            for(int i=0;i<pop_size;++i)
                individus[i] = new T(std::forward<Args>(args) ...);
        };

        ~GenetiqueEngine()
        {
            for(int i=0;i<size;++i)
                if(individus[i])
                    delete individus[i];
            delete individus;
        };

        //return le meilleur
        template <typename ... Args>
        T* run(const int nb_generation,const int size_enf,Args& ... args)
        {
            //eval initiale
            init(args ...);
            //boucle de génération
            for(int generation=0;generation<nb_generation;++generation)
                corps(size_enf,args ...);
            return end();
        };
        
        template <typename ... Args>
        T* run_while(bool (*f)(int best_score,Args ...),const int size_enf,Args& ... args)
        {
            //eval initiale
            init(args ...);
            int best;
            do
            {
                corps(size_enf,args ...);
                best = individus[0]->get_score();
            }while (not f(best,args...));

            return end();
        };


        struct gt_ptr : std::binary_function<T,T,bool>
        {
            bool operator() (const T* x, const T* y) const {return *x>*y;}
        };

           
    private:
        T** individus;
        const int size;

        template <typename ... Args>
        inline void init(Args& ... args)
        {
            for(int i=0;i<size;++i)
                individus[i]->eval(args...);
        };

        template <typename ... Args>
        inline void corps(const int size_enf,Args& ... args)
        {
            std::partial_sort(individus,individus+(size-size_enf),individus+size,gt_ptr());//en tri les size - size_enf
            //creation des enfants + evaluation
            T* enfants[size_enf];
            for(int i=0;i<size_enf;++i)//on prend que les meilleur, mais avec random
            {
                enfants[i] = individus[i]->crossOver(*individus[random(0,size-1)]);
                if (random(0.f,1.f) > 0.9)//Mutation !
                    enfants[i]->mutate();

                for (int j=100;j<enfants[i]->size();j+=100)
                    enfants[i]->mutate();

                enfants[i]->eval(args...);
            }
            //on mélange un peu histoir d'aider les mauvais en gardant un bout des bon (quand même)...
            
            /*const int borne = (size - size_enf)*0.85;
            std::shuffle(individus+borne, individus+size-size_enf, generator);
            */

            //réduction population
            for(int i=0;i<size_enf;++i)
            {
                std::swap(individus[size-size_enf+i],enfants[i]);
                delete enfants[i];
            }

            //evite de trop stagner
            for(int i=0;i<size;i+=100)
            {
                int j = random(1,size-1);
                individus[j]->mutate();
                individus[j]->eval(args ...);
            }

            static int generation = 0;
            std::cout<<"generation #"<<generation++<<std::endl; 
            #if GENETIQUE_SAVE_RESULTS
            {
                std::string filename = "fonction_finds/"+std::to_string(individus[0]->get_score())+".hpp";
                if(not std::ifstream(filename))
                {
                    time_t temps;
                    struct tm* datetime;
                    char  format[32];
                    time(&temps);
                    datetime = localtime(&temps);
                    strftime(format, 32, "%d-%m-%Y %H:%M", datetime);

                    std::ofstream file;
                    T* indiv = individus[0]->clone();
                    indiv->minimize();
                    file.open(filename);
                    file<<"/*"
                        <<"\n*\tdate: "<<format
                        <<"\n*\tscore: "<<indiv->get_score()
                        <<"\n*\tsize: "<<indiv->size()
                        <<"\n*\tgeneration: "<<generation
                        <<"\n*/"
                        <<"\n#include <math.h>"
                        <<"\ndouble calc_score(const double* const vals)"
                        <<"\n{"
                        <<"\n\tdouble res="<<*indiv<<";"
                        <<"\n\t return (res>="<<indiv->seuil<<")?res:0;"
                        <<"\n}";
                    file.close();
                    
                    std::cout<<format<<" best("<<indiv->get_score()<<"): "<<*indiv<<std::endl<<std::endl;
                    delete indiv;
                }
            }
            #endif
        };

        inline T* end()
        {
            std::partial_sort(individus,individus+1,individus+size,gt_ptr());//en tri les size - size_enf
            //on renvoi le meilleur
            T* res = individus[0];
            individus[0] = 0;
            return res;
        };
};

#endif
