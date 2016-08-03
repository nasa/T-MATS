/*		T-MATS -- calc_Pstatic_TMATS.c
 * % *************************************************************************
 * % written by Jeffryes Chapman based on work by D. Fredrick
 * % NASA Glenn Research Center, Cleveland, OH
 * % June 6th, 2013
 * %
 * %  standalone form of static pressure computation
 * % *************************************************************************/
#include "functions_TMATS.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/*------ the following code uses secant method to calc static pressure ----*/
double calc_Pstatic(double PtOut, double TtOut, double Wout, double Aexit, double *FAR_vec, double *Rt_vec, double *Tt_vec, double *gamma_array, double FAR, int A1, int B1)
{
    double Rt, gammat, tolerance;
    double x1, x2, x3, y1, y2, y3, z1, z2, z3, e1, e2, e3;
    int maxIters, ncount;
    int interpErr = 0;
    
    /*---   where gas constant is R = f(FAR), but NOT P & T ----*/
    Rt = interp1Ac(FAR_vec,Rt_vec,FAR,A1, &interpErr);             /* with FAR = 0 */
        if (interpErr == 1){
        printf("Warning in calc_Pstatic subroutine, Error calculating Rt. Vector definitions may need to be expanded.\n");
    }
    /*---- gamma depends on FAR and temp -----*/
    gammat = interp2Ac(FAR_vec,Tt_vec,gamma_array,FAR,TtOut,A1,B1, &interpErr);   /* with FAR = 0 */
            if (interpErr == 1){
        printf("Warning in calc_Pstatic subroutine, Error calculating gammat. Vector definitions may need to be expanded.\n");
    }
    
    /*------ iterate on Mach number until get computed flow = W ----*/
    tolerance = 0.02;     /* flow tolerance (pps) */
    maxIters = 10;
    
    /*----- use these 2 points to start the solution ----*/
    /*   where x ==> MN, y ==> W, and z ==> Ps */
    x1 = 0.2; 
    x2 = 0.5;
    
    y1 = calc_WvsMN(x1,PtOut,TtOut,Rt,gammat,Aexit); /* with FAR = 0 */
    z1 = calc_PsvsMN(x1,PtOut,gammat);
    y2 = calc_WvsMN(x2,PtOut,TtOut,Rt,gammat,Aexit); /* with FAR = 0 */
    z2 = calc_PsvsMN(x2,PtOut,gammat);
    
    e1 = Wout - y1;
    e2 = Wout - y2;
    e3 = 999;       /* force it into the while loop */
    
    ncount = 0;
    /*---- iterate x until the error is within tolerance ----*/
    while ((fabs(e3) > tolerance) && (ncount < maxIters)) {
        if (x2 - (e2*(x2-x1)*divby(e2-e1)) > 0.01)
            x3 = x2 - (e2*(x2-x1)*divby(e2-e1));
        else
            x3 = 0.01;
        
        /*----- compute y3 value (W) for new x3 (MN) ----- */
        /*       also compute z3 value (Ps) for new x3 */
        y3 = calc_WvsMN(x3,PtOut,TtOut,Rt,gammat,Aexit); /* with FAR = 0 */
        z3 = calc_PsvsMN(x3,PtOut,gammat);
        
        /*------ calculate the new error -----*/
        e3 = Wout - y3;
        /*---- want to keep solution bounded for next iteration ---*/
        if (e1*e3 <= 0) {    /* solution lies between x1 and x3, or at x3 */
            x2 = x3;
            y2 = y3;
            e2 = e3;
        }
        else {            /* solution lies between x2 and x3 */
            x1 = x3;
            y1 = y3;
            e1 = e3;
        }
        ncount++;
    }

    return z3;
}