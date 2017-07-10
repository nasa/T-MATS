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

// Forward declaration for Turbine body of calcs
extern void Turbine_TMATS_body(double*, const double*, const double*, const TurbStruct*);

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
}
#endif

static void mdlOutputs(SimStruct *S, int_T tid)
{
    TurbStruct turbPrms;
    /*--------Define Parameters-------*/
    turbPrms.NcDes              = *mxGetPr(NcDes_p(S));
    turbPrms.PRmapDes           = *mxGetPr(PRmapDes_p(S));
    turbPrms.EffDes             = *mxGetPr(EffDes_p(S));
    turbPrms.NDes               = *mxGetPr(NDes_p(S));
    turbPrms.IDes               = *mxGetPr(IDesign_p(S));
    turbPrms.BldPosLeng         = *mxGetPr(BldPosLeng_p(S));
    turbPrms.CoolFlwEn          = *mxGetPr(CoolFlwEn_p(S));
    turbPrms.ConfigNPSS         = *mxGetPr(ConfigNPSS_p(S));
    
    /* vector & array data */
    turbPrms.Y_T_NcVec            = mxGetPr(Y_T_NcVec_p(S));
    turbPrms.X_T_PRVec            = mxGetPr(X_T_PRVec_p(S));
    turbPrms.T_T_Map_WcArray      = mxGetPr(T_T_Map_WcArray_p(S));
    turbPrms.T_T_Map_EffArray     = mxGetPr(T_T_Map_EffArray_p(S));
    turbPrms.T_BldPos             = mxGetPr(T_BldPos_p(S));
    
    /*------get dimensions of parameter arrays-------*/
    turbPrms.A   = mxGetNumberOfElements(Y_T_NcVec_p(S));
    turbPrms.B   = mxGetNumberOfElements(X_T_PRVec_p(S));
    
    turbPrms.WcMapCol   = *mxGetPr(WcMapCol_p(S));
    turbPrms.EffMapCol  = *mxGetPr(EffMapCol_p(S));
    turbPrms.WcMapRw    = *mxGetPr(WcMapRw_p(S));
    turbPrms.EffMapRw   = *mxGetPr(EffMapRw_p(S));
    
    /* ------- get strings -------------- */
    int_T buflen;
    int_T status;
    
    /* Get name of block from dialog parameter (string) */
    buflen = mxGetN(BN_p(S))*sizeof(mxChar)+1;
    turbPrms.BlkNm = mxMalloc(buflen);
    status = mxGetString(BN_p(S), turbPrms.BlkNm, buflen);
    
    turbPrms.IWork = ssGetIWork(S);
    
    /*---------Define Inputs--------*/
    const real_T *u  = (const real_T*) ssGetInputPortSignal(S,0);
    
    /*---------Define Inputs for input port 2--------*/
    /* N 5x1 vectors consisting of W, ht, Tt, Pt and FAR, where N is the number of cooling flows */
    const real_T *CoolFlow = ssGetInputPortRealSignal(S, 1);
    
    real_T *y  = (real_T *)ssGetOutputPortRealSignal(S,0);   /* Output Array */
    
    /* Verify input bleed vector is a multiple of 5 */
    int cfWidth, Vtest, i;
    cfWidth = u[11];
    Vtest = cfWidth/5;
    
    Turbine_TMATS_body(y,u, CoolFlow, &turbPrms);
}

static void mdlTerminate(SimStruct *S)
{
}

#define MDL_RTW                        /* Change to #undef to remove function */
#if defined(MDL_RTW) && (defined(MATLAB_MEX_FILE) || defined(NRT))
/* Function: mdlRTW ============================================================
 * Abstract:
 *    This function is called when Real-Time Workshop is generating the
 *    model.rtw file. In this routine, you can call the following functions
 *    which add fields to the model.rtw file.
 *
 *    Important! Since this s-function has this mdlRTW method, it is required
 *    to have a corresponding .tlc file so as to work with RTW. You will find
 *    the sfun_directlook.tlc in <matlaroot>/toolbox/simulink/blocks/tlc_c/.
 */
