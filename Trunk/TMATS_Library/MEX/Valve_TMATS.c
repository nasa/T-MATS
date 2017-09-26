/*		T-MATS -- Valve_TMATS.c
 * % *************************************************************************
 * % written by Jeffryes Chapman based on work by R. May and Ananda Himansu
 * % NASA Glenn Research Center, Cleveland, OH
 * % March 1st, 2013
 * %
 * %  This file models the Valve Element for T-MATS simulation
 * % *************************************************************************/
#define S_FUNCTION_NAME  Valve_TMATS
#define S_FUNCTION_LEVEL 2
#include "simstruc.h"
#include "types_TMATS.h"

#define VlvfullyOpen_p(S)			ssGetSFcnParam(S,0)
#define VlvdeadZone_p(S)			ssGetSFcnParam(S,1)
#define Valve_Ae_p(S)			    ssGetSFcnParam(S,2)
#define X_V_PRVec_p(S)			    ssGetSFcnParam(S,3)
#define T_V_WcVec_p(S)              ssGetSFcnParam(S,4)
#define BN_p(S)                     ssGetSFcnParam(S,5)
#define NPARAMS 6
#define NERRORS 5

extern void Valve_TMATS_body(double* y, const double* u, const ValveStruct* prm);

#define MDL_SET_WORK_WIDTHS
#if defined(MDL_SET_WORK_WIDTHS) && defined(MATLAB_MEX_FILE)
static void mdlSetWorkWidths(SimStruct *S)
{
    const char_T *rtParamNames[] = {"VlvfullyOpen", "VlvdeadZone", "Valve_Ae", "X_V_PRVec", "T_V_WcVec"};
    ssRegAllTunableParamsAsRunTimeParams(S, rtParamNames);
}
#endif

static void mdlInitializeSizes(SimStruct *S)
{
    int i;
    ssSetNumSFcnParams(S, NPARAMS);  /* Number of expected parameters */
    if (ssGetNumSFcnParams(S) != ssGetSFcnParamsCount(S)) {
        /* Return if number of expected != number of actual parameters */
        return;
    }

    for (i = 0; i < NPARAMS; i++) {
        if (i != 5)
            ssSetSFcnParamTunable(S, i, 1);
        else
            ssSetSFcnParamTunable(S, i, 0);
    }

    ssSetOptions(S, SS_OPTION_WORKS_WITH_CODE_REUSE | 
                    SS_OPTION_USE_TLC_WITH_ACCELERATOR);

    ssSetNumContStates(S, 0);
    ssSetNumDiscStates(S, 0);

    if (!ssSetNumInputPorts(S, 1)) return;
    ssSetInputPortWidth(S, 0, 5);
    ssSetInputPortRequiredContiguous(S, 0, true);
    ssSetInputPortDirectFeedThrough(S, 0, 1);

    if (!ssSetNumOutputPorts(S, 1)) return;
    ssSetOutputPortWidth(S, 0, 2);

    ssSetNumSampleTimes(S, 1);
    ssSetNumRWork(S, 0);
    ssSetNumIWork(S, NERRORS);
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
    ssSetIWorkValue(S,Er1,0);
}
#endif

static void mdlOutputs(SimStruct *S, int_T tid)
{
    /* Input and output vectors */
    const real_T *u  = (const real_T*) ssGetInputPortSignal(S,0);

    real_T *y  = (real_T *)ssGetOutputPortRealSignal(S,0);

    /* Block name buffer length and string read status */
    int_T buflen;
    int_T status;

    /* Block mask parameter struct */
    ValveStruct valveStruct;
    valveStruct.VlvfullyOpen			= *mxGetPr(VlvfullyOpen_p(S));
    valveStruct.VlvdeadZone			= *mxGetPr(VlvdeadZone_p(S));
    valveStruct.Valve_Ae               = *mxGetPr(Valve_Ae_p(S));
    /* Vector & array data */
    valveStruct.X_V_PRVec			= mxGetPr(X_V_PRVec_p(S));
    valveStruct.T_V_WcVec          = mxGetPr(T_V_WcVec_p(S));
    /* Dimensions of parameter arrays */
    valveStruct.A   = mxGetNumberOfElements(X_V_PRVec_p(S));

    valveStruct.IWork      = ssGetIWork(S);

    /* Get name of block from dialog parameter (string) */
    buflen = mxGetN(BN_p(S))*sizeof(mxChar)+1;
    valveStruct.BlkNm = malloc(buflen);
    status = mxGetString(BN_p(S), valveStruct.BlkNm, buflen);

    /* Perform core block calculations */
    Valve_TMATS_body(y, u, &valveStruct);
    free(valveStruct.BlkNm);
}

static void mdlTerminate(SimStruct *S)
{
}

#define MDL_RTW
static void mdlRTW(SimStruct *S)
{
    if (!ssWriteRTWWorkVect(S, "IWork", 1 /* nNames */,
                            "Errors", 
                            ssGetNumIWork(S))) {
        return;
    }
    /*
      This registration of the error code symbols "Er1, etc." 
			allows tlc to call 
			LibBlockIWork(Er1,[...])
     */

}
#ifdef  MATLAB_MEX_FILE    /* Is this file being compiled as a MEX-file? */
#include "simulink.c"      /* MEX-file interface mechanism */
#else
#include "cg_sfun.h"       /* Code generation registration function */
#endif
/*==================*/
