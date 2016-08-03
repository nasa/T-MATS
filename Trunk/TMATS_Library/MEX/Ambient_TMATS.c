/*		T-MATS -- Ambient_TMATS.c
 * % *************************************************************************
 * % written by Jeffryes Chapman
 * % NASA Glenn Research Center, Cleveland, OH
 * % Aug 12, 2014
 * %
 * %  This file converts Altitude and MN to common engine input variables.
 * % *************************************************************************/
#define S_FUNCTION_NAME  Ambient_TMATS
#define S_FUNCTION_LEVEL 2
#include "simstruc.h"
#include "constants_TMATS.h"
#include "functions_TMATS.h"
#include <math.h>

#define X_A_AltVec_p(S)      ssGetSFcnParam(S,0)
#define T_A_TsVec_p(S)       ssGetSFcnParam(S,1)
#define T_A_PsVec_p(S)       ssGetSFcnParam(S,2)
#define X_A_FARVec_p(S)      ssGetSFcnParam(S,3)
#define Y_A_TVec_p(S)        ssGetSFcnParam(S,4)
#define T_A_RtArray_p(S)     ssGetSFcnParam(S,5)
#define T_A_gammaArray_p(S)  ssGetSFcnParam(S,6)
#define AFARc_p(S)           ssGetSFcnParam(S,7)
#define BN_p(S)              ssGetSFcnParam(S,8)
#define NPARAMS 9

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
    
    if (!ssSetNumInputPorts(S, 1)) return;
    ssSetInputPortWidth(S, 0, 3);
    ssSetInputPortRequiredContiguous(S, 0, true);
    ssSetInputPortDirectFeedThrough(S, 0, 1);
    
    if (!ssSetNumOutputPorts(S, 1)) return;
    ssSetOutputPortWidth(S, 0, 8);
    
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
    ssSetIWorkValue(S, Er1, 0);
    ssSetIWorkValue(S, Er2, 0);
    ssSetIWorkValue(S, Er3, 0);
    ssSetIWorkValue(S, Er4, 0);
    ssSetIWorkValue(S, Er5, 0);
}
#endif

