/*
*	date: 10-04-2013 11:40
*	score: 302
*	size: 13
*	generation: 20
*/
#include <math.h>
double calc_score(const double* const vals)
{
	double res=(((vals[9])-(0.754677))*(((0.0204809)*((vals[2])-(vals[1])))/(vals[4])));
	 return (res>=1)?res:0;
}