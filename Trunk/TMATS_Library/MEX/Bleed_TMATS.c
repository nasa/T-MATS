/*		T-MATS -- Bleed_TMATS.c
 * % *************************************************************************
 * % written by Jeffryes Chapman
 * % NASA Glenn Research Center, Cleveland, OH
 * % Feb 1, 2017
 * %
 * %  This file contains the model for a simple flow bleed.
 * % *************************************************************************/
#define S_FUNCTION_NAME  Bleed_TMATS
#define S_FUNCTION_LEVEL 2
#include "simstruc.h"
#include "constants_TMATS.h"
#include "functions_TMATS.h"
#include <math.h>

#define Wfrac_p(S)              ssGetSFcnParam(S,0)
#define WfracLen_p(S)           ssGetSFcnParam(S,1)
#define BN_p(S)                 ssGetSFcnParam(S,2)
#define NPARAMS 3

/* create enumeration for Iwork */
typedef enum {Er1 = 0, Er2, Er3, Er4, Er5, NUM_IWORK}IWorkIdx;

static void mdlInitializeSizes(SimStruct *S)
{
    int i;
    ssSetNumSFcnParams(S, NPARAMS);  /* Number of expected parameters */
    if (ssGetNumSFcnParams(S) != ssGetSFcnParamsCount(S)) {
        /* Return if number of expected != number of actual parameters */
        return;
    }
    
    for (i = 0; i < NPARAMS; i++)
        ssSetSFcnParamTunable(S, i, 0);
    
    ssSetNumContStates(S, 0);
    ssSetNumDiscStates(S, 0);
    
    if (!ssSetNumInputPorts(S, 2)) return;
    ssSetInputPortWidth(S, 0, 6);
    ssSetInputPortRequiredContiguous(S, 0, true);
    ssSetInputPortDirectFeedThrough(S, 0, 1);
    
    ssSetInputPortWidth(S, 1, DYNAMICALLY_SIZED);
    ssSetInputPortRequiredContiguous(S, 1, true);
    ssSetInputPortDirectFeedThrough(S, 1, 1);
    
    if (!ssSetNumOutputPorts(S, 2)) return;
    ssSetOutputPortWidth(S, 0, 6);
    ssSetOutputPortWidth(S, 1, DYNAMICALLY_SIZED);
    
    ssSetNumSampleTimes(S, 1);
    ssSetNumRWork(S, 0);
    ssSetNumIWork(S, NUM_IWORK);
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

static void mdlOutputs(SimStruct *S, int_T tid)
{
    /*--------Define Parameters-------*/
    const int_T WfracLen           = *mxGetPr(WfracLen_p(S));
    
    const real_T *Wfrac             = mxGetPr(Wfrac_p(S));
    
    /*---------Define Inputs--------*/
    const real_T *u  = (const real_T*) ssGetInputPortSignal(S,0);
    
    double WIn      = u[0];     /* Input Flow [pps] 	*/
    double htIn     = u[1];     /* Input Enthalpy [BTU/lbm] */
    double TtIn     = u[2];     /* Temperature Input [degR] 	*/
    double PtIn     = u[3];     /* Pressure [psia] 	*/
    double FARIn    = u[4];     /* combusted Fuel to Air Ratio [frac] 	*/
    double BldInLen = u[5];     /* Length of Bleed Vector input */
    
    const real_T *BldInFlow = ssGetInputPortRealSignal(S, 1);
    
    real_T *y  = (real_T *)ssGetOutputPortRealSignal(S,0);    /* Output Array port 1 */
    real_T *y1  = (real_T *)ssGetOutputPortRealSignal(S,1);   /* Output Array port 2 */
    
    /*--------Define Constants-------*/
    double Wfin, htin, Test;
    double WOut, htOut, TtOut, PtOut, FAROut, WbldoutTotal;
    double Ws1,hts1,Tts1,Pts1,FARs1, Wfs1;
    double dHblds1, dHbldout, Wblds1, Wbldout, Wfblds1, Wfbldout;
    int Vtest;
    int i;
    
    /* define arrays for bleed outputs */
    /* output bleeds */
    double WbldOut[100];
    double FARbldOut[100];
    double PtbldOut[100];
    double TtbldOut[100];
    double htbldOut[100];
    /* input bleeds */
    double Wbld[100];
    double FARbld[100];
    double Ptbld[100];
    double Ttbld[100];
    double htbld[100];
    
    /* ------- get strings -------------- */
    char * BlkNm;
    int_T buflen;
    int_T status;
    
    /* Get name of block from dialog parameter (string) */
    buflen = mxGetN(BN_p(S))*sizeof(mxChar)+1;
    BlkNm = mxMalloc(buflen);
    status = mxGetString(BN_p(S), BlkNm, buflen);
    
    /* Verify input bleed vector is a multiple of 5 */
    Vtest = BldInLen/5;
    if(5*Vtest != BldInLen && ssGetIWork(S)[Er1]==0){
        printf("Error in %s, one or more of the cooling flow input vector elements is missing(Vector form; 5x1: W,ht,Tt,Pt,FAR)\n",BlkNm);
        ssSetIWorkValue(S,Er1,1);
    }
    
    /* unpack BleedFlow vector */
    for (i = 0; i < BldInLen/5; i++)
    {
            Wbld[i] = BldInFlow[5*i];
            Ttbld[i] = BldInFlow[5*i+2];
            Ptbld[i] = BldInFlow[5*i+3];
            FARbld[i] = BldInFlow[5*i+4];
            htbld[i] = t2hc(Ttbld[i],FARbld[i]);
    }
    /* Initialize cooling flow sum constants */
    
    dHblds1 = 0;   /* enthalpy * mass cooling flow rate at stage 1 */
    Wblds1 = 0;    /* total cooling flow at stage 1*/
    Wfblds1 = 0;  /* combusted fuel flow in cooling at stage */
    
    /* calc station 1 bleed parameters */
    for (i = 0; i < BldInLen/5; i++)
    {
        Wblds1 = Wblds1 + Wbld[i];
        Wfblds1 = Wfblds1 + FARbld[i]*Wbld[i]*divby(1+FARbld[i]);
        dHblds1 = dHblds1 + htbld[i]*Wbld[i];
    }
    /* calc station 1 flow parameters */
    Ws1 = Wblds1 + WIn;
    Wfin = FARIn * WIn*divby(1+FARIn);
    Wfs1 = Wfblds1 + Wfin;
    FARs1 = Wfs1*divby(Ws1 - Wfs1);
    
    htin = t2hc(TtIn,FARIn);
    hts1 = (htin* WIn + dHblds1)*divby(Ws1);
    
    Tts1 = h2tc(hts1,FARs1);
    Pts1 = PtIn;
    
    
    htOut = hts1;
    TtOut = Tts1;
    PtOut = Pts1;
    FAROut = FARs1;
    
    WbldoutTotal = 0;
    for (i = 0; i < WfracLen; i++)
    {
        WbldOut[i] = Wfrac[i]*WIn;
        WbldoutTotal = WbldoutTotal +WbldOut[i];
    }
    
    WOut = Ws1 - WbldoutTotal;
    
    Test = PtIn;
    
    /*------Assign output values------------*/
    y[0] = WOut;            /* Outlet Total Flow [pps]   */
    y[1] = htOut;           /* Outlet Enthalpy [BTU/lbm]*/
    y[2] = TtOut;           /* Outlet Temperature [degR]      */
    y[3] = PtOut;           /* Outlet Pressure  [psia]     */
    y[4] = FAROut;          /* Outlet Fuel to Air Ratio [NA]	*/
    y[5] = Test;
        /*------Assign output values port2------------*/
    /* fractional bleed, typically used for turbine cooling flow */
    for (i = 0; i < WfracLen; i++)
    {
        *y1++ = WbldOut[i];
        *y1++ = htOut;
        *y1++ = TtOut;
        *y1++ = PtOut;
        *y1++ = FAROut;
    }
    
}

static void mdlTerminate(SimStruct *S)
{
}

#ifdef  MATLAB_MEX_FILE    /* Is this file being compiled as a MEX-file? */
#include "simulink.c"      /* MEX-file interface mechanism */
#else
#include "cg_sfun.h"       /* Code generation registration function */
#endif
/*==================*/