static void mdlOutputs(SimStruct *S, int_T tid)
{
    /*--------parameters defined in S-function block--------*/
    const real_T AFARc              = *mxGetPr(AFARc_p(S));
    
    /*-------- vector & array data -------*/
    const real_T *X_A_AltVec        = mxGetPr(X_A_AltVec_p(S));
    const real_T *T_A_TsVec         = mxGetPr(T_A_TsVec_p(S));
    const real_T *T_A_PsVec         = mxGetPr(T_A_PsVec_p(S));
    const real_T *X_A_FARVec        = mxGetPr(X_A_FARVec_p(S));
    const real_T *T_A_RtArray       = mxGetPr(T_A_RtArray_p(S));
    const real_T *Y_A_TVec          = mxGetPr(Y_A_TVec_p(S));
    const real_T *T_A_gammaArray    = mxGetPr(T_A_gammaArray_p(S));
    
    /*------get dimensions of parameter arrays-------*/
    const int_T A   = mxGetNumberOfElements(X_A_AltVec_p(S));
    const int_T B   = mxGetNumberOfElements(X_A_FARVec_p(S));
    const int_T C   = mxGetNumberOfElements(Y_A_TVec_p(S));
    
    /*---------Define Inputs--------*/
    const real_T *u  = (const real_T*) ssGetInputPortSignal(S,0);
    
    double AltIn     = u[0];     /* Altitude(ft) 	*/
    double dTempIn   = u[1];     /* delta Temperature [degF] 	*/
    double MNIn      = u[2];     /* Mach Number (frac) 	*/
    
    real_T *y  = (real_T *)ssGetOutputPortRealSignal(S,0);  /* Output Array */
    
    /*--------Define Constants-------*/
    double PsOut, TsOut, TtOut, PtOut, VengOut, TsStDayOut, Vsound;
    double Ttg, Ptg, Vg, Vsg, MNg, Sout, htg, gammasg, Rs, Rt;
    double hs, htOut, Test; 
    double er, er_old, erthr, Ptg_new, Ptg_old, FAR, FAROut;
    int iter, maxiter;
    
    int interpErr = 0;
    

    
    /* ------- get strings -------------- */
    char * BlkNm;
    int_T buflen;
    int_T status;
    
    /* Get name of block from dialog parameter (string) */
    buflen = mxGetN(BN_p(S))*sizeof(mxChar)+1;
    BlkNm = mxMalloc(buflen);
    status = mxGetString(BN_p(S), BlkNm, buflen);
    
    FAR = AFARc;
    
    Rt = interp1Ac(X_A_FARVec,T_A_RtArray,FAR,B,&interpErr);
    if (interpErr == 1 && ssGetIWork(S)[Er1] == 0){
        printf("Warning in %s, Error calculating Rt. Vector definitions may need to be expanded.\n", BlkNm);
        ssSetIWorkValue(S,Er1,1);
    }
    
    Rs = Rt;
    
    /*  Static Temperature */
    TsStDayOut = interp1Ac(X_A_AltVec,T_A_TsVec,AltIn,A,&interpErr);
    if (interpErr == 1 && ssGetIWork(S)[Er2] == 0){
        printf("Warning in %s, Error calculating TsStDayOut. Vector definitions may need to be expanded.\n", BlkNm);
        ssSetIWorkValue(S,Er2,1);
    }
    TsOut = TsStDayOut + dTempIn;
    
    /* Static Pressure*/
    PsOut = interp1Ac(X_A_AltVec,T_A_PsVec,AltIn,A,&interpErr);
    if (interpErr == 1 && ssGetIWork(S)[Er3] == 0){
        printf("Warning in %s, Error calculating PsOut. Vector definitions may need to be expanded.\n", BlkNm);
        ssSetIWorkValue(S,Er3,1);
    }
    
    /* Calc output entropy */
    Sout = pt2sc(PsOut, TsOut, FAR);
    /* Determine Static enthalpy */
    hs = t2hc(TsOut,FAR);
    
    /* Pt guess */
    /*------ Total Temperature ---------*/
    Ttg = TsOut * (1+MNIn*MNIn*(C_GAMMA-1)/2);
    /*------ Total Pressure ---------*/
    Ptg = PsOut*divby((powT((TsOut*divby(Ttg)),(C_GAMMA*divby(C_GAMMA-1)))));
    
    /* calculate total temperature */
    Ttg = sp2tc(Sout,Ptg,FAR);
    /* calculate total enthalpy */
    htg = t2hc(Ttg,FAR);
    /* calculate velocity */
    Vg = sqrtT(2 * (htg - hs)*C_GRAVITY*JOULES_CONST);
    
    gammasg = interp2Ac(X_A_FARVec,Y_A_TVec,T_A_gammaArray,FAR,TsOut,B,C,&interpErr);
    if (interpErr == 1 && ssGetIWork(S)[Er4] == 0){
        printf("Warning in %s, Error calculating iteration gammasg. Vector definitions may need to be expanded.\n", BlkNm);
        ssSetIWorkValue(S,Er4,1);
    }
    Vsg = sqrtT(gammasg*Rs*TsOut*C_GRAVITY*JOULES_CONST);
    MNg = Vg*divby(Vsg);
    er = MNIn - MNg;
    Ptg_new = Ptg + 0.05;
    maxiter = 15;
    iter = 0;
    erthr = 0.001;
    
    while (fabs(er) > erthr && iter < maxiter) {
        er_old = er;
        Ptg_old = Ptg;
        if(fabs(Ptg - Ptg_new) < 0.03)
            Ptg = Ptg + 0.05;
        else
            Ptg = Ptg_new;
        
        /* calculate Total emperature */
        Ttg = sp2tc(Sout,Ptg,FAR);
        /* calculate total enthalpy */
        htg = t2hc(Ttg,FAR);
        /* calculate velocity */
        Vg = sqrtT(2 * (htg - hs)*C_GRAVITY*JOULES_CONST);
        
        Vsg = sqrtT(gammasg*Rs*TsOut*C_GRAVITY*JOULES_CONST);
        MNg = Vg*divby(Vsg);
        er = MNIn - MNg;
        if (fabs(er) > erthr) {
            /* determine next guess pressure by secant algorithm */
            Ptg_new = Ptg - er *(Ptg - Ptg_old)*divby(er - er_old);
        }
        iter = iter + 1;
    }
    if (iter == maxiter && ssGetIWork(S)[Er5]==0 ){
        printf("Warning in %s, Error calculating Pt at input MN. There may be error in output pressure\n", BlkNm);
        ssSetIWorkValue(S,Er5,1);
    }
    
    htOut = htg;
    TtOut = Ttg;
    PtOut = Ptg;
    
    /*---- Engine Velocity ---------*/
    Vsound = Vsg;
    VengOut = Vsound * MNIn;
    
    FAROut = FAR;

    Test = divby(dTempIn);
    
    /*------Assign output values------------*/
    y[0] = htOut;      /* Total enthalpy */
    y[1] = TtOut;      /* Total Temperature [degR] */
    y[2] = PtOut;      /* Total Pressure [psia] */
    y[3] = FAROut;     /* Fuel to Air Ratio */
    y[4] = PsOut;      /* Static Pressure [psia] */
    y[5] = TsOut;      /* Static Temperature [degR] */
    y[6] = VengOut;    /* Engine Velocity [ft/sec] */
    y[7] = Test;       /* Test signal */
    
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
