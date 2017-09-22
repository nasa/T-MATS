/*		T-MATS -- Nozzle_TMATS.c
 * % *************************************************************************
 * % written by Jeffryes Chapman based on work by R. May and T. Lavelle
 * % NASA Glenn Research Center, Cleveland, OH
 * % February 27th, 2013
 * %
 * %  This file models the Nozzle Element for T-MATS simulation
 * % *************************************************************************/
#define S_FUNCTION_NAME  Nozzle_TMATS
#define S_FUNCTION_LEVEL 2
#include "simstruc.h"
#include "types_TMATS.h"

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
#define NERRORS 16 

// extern double pt2sc(double c, double d, double e);
// extern double sp2tc(double f, double g, double h);
// extern double t2hc(double i, double j);
// extern double interp1Ac(double a[], double b[], double c, int d, int *error);
// extern double interp2Ac(double aa[], double bb[], double cc[], double aaa, double bbb,int ccc, int ddd, int *error);
// extern void PcalcStat(double dd,double ee,double ff,double gg,double hh,double mm,double *nn,double *oo,double *pp,double *qq,double *rr);

extern void Nozzle_TMATS_body(double* y, const double* u, const NozzleStruct* prm);

#define MDL_SET_WORK_WIDTHS
#if defined(MDL_SET_WORK_WIDTHS) && defined(MATLAB_MEX_FILE)
static void mdlSetWorkWidths(SimStruct *S)
{
    const char_T *rtParamNames[] = {"SwitchType", "flowLoss", "Y_N_FARVec", "T_N_RtArray", "X_N_TtVec", "T_N_MAP_gammaArray", "X_N_PEQPaVec", "T_N_CdThArray", "T_N_CvArray", "T_N_CfgArray", "T_N_TGArray", "X_N_TtVecTG", "iDesign", "WDes", "CfgEn"};
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
        if (i != 15)
            ssSetSFcnParamTunable(S, i, 1);
        else
            ssSetSFcnParamTunable(S, i, 0);
    }

    ssSetOptions(S, SS_OPTION_WORKS_WITH_CODE_REUSE | 
                    SS_OPTION_USE_TLC_WITH_ACCELERATOR);
    
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
    ssSetIWorkValue(S,Er16,0);
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
    NozzleStruct nozzleStruct;
    nozzleStruct.SwitchType             = *mxGetPr(SwitchType_p(S)); /* Nozzle type */
    nozzleStruct.flowLoss               = *mxGetPr(flowLoss_p(S));
    nozzleStruct.IDes                   = *mxGetPr(iDesign_p(S));
    nozzleStruct.WDes                   = *mxGetPr(WDes_p(S));
    nozzleStruct.CfgEn                  = *mxGetPr(CfgEn_p(S));
    /* vector & array data */
    nozzleStruct.Y_N_FARVec            = mxGetPr(Y_N_FARVec_p(S));
    nozzleStruct.T_N_RtArray           = mxGetPr(T_N_RtArray_p(S));
    nozzleStruct.X_N_TtVec             = mxGetPr(X_N_TtVec_p(S));
    nozzleStruct.T_N_MAP_gammaArray    = mxGetPr(T_N_MAP_gammaArray_p(S));
    nozzleStruct.X_N_PEQPaVec          = mxGetPr(X_N_PEQPaVec_p(S));
    nozzleStruct.T_N_CdThArray         = mxGetPr(T_N_CdThArray_p(S));
    nozzleStruct.T_N_CvArray           = mxGetPr(T_N_CvArray_p(S));
    nozzleStruct.T_N_CfgArray          = mxGetPr(T_N_CfgArray_p(S));
    nozzleStruct.T_N_TGArray           = mxGetPr(T_N_TGArray_p(S));
    nozzleStruct.X_N_TtVecTG           = mxGetPr(X_N_TtVecTG_p(S));
    /* Dimensions of parameter arrays */
    nozzleStruct.A   = mxGetNumberOfElements(Y_N_FARVec_p(S));
    nozzleStruct.B   = mxGetNumberOfElements(X_N_TtVec_p(S));
    nozzleStruct.B1  = mxGetNumberOfElements(X_N_PEQPaVec_p(S));
    nozzleStruct.C   = mxGetNumberOfElements(X_N_TtVecTG_p(S));

    nozzleStruct.IWork      = ssGetIWork(S);    

    /* Get name of block from dialog parameter (string) */
    buflen = mxGetN(BN_p(S))*sizeof(mxChar)+1;
    nozzleStruct.BlkNm = mxMalloc(buflen);
    status = mxGetString(BN_p(S), nozzleStruct.BlkNm, buflen);

    /* Perform core block calculations */
    Nozzle_TMATS_body(y, u, &nozzleStruct);
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
