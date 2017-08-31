/*		T-MATS -- Compressor_TMATS.c
 * % *************************************************************************
 * % written by Jeffryes Chapman based on work by R. May and T. Lavelle
 * % NASA Glenn Research Center, Cleveland, OH
 * % February 27th, 2013
 * %
 * %  This file models the Compressor Element for T-MATS simulation
 * % *************************************************************************/
#define S_FUNCTION_NAME  Compressor_TMATS
#define S_FUNCTION_LEVEL 2
#include "simstruc.h"
#include "types_TMATS.h"

#define Y_C_Map_NcVec_p(S)      ssGetSFcnParam(S,0)
#define X_C_RlineVec_p(S)       ssGetSFcnParam(S,1)
#define Z_C_AlphaVec_p(S)       ssGetSFcnParam(S,2)
#define T_C_Map_WcArray_p(S)    ssGetSFcnParam(S,3)
#define T_C_Map_PRArray_p(S)    ssGetSFcnParam(S,4)
#define T_C_Map_EffArray_p(S)   ssGetSFcnParam(S,5)
#define FracCusBldht_p(S)       ssGetSFcnParam(S,6)
#define FracCusBldPt_p(S)       ssGetSFcnParam(S,7)
#define FracBldht_p(S)          ssGetSFcnParam(S,8)
#define FracBldPt_p(S)          ssGetSFcnParam(S,9)
#define X_C_Map_WcSurgeVec_p(S) ssGetSFcnParam(S,10)
#define T_C_Map_PRSurgeVec_p(S) ssGetSFcnParam(S,11)
#define IDesign_p(S)            ssGetSFcnParam(S,12)
#define NcDes_p(S)              ssGetSFcnParam(S,13)
#define EffDes_p(S)             ssGetSFcnParam(S,14)
#define PRDes_p(S)              ssGetSFcnParam(S,15)
#define RlineDes_p(S)           ssGetSFcnParam(S,16)
#define CustBldEn_p(S)          ssGetSFcnParam(S,17)
#define FBldEn_p(S)             ssGetSFcnParam(S,18)
#define BN_p(S)                 ssGetSFcnParam(S,19)
#define CustBldNm_p(S)          ssGetSFcnParam(S,20)
#define FracBldNm_p(S)          ssGetSFcnParam(S,21)
#define WcMapCol_p(S)           ssGetSFcnParam(S,22)
#define PRMapCol_p(S)           ssGetSFcnParam(S,23)
#define EffMapCol_p(S)          ssGetSFcnParam(S,24)
#define WcMapRw_p(S)            ssGetSFcnParam(S,25)
#define PRMapRw_p(S)            ssGetSFcnParam(S,26)
#define EffMapRw_p(S)           ssGetSFcnParam(S,27)
#define WcMapLay_p(S)           ssGetSFcnParam(S,28)
#define PRMapLay_p(S)           ssGetSFcnParam(S,29)
#define EffMapLay_p(S)          ssGetSFcnParam(S,30)
#define NPARAMS 31
#define NERRORS 5

extern void Compressor_TMATS_body(double* y, double* y1, double* y2, const double* u, const double* Wcust, const double* FracWbld, const CompressorStruct* prm);

