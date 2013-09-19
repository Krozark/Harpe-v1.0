#ifndef DEFINES_AA_HPP
#define DEFINES_AA_HPP

#include <iostream>

/// \breif Définie la couleure rouge pour le terminal
#define ROUGE  	"\033[31m"
/// \breif Définie la couleure vert pour le terminal
#define VERT   	"\033[32m"
/// \breif Définie la couleure normal pour le terminal
#define BLANC 	"\033[00m"

/**
 * \breif Permet de savoir si la valeur des encadré par to_find à +- error
 *
 * \param value Valeur à evaluer
 * \param to_find Valeur à approcher
 * \param error Marge d'erreur autorisée
 *
 * \return bool
 **/
#define eq_error(value,to_find,error) (value >= (to_find - error) && value <= (to_find + error))


/**
 * \breif print python like
 **/
void print ()
{
    std::cout<<std::endl;
};

/**
 * \breif print python like
 **/
template <typename T,typename ... Args>
void print(T v,Args ... args)
{
    std::cout<<v<<" ";
    print(args...);
};

/**
 * \breif print python like
 **/
template <typename T,typename ... Args>
void print_e(T v,Args ... args)
{
    std::cout<<ROUGE<<v<<" ";
    print(args...,BLANC);
};

/**
 * \breif print python like
 **/
template <typename T,typename ... Args>
void print_v(T v,Args ... args)
{
    std::cout<<VERT<<v<<" ";
    print(args...,BLANC);
};


#endif
