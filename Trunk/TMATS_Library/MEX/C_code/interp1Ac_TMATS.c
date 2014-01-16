/*		T-MATS -- interp1Ac.c
% *************************************************************************
% written by Ryan May based on work by D. Fredrick
% NASA Glenn Research Center, Cleveland, OH
% February 2nd, 2012
%
%  This is a file that performs a 1-D interpolation similar to the MATLAB interp1 function.  Note that this
% function will only perform interpolation.  If it tries to extrapolate, the upper bound value will be used and
% the interpErr value will be returned as a 1.
% *************************************************************************/

/*--------------------------------------------------------------*/
/*  How does it work?                                           */
/*  (A) The C code has four input arguments and returns the     */
/*      interpolated value yi as a double. The inputs are:      */
/*          *X :: pointer to X vector                           */
/*          *Y :: pointer to Y vector                           */
/*          xi :: value of independent variable                 */
/*          A  :: number of elements in X & Y (must be same)    */
/*      We always start looking at the top end of the X values  */
/*      to find the index ii, such that X(ii)<=xi<X(ii+1).      */
/*      Then calculate the slope dy/dx and compute yi           */
/*      according to yi = Y(ii) + slope * (xi - X(ii)).         */
/*      Finally, return yi as a double.                         */
/*                                                              */
/*  (B) The gateway code, which is between Matlab and the       */
/*      C code, involves the following:                         */
/*          *v :: pointer to first element of vector X          */
/*          *w :: pointer to first element of vector Y          */
/*          x  :: value of xi, independent variable             */
/*          A  :: number of elements in X (obtained from X)     */
/*          B  :: number of elements in Y (obtained from Y)     */
/*      The following tests are performed to ensure that the    */
/*      inputs are consistent:                                  */
/*          (i) X & Y have same number of elements              */
/*          (ii) X has at least two elements                    */
/*          (iii) xi >= first element of X                      */
/*          (iv) xi <= last element of X                        */
/*      After these tests are satisfied, the gateway code       */
/*      connects to the C code by setting the pointer to        */
/*      the returned value (yi) of the C code.                  */
/*--------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/*------ C code for interpolation calcs ----*/
double interp1Ac(double *X, double *Y, double xi, int A, int *error)
{
    int i, ii;
    double slope, yi;
    i = A - 2;      /* index of next-to-largest X value */

	/* initialize the error to "false" */
	*error = 0;

	/*check for out of bounds errors*/
	if (xi < X[0]){
        *error = 1;
		xi = X[0];
    }
    else if (xi > X[A-1]){
        *error = 1;
		xi = X[A-1];
    }
	else if (!(xi >= X[0]))
	{
		/* xi must be NaN to have failed by < X[0]  and >= X[0] */
		*error = 1;
		xi = X[0];
	}

	/*--- determine which X values are just below & just above xi ----*/
	while (i >= 0){
		if (xi >= X[i]){
			ii = i;
			break;
		}
		else
			i = i - 1;
    }
	slope = (Y[ii+1] - Y[ii])/(X[ii+1] - X[ii]);
    yi = Y[ii] + (slope * (xi - X[ii]));
    return yi;
}