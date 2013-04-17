#ifndef INDIVIDULINEAR_HPP
#define INDIVIDULINEAR_HPP

#include <cstdlib>
#include <iostream>

template<int I>
class IndividuLinear
{
    public:
        IndividuLinear(const float t=1.0):size(I), score(0), tauxMutation(t)
        {
            for(unsigned int i=0;i<size;++i)
                coefs[i]=0;
        };

        void mutate() //fait mutter un coef en ajoutant [-1,1]
        {
            float rf = std::rand() / (float)RAND_MAX;
            if (rf <= tauxMutation)
            {
                int ri = std::rand() / (float)(RAND_MAX / I);
                rf = std::rand() / (float)(RAND_MAX / 2) -1;
                coefs[ri] += rf;
            }
        };

        void crossOverMulti(IndividuLinear<I>& other)
        {
            int i=0;
            bool swap = true;
            while (i < I)
            {
                int ri = std::rand() / (float)(RAND_MAX / I) + i;
                if (swap)
                    for(;i < ri && i < I; ++i)
                    {
                        double tmp = (*this)[i];
                        (*this)[i] = other[i];
                        other[i] = tmp;
                    }
                else
                    i = ri;
                swap = !swap;
            }
        };

        inline void crossOverOne(IndividuLinear<I>& other)
        {
            int ri = std::rand() / (float)(RAND_MAX / I);
            for(int i=0;i < ri && i < I; ++i)
            {
                double tmp = (*this)[i];
                (*this)[i] = other[i];
                other[i] = tmp;
            }
        };

        void __print__()
        {
            for(unsigned int i=0;i<size;++i)
                std::cout<<coefs[i]<<" ";
            std::cout<<std::endl;
        };

        double& operator[] (const int& c) {return coefs[c];}
        const double& operator[] (const int& c) const {return coefs[c];}
        

    private:
        float score;
        int size;
        double coefs[I];
        const float tauxMutation;
};

#endif
