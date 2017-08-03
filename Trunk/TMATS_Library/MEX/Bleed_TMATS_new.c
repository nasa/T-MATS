/*		T-MATS -- Bleed_TMATS.c
 * % *************************************************************************
 * % written by Jeffryes Chapman
 * % NASA Glenn Research Center, Cleveland, OH
 * % Feb 1, 2017
 * %
 * %  This file contains the model for a simple flow bleed.
 * % *************************************************************************/
#define S_FUNCTION_NAME  Bleed_TMATS_new
#define S_FUNCTION_LEVEL 2
#include "simstruc.h"
#include "types_TMATS.h"

#define Wfrac_p(S)              ssGetSFcnParam(S,0)
#define WfracLen_p(S)           ssGetSFcnParam(S,1)
#define BN_p(S)                 ssGetSFcnParam(S,2)
#define NPARAMS 3
#define NERRORS 5

extern void Bleed_TMATS_body(real_T *y, real_T *y1, const real_T *u, const real_T *BldInFlow, const BleedStruct* prm);

#define MDL_SET_WORK_WIDTHS
#if defined(MDL_SET_WORK_WIDTHS) && defined(MATLAB_MEX_FILE)
static void mdlSetWorkWidths(SimStruct *S)
{
    const char_T *rtParamNames[] = {"Wfrac", "WfracLen"};
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
        if (i != 2)
            ssSetSFcnParamTunable(S, i, 1);
        else
            ssSetSFcnParamTunable(S, i, 0);
    }

    ssSetOptions(S, SS_OPTION_WORKS_WITH_CODE_REUSE);
    
    ssSetNumContStates(S, 0);
    ssSetNumDiscStates(S, 0);
    
    if (!ssSetNumInputPorts(S, 2)) return;
    ssSetInputPortWidth(S, 0, 6);
    ssSetInputPortRequiredContiguous(S, 0, true);
    ssSetInputPortDirectFeedThrough(S, 0, 1);
    
    ssSetInputPortWidth(S, 1, DYNAMICALLY_SIZED);
    ssSetInputPortRequiredContiguous(S, 1, true);
    ssSetInputPortDirectFeedThrough(S, 1, 1);
    
    if (!ssSetNumOutputPorts(S, 2)) return;
    ssSetOutputPortWidth(S, 0, 6);
    ssSetOutputPortWidth(S, 1, DYNAMICALLY_SIZED);
    
    ssSetNumSampleTimes(S, 1);
    ssSetNumRWork(S, 0);
    ssSetNumIWork(S, NERRORS);
    ssSetNumPWork(S, 0);
    ssSetNumModes(S, 0);
    ssSetNumNonsampledZCs(S, 0);
    
}

#if defined(MATLAB_MEX_FILE)
# define MDL_SET_INPUT_PORT_WIDTH
static void mdlSetInputPortWidth(SimStruct *S, int_T port,
        int_T inputPortWidth)
{
    ssSetInputPortWidth(S,port,inputPortWidth);
}

# define MDL_SET_OUTPUT_PORT_WIDTH
static void mdlSetOutputPortWidth(SimStruct *S, int_T port,
        int_T outputPortWidth)
{
    ssSetOutputPortWidth(S,port,outputPortWidth);
}
#endif

static void mdlInitializeSampleTimes(SimStruct *S)
{
    ssSetSampleTime(S, 0, INHERITED_SAMPLE_TIME);
    ssSetOffsetTime(S, 0, 0.0);
    ssSetModelReferenceSampleTimeDefaultInheritance(S);
}

static void mdlOutputs(SimStruct *S, int_T tid)
{
    /* Input and output vectors */
    const real_T *u  = (const real_T*) ssGetInputPortSignal(S,0);
    const real_T *BldInFlow = ssGetInputPortRealSignal(S, 1);
    
    real_T *y  = (real_T *)ssGetOutputPortRealSignal(S,0);
    real_T *y1  = (real_T *)ssGetOutputPortRealSignal(S,1);
    
    /* Block name buffer length and string read status */
    int_T buflen;
    int_T status;

    /* Block mask parameter struct */
    BleedStruct bleedStruct;
    bleedStruct.WfracLen           = *mxGetPr(WfracLen_p(S));
    bleedStruct.Wfrac              = mxGetPr(Wfrac_p(S));

    /* Get name of block from dialog parameter (string) */
    buflen = mxGetN(BN_p(S))*sizeof(mxChar)+1;
    bleedStruct.BlkNm = mxMalloc(buflen);
    status = mxGetString(BN_p(S), bleedStruct.BlkNm, buflen);

    /* Perform core block calculations */
    Bleed_TMATS_body(y, y1, u, BldInFlow, &bleedStruct);
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
