/*		T-MATS -- ValvePHY_TMATS.c
 * % *************************************************************************
 * % written by Jeffryes Chapman based on work by J.A. DeCastro
 * % NASA Glenn Research Center, Cleveland, OH
 * % June 6th, 2013
 * %
 * %  This file models a physics based Valve Element for T-MATS simulation
 * % *************************************************************************/
#define S_FUNCTION_NAME  ValvePHY_TMATS_new
#define S_FUNCTION_LEVEL 2
#include "simstruc.h"
#include "types_TMATS.h"

#define s_V_Ae_vlv_p(S)                 ssGetSFcnParam(S,0)
#define s_V_Ae_byp_p(S)                 ssGetSFcnParam(S,1)
#define s_V_Ae_th_p(S)                  ssGetSFcnParam(S,2)
#define X_V_FAR_vec_p(S)                ssGetSFcnParam(S,3)
#define T_V_Rt_vec_p(S)                 ssGetSFcnParam(S,4)
#define Y_V_Tt_vec_p(S)                 ssGetSFcnParam(S,5)
#define T_V_gamma_array_p(S)            ssGetSFcnParam(S,6)
#define BN_p(S)                         ssGetSFcnParam(S,7)
#define NPARAMS 8
#define NERRORS 3

extern void ValvePHY_TMATS_body(real_T *y, const real_T *u, const ValvePHYStruct* prm);

#define MDL_SET_WORK_WIDTHS
#if defined(MDL_SET_WORK_WIDTHS) && defined(MATLAB_MEX_FILE)
static void mdlSetWorkWidths(SimStruct *S)
{
    const char_T *rtParamNames[] = {"s_V_Ae_vlv", "s_V_Ae_byp", "s_V_Ae_th", "X_V_FAR_vec", "T_V_Rt_vec", "Y_V_Tt_vec", "T_V_gamma_array"};
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
    ssSetInputPortWidth(S, 0, 9);
    ssSetInputPortRequiredContiguous(S, 0, true);
    ssSetInputPortDirectFeedThrough(S, 0, 1);

    if (!ssSetNumOutputPorts(S, 1)) return;
    ssSetOutputPortWidth(S, 0, 3);

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
    ValvePHYStruct valvePHYStruct;
    valvePHYStruct.s_V_Ae_vlv                 = *mxGetPr(s_V_Ae_vlv_p(S));
    valvePHYStruct.s_V_Ae_byp                 = *mxGetPr(s_V_Ae_byp_p(S));
    valvePHYStruct.s_V_Ae_th                  = *mxGetPr(s_V_Ae_th_p(S));
    /* Vector & array data */
    valvePHYStruct.X_V_FAR_vec               = mxGetPr(X_V_FAR_vec_p(S));
    valvePHYStruct.T_V_Rt_vec                = mxGetPr(T_V_Rt_vec_p(S));
    valvePHYStruct.Y_V_Tt_vec                = mxGetPr(Y_V_Tt_vec_p(S));
    valvePHYStruct.T_V_gamma_array           = mxGetPr(T_V_gamma_array_p(S));
    /* Dimensions of parameter arrays */
    valvePHYStruct.A1  = mxGetNumberOfElements(X_V_FAR_vec_p(S));
    valvePHYStruct.B1  = mxGetNumberOfElements(Y_V_Tt_vec_p(S));

    valvePHYStruct.IWork      = ssGetIWork(S);

    /* Get name of block from dialog parameter (string) */
    buflen = mxGetN(BN_p(S))*sizeof(mxChar)+1;
    valvePHYStruct.BlkNm = mxMalloc(buflen);
    status = mxGetString(BN_p(S), valvePHYStruct.BlkNm, buflen);

    /* Perform core block calculations */
    ValvePHY_TMATS_body(y, u, &valvePHYStruct);
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
