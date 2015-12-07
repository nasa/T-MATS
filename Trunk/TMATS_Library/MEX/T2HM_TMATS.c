/*		T-MATS -- T2HM_TMATS.c
 * % *************************************************************************
 * % written by Jeffryes Chapman
 * % NASA Glenn Research Center, Cleveland, OH
 * % December 4, 2015
 * %
 * %  This file calculates enthaply (h) based on temperature (T) and combused fuel to air ratio.
 * % *************************************************************************/

#include "mex.h"

extern double t2hc(double c, double d);

/* The gateway function */
void mexFunction(int nlhs, mxArray *plhs[],
        int nrhs, const mxArray *prhs[])
{
    /* define I/O */
    double T;              /* temperature */
    double FAR;            /* fuel to air ratio */
    double *h;             /* enthalpy */
    
    /* check for proper number of arguments */
    if(nrhs!=2) {
        mexErrMsgIdAndTxt("TMATS:t2h:nrhs","Two inputs required.");
    }
    if(nlhs!=1) {
        mexErrMsgIdAndTxt("TMATS:t2h:nlhs","One output required.");
    }
    
    /* make sure the first input argument is scalar */
    if( !mxIsDouble(prhs[0]) ||
            mxIsComplex(prhs[0]) ||
            mxGetNumberOfElements(prhs[0])!=1 ) {
        mexErrMsgIdAndTxt("TMATS:t2h:notScalar","temperature input must be a scalar.");
    }
    /* make sure the second input argument is scalar */
    if( !mxIsDouble(prhs[1]) ||
            mxIsComplex(prhs[1]) ||
            mxGetNumberOfElements(prhs[1])!=1 ) {
        mexErrMsgIdAndTxt("TMATS:t2h:notScalar","FAR input must be a scalar.");
    }

    /* get the value of the first scalar input  */
    T = mxGetScalar(prhs[0]);

    /* get the value of the second scalar input  */
    FAR = mxGetScalar(prhs[1]);

    /* initialize the output  */
    plhs[0] = mxCreateDoubleScalar(mxREAL);
    h = mxGetPr(plhs[0]);
    
    /* call C-code */
    *h = t2hc(T, FAR);
}