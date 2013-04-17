/*
*	date: 11-04-2013 10:57
*	score: 304
*	size: 65
*	generation: 25
*/
#include <math.h>
double calc_score(const double* const vals)
{
	double res=((((sin(((vals[3])+((vals[2])/(vals[0])))/(0.0576108)))+(((sin(vals[9]))+((vals[6])/(0.0963528)))-(vals[6])))+(((vals[1])+(((vals[3])*(0.953051))-((vals[8])*(vals[1]))))-((((vals[6])*(vals[3]))+(vals[4]))/(((vals[3])/(0.0569488))-((vals[2])*(0.131727))))))+((((vals[3])+((sin(vals[6]))-(vals[0])))*((((0.595076)*(vals[9]))+((vals[3])/(0.920986)))-(0.908708)))+(sin((vals[0])+(0.315244)))));
return res
}