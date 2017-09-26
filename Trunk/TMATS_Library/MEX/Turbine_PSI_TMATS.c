/*		T-MATS -- Turbine_PSI_TMATS.c
 * % *************************************************************************
 * % written by Jeffryes Chapman based on work by R. May and T. Lavelle
 * % NASA Glenn Research Center, Cleveland, OH
 * % March 29th, 2013
 * %
 * %  This file models the Turbine Element for T-MATS simulation using PSI maps
 * % *************************************************************************/
#define S_FUNCTION_NAME  Turbine_PSI_TMATS
#define S_FUNCTION_LEVEL 2
#include "simstruc.h"
#include "types_TMATS.h"

#define Y_T_NcpsiVec_p(S)       ssGetSFcnParam(S,0)
#define X_T_PRpsiVec_p(S)       ssGetSFcnParam(S,1)
#define Y_T_NcwowVec_p(S)       ssGetSFcnParam(S,2)
#define X_T_PRwowVec_p(S)       ssGetSFcnParam(S,3)
#define T_T_Map_WoWArray_p(S)   ssGetSFcnParam(S,4)
#define T_T_Map_psiArray_p(S)   ssGetSFcnParam(S,5)
#define T_BldPos_p(S)           ssGetSFcnParam(S,6)
#define BldPosLeng_p(S)         ssGetSFcnParam(S,7)
#define CoolFlwEn_p(S)          ssGetSFcnParam(S,8)
#define IDesign_p(S)            ssGetSFcnParam(S,9)
#define NcDes_p(S)              ssGetSFcnParam(S,10)
#define EffDes_p(S)             ssGetSFcnParam(S,11)
#define PRmapDes_p(S)           ssGetSFcnParam(S,12)
#define NDes_p(S)               ssGetSFcnParam(S,13)
#define s_T_hi_p(S)				ssGetSFcnParam(S,14)
#define gamma_T_p(S)			ssGetSFcnParam(S,15)
#define Rt_T_p(S)				ssGetSFcnParam(S,16)
#define BN_p(S)                 ssGetSFcnParam(S,17)
#define NPARAMS 18
#define NERRORS 5 

// extern double h2tc(double a, double b);
// extern double pt2sc(double c, double d, double e);
// extern double sp2tc(double f, double g, double h);
// extern double t2hc(double i, double j);
// extern double interp2Ac(double kk[], double ll[], double mm[], double nn, double oo,int pp, int qq, int *error);

extern void Turbine_PSI_TMATS_body(real_T *y, const real_T *u, const real_T *CoolFlow, const Turbine_PSIStruct* prm);

#define MDL_SET_WORK_WIDTHS
#if defined(MDL_SET_WORK_WIDTHS) && defined(MATLAB_MEX_FILE)
static void mdlSetWorkWidths(SimStruct *S)
{
    const char_T *rtParamNames[] = {"Y_T_NcpsiVec", "X_T_PRpsiVec", "Y_T_NcwowVec", "X_T_PRwowVec", "T_T_Map_WoWArray", "T_T_Map_psiArray", "T_BldPos", "BldPosLeng", "CoolFlwEn", "IDesign", "NcDes", "EffDes", "PRmapDes", "NDes", "s_T_hi", "gamma_T", "Rt_T"};
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
        if (i != 17)
            ssSetSFcnParamTunable(S, i, 1);
        else
            ssSetSFcnParamTunable(S, i, 0);
    }

    ssSetOptions(S, SS_OPTION_WORKS_WITH_CODE_REUSE | 
                    SS_OPTION_USE_TLC_WITH_ACCELERATOR);

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
    ssSetOutputPortWidth(S, 0, 12);

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
    Turbine_PSIStruct turbine_PSIStruct;
    turbine_PSIStruct.NcDes              = *mxGetPr(NcDes_p(S));
    turbine_PSIStruct.PRmapDes           = *mxGetPr(PRmapDes_p(S));
    turbine_PSIStruct.EffDes             = *mxGetPr(EffDes_p(S));
    turbine_PSIStruct.NDes               = *mxGetPr(NDes_p(S));
    turbine_PSIStruct.IDes               = *mxGetPr(IDesign_p(S));
    turbine_PSIStruct.s_T_hi             = *mxGetPr(s_T_hi_p(S));
    turbine_PSIStruct.gamma_T            = *mxGetPr(gamma_T_p(S));
    turbine_PSIStruct.Rt_T               = *mxGetPr(Rt_T_p(S));
    turbine_PSIStruct.BldPosLeng         = *mxGetPr(BldPosLeng_p(S));
    turbine_PSIStruct.CoolFlwEn          = *mxGetPr(CoolFlwEn_p(S));
    /* vector & array data */
    turbine_PSIStruct.Y_T_NcpsiVec         = mxGetPr(Y_T_NcpsiVec_p(S));
    turbine_PSIStruct.X_T_PRpsiVec         = mxGetPr(X_T_PRpsiVec_p(S));
    turbine_PSIStruct.Y_T_NcwowVec         = mxGetPr(Y_T_NcwowVec_p(S));
    turbine_PSIStruct.X_T_PRwowVec         = mxGetPr(X_T_PRwowVec_p(S));
    turbine_PSIStruct.T_T_Map_WoWArray     = mxGetPr(T_T_Map_WoWArray_p(S));
    turbine_PSIStruct.T_T_Map_psiArray     = mxGetPr(T_T_Map_psiArray_p(S));
    turbine_PSIStruct.T_BldPos             = mxGetPr(T_BldPos_p(S));
    /* Dimensions of parameter arrays */
    turbine_PSIStruct.A   = mxGetNumberOfElements(Y_T_NcpsiVec_p(S));
    turbine_PSIStruct.B   = mxGetNumberOfElements(X_T_PRpsiVec_p(S));
    turbine_PSIStruct.C   = mxGetNumberOfElements(Y_T_NcwowVec_p(S));
    turbine_PSIStruct.D   = mxGetNumberOfElements(X_T_PRwowVec_p(S));

    turbine_PSIStruct.IWork      = ssGetIWork(S);

    /* Get name of block from dialog parameter (string) */
    buflen = mxGetN(BN_p(S))*sizeof(mxChar)+1;
    turbine_PSIStruct.BlkNm = malloc(buflen);
    status = mxGetString(BN_p(S), turbine_PSIStruct.BlkNm, buflen);

    /* Perform core block calculations */
    Turbine_PSI_TMATS_body(y, u, CoolFlow, &turbine_PSIStruct);
    free(turbine_PSIStruct.BlkNm);
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
