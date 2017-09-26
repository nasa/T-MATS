/*		T-MATS -- Mixer_TMATS.c
 * % *************************************************************************
 * % written by Jeffryes Chapman based on work by R. May and Ananda Himansu
 * % NASA Glenn Research Center, Cleveland, OH
 * % August 26, 2014
 * %
 * %  This file models the Mixer Element for T-MATS simulation
 *
 * %  Code Description:
 * ----------------------------------------------------------------------
 * %  Input primary - on-design:
 * %  Set primary MN to input (MNp)
 * %  guess primary Ps
 * %  Use PcalcStat to get V, Ts, and rho
 * %  MNg = V /(rho*Rs*Ts)
 * %  iterate until MNg = MNp
 * %  Calculate primary area (A1 or A2), Area = W / (rho * V)
 *
 * %  Input secondary - on-design:
 * %  Set secondary Ps = primary Ps
 * %  Use PcalcStat to get V and rho
 * %  Calculate secondary area (A1 or A2), Area = W / (rho * V)
 *
 * %  Output area (Aout) = primary area + secondary area = A1 + A2
 * ----------------------------------------------------------------------
 * %  Input 1 - off-design: 
 * %  Guess Ps1
 * %  Use PcalcStat to get V and rho
 * %  Ag = W / (rho * V)
 * %  iterate until Ag = A1 input 
 *
 * %  Input 2 - off-design: 
 * %  Guess Ps2
 * %  Use PcalcStat to get V and rho
 * %  Ag = W / (rho * V)
 * %  iterate until Ag = A2 input 
 *
 * %  Output area (Aout) = A1 + A2
 * -----------------------------------------------------------------------
 * %  Generate Outputs
 *  Flow
 * %  Wout = W1 + W2
 * FAR
 * % FARout = (FAR1 * W1 + FAR2 *W2)/Wout;
 *  Total Temperature
 * %  ht1 = t2hc(Tt1,FAR1)
 * %  ht2 = t2hc(Tt2,FAR2)
 * %  htout = (W1*ht1 + W2*ht2)/Wout
 * %  Ttout = h2t(htout, FARout)
 *  Total Pressure
 * %  Guess output Pt or Ptout
 * %  Guess output Ps or Psout
 * %  Use PcalcStat to get V and rho
 * %  Ag = W / (rho * V)
 * %  iterate Ps until Ag = Aout,  Vout = V
 * %  ImpulseMixed = Ps1*A1 + W1*V1 + Ps2*A2 + W2*V2
 * %  ImpulseOut = Psout*Aout + Wout*Vout
 * %  iterate Ptout until ImpulseMixed = ImpulseOut
 * -----------------------------------------------------------------------
 * % Dependents
 * Off-Design
 * % (Ps1 - Ps2)/ Ps1
 * On-Design
 * % (BPR - BPRdes)/BPR
 * % *************************************************************************/
#define S_FUNCTION_NAME  Mixer_TMATS
#define S_FUNCTION_LEVEL 2
#include "simstruc.h"
#include "types_TMATS.h"


#define Aphy1In_p(S)                    ssGetSFcnParam(S,0)
#define Aphy2In_p(S)                    ssGetSFcnParam(S,1)
#define Y_M_FARVec_p(S)                 ssGetSFcnParam(S,2)
#define X_M_TVec_p(S)                   ssGetSFcnParam(S,3)
#define T_M_RtArray_p(S)                ssGetSFcnParam(S,4)
#define T_M_gammaArray_p(S)             ssGetSFcnParam(S,5)
#define s_M_Imp1_p(S)                   ssGetSFcnParam(S,6)
#define s_M_Imp2_p(S)                   ssGetSFcnParam(S,7)
#define s_M_V1_p(S)                     ssGetSFcnParam(S,8)
#define s_M_V2_p(S)                     ssGetSFcnParam(S,9)
#define IDes_p(S)                       ssGetSFcnParam(S,10)
#define SWPS_p(S)                       ssGetSFcnParam(S,11)
#define BPRdes_p(S)                     ssGetSFcnParam(S,12)
#define MNp_p(S)                        ssGetSFcnParam(S,13)
#define BN_p(S)                         ssGetSFcnParam(S,14)
#define NPARAMS 15
#define NERRORS 17

