#ifndef MODELS_HPP
#define MODELS_HPP

#include <unordered_map> 
#include <vector>
#include <sstream>
#include <utility>

#include "config.hpp"

inline void addToSS(std::stringstream& ss)
{
    ss<<";";
};

template<typename T, typename ... Args>
inline void addToSS(std::stringstream& ss,T v,Args&& ... args)
{
    ss<<v<<" ";
    addToSS(ss,std::forward<decltype(args)>(args)...);
};

template<typename T, typename ... Args>
inline void addToSSComa(std::stringstream& ss,T v,Args&& ... args)
{
    ss<<v;
    if (sizeof ... (args) > 1)
    {
        ss<<",";
        addToSS(ss,std::forward<decltype(args)>(args)...);
    }
};

/*
 * classe qui permet de lier un objets c++
 * à la base de donnée avec quelques methodes prédéfiniee.
 * L'objet c++ doit en hériter
 * Class MaClasse : public SqlObject<MaClasse> {...};
 */
template<class T>
class SqlObject
{
    /** A DEFINIR dans les sous classes :
    * T::T(sql::ResultSet* query);
    * void save(bool commit=false) [ if ( not commit) update(...) else insert(...) si ;
    * string TableName;
    **/

    public:
        virtual void save(bool commit=false) =0;
        static T get(int pk)
        {
           std::string req = "SELECT * from "+T::TableName+" where id = "+std::to_string(pk)+";";

           bdd_res = bdd->executeQuery(req);
           bdd_res->next();
           T res = T(bdd_res);
           delete bdd_res;
           return res;
        };

        template <typename ... Args>
        void update(Args&& ... args) const
        {
            
           std::stringstream ss;
           ss<<"UPDATE " << T::TableName <<" set ";
           addToSS(ss,std::forward<Args>(args)...);
           ss<<"where id="<<this->pk;

           bdd_res = bdd->executeQuery(ss.str()); 
           delete bdd_res;
        };
        
        template <typename ... Args>
        void insert(const std::string into,Args ... args) const
        {
           std::stringstream ss;
           ss<<"INSERT into "<<T::TableName<<"( "<<into<<") values (";
           addToSSComa(ss,std::forward<Args>(args)...);
           ss<<");";

           bdd_res = bdd->executeQuery(ss.str()); 
           delete bdd_res;
        };

        template <typename ... Args>
        static std::vector<T> filter(Args&& ... args)
        {
           std::stringstream ss;
           ss<<"SELECT * from " << T::TableName <<" where ";
           addToSS(ss,std::forward<Args>(args)...);

           bdd_res = bdd->executeQuery(ss.str()); 

           std::vector<T> res;

           while (bdd_res->next())
               res.emplace_back(T(bdd_res));
           delete bdd_res;

           return res;
        };

        static std::vector<T> all()
        {
           std::stringstream ss;
           ss<<"SELECT * from " << T::TableName <<";";
           bdd_res = bdd->executeQuery(ss.str()); 

           std::vector<T> res;

           while (bdd_res->next())
               res.emplace_back(T(bdd_res));
           delete bdd_res;

           return res;
        }

        //getInt(colname/num)
        //
        const int get_pk() const {return pk;};
    protected:
        int pk;
};

#endif
