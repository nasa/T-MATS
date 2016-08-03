/*		T-MATS -- calc_WvsMN_TMATS.c
 * % *************************************************************************
 * % written by Jeffryes Chapman based on work by JA DeCastro
 * % NASA Glenn Research Center, Cleveland, OH
 * % June 6th, 2013
 * %
 * % This module contains a function to calculate of flow rate (W)
 * % in terms of total pressure (Pt), total temp (Tt), and 
 * % Mach number (MN). Also needs gas constant (Rt) and gammat (Gt).
 * % *************************************************************************/

#include "functions_TMATS.h"
#include "constants_TMATS.h"
#include <math.h>


double calc_WvsMN(double MN, double Pt, double Tt, double Rt, double gammat, double A)
{
    double temp, Ts, V, rhot, rhos, W;
    
    /*-------- calc Ts from Mack's eqn --------*/
    temp = 1 + (0.5*(gammat-1)*MN*MN);
    Ts = Tt*divby(temp);
    /*---- calc V from Tom's eqn-------------*/
    V = MN*sqrtT(gammat*Rt*Ts*C_GRAVITY*JOULES_CONST); 
    /*---- calc total density via Tom's eqn----------------*/
    rhot = Pt*144*divby(Rt*Tt*778.161);               
    /*----- calc static density from Ton's eqn---------*/
    rhos = rhot*powT((Ts*divby(Tt)),(1*divby(gammat-1)));       
    /*---- calc flow rate from Tom's eqn ------------*/
    W = A*rhos*V/144;                            
    return W;
}
