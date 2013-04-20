#include <typeinfo>
#include <cxxabi.h>
#include <map>

class Base{};

template<typename T> void* createT() { return new T; }

struct BaseFactory {
    typedef std::map<std::string, void*(*)()> map_type;

    static void* createInstance(std::string const& s) {
        map_type::iterator it = getMap()->find(s);
        if(it == getMap()->end())
            return 0;
        char* name = abi::__cxa_demangle(typeid(it->second()).name(),0,0,0);
        std::cout<<typeid(it->second()).name()<<" "<<name<<std::endl;
        return it->second();
    }

    protected:
    static map_type * getMap() {
        // never delete'ed. (exist until program termination)
        // because we can't guarantee correct destruction order 
        if(!map) { map = new map_type; } 
        return map; 
    }

    private:
    static map_type * map;
};
BaseFactory::map_type* BaseFactory::map = new BaseFactory::map_type;

template<typename T>
struct Register : BaseFactory { 
    Register() { 
        char* name = abi::__cxa_demangle(typeid(T).name(),0,0,0);
        getMap()->insert(std::make_pair(name, &createT<T>));
        free(name);
    }
};

class A {
    public:
    A(){std::cout<<"create A"<<std::endl;};
    int i;
    private:
        static Register<A> reg;
};
Register<A> A::reg;

class B : public A{
    public:
    B() : A() {std::cout<<"create B"<<std::endl;};
    float f;
    private:
        static Register<B> reg;
};
Register<B> B::reg;

class C : public B {
    public:
    C() : B() {std::cout<<"create C"<<std::endl;};
    double d;
    private:
        static Register<C> reg;
};
Register<C> C::reg;

 {
        int status;
        std::cout<<"test"<<std::endl;
        B* a = (B*)BaseFactory::createInstance("C");
        char* a_name = abi::__cxa_demangle(typeid(*a).name(),0,0,0);
        std::cout<<typeid(a).name()<<" "<<a_name<<std::endl;

        B b;
        char* b_name = abi::__cxa_demangle(typeid(b).name(),0,0,0);
        std::cout<<typeid(b).name()<<" "<<b_name<<std::endl;

        C c;
        char* c_name = abi::__cxa_demangle(typeid(c).name(),0,0,0);
        std::cout<<typeid(c).name()<<" "<<c_name<<std::endl;



        free(a_name);
        free(b_name);
        free(c_name);

    }