#define MDL_SET_WORK_WIDTHS
#if defined(MDL_SET_WORK_WIDTHS) && defined(MATLAB_MEX_FILE)
static void mdlSetWorkWidths(SimStruct *S)
{
    const char_T *rtParamNames[] = {"Y_C_Map_NcVec", "X_C_RlineVec", "Z_C_AlphaVec", "T_C_Map_WcArray", "T_C_Map_PRArray", "T_C_Map_EffArray", "FracCusBldht", "FracCusBldPt", "FracBldht", "FracBldPt", "X_C_Map_WcSurgeVec", "T_C_Map_PRSurgeVec", "IDesign", "NcDes", "EffDes", "PRDes", "RlineDes", "CustBldEn", "FBldEn", "CustBldNm", "FracBldNm", "WcMapCol", "PRMapCol", "EffMapCol", "WcMapRw", "PRMapRw", "EffMapRw", "WcMapLay", "PRMapLay", "EffMapLay"};
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
        if (i != 19)
            ssSetSFcnParamTunable(S, i, 1);
        else
            ssSetSFcnParamTunable(S, i, 0);
    }

    ssSetOptions(S, SS_OPTION_WORKS_WITH_CODE_REUSE |
                    SS_OPTION_USE_TLC_WITH_ACCELERATOR);
    
    ssSetNumContStates(S, 0);
    ssSetNumDiscStates(S, 0);
    
    if (!ssSetNumInputPorts(S, 3)) return;
    ssSetInputPortWidth(S, 0, 12);
    ssSetInputPortRequiredContiguous(S, 0, true);
    ssSetInputPortDirectFeedThrough(S, 0, 1);
    
    ssSetInputPortWidth(S, 1, DYNAMICALLY_SIZED);
    ssSetInputPortRequiredContiguous(S, 1, true);
    ssSetInputPortDirectFeedThrough(S, 1, 1);
    
    ssSetInputPortWidth(S, 2, DYNAMICALLY_SIZED);
    ssSetInputPortRequiredContiguous(S, 2, true);
    ssSetInputPortDirectFeedThrough(S, 2, 1);
    
    if (!ssSetNumOutputPorts(S, 3)) return;
    ssSetOutputPortWidth(S, 0, 27);
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
    const real_T *Wcust = ssGetInputPortRealSignal(S, 1);
    const real_T *FracWbld  = ssGetInputPortSignal(S,2);

    real_T *y  = (real_T *)ssGetOutputPortRealSignal(S,0);
    real_T *y1  = (real_T *)ssGetOutputPortRealSignal(S,1);
    real_T *y2  = (real_T *)ssGetOutputPortRealSignal(S,2);
    
    /* Block name buffer length and string read status */
    int_T buflen;
    int_T status;
    
    /* Block mask parameter struct */
    CompressorStruct compressorStruct;
    compressorStruct.NcDes              = *mxGetPr(NcDes_p(S));
    compressorStruct.PRDes              = *mxGetPr(PRDes_p(S));
    compressorStruct.EffDes             = *mxGetPr(EffDes_p(S));
    compressorStruct.RlineDes           = *mxGetPr(RlineDes_p(S));
    compressorStruct.IDes               = *mxGetPr(IDesign_p(S));
    compressorStruct.CustBldEn          = *mxGetPr(CustBldEn_p(S));
    compressorStruct.FBldEn             = *mxGetPr(FBldEn_p(S));
    compressorStruct.CustBldNm          = *mxGetPr(CustBldNm_p(S));
    compressorStruct.FracBldNm          = *mxGetPr(FracBldNm_p(S));
    /* vector & array data */
    compressorStruct.Y_C_Map_NcVec        = mxGetPr(Y_C_Map_NcVec_p(S));
    compressorStruct.X_C_RlineVec         = mxGetPr(X_C_RlineVec_p(S));
    compressorStruct.Z_C_AlphaVec         = mxGetPr(Z_C_AlphaVec_p(S));
    compressorStruct.T_C_Map_WcArray      = mxGetPr(T_C_Map_WcArray_p(S));
    compressorStruct.T_C_Map_PRArray      = mxGetPr(T_C_Map_PRArray_p(S));
    compressorStruct.T_C_Map_EffArray     = mxGetPr(T_C_Map_EffArray_p(S));
    compressorStruct.FracCusBldht         = mxGetPr(FracCusBldht_p(S));
    compressorStruct.FracCusBldPt         = mxGetPr(FracCusBldPt_p(S));
    compressorStruct.FracBldht            = mxGetPr(FracBldht_p(S));
    compressorStruct.FracBldPt            = mxGetPr(FracBldPt_p(S));
    compressorStruct.X_C_Map_WcSurgeVec   = mxGetPr(X_C_Map_WcSurgeVec_p(S));
    compressorStruct.T_C_Map_PRSurgeVec   = mxGetPr(T_C_Map_PRSurgeVec_p(S));
    /* Dimensions of parameter arrays */
    compressorStruct.A   = mxGetNumberOfElements(Y_C_Map_NcVec_p(S));
    compressorStruct.B   = mxGetNumberOfElements(X_C_RlineVec_p(S));
    compressorStruct.C   = mxGetNumberOfElements(Z_C_AlphaVec_p(S));
    compressorStruct.D   = mxGetNumberOfElements(X_C_Map_WcSurgeVec_p(S));
    compressorStruct.WcMapCol  = *mxGetPr(WcMapCol_p(S));
    compressorStruct.PRMapCol  = *mxGetPr(PRMapCol_p(S));
    compressorStruct.EffMapCol = *mxGetPr(EffMapCol_p(S));
    compressorStruct.WcMapRw   = *mxGetPr(WcMapRw_p(S));
    compressorStruct.PRMapRw   = *mxGetPr(PRMapRw_p(S));
    compressorStruct.EffMapRw  = *mxGetPr(EffMapRw_p(S));
    compressorStruct.WcMapLay   = *mxGetPr(WcMapLay_p(S));
    compressorStruct.PRMapLay   = *mxGetPr(PRMapLay_p(S));
    compressorStruct.EffMapLay  = *mxGetPr(EffMapLay_p(S));
    
    compressorStruct.IWork      = ssGetIWork(S);

    /* Get name of block from dialog parameter (string) */
    buflen = mxGetN(BN_p(S))*sizeof(mxChar)+1;
    compressorStruct.BlkNm = mxMalloc(buflen);
    status = mxGetString(BN_p(S), compressorStruct.BlkNm, buflen);

    /* Perform core block calculations */
    Compressor_TMATS_body(y, y1, y2, u, Wcust, FracWbld, &compressorStruct);
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
