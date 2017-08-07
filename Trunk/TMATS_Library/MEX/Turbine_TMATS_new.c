/*		T-MATS -- Turbine_TMATS.c
 * % *************************************************************************
 * % written by Jeffryes Chapman based on work by R. May and T. Lavelle
 * % NASA Glenn Research Center, Cleveland, OH
 * % February 15th, 2013
 * %
 * %  This file models the Turbine Element for T-MATS simulation
 * % *************************************************************************/
#define S_FUNCTION_NAME  Turbine_TMATS_new
#define S_FUNCTION_LEVEL 2
#include "simstruc.h"
#include "types_TMATS.h"

#define Y_T_NcVec_p(S)          ssGetSFcnParam(S,0)
#define X_T_PRVec_p(S)          ssGetSFcnParam(S,1)
#define T_T_Map_WcArray_p(S)    ssGetSFcnParam(S,2)
#define T_T_Map_EffArray_p(S)   ssGetSFcnParam(S,3)
#define T_BldPos_p(S)           ssGetSFcnParam(S,4)
#define BldPosLeng_p(S)         ssGetSFcnParam(S,5)
#define CoolFlwEn_p(S)          ssGetSFcnParam(S,6)
#define IDesign_p(S)            ssGetSFcnParam(S,7)
#define NcDes_p(S)              ssGetSFcnParam(S,8)
#define EffDes_p(S)             ssGetSFcnParam(S,9)
#define PRmapDes_p(S)           ssGetSFcnParam(S,10)
#define NDes_p(S)               ssGetSFcnParam(S,11)
#define BN_p(S)                 ssGetSFcnParam(S,12)
#define WcMapCol_p(S)           ssGetSFcnParam(S,13)
#define EffMapCol_p(S)          ssGetSFcnParam(S,14)
#define WcMapRw_p(S)            ssGetSFcnParam(S,15)
#define EffMapRw_p(S)           ssGetSFcnParam(S,16)
#define ConfigNPSS_p(S)         ssGetSFcnParam(S,17)
#define NPARAMS 18
#define NERRORS 5 

extern void Turbine_TMATS_body(double* y, const double* u, const double* CoolFlow, const TurbineStruct* prm);

#define MDL_SET_WORK_WIDTHS
#if defined(MDL_SET_WORK_WIDTHS) && defined(MATLAB_MEX_FILE)
static void mdlSetWorkWidths(SimStruct *S)
{
    const char_T *rtParamNames[] = {"Y_T_NcVec", "X_T_PRVec", "T_T_Map_WcArray", "T_T_Map_EffArray", "T_BldPos", "BldPosLeng", "CoolFlwEn", "IDesign", "NcDes", "EffDes", "PRmapDes", "NDes", "WcMapCol", "EffMapCol", "WcMapRw", "EffMapRw", "ConfigNPSS"};
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
        if (i != 12)
            ssSetSFcnParamTunable(S, i, 1);
        else
            ssSetSFcnParamTunable(S, i, 0);
    }

    ssSetOptions(S, SS_OPTION_WORKS_WITH_CODE_REUSE);
    
    ssSetNumContStates(S, 0);
    ssSetNumDiscStates(S, 0);
    
    if (!ssSetNumInputPorts(S, 2)) return;
    ssSetInputPortWidth(S, 0, 12);
    ssSetInputPortRequiredContiguous(S, 0, true);
    ssSetInputPortDirectFeedThrough(S, 0, 1);
    
    ssSetInputPortWidth(S, 1, DYNAMICALLY_SIZED);
    ssSetInputPortRequiredContiguous(S, 1, true);
    ssSetInputPortDirectFeedThrough(S, 1, 1);
    
    if (!ssSetNumOutputPorts(S, 1)) return;
    ssSetOutputPortWidth(S, 0, 20);
    
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
    const real_T *CoolFlow = ssGetInputPortRealSignal(S, 1); /* N 5x1 vectors consisting of W, ht, Tt, Pt and FAR, where N is the number of cooling flows */

    real_T *y  = (real_T *)ssGetOutputPortRealSignal(S,0);
    
    /* Block name buffer length and string read status */
    int_T buflen;
    int_T status;

    /* Block mask parameter struct */
    TurbineStruct turbineStruct;
    turbineStruct.NcDes              = *mxGetPr(NcDes_p(S));
    turbineStruct.PRmapDes           = *mxGetPr(PRmapDes_p(S));
    turbineStruct.EffDes             = *mxGetPr(EffDes_p(S));
    turbineStruct.NDes               = *mxGetPr(NDes_p(S));
    turbineStruct.IDes               = *mxGetPr(IDesign_p(S));
    turbineStruct. BldPosLeng         = *mxGetPr(BldPosLeng_p(S));
    turbineStruct. CoolFlwEn          = *mxGetPr(CoolFlwEn_p(S));
    turbineStruct. ConfigNPSS         = *mxGetPr(ConfigNPSS_p(S));
    /* vector & array data */
    turbineStruct.Y_T_NcVec            = mxGetPr(Y_T_NcVec_p(S));
    turbineStruct.X_T_PRVec            = mxGetPr(X_T_PRVec_p(S));
    turbineStruct.T_T_Map_WcArray      = mxGetPr(T_T_Map_WcArray_p(S));
    turbineStruct.T_T_Map_EffArray     = mxGetPr(T_T_Map_EffArray_p(S));
    turbineStruct.T_BldPos             = mxGetPr(T_BldPos_p(S));
    /* Dimensions of parameter arrays */
    turbineStruct.A   = mxGetNumberOfElements(Y_T_NcVec_p(S));
    turbineStruct.B   = mxGetNumberOfElements(X_T_PRVec_p(S));
    turbineStruct.WcMapCol   = *mxGetPr(WcMapCol_p(S));
    turbineStruct.EffMapCol  = *mxGetPr(EffMapCol_p(S));
    turbineStruct.WcMapRw    = *mxGetPr(WcMapRw_p(S));
    turbineStruct.EffMapRw   = *mxGetPr(EffMapRw_p(S));

    turbineStruct.IWork      = ssGetIWork(S);
        
    /* Get name of block from dialog parameter (string) */
    buflen = mxGetN(BN_p(S))*sizeof(mxChar)+1;
    turbineStruct.BlkNm = mxMalloc(buflen);
    status = mxGetString(BN_p(S), turbineStruct.BlkNm, buflen);

    /* Perform core block calculations */
    Turbine_TMATS_body(y, u, CoolFlow, &turbineStruct);
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
