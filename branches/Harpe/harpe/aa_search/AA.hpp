#ifndef AA_HPP
#define AA_HPP
//#include <stdio.h>
#include <iostream>
#include <algorithm>

#include "../sql/SqlObject.hpp"

class AA : public SqlObject<AA> {
    public:
        AA(const char* name,const char id, double masse);
        AA(const char* name,const char* slug,const char id, double masse);
        AA();
        ~AA();
        friend std::ostream& operator<<(std::ostream& output,const AA& p);

        inline void __print__() const {std::cout<<slug<<" "<<masse<<std::endl;};

        bool operator < (const AA& other) const {return this->masse < other.masse;};

        //friend class AA_Tab;
        char name[256];
        char slug[16];
        int id;
        double masse;
        

        AA(sql::ResultSet* query);
        void save(bool commit=true);
        static const std::string TableName;//= "website_aa";
};


#endif
