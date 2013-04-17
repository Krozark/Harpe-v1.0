/*
*	date: 10-04-2013 11:40
*	score: 294
*	size: 65
*	generation: 15
*/
#include <math.h>
double calc_score(const double* const vals)
{
	double res=(((sin((vals[3])*(0.777946)))*((vals[2])*(((0.00749604)+(((((vals[5])/(vals[1]))+((vals[3])+((vals[9])*((vals[8])*(0.0292547)))))*(sin((vals[1])*((sin(vals[9]))-((vals[3])+(0.442617))))))*(0.914811)))+((((vals[2])*((vals[0])-(0.162302)))/(0.889908))-(0.595013)))))*((((vals[5])-(vals[5]))-((((0.0162131)/(vals[1]))-(vals[9]))-((sin(vals[9]))*((vals[0])-(0.699318)))))*(vals[6])));
	 return (res>=1)?res:0;
}