/*
*	date: 10-04-2013 11:45
*	score: 322
*	size: 49
*	generation: 49
*/
#include <math.h>
double calc_score(const double* const vals)
{
	double res=(((vals[9])+(0.616653))+(((((0.0204809)*(sin((vals[8])-(vals[9]))))/(((vals[6])/(vals[8]))-(-0.367151)))*(0.173109))-(((0.565518)+((0.757306)/(((vals[6])-(0.651162))+((vals[5])-(vals[1])))))/((0.376519)*(((vals[1])/(vals[4]))/((0.951389)/(vals[5])))))));
	 return (res>=1)?res:0;
}