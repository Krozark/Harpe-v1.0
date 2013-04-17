#include <iostream>
#include <vector>
#include <stddef.h>


union u_test {

    std::vector<int> v_i;
    int* tab;

    u_test(){
        new(&v_i) std::vector<int>;
    }

    ~u_test(){
        v_i.~vector<int>();
    }
};

class P{
    public:
    P(int ii): i(ii), d (ii){
    }

    int i;
    double d;
};

union u_test2 {

    struct __1 {
        std::vector<P> v;
    } _1;

    struct __2 {
        int* tab;
    } _2;

    u_test2(){
        new(&_1.v) std::vector<P>;
    };

    ~u_test2(){
        _1.v.~vector<P>();
    };
};


class A{

    public :
    A(int t) : type((A::Type)t) {
        std::cout<<"--A---"<<std::endl;
        std::cout<<type<<std::endl;
        switch(type)
        {
            case TYPE1: std::cout<<"type1"<<std::endl;break;
            case TYPE2: std::cout<<"type2"<<std::endl;break;
            case TYPE3: std::cout<<"type3"<<std::endl;break;
            default: std::cout<<"default"<<std::endl;break;
        }
        std::cout<<"-- end A --"<<std::endl;
    };

    ~A(){}
    
    //private:

        enum Type {TYPE1=0,TYPE2,TYPE3} type;

        union{
            struct{
                std::vector<int> v;
            }type1;

            struct{
                std::vector<float> v;
            }type2;

            struct{
                double d;
            } type3;

            /*U() {
                cout<<" ++ U ++"<<endl;
                A* pa = static_cast<A*>((void*)(this - offsetof(A,u)));
                cout<<pa->type<<endl;
                switch(pa->type)
                {
                    case TYPE1: cout<<"type1"<<endl;break;
                    case TYPE2: cout<<"type2"<<endl;break;
                    case TYPE3: cout<<"type3"<<endl;break;
                    default: cout<<"default"<<endl;break;
                }

                cout<<"++ end U ++"<<endl;
            }

            ~U(){};*/
        };
};

#include <list>
#include <memory>

class B {
    public:
    B(int t) : type(B::Type(t))
    {
        switch (t)
        {
            case T1:
            {
                for (int i=0;i<4;++i)
                new(&_1.t[i].l) std::list<int>();
            }break;

            case T2 : 
            {
            }break;

            default : exit(1);
        }

    }

    ~B()
    {
        switch (type)
        {
            case T1:
            {
                for(int i=0;i<4;++i)
                _1.t[i].l.~list<int>();
            } break;
            default : break;
        }
    };

    B(const B& other)
    {

    }

    enum Type {T1=0,T2} type;

    union {
        struct {
            float f;
            double d;
            struct {
                float f;
                std::list<int> l;
            } t[4];
        } _1;

        struct {
            int i;
            double d;
        } _2;
    };
};
//#define test 4
int main(int agc, char* argf[])
{
    /*#if test == 1
    u_test t;

    t.v_i.push_back(12);

    for(int &i : t.v_i)
        std::cout<<i<<std::endl;
    std::cout<<t.tab[0]<<std::endl;

    std::cout<<"****************"<<std::endl;
    #elif test == 2
    u_test2 t2;

    t2._1.v.emplace_back(12);

    for(int i=0; i<t2._1.v.size();++i)
        std::cout<<t2._1.v[i].i<<std::endl;

    std::cout<<t2._2.tab[0]<<std::endl;
    #elif test == 3

    A a(1);

    #elif test == 4*/
    B b1(0);

    b1._1.f =42.1;

    for (int i=0;i<10;++i)
        b1._1.t[2].l.push_back(i);

    for (auto i=b1._1.t[2].l.begin();i != b1._1.t[2].l.end();++i)
        std::cout<<(*i)<<std::endl;

    B b2 = b1;
    for (auto i=b2._1.t[2].l.begin();i != b2._1.t[2].l.end();++i)
        std::cout<<(*i)<<std::endl;
    
    b2._2.i = 45;

    //#endif
    return 0;
};
