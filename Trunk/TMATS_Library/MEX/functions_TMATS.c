/*		T-MATS -- functions_TMATS.c
 * % *************************************************************************
 * % written by Jeffryes Chapman
 * % NASA Glenn Research Center, Cleveland, OH
 * % August 2nd, 2016
 * %
 * %  This is a file that contains various functions for use in the T-MATS C-code
 * %  Functions:
 * %  sqrtT - square root with input limits
 * %  divby - divide by X with input limits
 * %  powT  - raised to the power of with input limits
 * % *************************************************************************/

#include "constants_TMATS.h"
#include <math.h>

double sqrtT(double X)
/* square root with input limits */
{
    double H;
    /* value must be greater than or equal to 0 */
    if(X < 0)
        H = 0;
    else
        H = sqrt(X);
    
    
    return H;
}

double divby(double X)
/* division with input limits */
{
    double H;
    if(X < pow(10,-10) && X > -pow(10,-10))
        H = ((X >= 0) - (X < 0))*pow(10,10);
    else
        H = 1/X;
    
    return H;
}

double powT(double A, double N)
/* exponential function with input limits */
{
    double H;
    
    if(A < pow(10,-10) && A > -pow(10,-10) && N < 0)
        H = pow(10,10);
    else
        H = pow(A,N);
   
    return H;
}



