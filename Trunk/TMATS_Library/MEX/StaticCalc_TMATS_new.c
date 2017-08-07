/*		T-MATS -- StaticCalc_TMATS.c
 * % *************************************************************************
 * % written by Jeffryes Chapman
 * % NASA Glenn Research Center, Cleveland, OH
 * % Feb 24, 2014
 * %
 * %  This file caclulates static Pressure and Temperature based on current
 * %  conditions and assumptions in throat area or MN.
 * % *************************************************************************/
#define S_FUNCTION_NAME  StaticCalc_TMATS_new
#define S_FUNCTION_LEVEL 2
#include "simstruc.h"
#include "types_TMATS.h"

#define AthroatIn_p(S)              ssGetSFcnParam(S,0)
#define MNIn_p(S)                   ssGetSFcnParam(S,1)
#define SolveType_p(S)              ssGetSFcnParam(S,2)
#define X_FARVec_p(S)               ssGetSFcnParam(S,3)
#define T_RtArray_p(S)              ssGetSFcnParam(S,4)
#define Y_TtVec_p(S)                ssGetSFcnParam(S,5)
#define T_gammaArray_p(S)           ssGetSFcnParam(S,6)
#define BN_p(S)                     ssGetSFcnParam(S,7)
#define NPARAMS 8
#define NERRORS 5 

extern void StaticCalc_TMATS_body(real_T *y, const real_T *u, const StaticCalcStruct* prm);

#define MDL_SET_WORK_WIDTHS
#if defined(MDL_SET_WORK_WIDTHS) && defined(MATLAB_MEX_FILE)
static void mdlSetWorkWidths(SimStruct *S)
{
    const char_T *rtParamNames[] = {"AthroatIn", "MNIn", "SolveType", "X_FARVec", "T_RtArray", "Y_TtVec", "T_gammaArray"};
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
        if (i != 7)
            ssSetSFcnParamTunable(S, i, 1);
        else
            ssSetSFcnParamTunable(S, i, 0);
    }

    ssSetOptions(S, SS_OPTION_WORKS_WITH_CODE_REUSE);
    
    ssSetNumContStates(S, 0);
    ssSetNumDiscStates(S, 0);
    
    if (!ssSetNumInputPorts(S, 1)) return;
    ssSetInputPortWidth(S, 0, 5);
    ssSetInputPortRequiredContiguous(S, 0, true);
    ssSetInputPortDirectFeedThrough(S, 0, 1);
    
    if (!ssSetNumOutputPorts(S, 1)) return;
    ssSetOutputPortWidth(S, 0, 5);
    
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
    ssSetIWorkValue(S,Er2,0);
    ssSetIWorkValue(S,Er3,0);
    ssSetIWorkValue(S,Er4,0);
    ssSetIWorkValue(S,Er5,0);
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
    StaticCalcStruct staticCalcStruct;
    staticCalcStruct.AthroatIn              = *mxGetPr(AthroatIn_p(S));  /* input throat area (sq-in) */
    staticCalcStruct.MNIn                   = *mxGetPr(MNIn_p(S));       /* input throat area (sq-in), note will use as initial guess when calculting based on Ath */
    staticCalcStruct.SolveType               = *mxGetPr(SolveType_p(S));  /* 0-solve based on Ath, 1-solve based on MNIn*/
    /* Vector & array data */
    staticCalcStruct.X_FARVec            = mxGetPr(X_FARVec_p(S));
    staticCalcStruct.T_RtArray           = mxGetPr(T_RtArray_p(S));
    staticCalcStruct.Y_TtVec             = mxGetPr(Y_TtVec_p(S));
    staticCalcStruct.T_gammaArray    = mxGetPr(T_gammaArray_p(S));
    /* Dimensions of parameter arrays */
    staticCalcStruct.A   = mxGetNumberOfElements(X_FARVec_p(S));
    staticCalcStruct.B   = mxGetNumberOfElements(Y_TtVec_p(S));

    staticCalcStruct.IWork      = ssGetIWork(S);

    /* Get name of block from dialog parameter (string) */
    buflen = mxGetN(BN_p(S))*sizeof(mxChar)+1;
    staticCalcStruct.BlkNm = mxMalloc(buflen);
    status = mxGetString(BN_p(S), staticCalcStruct.BlkNm, buflen);

    /* Perform core block calculations */
    StaticCalc_TMATS_body(y, u, &staticCalcStruct);
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
