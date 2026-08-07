/*		T-MATS -- Compressor_Bleeds_TMATS.c
 * % *************************************************************************
 * % written by Jonathan Kratz based on work by Jeffryes Chapman
 * % NASA Glenn Research Center, Cleveland, OH
 * % January 21st, 2025
 * %
 * %  This file models the Compressor Bleeds T-MATS simulation and has been
 * %  created to enable adjustements to compressor performance due to heat
 * %  soak and tip clearance effects. The code was developed to be employed
 * %  in the Compressor_HX block.
 * % *************************************************************************/
#define S_FUNCTION_NAME  Compressor_Bleeds_TMATS
#define S_FUNCTION_LEVEL 2
#include "simstruc.h"
#include "types_TMATS.h"

#define FracCusBldht_p(S)       ssGetSFcnParam(S,0)
#define FracCusBldPt_p(S)       ssGetSFcnParam(S,1)
#define FracBldht_p(S)          ssGetSFcnParam(S,2)
#define FracBldPt_p(S)          ssGetSFcnParam(S,3)
#define CustBldEn_p(S)          ssGetSFcnParam(S,4)
#define FBldEn_p(S)             ssGetSFcnParam(S,5)
#define BN_p(S)                 ssGetSFcnParam(S,6)
#define CustBldNm_p(S)          ssGetSFcnParam(S,7)
#define FracBldNm_p(S)          ssGetSFcnParam(S,8)
#define NPARAMS 9
#define NERRORS 5

extern void Compressor_Bleeds_TMATS_body(double* y, double* y1, double* y2, const double* u, const double* Wcust, const double* FracWbld, const CompressorStruct* prm);

#define MDL_SET_WORK_WIDTHS
#if defined(MDL_SET_WORK_WIDTHS) && defined(MATLAB_MEX_FILE)
static void mdlSetWorkWidths(SimStruct *S)
{
    const char_T *rtParamNames[] = {"FracCusBldht", "FracCusBldPt", "FracBldht", "FracBldPt", "CustBldEn", "FBldEn", "CustBldNm", "FracBldNm"};
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
        if (i != 6)
            ssSetSFcnParamTunable(S, i, 1);
        else
            ssSetSFcnParamTunable(S, i, 0);
    }

    ssSetOptions(S, SS_OPTION_WORKS_WITH_CODE_REUSE |
                    SS_OPTION_USE_TLC_WITH_ACCELERATOR);
    
    ssSetNumContStates(S, 0);
    ssSetNumDiscStates(S, 0);
    
    if (!ssSetNumInputPorts(S, 3)) return;
    ssSetInputPortWidth(S, 0, 7); 
    ssSetInputPortRequiredContiguous(S, 0, true);
    ssSetInputPortDirectFeedThrough(S, 0, 1);
    
    ssSetInputPortWidth(S, 1, DYNAMICALLY_SIZED);
    ssSetInputPortRequiredContiguous(S, 1, true);
    ssSetInputPortDirectFeedThrough(S, 1, 1);
    
    ssSetInputPortWidth(S, 2, DYNAMICALLY_SIZED);
    ssSetInputPortRequiredContiguous(S, 2, true);
    ssSetInputPortDirectFeedThrough(S, 2, 1);

    if (!ssSetNumOutputPorts(S, 3)) return;
    ssSetOutputPortWidth(S, 0, 3);
    ssSetOutputPortWidth(S, 1, DYNAMICALLY_SIZED);
    ssSetOutputPortWidth(S, 2, DYNAMICALLY_SIZED);
    
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
    const real_T *Wcust = ssGetInputPortRealSignal(S,1);
    const real_T *FracWbld  = ssGetInputPortSignal(S,2);

    real_T *y  = (real_T *)ssGetOutputPortRealSignal(S,0);
    real_T *y1  = (real_T *)ssGetOutputPortRealSignal(S,1);
    real_T *y2  = (real_T *)ssGetOutputPortRealSignal(S,2);
    
    /* Block name buffer length and string read status */
    int_T buflen;
    int_T status;
    
    /* Block mask parameter struct */
    CompressorStruct compressorStruct;
    compressorStruct.CustBldEn          = *mxGetPr(CustBldEn_p(S));
    compressorStruct.FBldEn             = *mxGetPr(FBldEn_p(S));
    compressorStruct.CustBldNm          = *mxGetPr(CustBldNm_p(S));
    compressorStruct.FracBldNm          = *mxGetPr(FracBldNm_p(S));
    /* vector & array data */
    compressorStruct.FracCusBldht         = mxGetPr(FracCusBldht_p(S));
    compressorStruct.FracCusBldPt         = mxGetPr(FracCusBldPt_p(S));
    compressorStruct.FracBldht            = mxGetPr(FracBldht_p(S));
    compressorStruct.FracBldPt            = mxGetPr(FracBldPt_p(S));
    
    compressorStruct.IWork      = ssGetIWork(S);

    /* Get name of block from dialog parameter (string) */
    buflen = mxGetN(BN_p(S))*sizeof(mxChar)+1;
    compressorStruct.BlkNm = malloc(buflen);
    status = mxGetString(BN_p(S), compressorStruct.BlkNm, buflen);

    /* Perform core block calculations */
    Compressor_Bleeds_TMATS_body(y, y1, y2, u, Wcust, FracWbld, &compressorStruct);
    free(compressorStruct.BlkNm);
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
