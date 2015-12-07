/*		T-MATS -- PT2SM_TMATS.c
 * % *************************************************************************
 * % written by Jeffryes Chapman
 * % NASA Glenn Research Center, Cleveland, OH
 * % December 4, 2015
 * %
 * %  This file calculates entropy (S) based on temperature, pressure, and combused fuel to air ratio.
 * % *************************************************************************/

#include "mex.h"

extern double pt2sc(double c, double d, double e);

/* The gateway function */
void mexFunction(int nlhs, mxArray *plhs[],
        int nrhs, const mxArray *prhs[])
{
    /* define I/O */
    double P;              /* pressure */
    double T;              /* temperature */
    double FAR;            /* fuel to air ratio */
    double *s;             /* entropy */
    
    /* check for proper number of arguments */
    if(nrhs!=3) {
        mexErrMsgIdAndTxt("TMATS:PT2S:nrhs","Three inputs required.");
    }
    if(nlhs!=1) {
        mexErrMsgIdAndTxt("TMATS:PT2S:nlhs","One output required.");
    }
    
    /* make sure the first input argument is scalar */
    if( !mxIsDouble(prhs[0]) ||
            mxIsComplex(prhs[0]) ||
            mxGetNumberOfElements(prhs[0])!=1 ) {
        mexErrMsgIdAndTxt("TMATS:pt2s:notScalar","Pressure input must be a scalar.");
    }
    /* make sure the second input argument is scalar */
    if( !mxIsDouble(prhs[0]) ||
            mxIsComplex(prhs[0]) ||
            mxGetNumberOfElements(prhs[0])!=1 ) {
        mexErrMsgIdAndTxt("TMATS:pt2s:notScalar","Temperature input must be a scalar.");
    }
    /* make sure the third input argument is scalar */
    if( !mxIsDouble(prhs[0]) ||
            mxIsComplex(prhs[0]) ||
            mxGetNumberOfElements(prhs[0])!=1 ) {
        mexErrMsgIdAndTxt("TMATS:pt2s:notScalar","FAR input must be a scalar.");
    }
        /* get the value of the scalar input  */
    P = mxGetScalar(prhs[0]);

        /* get the value of the scalar input  */
    T = mxGetScalar(prhs[1]);

        /* get the value of the scalar input  */
    FAR = mxGetScalar(prhs[2]);

    /* get the value of the scalar input  */
    plhs[0] = mxCreateDoubleScalar(mxREAL);
    
    s = mxGetPr(plhs[0]);
    
    /* call C-code */
    *s = pt2sc(P, T, FAR);
}