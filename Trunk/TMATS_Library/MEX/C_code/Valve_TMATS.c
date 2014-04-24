/*		T-MATS -- Valve_TMATS.c
 * % *************************************************************************
 * % written by Jeffryes Chapman based on work by R. May and Ananda Himansu
 * % NASA Glenn Research Center, Cleveland, OH
 * % March 1st, 2013
 * %
 * %  This file models the Valve Element for T-MATS simulation
 * % *************************************************************************/
#define S_FUNCTION_NAME  Valve_TMATS
#define S_FUNCTION_LEVEL 2
#include "simstruc.h"
#include <math.h>

#define VlvfullyOpen_p(S)			ssGetSFcnParam(S,0)
#define VlvdeadZone_p(S)			ssGetSFcnParam(S,1)
#define Valve_Ae_p(S)			    ssGetSFcnParam(S,2)
#define X_V_PRVec_p(S)			    ssGetSFcnParam(S,3)
#define T_V_WcVec_p(S)              ssGetSFcnParam(S,4)
#define BN_p(S)                     ssGetSFcnParam(S,5)
#define NPARAMS 6

extern double interp1Ac(double aa[], double bb[], double cc, int ii,int *error);

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

    if (!ssSetNumInputPorts(S, 1)) return;
    ssSetInputPortWidth(S, 0, 5);
    ssSetInputPortRequiredContiguous(S, 0, true);
    ssSetInputPortDirectFeedThrough(S, 0, 1);

    if (!ssSetNumOutputPorts(S, 1)) return;
    ssSetOutputPortWidth(S, 0, 2);

    ssSetNumSampleTimes(S, 1);
    ssSetNumRWork(S, 0);
    ssSetNumIWork(S, 1);
    ssSetNumPWork(S, 0);
    ssSetNumModes(S, 0);
    ssSetNumNonsampledZCs(S, 0);

    ssSetOptions(S,
            SS_OPTION_WORKS_WITH_CODE_REUSE |
            SS_OPTION_USE_TLC_WITH_ACCELERATOR);
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
    ssSetIWorkValue(S,0,0);
}
#endif

static void mdlOutputs(SimStruct *S, int_T tid)
{
    /*--------Define Parameters-------*/
    const real_T VlvfullyOpen			= *mxGetPr(VlvfullyOpen_p(S));
    const real_T VlvdeadZone			= *mxGetPr(VlvdeadZone_p(S));
    const real_T Valve_Ae               = *mxGetPr(Valve_Ae_p(S));

    /*-------- vector & array data -------*/
    const real_T *X_V_PRVec			= mxGetPr(X_V_PRVec_p(S));
    const real_T *T_V_WcVec          = mxGetPr(T_V_WcVec_p(S));

    /*------get dimensions of parameter arrays-------*/
    const int_T A   = mxGetNumberOfElements(X_V_PRVec_p(S));

    /*---------Define Inputs--------*/
    const real_T *u  = (const real_T*) ssGetInputPortSignal(S,0);

    double PtbyIn   = u[0];     /* Bypass disch. pressure [psia] 	*/
    double VlvPosIn	= u[1];     /* Valve Position [frac, 0-1] 	*/
    double WmfpIn	= u[2];     /* Main flow path flow rate [pps] 	*/
    double TtmfpIn	= u[3];     /* Main flow path Temprature [degR] 	*/
    double PtmfpIn	= u[4];     /* Main flow path Pressure Input [psia] 	*/


    real_T *y  = (real_T *)ssGetOutputPortRealSignal(S,0);  /* Output Array */

    /*--------Define Constants-------*/
    double ValveFrac, ValvePR, WthOut, Valve_active_Ae;
    double bleedFlxCr, Test;

    int interpErr = 0;

    /* ------- get strings -------------- */
    char * BlkNm;
    int_T buflen;
    int_T status;

    /* Get name of block from dialog parameter (string) */
    buflen = mxGetN(BN_p(S))*sizeof(mxChar)+1;
    BlkNm = mxMalloc(buflen);
    status = mxGetString(BN_p(S), BlkNm, buflen);

    /* Compute Valve open fraction */
    ValveFrac = (VlvPosIn-VlvdeadZone)/(VlvfullyOpen-VlvdeadZone);

    /* ratio of total pressures at inlet and exit of bleed line */
    ValvePR = PtmfpIn/PtbyIn;

    /* Determine flow properties of the valve */
    if ((ValveFrac <= 0) || (ValvePR <= 1.0))	/* dead zone or one-way valve */
    {

        WthOut = 0;                 /* Valve throat flow [pps] */
    }
    else	/* the valve is open and flow is moving */
    {
        /*------ Compute Active Area ---------*/
        Valve_active_Ae = ValveFrac*Valve_Ae;

        /* compute corrected flow based on pressure ratio */
        bleedFlxCr = interp1Ac(X_V_PRVec,T_V_WcVec,ValvePR,A,&interpErr);
        if (interpErr == 1 && ssGetIWork(S)[0]==0){
            printf("Warning in %s, Error calculating bleedFlxCr. Vector definitions may need to be expanded.\n", BlkNm);
            ssSetIWorkValue(S,0,1);
        }
        /*------ Compute Air flow through valve ---------*/
        WthOut = bleedFlxCr*PtmfpIn/sqrt(TtmfpIn)*Valve_active_Ae;  /* Valve throat flow [pps] */
        if (WthOut > WmfpIn) /* Flow check */
        {
            WthOut = WmfpIn;
        }
    }

    Test = WthOut;

    /*------Assign output values------------*/
    y[0] = WthOut;      /* Valve throat flow [pps] */
    y[1] = Test;        /* Output Test Point */

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
