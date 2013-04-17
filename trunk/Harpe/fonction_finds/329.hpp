/*
*	date: 10-04-2013 11:48
*	score: 329
*	size: 51
*	generation: 64
*/
#include <math.h>
double calc_score(const double* const vals)
{
	double res=((0.273695)+((((vals[0])/((sin((0.391187)*((vals[9])-(0.532497))))-(-0.367151)))*(0.835887))-(((0.565518)+(((vals[3])/(vals[2]))/(-0.385517)))/((sin(sin((vals[3])/(vals[6]))))*(((vals[1])/((0.658039)*(vals[6])))/(((0.367266)-(vals[0]))/(vals[5])))))));
	 return (res>=1)?res:0;
}