static void mdlRTW(SimStruct *S)
{
    real_T* NcDes              = mxGetPr(NcDes_p(S));
    real_T* PRmapDes           = mxGetPr(PRmapDes_p(S));
    real_T* EffDes             = mxGetPr(EffDes_p(S));
    real_T* NDes               = mxGetPr(NDes_p(S));
    real_T* IDes               = mxGetPr(IDesign_p(S));
    real_T* BldPosLeng         = mxGetPr(BldPosLeng_p(S));
    real_T* CoolFlwEn          = mxGetPr(CoolFlwEn_p(S));
    real_T* ConfigNPSS         = mxGetPr(ConfigNPSS_p(S));
    real_T* Y_T_NcVec          = mxGetPr(Y_T_NcVec_p(S));
    real_T* X_T_PRVec          = mxGetPr(X_T_PRVec_p(S));
    real_T* T_T_Map_WcArray    = mxGetPr(T_T_Map_WcArray_p(S));
    real_T* T_T_Map_EffArray   = mxGetPr(T_T_Map_EffArray_p(S));
    real_T* T_BldPos           = mxGetPr(T_BldPos_p(S));
    real_T* WcMapCol           = mxGetPr(WcMapCol_p(S));
    real_T* EffMapCol          = mxGetPr(EffMapCol_p(S));
    real_T* WcMapRw            = mxGetPr(WcMapRw_p(S));
    real_T* EffMapRw           = mxGetPr(EffMapRw_p(S));
    
    if (!ssWriteRTWParameters(S, 17,
        SSWRITE_VALUE_VECT, "Y_T_NcVec",        "",  Y_T_NcVec,        mxGetNumberOfElements(Y_T_NcVec_p(S)),
        SSWRITE_VALUE_VECT, "X_T_PRVec",        "",  X_T_PRVec,        mxGetNumberOfElements(X_T_PRVec_p(S)),
        SSWRITE_VALUE_VECT, "T_T_Map_WcArray",  "",  T_T_Map_WcArray,  mxGetNumberOfElements(T_T_Map_WcArray_p(S)),
        SSWRITE_VALUE_VECT, "T_T_Map_EffArray", "",  T_T_Map_EffArray, mxGetNumberOfElements(T_T_Map_EffArray_p(S)),
        SSWRITE_VALUE_VECT, "T_BldPos",         "",  T_BldPos,         mxGetNumberOfElements(T_BldPos_p(S)),
        SSWRITE_VALUE_VECT, "BldPosLeng",       "",  BldPosLeng,       1,
        SSWRITE_VALUE_VECT, "CoolFlwEn",        "",  CoolFlwEn,        1,
        SSWRITE_VALUE_VECT, "IDes",             "",  IDes,             1,
        SSWRITE_VALUE_VECT, "NcDes",            "",  NcDes,            1,
        SSWRITE_VALUE_VECT, "EffDes",           "",  EffDes,           1,
        SSWRITE_VALUE_VECT, "PRmapDes",         "",  PRmapDes,         1,
        SSWRITE_VALUE_VECT, "NDes",             "",  NDes,             1,
        SSWRITE_VALUE_VECT, "WcMapCol",         "",  WcMapCol,         1,
        SSWRITE_VALUE_VECT, "EffMapCol",        "",  EffMapCol,        1,
        SSWRITE_VALUE_VECT, "WcMapRw",          "",  WcMapRw,          1,
        SSWRITE_VALUE_VECT, "EffMapRw",         "",  EffMapRw,         1,
        SSWRITE_VALUE_VECT, "ConfigNPSS",       "",  ConfigNPSS,       1
        ))
    {
        return;/* An error occurred which will be reported by Simulink */
    }
}

#endif /* MDL_RTW */

#ifdef  MATLAB_MEX_FILE    /* Is this file being compiled as a MEX-file? */
#include "simulink.c"      /* MEX-file interface mechanism */
#else
#include "cg_sfun.h"       /* Code generation registration function */
#endif
/*==================*/
