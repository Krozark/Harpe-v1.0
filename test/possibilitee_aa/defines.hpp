#ifndef DEFINES_HPP
#define DEFINES_HPP

#include <iostream>

#define ROUGE  	"\033[31m"
#define VERT   	"\033[32m"
#define BLANC 	"\033[00m"

#define eq_error(value,to_find,error) (value >= (to_find - error) && value <= (to_find + error))



void print ()
{
    std::cout<<std::endl;
};

template <typename T,typename ... Args>
void print(T v,Args ... args)
{
    std::cout<<v<<" ";
    print(args...);
};

template <typename T,typename ... Args>
void print_e(T v,Args ... args)
{
    std::cout<<ROUGE<<v<<" ";
    print(args...,BLANC);
};

template <typename T,typename ... Args>
void print_v(T v,Args ... args)
{
    std::cout<<VERT<<v<<" ";
    print(args...,BLANC);
};


#endif
