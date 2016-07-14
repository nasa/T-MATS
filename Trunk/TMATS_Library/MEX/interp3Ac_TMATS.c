/*		C-MAPSS40k -- interp3Ac.c
% *************************************************************************
% written by Jeffryes Chapman based on work by D. Fredrick and Ryan May
% NASA Glenn Research Center, Cleveland, OH
% July 8, 2016
%
%  This is a file that performs a 3-D interpolation similar to the MATLAB interp3 function.  Note that this
% function will only perform interpolation.  If it tries to extrapolate, the upper bound value will be used and
% the interpErr value will be returned as a 1.
% *************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/*------ C code for interpolation calcs ----*/
double interp3Ac(double *X, double *Y, double *Z, double *V,
                        double xi, double yi, double zi, int A, int B, 
                        int C, int *error)
{
	int errValue = 0;
    int i, ii, j, jj, k, kk;
    double vi, v11, v21, v31, v12, v22, v32;
    double slope1a, slope2a, slope3a, slope1b, slope2b, slope3b, slope4;
    i = A - 2;      /* index of next-to-largest X value */
    j = B - 2;      /* index of next-to-largest Y value */
    k = C - 2;      /* index of next-to-largest Z value */
   
	/* initialize the error to "false" */
	errValue = 0;

	/*check for out of bounds errors*/
    /*--------------------------------------*/
    /* X boundary errors */
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
    /* Y boundary errors */
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

    /* Z boundary errors */
        if (zi < Z[0]){
            errValue = 1;
            zi = Z[0];
        }
        else if (zi > Z[C-1]){
            errValue = 1;
            zi = Z[C-1];
        }
        else if (!(zi >= Z[0]))
        {
            /* zi must be NaN to have failed by < Z[0]  and >= Z[0] */
            errValue = 1;
            zi = Z[0];
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
    
    	/*--- determine which Z values are just below & just above zi --*/
	while (k >= 0){
		if (zi >= Z[k]){
			kk = k;
			break;
		}
		else
			k = k - 1;
    }
    
    /* Get values from table for interpolation */
    
    /* Process again at Z(kk) */
        
    /*--- find v11 value for Y(jj)    ---*/    
        slope1a = (V[jj+B*(ii+1)+(B*A)*kk] - V[jj+B*ii+(B*A)*kk])/(X[ii+1] - X[ii]);
        v11 = V[jj+B*ii+(B*A)*kk] + (slope1a * (xi - X[ii]));
        
    /*--- find v21 value for Y(jj+1) ---*/
        slope2a = (V[jj+1+B*(ii+1)+(B*A)*kk] - V[jj+1+B*ii+(B*A)*kk])/(X[ii+1] - X[ii]);
        v21 = V[jj+1+B*ii+(B*A)*kk] + (slope2a * (xi - X[ii]));
        
    /*-- find v31 value by linear interpolation along line between the points
      (1) v=V(ii+deltaX), y=Y(jj) having value v1
      (2) v=V(ii+deltaX), y=Y(jj+1) having value v2   */
        slope3a = (v21 - v11)/(Y[jj+1] - Y[jj]);
        v31 = v11 + slope3a * (yi - Y[jj]); 
        
        
    /* Process again at Z(kk+1) */
        
    /*--- find v12 value for Y(jj)    ---*/    
        slope1b = (V[jj+B*(ii+1)+(B*A)*(kk+1)] - V[jj+B*ii+(B*A)*(kk+1)])/(X[ii+1] - X[ii]);
        v12 = V[jj+B*ii+(B*A)*(kk+1)] + (slope1b * (xi - X[ii]));
        
    /*--- find v22 value for Y(jj+1) ---*/
        slope2b = (V[jj+1+B*(ii+1)+(B*A)*(kk+1)] - V[jj+1+B*ii+(B*A)*(kk+1)])/(X[ii+1] - X[ii]);
        v22 = V[jj+1+B*ii+(B*A)*(kk+1)] + (slope2b * (xi - X[ii]));
        
    /*-- find v32 value by linear interpolation along line between the points */
        slope3b = (v22 - v12)/(Y[jj+1] - Y[jj]);
        v32 = v12 + slope3b * (yi - Y[jj]); 
    
    /* Process final value */
        
    /*-- find final value (vi) value by linear interpolation along line between the points */ 
        slope4 = (v32 - v31)/(Z[kk+1] - Z[kk]);
        vi = v31 + slope4 * (zi - Z[kk]);

    return vi;
}
