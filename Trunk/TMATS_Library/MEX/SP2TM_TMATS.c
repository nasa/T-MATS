/*		T-MATS -- SP2TM_TMATS.c
 * % *************************************************************************
 * % written by Jeffryes Chapman
 * % NASA Glenn Research Center, Cleveland, OH
 * % December 4, 2015
 * %
 * %  This file calculates entropy (T) based on entropy, pressure, and combused fuel to air ratio.
 * % *************************************************************************/

#include "mex.h"

extern double sp2tc(double c, double d, double e);

/* The gateway function */
void mexFunction(int nlhs, mxArray *plhs[],
        int nrhs, const mxArray *prhs[])
{
    /* define I/O */
    double P;              /* pressure */
    double s;              /* entropy */
    double FAR;            /* fuel to air ratio */
    double *T;             /* temperature */
    
    /* check for proper number of arguments */
    if(nrhs!=3) {
        mexErrMsgIdAndTxt("TMATS:sp2t:nrhs","Three inputs required.");
    }
    if(nlhs!=1) {
        mexErrMsgIdAndTxt("TMATS:sp2t:nlhs","One output required.");
    }
    
    /* make sure the first input argument is scalar */
    if( !mxIsDouble(prhs[0]) ||
            mxIsComplex(prhs[0]) ||
            mxGetNumberOfElements(prhs[0])!=1 ) {
        mexErrMsgIdAndTxt("TMATS:sp2t:notScalar","Pressure input must be a scalar.");
    }
    /* make sure the second input argument is scalar */
    if( !mxIsDouble(prhs[0]) ||
            mxIsComplex(prhs[0]) ||
            mxGetNumberOfElements(prhs[0])!=1 ) {
        mexErrMsgIdAndTxt("TMATS:sp2t:notScalar","Entropy input must be a scalar.");
    }
    /* make sure the third input argument is scalar */
    if( !mxIsDouble(prhs[0]) ||
            mxIsComplex(prhs[0]) ||
            mxGetNumberOfElements(prhs[0])!=1 ) {
        mexErrMsgIdAndTxt("TMATS:sp2t:notScalar","FAR input must be a scalar.");
    }
        /* get the value of the scalar input  */
    s = mxGetScalar(prhs[0]);

        /* get the value of the scalar input  */
    P = mxGetScalar(prhs[1]);

        /* get the value of the scalar input  */
    FAR = mxGetScalar(prhs[2]);

    /* get the value of the scalar input  */
    plhs[0] = mxCreateDoubleScalar(mxREAL);
    
    T = mxGetPr(plhs[0]);
    
    /* call C-code */
    *T = sp2tc(s, P, FAR);
}