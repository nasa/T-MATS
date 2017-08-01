/*		T-MATS -- Temp2Enth_TMATS.c
 * % *************************************************************************
 * % written by Jeffryes Chapman
 * % NASA Glenn Research Center, Cleveland, OH
 * % March 18, 2013
 * %
 * %  This file calculates enthalpy based on temperature and combused fuel to air ratio.
 * % *************************************************************************/
#define S_FUNCTION_NAME  Temp2Enth_TMATS_new
#define S_FUNCTION_LEVEL 2
#include "simstruc.h"
#include <math.h>

#define NPARAMS 0

extern double t2hc(double i, double j);

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
    ssSetInputPortWidth(S, 0, 2);
    ssSetInputPortRequiredContiguous(S, 0, true);
    ssSetInputPortDirectFeedThrough(S, 0, 1);

    if (!ssSetNumOutputPorts(S, 1)) return;
    ssSetOutputPortWidth(S, 0, 1);

    ssSetNumSampleTimes(S, 1);
    ssSetNumRWork(S, 0);
    ssSetNumIWork(S, 0);
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

static void mdlOutputs(SimStruct *S, int_T tid)
{

    /*---------Define Inputs--------*/
    const real_T *u  = (const real_T*) ssGetInputPortSignal(S,0);

    double TIn      = u[0];     /* Temperature [degR] 	*/
    double FARcIn   = u[1];     /* combusted Fuel to Air Ratio [frac] 	*/



    real_T *y  = (real_T *)ssGetOutputPortRealSignal(S,0);  /* Output Array */

    /*--------Define Constants-------*/
    double hOut;


    /* Calculate Enthalpy */
    /* Enthalpy will be total or static based on input temperature (total "Tt" or static "Ts") */
    hOut = t2hc(TIn,FARcIn);

    /*------Assign output values------------*/
    y[0] = hOut;      /* enthalpy (BTU/lbm) */

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
