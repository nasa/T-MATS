/*		T-MATS -- PcalcStat_TMATS.c
% *************************************************************************
% written by Jeffryes Chapman based on work by T. Lavelle
% NASA Glenn Research Center, Cleveland, OH
% February 2nd, 2012
%
%  (S, Ts, hs, rho, V) = f(Pt, Ps, Tt, ht, FAR, Rt).
% *************************************************************************/

#include <math.h>

extern double t2hc(double a, double b);
extern double pt2sc(double c, double d, double e);
extern double sp2tc(double f, double g, double h);

void PcalcStat(double Pt, double Ps, double Tt, double ht, double FAR, double Rt, double *S, double *Ts, double *hs, double *rhos, double *V)
{
            double Rs;
            double C_GRAVITY    =  32.174;
            double JOULES_CONST = 778.169;
            double C_PSItoPSF   = 144; 

            /* Compute entropy */
            *S = pt2sc(Pt, Tt, FAR);
            /* Compute Static Temperature */
            *Ts = sp2tc(*S,Ps,FAR);
            if (*Ts > Tt) {
                    *Ts = Tt;
            }
            /* Compute static enthalpy */
            *hs = t2hc(*Ts,FAR);
            if (*hs > ht) {
                    *hs = ht;
            }
            /* Assume Rt = Rs */
            Rs = Rt;
            /* Compute static rho */
            *rhos = Ps * C_PSItoPSF/(Rs* *Ts * JOULES_CONST);
            /* Compute Velocity */
            *V = sqrt(2 * (ht - *hs)*C_GRAVITY*JOULES_CONST);
            
}