/*		T-MATS -- Ambient_TMATS.c
 * % *************************************************************************
 * % written by Jeffryes Chapman
 * % NASA Glenn Research Center, Cleveland, OH
 * % March 19, 2013
 * %
 * %  This file converts Altitude and MN to common engine input variables.
 * % *************************************************************************/
#define S_FUNCTION_NAME  Ambient_TMATS
#define S_FUNCTION_LEVEL 2
#include "simstruc.h"
#include "constants_TMATS.h"
#include <math.h>

#define X_A_AltVec_p(S)     ssGetSFcnParam(S,0)
#define T_A_TsVec_p(S)      ssGetSFcnParam(S,1)
#define T_A_PsVec_p(S)      ssGetSFcnParam(S,2)
#define BN_p(S)             ssGetSFcnParam(S,3)
#define NPARAMS 4

extern double interp1Ac(double aa[], double bb[], double cc, int ii,int *error);

static void mdlInitializeSizes(SimStruct *S)
{
    int i;
    ssSetNumSFcnParams(S, NPARAMS);  /* Number of expected parameters */
    if (ssGetNumSFcnParams(S) != ssGetSFcnParamsCount(S)) {
        /* Return if number of expected != number of actual parameters */
        return;
    }

    for (i = 0; i < NPARAMS; i++)
        ssSetSFcnParamTunable(S, i, 0);

    ssSetNumContStates(S, 0);
    ssSetNumDiscStates(S, 0);

    if (!ssSetNumInputPorts(S, 1)) return;
    ssSetInputPortWidth(S, 0, 3);
    ssSetInputPortRequiredContiguous(S, 0, true);
    ssSetInputPortDirectFeedThrough(S, 0, 1);

    if (!ssSetNumOutputPorts(S, 1)) return;
    ssSetOutputPortWidth(S, 0, 5);

    ssSetNumSampleTimes(S, 1);
    ssSetNumRWork(S, 0);
    ssSetNumIWork(S, 2);
    ssSetNumPWork(S, 0);
    ssSetNumModes(S, 0);
    ssSetNumNonsampledZCs(S, 0);

}

static void mdlInitializeSampleTimes(SimStruct *S)
{
    ssSetSampleTime(S, 0, INHERITED_SAMPLE_TIME);
    ssSetOffsetTime(S, 0, 0.0);
    ssSetModelReferenceSampleTimeDefaultInheritance(S);
}

#define MDL_START
#if defined(MDL_START)
static void mdlStart(SimStruct *S)
{
    /* initialize print error variables */
    ssSetIWorkValue(S,0,0);
    ssSetIWorkValue(S,1,0);
}
#endif

static void mdlOutputs(SimStruct *S, int_T tid)
{
    /*-------- vector & array data -------*/
    const real_T *X_A_AltVec        = mxGetPr(X_A_AltVec_p(S));
    const real_T *T_A_TsVec         = mxGetPr(T_A_TsVec_p(S));
    const real_T *T_A_PsVec         = mxGetPr(T_A_PsVec_p(S));

    /*------get dimensions of parameter arrays-------*/
    const int_T A   = mxGetNumberOfElements(X_A_AltVec_p(S));

    /*---------Define Inputs--------*/
    const real_T *u  = (const real_T*) ssGetInputPortSignal(S,0);

    double AltIn     = u[0];     /* Altitude(ft) 	*/
    double dTempIn   = u[1];     /* delta Temperature [degF] 	*/
    double MNIn      = u[2];     /* Mach Number (frac) 	*/

    real_T *y  = (real_T *)ssGetOutputPortRealSignal(S,0);  /* Output Array */

    /*--------Define Constants-------*/
    double PsOut, TsOut, TtOut, PtOut, VengOut, TsStDayOut, Vsound;

    int interpErr = 0;

    /* ------- get strings -------------- */
    char * BlkNm;
    int_T buflen;
    int_T status;

    /* Get name of block from dialog parameter (string) */
    buflen = mxGetN(BN_p(S))*sizeof(mxChar)+1;
    BlkNm = mxMalloc(buflen);
    status = mxGetString(BN_p(S), BlkNm, buflen);

    /*  Static Temperature */
    TsStDayOut = interp1Ac(X_A_AltVec,T_A_TsVec,AltIn,A,&interpErr);
        if (interpErr == 1 && ssGetIWork(S)[0]==0){
        printf("Warning in %s, Error calculating TsStDayOut. Vector definitions may need to be expanded.\n", BlkNm);
        ssSetIWorkValue(S,0,1);
    }
    TsOut = TsStDayOut + dTempIn;

    /* Static Pressure*/
    PsOut = interp1Ac(X_A_AltVec,T_A_PsVec,AltIn,A,&interpErr);
        if (interpErr == 1 && ssGetIWork(S)[1]==0){
        printf("Warning in %s, Error calculating PsOut. Vector definitions may need to be expanded.\n", BlkNm);
        ssSetIWorkValue(S,1,1);
    }

    /*------ Total Temperature ---------*/
    TtOut = TsOut * (1+MNIn*MNIn*(C_GAMMA-1)/2);

    /*------ Total Pressure ---------*/
    PtOut = PsOut/(pow((TsOut/TtOut),(C_GAMMA/(C_GAMMA-1))));

    /*---- Engine Velocity ---------*/
    Vsound = C_MACH1 * sqrt(TsOut);
    VengOut = Vsound * MNIn;

    /*------Assign output values------------*/
    y[0] = TtOut;      /* Total Temperature [degR] */
    y[1] = PtOut;      /* Total Pressure [psia] */
    y[2] = PsOut;      /* Static Pressure [psia] */
    y[3] = TsOut;      /* Static Temperature [degR] */
    y[4] = VengOut;    /* Engine Velocity [ft/sec] */

}

static void mdlTerminate(SimStruct *S)
{
}

#ifdef  MATLAB_MEX_FILE    /* Is this file being compiled as a MEX-file? */
#include "simulink.c"      /* MEX-file interface mechanism */
#else
#include "cg_sfun.h"       /* Code generation registration function */
#endif
/*==================*/
