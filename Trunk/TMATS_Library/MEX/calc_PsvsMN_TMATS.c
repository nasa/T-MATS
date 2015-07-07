/*		T-MATS -- calc_PsvsMN_TMATS.c
 * % *************************************************************************
 * % written by Jeffryes Chapman based on work by JA DeCastro
 * % NASA Glenn Research Center, Cleveland, OH
 * % June 6th, 2013
 * %
 * % This module contains a function to calculate of static pressure (Ps)
 * % in terms of total pressure (Pt) and Mach number (MN). 
 * % Also needs gas constant (Rt) and gammat (Gt).
 * % *************************************************************************/

#include <math.h>

double calc_PsvsMN(double MN, double Pt, double gammat)
{
    /*------ calc Ps via Mack's eqn  -----------*/
    double temp = 1 + (0.5*(gammat-1)*MN*MN);
    double Ps = Pt*pow((1/temp),(gammat/(gammat-1)));
    return Ps;
}
