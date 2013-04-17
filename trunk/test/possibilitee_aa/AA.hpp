#ifndef AA_HPP
#define AA_HPP
//#include <stdio.h>
#include <iostream>

class AA {
    public:
        AA(const char* name,const char id, double masse);
        AA();
        ~AA();

        inline void __print__(){std::cout<<name<<" "<<masse<<std::endl;};

        //friend class AA_Tab;
        char name[256];
        int id;
        double masse;
};

#endif