extern void Mixer_TMATS_body(double* y, const double* u, const MixerStruct* prm);

#define MDL_SET_WORK_WIDTHS
#if defined(MDL_SET_WORK_WIDTHS) && defined(MATLAB_MEX_FILE)
static void mdlSetWorkWidths(SimStruct *S)
{
    const char_T *rtParamNames[] = {"Aphy1In", "Aphy2In", "Y_M_FARVec", "X_M_TVec", "T_M_RtArray", "T_M_gammaArray", "s_M_Imp1", "s_M_Imp2", "s_M_V1", "s_M_V2", "IDes", "SWPS", "BPRdes", "MNp"};
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
        if (i != 14)
            ssSetSFcnParamTunable(S, i, 1);
        else
            ssSetSFcnParamTunable(S, i, 0);
    }

    ssSetOptions(S, SS_OPTION_WORKS_WITH_CODE_REUSE | 
                    SS_OPTION_USE_TLC_WITH_ACCELERATOR);
    
    ssSetNumContStates(S, 0);
    ssSetNumDiscStates(S, 0);
    
    if (!ssSetNumInputPorts(S, 1)) return;
    ssSetInputPortWidth(S, 0, 10);
    ssSetInputPortRequiredContiguous(S, 0, true);
    ssSetInputPortDirectFeedThrough(S, 0, 1);
    
    if (!ssSetNumOutputPorts(S, 1)) return;
    ssSetOutputPortWidth(S, 0, 8);
    
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
    ssSetIWorkValue(S,Er17,0);
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
    MixerStruct mixerStruct;
    mixerStruct.Aphy1In                   = *mxGetPr(Aphy1In_p(S));
    mixerStruct.Aphy2In                   = *mxGetPr(Aphy2In_p(S));
    mixerStruct.s_M_Imp1                  = *mxGetPr(s_M_Imp1_p(S));
    mixerStruct.s_M_Imp2                  = *mxGetPr(s_M_Imp2_p(S));
    mixerStruct.s_M_V1                    = *mxGetPr(s_M_V1_p(S));
    mixerStruct.s_M_V2                    = *mxGetPr(s_M_V2_p(S));
    mixerStruct.BPRdes                    = *mxGetPr(BPRdes_p(S));
    mixerStruct.IDes                      = *mxGetPr(IDes_p(S));
    mixerStruct.SWPS                      = *mxGetPr(SWPS_p(S));
    mixerStruct.MNp                       = *mxGetPr(MNp_p(S));
    /* Vector & array data */
    mixerStruct.Y_M_FARVec		= mxGetPr(Y_M_FARVec_p(S));
    mixerStruct.X_M_TVec          = mxGetPr(X_M_TVec_p(S));
    mixerStruct.T_M_RtArray       = mxGetPr(T_M_RtArray_p(S));
    mixerStruct.T_M_gammaArray    = mxGetPr(T_M_gammaArray_p(S));
    /* Dimensions of parameter arrays */
    mixerStruct.A   = mxGetNumberOfElements(Y_M_FARVec_p(S));
    mixerStruct.B   = mxGetNumberOfElements(X_M_TVec_p(S));

    mixerStruct.IWork      = ssGetIWork(S);

    /* Get name of block from dialog parameter (string) */
    buflen = mxGetN(BN_p(S))*sizeof(mxChar)+1;
    mixerStruct.BlkNm = malloc(buflen);
    status = mxGetString(BN_p(S), mixerStruct.BlkNm, buflen);

    /* Perform core block calculations */
    Mixer_TMATS_body(y, u, &mixerStruct);
    free(mixerStruct.BlkNm);
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
