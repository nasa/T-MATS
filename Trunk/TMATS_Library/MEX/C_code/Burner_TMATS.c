/*		T-MATS -- Burner_TMATS.c
% *************************************************************************
% written by Jeffryes Chapman based on work by R. May and T. Lavelle
% NASA Glenn Research Center, Cleveland, OH
% February 27th, 2013
%
%  This file models the Burner Element for T-MATS simulation
% *************************************************************************/
#define S_FUNCTION_NAME  Burner_TMATS
#define S_FUNCTION_LEVEL 2
#include "simstruc.h"
#include <math.h>

#define LHV_p(S)			    ssGetSFcnParam(S,0)
#define dPnormBurner_p(S)		ssGetSFcnParam(S,1)
#define Efficiency_p(S)         ssGetSFcnParam(S,2)
#define NPARAMS 3

extern double t2hc(double a, double b);
extern double h2tc(double c, double d);

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
    ssSetInputPortWidth(S, 0, 6);
    ssSetInputPortRequiredContiguous(S, 0, true);
    ssSetInputPortDirectFeedThrough(S, 0, 1);

    if (!ssSetNumOutputPorts(S, 1)) return;
    ssSetOutputPortWidth(S, 0, 6);

    ssSetNumSampleTimes(S, 1);
    ssSetNumRWork(S, 0);
    ssSetNumIWork(S, 0);
    ssSetNumPWork(S, 0);
    ssSetNumModes(S, 0);
    ssSetNumNonsampledZCs(S, 0);

    ssSetOptions(S,
            SS_OPTION_WORKS_WITH_CODE_REUSE |
            SS_OPTION_EXCEPTION_FREE_CODE |
            SS_OPTION_USE_TLC_WITH_ACCELERATOR);
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

}
#endif

static void mdlOutputs(SimStruct *S, int_T tid)
{
    /*--------Define Parameters-------*/
    const real_T LHV			= *mxGetPr(LHV_p(S));
    const real_T dPnormBurner   = *mxGetPr(dPnormBurner_p(S));
    const real_T Eff            = *mxGetPr(Efficiency_p(S));

    /*---------Define Inputs--------*/
    const real_T *u  = (const real_T*) ssGetInputPortSignal(S,0);

	double WfIn	= u[0];     /* Input Fuel Flow[pps] 	*/
    double WIn = u[1];     /* Input Flow [pps]  */
	double htIn = u[2];    /* Input enthalpy [BTU/lbm] */
	double TtIn	= u[3];     /* Temperature Input [degR] 	*/
	double PtIn	= u[4];     /* Pressure Input [psia] 	*/
    double FARcIn = u[5];   /* Combusted Fuel to Air Ratio [frac] 	*/



	real_T *y  = (real_T *)ssGetOutputPortRealSignal(S,0);  /* Output Array */

    /*--------Define Constants-------*/
	double htin;
    double htOut, TtOut, PtOut, FARcOut, WOut;
    double Test;

    /*-- Compute Input enthalpy (empirical) --------*/

	htin = t2hc(TtIn,FARcIn);

    /*-- Compute Flow output  --------*/

	WOut = WIn + WfIn;     /*Perfect combustion*/

    /*-- Compute Input fuel to air ratio --*/

    FARcOut = (WIn* FARcIn + WfIn)/(WIn*(1-FARcIn));

    /*------ Compute enthalpy output ---------*/

	htOut = (WIn*htin + WfIn*LHV*Eff)/WOut;

    /*------ Compute Temperature output ---------*/

	TtOut = h2tc(htOut,FARcOut);

    /*------ Compute pressure output ---------*/
	PtOut = (1- dPnormBurner) * PtIn;

    Test = htin;

    /*------Assign output values------------*/
	y[0] = WOut;     /* Output Air Flow [pps]	*/
    y[1] = htOut; 	  /* Output Enthalpy [BTU/lbm] */
	y[2] = TtOut;     /* Output Temperature  [degR]*/
	y[3] = PtOut;     /* Output Pressure [psia]	*/
	y[4] = FARcOut;   /* Output Combusted Fuel to Air Ratio [frac] */
    y[5] = Test;      /* Output Test Point */

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
