/*		T-MATS -- Nozzle_TMATS.c
 * % *************************************************************************
 * % written by Jeffryes Chapman based on work by R. May and T. Lavelle
 * % NASA Glenn Research Center, Cleveland, OH
 * % February 27th, 2013
 * %
 * %  This file models the Nozzle Element for T-MATS simulation
 * % *************************************************************************/
#define S_FUNCTION_NAME  Nozzle_TMATS_new
#define S_FUNCTION_LEVEL 2
#include "simstruc.h"
#include "constants_TMATS.h"
#include "functions_TMATS.h"
#include "types_TMATS.h"
#include <math.h>

#define SwitchType_p(S)             ssGetSFcnParam(S,0)
#define flowLoss_p(S)               ssGetSFcnParam(S,1)
#define Y_N_FARVec_p(S)             ssGetSFcnParam(S,2)
#define T_N_RtArray_p(S)            ssGetSFcnParam(S,3)
#define X_N_TtVec_p(S)              ssGetSFcnParam(S,4)
#define T_N_MAP_gammaArray_p(S)     ssGetSFcnParam(S,5)
#define X_N_PEQPaVec_p(S)           ssGetSFcnParam(S,6)
#define T_N_CdThArray_p(S)          ssGetSFcnParam(S,7)
#define T_N_CvArray_p(S)            ssGetSFcnParam(S,8)
#define T_N_CfgArray_p(S)           ssGetSFcnParam(S,9)
#define T_N_TGArray_p(S)            ssGetSFcnParam(S,10)
#define X_N_TtVecTG_p(S)            ssGetSFcnParam(S,11)
#define iDesign_p(S)                ssGetSFcnParam(S,12)
#define WDes_p(S)                   ssGetSFcnParam(S,13)
#define CfgEn_p(S)                  ssGetSFcnParam(S,14)
#define BN_p(S)                     ssGetSFcnParam(S,15)
#define NPARAMS 16

// extern double pt2sc(double c, double d, double e);
// extern double sp2tc(double f, double g, double h);
// extern double t2hc(double i, double j);
// extern double interp1Ac(double a[], double b[], double c, int d, int *error);
// extern double interp2Ac(double aa[], double bb[], double cc[], double aaa, double bbb,int ccc, int ddd, int *error);
// extern void PcalcStat(double dd,double ee,double ff,double gg,double hh,double mm,double *nn,double *oo,double *pp,double *qq,double *rr);

extern void Nozzle_TMATS_body(double*, const double*, NozzleStruct*);
NozzleStruct nozPrms;

static void mdlInitializeSizes(SimStruct *S)
{
    int i;
    ssSetNumSFcnParams(S, NPARAMS);  /* Number of expected parameters */
    if (ssGetNumSFcnParams(S) != ssGetSFcnParamsCount(S)) {
        /* Return if number of expected != number of actual parameters */
        return;
    }
    
    for (i = 0; i < NPARAMS; i++) {
        if (i != 15)
            ssSetSFcnParamTunable(S, i, 1);
        else
            ssSetSFcnParamTunable(S, i, 0);
    }
    
    ssSetNumContStates(S, 0);
    ssSetNumDiscStates(S, 0);
    
    if (!ssSetNumInputPorts(S, 1)) return;
    ssSetInputPortWidth(S, 0, 8);
    ssSetInputPortRequiredContiguous(S, 0, true);
    ssSetInputPortDirectFeedThrough(S, 0, 1);
    
    if (!ssSetNumOutputPorts(S, 1)) return;
    ssSetOutputPortWidth(S, 0, 17);
    
    ssSetNumSampleTimes(S, 1);
    ssSetNumRWork(S, 0);
    ssSetNumIWork(S, NUM_IWORK);
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
    /*ssSetIWorkValue(S,Er1,0);
    ssSetIWorkValue(S,Er2,0);
    ssSetIWorkValue(S,Er3,0);
    ssSetIWorkValue(S,Er4,0);
    ssSetIWorkValue(S,Er5,0);
    ssSetIWorkValue(S,Er6,0);
    ssSetIWorkValue(S,Er7,0);
    ssSetIWorkValue(S,Er8,0);
    ssSetIWorkValue(S,Er9,0);
    ssSetIWorkValue(S,Er10,0);
    ssSetIWorkValue(S,Er11,0);
    ssSetIWorkValue(S,Er12,0);
    ssSetIWorkValue(S,Er13,0);
    ssSetIWorkValue(S,Er14,0);
    ssSetIWorkValue(S,Er15,0);
    ssSetIWorkValue(S,Er16,0);*/
}
#endif

