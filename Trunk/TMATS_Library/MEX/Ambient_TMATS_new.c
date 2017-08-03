/*		T-MATS -- Ambient_TMATS.c
 * % *************************************************************************
 * % written by Jeffryes Chapman
 * % NASA Glenn Research Center, Cleveland, OH
 * % Aug 12, 2014
 * %
 * %  This file converts Altitude and MN to common engine input variables.
 * % *************************************************************************/
#define S_FUNCTION_NAME  Ambient_TMATS_new
#define S_FUNCTION_LEVEL 2
#include "simstruc.h"
#include "types_TMATS.h"

#define X_A_AltVec_p(S)      ssGetSFcnParam(S,0)
#define T_A_TsVec_p(S)       ssGetSFcnParam(S,1)
#define T_A_PsVec_p(S)       ssGetSFcnParam(S,2)
#define X_A_FARVec_p(S)      ssGetSFcnParam(S,3)
#define Y_A_TVec_p(S)        ssGetSFcnParam(S,4)
#define T_A_RtArray_p(S)     ssGetSFcnParam(S,5)
#define T_A_gammaArray_p(S)  ssGetSFcnParam(S,6)
#define AFARc_p(S)           ssGetSFcnParam(S,7)
#define BN_p(S)              ssGetSFcnParam(S,8)
#define NPARAMS 9
#define NERRORS 5

extern void Ambient_TMATS_body(real_T *y, const real_T *u, const AmbientStruct* prm);

#define MDL_SET_WORK_WIDTHS
#if defined(MDL_SET_WORK_WIDTHS) && defined(MATLAB_MEX_FILE)
static void mdlSetWorkWidths(SimStruct *S)
{
    const char_T *rtParamNames[] = {"X_A_AltVec", "T_A_TsVec", "T_A_PsVec", "X_A_FARVec", "Y_A_TVec", "T_A_RtArray", "T_A_gammaArray", "AFARc"};
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
        if (i != 8)
            ssSetSFcnParamTunable(S, i, 1);
        else
            ssSetSFcnParamTunable(S, i, 0);
    }

    ssSetOptions(S, SS_OPTION_WORKS_WITH_CODE_REUSE);
    
    ssSetNumContStates(S, 0);
    ssSetNumDiscStates(S, 0);
    
    if (!ssSetNumInputPorts(S, 1)) return;
    ssSetInputPortWidth(S, 0, 3);
    ssSetInputPortRequiredContiguous(S, 0, true);
    ssSetInputPortDirectFeedThrough(S, 0, 1);
    
    if (!ssSetNumOutputPorts(S, 1)) return;
    ssSetOutputPortWidth(S, 0, 8);
    
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
    ssSetIWorkValue(S, Er1, 0);
    ssSetIWorkValue(S, Er2, 0);
    ssSetIWorkValue(S, Er3, 0);
    ssSetIWorkValue(S, Er4, 0);
    ssSetIWorkValue(S, Er5, 0);
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
    AmbientStruct ambientStruct;
    ambientStruct.AFARc              = *mxGetPr(AFARc_p(S));
    /* Vector & array data */
    ambientStruct.X_A_AltVec        = mxGetPr(X_A_AltVec_p(S));
    ambientStruct.T_A_TsVec         = mxGetPr(T_A_TsVec_p(S));
    ambientStruct.T_A_PsVec         = mxGetPr(T_A_PsVec_p(S));
    ambientStruct.X_A_FARVec        = mxGetPr(X_A_FARVec_p(S));
    ambientStruct.T_A_RtArray       = mxGetPr(T_A_RtArray_p(S));
    ambientStruct.Y_A_TVec          = mxGetPr(Y_A_TVec_p(S));
    ambientStruct.T_A_gammaArray    = mxGetPr(T_A_gammaArray_p(S));
    /* Dimensions of parameter arrays */
    ambientStruct.A   = mxGetNumberOfElements(X_A_AltVec_p(S));
    ambientStruct.B   = mxGetNumberOfElements(X_A_FARVec_p(S));
    ambientStruct.C   = mxGetNumberOfElements(Y_A_TVec_p(S));
        
    /* Get name of block from dialog parameter (string) */
    buflen = mxGetN(BN_p(S))*sizeof(mxChar)+1;
    ambientStruct.BlkNm = mxMalloc(buflen);
    status = mxGetString(BN_p(S), ambientStruct.BlkNm, buflen);

    /* Perform core block calculations */
    Ambient_TMATS_body(y, u, &ambientStruct);
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
