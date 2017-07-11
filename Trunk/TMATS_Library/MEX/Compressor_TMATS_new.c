/*		T-MATS -- Compressor_TMATS.c
 * % *************************************************************************
 * % written by Jeffryes Chapman based on work by R. May and T. Lavelle
 * % NASA Glenn Research Center, Cleveland, OH
 * % February 27th, 2013
 * %
 * %  This file models the Compressor Element for T-MATS simulation
 * % *************************************************************************/
#define S_FUNCTION_NAME  Compressor_TMATS_new
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

// Forward declaration for Compressor body of calcs
extern void Compressor_TMATS_body(double*, double*, double*, const double*, const double*, const double*, const CompStruct*);

static void mdlInitializeSizes(SimStruct *S)
{
    int i;
    ssSetNumSFcnParams(S, NPARAMS);  /* Number of expected parameters */
    if (ssGetNumSFcnParams(S) != ssGetSFcnParamsCount(S)) {
        /* Return if number of expected != number of actual parameters */
        return;
    }
    
    for (i = 0; i < NPARAMS; i++)
        if (i != 19) {
            ssSetSFcnParamTunable(S, i, 1);
        } else {
            ssSetSFcnParamTunable(S, i, 0);
        }
    
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
    CompStruct compPrms;
    /*--------Define Parameters-------*/
    compPrms.NcDes              = *mxGetPr(NcDes_p(S));
    compPrms.PRDes              = *mxGetPr(PRDes_p(S));
    compPrms.EffDes             = *mxGetPr(EffDes_p(S));
    compPrms.RlineDes           = *mxGetPr(RlineDes_p(S));
    compPrms.IDes               = *mxGetPr(IDesign_p(S));
    compPrms.CustBldEn          = *mxGetPr(CustBldEn_p(S));
    compPrms.FBldEn             = *mxGetPr(FBldEn_p(S));
    compPrms.CustBldNm          = *mxGetPr(CustBldNm_p(S));
    compPrms.FracBldNm          = *mxGetPr(FracBldNm_p(S));
    
    /* vector & array data */
    compPrms.Y_C_Map_NcVec        = mxGetPr(Y_C_Map_NcVec_p(S));
    compPrms.X_C_RlineVec         = mxGetPr(X_C_RlineVec_p(S));
    compPrms.Z_C_AlphaVec         = mxGetPr(Z_C_AlphaVec_p(S));
    compPrms.T_C_Map_WcArray      = mxGetPr(T_C_Map_WcArray_p(S));
    compPrms.T_C_Map_PRArray      = mxGetPr(T_C_Map_PRArray_p(S));
    compPrms.T_C_Map_EffArray     = mxGetPr(T_C_Map_EffArray_p(S));
    
    compPrms.FracCusBldht         = mxGetPr(FracCusBldht_p(S));
    compPrms.FracCusBldPt         = mxGetPr(FracCusBldPt_p(S));
    compPrms.FracBldht            = mxGetPr(FracBldht_p(S));
    compPrms.FracBldPt            = mxGetPr(FracBldPt_p(S));
    
    compPrms.X_C_Map_WcSurgeVec   = mxGetPr(X_C_Map_WcSurgeVec_p(S));
    compPrms.T_C_Map_PRSurgeVec   = mxGetPr(T_C_Map_PRSurgeVec_p(S));
    
    /*------get dimensions of parameter arrays-------*/
    compPrms.Y_C_Map_NcVecLen       = mxGetNumberOfElements(Y_C_Map_NcVec_p(S));
    compPrms.X_C_RlineVecLen        = mxGetNumberOfElements(X_C_RlineVec_p(S));
    compPrms.Z_C_AlphaVecLen        = mxGetNumberOfElements(Z_C_AlphaVec_p(S));
    compPrms.X_C_Map_WcSurgeVecLen  = mxGetNumberOfElements(X_C_Map_WcSurgeVec_p(S));
    
    compPrms.WcMapCol  = *mxGetPr(WcMapCol_p(S));
    compPrms.PRMapCol  = *mxGetPr(PRMapCol_p(S));
    compPrms.EffMapCol = *mxGetPr(EffMapCol_p(S));
    compPrms.WcMapRw   = *mxGetPr(WcMapRw_p(S));
    compPrms.PRMapRw   = *mxGetPr(PRMapRw_p(S));
    compPrms.EffMapRw  = *mxGetPr(EffMapRw_p(S));
    compPrms.WcMapLay   = *mxGetPr(WcMapLay_p(S));
    compPrms.PRMapLay   = *mxGetPr(PRMapLay_p(S));
    compPrms.EffMapLay  = *mxGetPr(EffMapLay_p(S));
    
    /*---------Define Inputs for input port 1--------*/
    const real_T *u  = (const real_T*) ssGetInputPortSignal(S,0);
    
    /*---------Define Inputs for input port 2--------*/
    const real_T *Wcust = ssGetInputPortRealSignal(S, 1);
    
    /*---------Define Inputs for input port 3--------*/
    const real_T *FracWbld  = ssGetInputPortSignal(S,2);
    
    real_T *y  = (real_T *)ssGetOutputPortRealSignal(S,0);   /* Output Array port 1 */
    real_T *y1  = (real_T *)ssGetOutputPortRealSignal(S,1);   /* Output Array port 2 */
    real_T *y2  = (real_T *)ssGetOutputPortRealSignal(S,2);   /* Output Array port 3 */
	
	Compressor_TMATS_body(y, y1, y2, u, Wcust, FracWbld, &compPrms);
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
    real_T* PRDes              = mxGetPr(PRDes_p(S));
    real_T* EffDes             = mxGetPr(EffDes_p(S));
    real_T* RlineDes           = mxGetPr(RlineDes_p(S));
    real_T* IDes               = mxGetPr(IDesign_p(S));
    real_T* CustBldEn          = mxGetPr(CustBldEn_p(S));
    real_T* FBldEn             = mxGetPr(FBldEn_p(S));
    real_T* CustBldNm          = mxGetPr(CustBldNm_p(S));
    real_T* FracBldNm          = mxGetPr(FracBldNm_p(S));
    real_T* Y_C_Map_NcVec      = mxGetPr(Y_C_Map_NcVec_p(S));
    real_T* X_C_RlineVec       = mxGetPr(X_C_RlineVec_p(S));
    real_T* Z_C_AlphaVec       = mxGetPr(Z_C_AlphaVec_p(S));
    real_T* T_C_Map_WcArray    = mxGetPr(T_C_Map_WcArray_p(S));
    real_T* T_C_Map_PRArray    = mxGetPr(T_C_Map_PRArray_p(S));
    real_T* T_C_Map_EffArray   = mxGetPr(T_C_Map_EffArray_p(S));
    real_T* FracCusBldht       = mxGetPr(FracCusBldht_p(S));
    real_T* FracCusBldPt       = mxGetPr(FracCusBldPt_p(S));
    real_T* FracBldht          = mxGetPr(FracBldht_p(S));
    real_T* FracBldPt          = mxGetPr(FracBldPt_p(S));
    real_T* X_C_Map_WcSurgeVec = mxGetPr(X_C_Map_WcSurgeVec_p(S));
    real_T* T_C_Map_PRSurgeVec = mxGetPr(T_C_Map_PRSurgeVec_p(S));
    real_T* WcMapCol           = mxGetPr(WcMapCol_p(S));
    real_T* PRMapCol           = mxGetPr(PRMapCol_p(S));
    real_T* EffMapCol          = mxGetPr(EffMapCol_p(S));
    real_T* WcMapRw            = mxGetPr(WcMapRw_p(S));
    real_T* PRMapRw            = mxGetPr(PRMapRw_p(S));
    real_T* EffMapRw           = mxGetPr(EffMapRw_p(S));
    real_T* WcMapLay           = mxGetPr(WcMapLay_p(S));
    real_T* PRMapLay           = mxGetPr(PRMapLay_p(S));
    real_T* EffMapLay          = mxGetPr(EffMapLay_p(S));
    
    if (!ssWriteRTWParameters(S, 30,
        SSWRITE_VALUE_VECT, "Y_C_Map_NcVec",      "",  Y_C_Map_NcVec,      mxGetNumberOfElements(Y_C_Map_NcVec_p(S)),
        SSWRITE_VALUE_VECT, "X_C_RlineVec",       "",  X_C_RlineVec,       mxGetNumberOfElements(X_C_RlineVec_p(S)),
        SSWRITE_VALUE_VECT, "Z_C_AlphaVec",       "",  Z_C_AlphaVec,       mxGetNumberOfElements(Z_C_AlphaVec_p(S)),
        SSWRITE_VALUE_VECT, "T_C_Map_WcArray",    "",  T_C_Map_WcArray,    mxGetNumberOfElements(T_C_Map_WcArray_p(S)),
        SSWRITE_VALUE_VECT, "T_C_Map_PRArray",    "",  T_C_Map_PRArray,    mxGetNumberOfElements(T_C_Map_PRArray_p(S)),
        SSWRITE_VALUE_VECT, "T_C_Map_EffArray",   "",  T_C_Map_EffArray,   mxGetNumberOfElements(T_C_Map_EffArray_p(S)),
        SSWRITE_VALUE_VECT, "FracCusBldht",       "",  FracCusBldht,       mxGetNumberOfElements(FracCusBldht_p(S)),
        SSWRITE_VALUE_VECT, "FracCusBldPt",       "",  FracCusBldPt,       mxGetNumberOfElements(FracCusBldPt_p(S)),
        SSWRITE_VALUE_VECT, "FracBldht",          "",  FracBldht,          mxGetNumberOfElements(FracBldht_p(S)),
        SSWRITE_VALUE_VECT, "FracBldPt",          "",  FracBldPt,          mxGetNumberOfElements(FracBldPt_p(S)),
        SSWRITE_VALUE_VECT, "X_C_Map_WcSurgeVec", "",  X_C_Map_WcSurgeVec, mxGetNumberOfElements(X_C_Map_WcSurgeVec_p(S)),
        SSWRITE_VALUE_VECT, "T_C_Map_PRSurgeVec", "",  T_C_Map_PRSurgeVec, mxGetNumberOfElements(T_C_Map_PRSurgeVec_p(S)),
		SSWRITE_VALUE_VECT, "IDes",               "",  IDes,               1,
        SSWRITE_VALUE_VECT, "NcDes",              "",  NcDes,              1,
        SSWRITE_VALUE_VECT, "EffDes",             "",  EffDes,             1,
        SSWRITE_VALUE_VECT, "PRDes",              "",  PRDes,              1,
        SSWRITE_VALUE_VECT, "RlineDes",           "",  RlineDes,           1,
        SSWRITE_VALUE_VECT, "CustBldEn",          "",  CustBldEn,          1,
        SSWRITE_VALUE_VECT, "FBldEn",             "",  FBldEn,             1,
        SSWRITE_VALUE_VECT, "CustBldNm",          "",  CustBldNm,          1,
        SSWRITE_VALUE_VECT, "FracBldNm",          "",  FracBldNm,          1,
        SSWRITE_VALUE_VECT, "WcMapCol",           "",  WcMapCol,           1,
        SSWRITE_VALUE_VECT, "PRMapCol",           "",  PRMapCol,           1,
        SSWRITE_VALUE_VECT, "EffMapCol",          "",  EffMapCol,          1,
        SSWRITE_VALUE_VECT, "WcMapRw",            "",  WcMapRw,            1,
        SSWRITE_VALUE_VECT, "PRMapRw",            "",  PRMapRw,            1,
        SSWRITE_VALUE_VECT, "EffMapRw",           "",  EffMapRw,           1,
        SSWRITE_VALUE_VECT, "WcMapLay",           "",  WcMapLay,           1,
        SSWRITE_VALUE_VECT, "PRMapLay",           "",  PRMapLay,           1,
        SSWRITE_VALUE_VECT, "EffMapLay",          "",  EffMapLay,          1
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
