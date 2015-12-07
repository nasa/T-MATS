/*		T-MATS -- H2TM_TMATS.c
 * % *************************************************************************
 * % written by Jeffryes Chapman
 * % NASA Glenn Research Center, Cleveland, OH
 * % December 4, 2015
 * %
 * %  This file calculates enthaply (h) based on temperature (T) and combused fuel to air ratio.
 * % *************************************************************************/

#include "mex.h"

extern double h2tc(double c, double d);

/* The gateway function */
void mexFunction(int nlhs, mxArray *plhs[],
        int nrhs, const mxArray *prhs[])
{
    /* define I/O */
    double h;              /* enthalpy */
    double FAR;            /* fuel to air ratio */
    double *T;             /* temperature */
    
    /* check for proper number of arguments */
    if(nrhs!=2) {
        mexErrMsgIdAndTxt("TMATS:h2t:nrhs","Two inputs required.");
    }
    if(nlhs!=1) {
        mexErrMsgIdAndTxt("TMATS:h2t:nlhs","One output required.");
    }
    
    /* make sure the first input argument is scalar */
    if( !mxIsDouble(prhs[0]) ||
            mxIsComplex(prhs[0]) ||
            mxGetNumberOfElements(prhs[0])!=1 ) {
        mexErrMsgIdAndTxt("TMATS:h2t:notScalar","entropy input must be a scalar.");
    }
    /* make sure the second input argument is scalar */
    if( !mxIsDouble(prhs[1]) ||
            mxIsComplex(prhs[1]) ||
            mxGetNumberOfElements(prhs[1])!=1 ) {
        mexErrMsgIdAndTxt("TMATS:h2t:notScalar","FAR input must be a scalar.");
    }

    /* get the value of the first scalar input  */
    h = mxGetScalar(prhs[0]);

    /* get the value of the second scalar input  */
    FAR = mxGetScalar(prhs[1]);

    /* initialize the output  */
    plhs[0] = mxCreateDoubleScalar(mxREAL);
    T = mxGetPr(plhs[0]);
    
    /* call C-code */
    *T = h2tc(h, FAR);
}