static void mdlOutputs(SimStruct *S, int_T tid)
{
    /*--------parameters defined in S-function block--------*/
    nozPrms.SwitchType             = *mxGetPr(SwitchType_p(S)); /* Nozzle type */
    nozPrms.flowLoss               = *mxGetPr(flowLoss_p(S));
    nozPrms.IDes                   = *mxGetPr(iDesign_p(S));
    nozPrms.WDes                   = *mxGetPr(WDes_p(S));
    nozPrms.CfgEn                  = *mxGetPr(CfgEn_p(S));
    
    /* vector & array data */
    nozPrms.Y_N_FARVec            = mxGetPr(Y_N_FARVec_p(S));
    nozPrms.T_N_RtArray           = mxGetPr(T_N_RtArray_p(S));
    nozPrms.X_N_TtVec             = mxGetPr(X_N_TtVec_p(S));
    nozPrms.T_N_MAP_gammaArray    = mxGetPr(T_N_MAP_gammaArray_p(S));
    nozPrms.X_N_PEQPaVec          = mxGetPr(X_N_PEQPaVec_p(S));
    nozPrms.T_N_CdThArray         = mxGetPr(T_N_CdThArray_p(S));
    nozPrms.T_N_CvArray           = mxGetPr(T_N_CvArray_p(S));
    nozPrms.T_N_CfgArray          = mxGetPr(T_N_CfgArray_p(S));
    nozPrms.T_N_TGArray           = mxGetPr(T_N_TGArray_p(S));
    nozPrms.X_N_TtVecTG           = mxGetPr(X_N_TtVecTG_p(S));
    
    /*------get dimensions of parameter arrays-------*/
    nozPrms.A   = mxGetNumberOfElements(Y_N_FARVec_p(S));
    nozPrms.B   = mxGetNumberOfElements(X_N_TtVec_p(S));
    nozPrms.B1  = mxGetNumberOfElements(X_N_PEQPaVec_p(S));
    nozPrms.C   = mxGetNumberOfElements(X_N_TtVecTG_p(S));
    
    
    /*---------Define Inputs--------*/
    const real_T *u  = (const real_T*) ssGetInputPortSignal(S,0);
    
    real_T *y  = (real_T *)ssGetOutputPortRealSignal(S,0);   /* Output Array */
    
    /* ------- get strings -------------- */
    //char * BlkNm;
    //int_T buflen;
    //int_T status;
    
    /* Get name of block from dialog parameter (string) */
    //buflen = mxGetN(BN_p(S))*sizeof(mxChar)+1;
    //BlkNm = mxMalloc(buflen);
    //status = mxGetString(BN_p(S), BlkNm, buflen);
    
    /*if (interpErr == 1 && ssGetIWork(S)[Er1]==0){
        printf("Warning in %s, Error calculating Rt1. Vector definitions may need to be expanded.\n", BlkNm);
        ssSetIWorkValue(S,Er1,1);
    }

        if (ssGetIWork(S)[Er2]==0){
            printf("Warning in %s, Backflow warning PtIn <= Pamb\n", BlkNm);
            ssSetIWorkValue(S,Er2,1);
        }

	if (interpErr == 1 && ssGetIWork(S)[Er3]==0){
        printf("Warning in %s, Error calculating gammas. Vector definitions may need to be expanded.\n", BlkNm);
        ssSetIWorkValue(S,Er3,1);
    }
    
    if (interpErr == 1 && ssGetIWork(S)[Er4]==0){
        printf("Warning in %s, Error calculating gammatg. Vector definitions may need to be expanded.\n", BlkNm);
        ssSetIWorkValue(S,Er4,1);
    }
	
	if (interpErr == 1 && ssGetIWork(S)[Er4]==0){
        printf("Warning in %s, Error calculating gammasg. Vector definitions may need to be expanded.\n", BlkNm);
        ssSetIWorkValue(S,Er4,1);
    }
    
		if (interpErr == 1 && ssGetIWork(S)[Er5]==0){
            printf("Warning in %s, Error calculating iteration gammasg. Vector definitions may need to be expanded.\n", BlkNm);
            ssSetIWorkValue(S,Er5,1);
        }
	
    if (iter == maxiter && ssGetIWork(S)[Er6]==0 ){
        printf("Warning in %s, Error calculating Ps at MN = 1.\n", BlkNm);
        ssSetIWorkValue(S,Er6,1);
    }
	
		if (interpErr == 1 && ssGetIWork(S)[Er7]==0){
            printf("Warning in %s, Error calculating iteration gammasg. Vector definitions may need to be expanded.\n", BlkNm);
            ssSetIWorkValue(S,Er7,1);
        }
    
    /* error('Nozzle Error: Negative Mach number!!')
    if (MNth<0 && ssGetIWork(S)[Er8]==0){
        printf("Error in %s: negative throat mach number,  MN = %f.\n", BlkNm, MNth);
        ssSetIWorkValue(S,Er8,1);
    }
    
    if (interpErr == 1 && ssGetIWork(S)[Er9]==0){
        printf("Warning in %s, Error calculating CdTh. Vector definitions may need to be expanded.\n", BlkNm);
        ssSetIWorkValue(S,Er9,1);
    }
    
	if (interpErr == 1 && ssGetIWork(S)[Er10]==0){
        printf("Warning in %s, Error calculating Therm_growth. Vector definitions may need to be expanded.\n", BlkNm);
        ssSetIWorkValue(S,Er10,1);
    }
    
		if (choked == 0 && ssGetIWork(S)[Er11]==0){
            printf("Warning in %s, Calculating IDes Area with un-choked nozzle.\n", BlkNm);
            ssSetIWorkValue(S,Er11,1);
        }
    
        if (iterx == maxiterx && ssGetIWork(S)[Er12]==0 ){
            printf("Warning in %s, Error calculating Ps at exit.\n", BlkNm);
            ssSetIWorkValue(S,Er12,1);
        }

        if (interpErr == 1 && ssGetIWork(S)[Er13]==0){
            printf("Warning in %s, Error calculating gammas. Vector definitions may need to be expanded.\n", BlkNm);
            ssSetIWorkValue(S,Er13,1);
        }
    
    /* error('Nozzle Error: Negative Mach number!!')
    if (MNx<0 && ssGetIWork(S)[Er14]==0){
        printf("Error in %s: negative exit mach number,  MN = %f.\n", BlkNm, MNx);
        ssSetIWorkValue(S,Er14,1);
    }
    
        if (interpErr == 1 && ssGetIWork(S)[Er15]==0){
            printf("Warning in %s, Error calculating Cv. Vector definitions may need to be expanded.\n", BlkNm);
            ssSetIWorkValue(S,Er15,1);
        }

        if (interpErr == 1 && ssGetIWork(S)[Er16]==0){
            printf("Warning in %s, Error calculating Cfg. Vector definitions may need to be expanded.\n", BlkNm);
            ssSetIWorkValue(S,Er16,1);
        }
    */
	
	Nozzle_TMATS_body(y, u, &nozPrms);
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
    real_T* SwitchType            = mxGetPr(SwitchType_p(S));
    real_T* flowLoss              = mxGetPr(flowLoss_p(S));
    real_T* IDes                  = mxGetPr(iDesign_p(S));
    real_T* WDes                  = mxGetPr(WDes_p(S));
    real_T* CfgEn                 = mxGetPr(CfgEn_p(S));
    real_T* Y_N_FARVec            = mxGetPr(Y_N_FARVec_p(S));
    real_T* T_N_RtArray           = mxGetPr(T_N_RtArray_p(S));
    real_T* X_N_TtVec             = mxGetPr(X_N_TtVec_p(S));
    real_T* T_N_MAP_gammaArray    = mxGetPr(T_N_MAP_gammaArray_p(S));
    real_T* X_N_PEQPaVec          = mxGetPr(X_N_PEQPaVec_p(S));
    real_T* T_N_CdThArray         = mxGetPr(T_N_CdThArray_p(S));
    real_T* T_N_CvArray           = mxGetPr(T_N_CvArray_p(S));
    real_T* T_N_CfgArray          = mxGetPr(T_N_CfgArray_p(S));
    real_T* T_N_TGArray           = mxGetPr(T_N_TGArray_p(S));
    real_T* X_N_TtVecTG           = mxGetPr(X_N_TtVecTG_p(S));
    
    if (!ssWriteRTWParameters(S, 15,
		SSWRITE_VALUE_VECT, "SwitchType",         "",  SwitchType,         1,
		SSWRITE_VALUE_VECT, "flowLoss",           "",  flowLoss,           1,
		SSWRITE_VALUE_VECT, "Y_N_FARVec",         "",  Y_N_FARVec,         mxGetNumberOfElements(Y_N_FARVec_p(S)),
		SSWRITE_VALUE_VECT, "T_N_RtArray",        "",  T_N_RtArray,        mxGetNumberOfElements(T_N_RtArray_p(S)),
		SSWRITE_VALUE_VECT, "X_N_TtVec",          "",  X_N_TtVec,          mxGetNumberOfElements(X_N_TtVec_p(S)),
		SSWRITE_VALUE_VECT, "T_N_MAP_gammaArray", "",  T_N_MAP_gammaArray, mxGetNumberOfElements(T_N_MAP_gammaArray_p(S)),
		SSWRITE_VALUE_VECT, "X_N_PEQPaVec",       "",  X_N_PEQPaVec,       mxGetNumberOfElements(X_N_PEQPaVec_p(S)),
		SSWRITE_VALUE_VECT, "T_N_CdThArray",      "",  T_N_CdThArray,      mxGetNumberOfElements(T_N_CdThArray_p(S)),
		SSWRITE_VALUE_VECT, "T_N_CvArray",        "",  T_N_CvArray,        mxGetNumberOfElements(T_N_CvArray_p(S)),
		SSWRITE_VALUE_VECT, "T_N_CfgArray",       "",  T_N_CfgArray,       mxGetNumberOfElements(T_N_CfgArray_p(S)),
		SSWRITE_VALUE_VECT, "T_N_TGArray",        "",  T_N_TGArray,        mxGetNumberOfElements(T_N_TGArray_p(S)),
		SSWRITE_VALUE_VECT, "X_N_TtVecTG",        "",  X_N_TtVecTG,        mxGetNumberOfElements(X_N_TtVecTG_p(S)),
		SSWRITE_VALUE_VECT, "IDes",               "",  IDes,               1,
		SSWRITE_VALUE_VECT, "WDes",               "",  WDes,               1,
		SSWRITE_VALUE_VECT, "CfgEn",              "",  CfgEn,              1
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
