/*
*	date: 10-04-2013 13:46
*	score: 351
*	size: 10
*	generation: 837
*/
#include <math.h>
double calc_score(const double* const vals)
{
	double res=((sin(((vals[3])/(vals[2]))/(vals[0])))+(-0.287297));
	 return (res>=1)?res:0;
}