/*		C-MAPSS40k -- interp2Ac.c
% *************************************************************************
% written by Ryan May based on work by D. Fredrick
% NASA Glenn Research Center, Cleveland, OH
% February 2nd, 2012
%
%  This is a file that performs a 2-D interpolation similar to the MATLAB interp2 function.  Note that this
% function will only perform interpolation.  If it tries to extrapolate, the upper bound value will be used and
% the interpErr value will be returned as a 1.
% *************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/*------ C code for interpolation calcs ----*/
double interp2Ac(double *X, double *Y, double *Z,
                        double xi, double yi, int A, int B,
						int *error)
{
	int errValue = 0;
    int i, ii, j, jj;
    double slope1, slope2, slope3, zi, z1, z2;
    i = A - 2;      /* index of next-to-largest X value */
    j = B - 2;      /* index of next-to-largest Y value */



	/* initialize the error to "false" */
	errValue = 0;

	/*check for out of bounds errors*/
	if (xi < X[0]){
        errValue = 1;
		xi = X[0];
    }
    else if (xi > X[A-1]){
        errValue = 1;
		xi = X[A-1];
    }
	else if (!(xi >= X[0]))
	{
		/* xi must be NaN to have failed by < X[0]  and >= X[0] */
		errValue = 1;
		xi = X[0];
	}

	if (yi < Y[0]){
        errValue = 1;
		yi = Y[0];
    }
    else if (yi > Y[B-1]){
        errValue = 1;
		yi = Y[B-1];
    }
	else if (!(yi >= Y[0]))
	{
		/* yi must be NaN to have failed by < Y[0]  and >= Y[0] */
		errValue = 1;
		yi = Y[0];
	}

	*error = errValue;


	/*--- determine which X values are just below & just above xi --*/
	while (i >= 0){
		if (xi >= X[i]){
			ii = i;
			break;
		}
		else
			i = i - 1;
    }
	/*--- determine which Y values are just below & just above yi --*/
	while (j >= 0){
		if (yi >= Y[j]){
			jj = j;
			break;
		}
		else
			j = j - 1;
    }

    /*--- find z1 value for Y(jj) ---*/
	slope1 = (Z[jj+B*(ii+1)] - Z[jj+B*ii])/(X[ii+1] - X[ii]);
    z1 = Z[jj+B*ii] + (slope1 * (xi - X[ii]));

    /*--- find z2 value for Y(jj+1) ---*/
	slope2 = (Z[jj+1+B*(ii+1)] - Z[jj+1+B*ii])/(X[ii+1] - X[ii]);
    z2 = Z[jj+1+B*ii] + (slope2 * (xi - X[ii]));

    /*-- find zi value by linear interpolation along line between the points
      (1) x=X(ii+deltaX), y=Y(jj) having value z1
      (2) x=X(ii+deltaX), y=Y(jj+1) having value z2   */
    slope3 = (z2 - z1)/(Y[jj+1] - Y[jj]);
    zi = z1 + (slope3 * (yi - Y[jj]));
    return zi;
}
