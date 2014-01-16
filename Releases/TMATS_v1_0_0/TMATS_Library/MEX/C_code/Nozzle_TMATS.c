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
#include "constants_TMATS.h"
#include <math.h>
#include "funcs_TMATS.h"

#define Athroat_p(S)                ssGetSFcnParam(S,0)
#define flowLoss_p(S)               ssGetSFcnParam(S,1)
#define Y_N_FARVec_p(S)             ssGetSFcnParam(S,2)
#define T_N_RtArray_p(S)            ssGetSFcnParam(S,3)
#define X_N_TtVec_p(S)              ssGetSFcnParam(S,4)
#define T_N_MAP_gammaArray_p(S)     ssGetSFcnParam(S,5)
#define X_N_PEQPaVec_p(S)           ssGetSFcnParam(S,6)
#define T_N_CdThArray_p(S)          ssGetSFcnParam(S,7)
#define T_N_CvArray_p(S)            ssGetSFcnParam(S,8)
#define T_N_TGArray_p(S)            ssGetSFcnParam(S,9)
#define X_N_TtVecTG_p(S)            ssGetSFcnParam(S,10)
#define iDesign_p(S)                ssGetSFcnParam(S,11)
#define WDes_p(S)                   ssGetSFcnParam(S,12)
#define BN_p(S)                     ssGetSFcnParam(S,13)
#define NPARAMS 14

extern double pt2sc(double c, double d, double e);
extern double sp2tc(double f, double g, double h);
extern double t2hc(double i, double j);
extern double interp1Ac(double a[], double b[], double c, int d, int *error);
extern double interp2Ac(double aa[], double bb[], double cc[], double aaa, double bbb,int ccc, int ddd, int *error);
extern void PcalcStat(double dd,double ee,double ff,double gg,double hh,double mm,double *nn,double *oo,double *pp,double *qq,double *rr);


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
    ssSetInputPortWidth(S, 0, 6);
    ssSetInputPortRequiredContiguous(S, 0, true);
    ssSetInputPortDirectFeedThrough(S, 0, 1);
    
    if (!ssSetNumOutputPorts(S, 1)) return;
    ssSetOutputPortWidth(S, 0, 4);
    
    ssSetNumSampleTimes(S, 1);
    ssSetNumRWork(S, 0);
    ssSetNumIWork(S, 10);
    ssSetNumPWork(S, 0);
    ssSetNumModes(S, 0);
    ssSetNumNonsampledZCs(S, 0);
    
    ssSetOptions(S,
            SS_OPTION_WORKS_WITH_CODE_REUSE |
            SS_OPTION_EXCEPTION_FREE_CODE |
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
    ssSetIWorkValue(S,1,0);
    ssSetIWorkValue(S,2,0);
    ssSetIWorkValue(S,3,0);
    ssSetIWorkValue(S,4,0);
    ssSetIWorkValue(S,5,0);
    ssSetIWorkValue(S,6,0);
    ssSetIWorkValue(S,7,0);
    ssSetIWorkValue(S,8,0);
    ssSetIWorkValue(S,9,0);
    
}
#endif

static void mdlOutputs(SimStruct *S, int_T tid)
{
    
    /*--------parameters defined in S-function block--------*/
    const real_T Athroat                = *mxGetPr(Athroat_p(S)); /* hot throat area computed in design phase (sq-in) */
    const real_T flowLoss               = *mxGetPr(flowLoss_p(S));
    const real_T IDes                   = *mxGetPr(iDesign_p(S));
    const real_T WDes                   = *mxGetPr(WDes_p(S));
    
    /* vector & array data */
    const real_T *Y_N_FARVec            = mxGetPr(Y_N_FARVec_p(S));
    const real_T *T_N_RtArray           = mxGetPr(T_N_RtArray_p(S));
    const real_T *X_N_TtVec             = mxGetPr(X_N_TtVec_p(S));
    const real_T *T_N_MAP_gammaArray    = mxGetPr(T_N_MAP_gammaArray_p(S));
    const real_T *X_N_PEQPaVec          = mxGetPr(X_N_PEQPaVec_p(S));
    const real_T *T_N_CdThArray         = mxGetPr(T_N_CdThArray_p(S));
    const real_T *T_N_CvArray           = mxGetPr(T_N_CvArray_p(S));
    const real_T *T_N_TGArray           = mxGetPr(T_N_TGArray_p(S));
    const real_T *X_N_TtVecTG           = mxGetPr(X_N_TtVecTG_p(S));
    
    /*------get dimensions of parameter arrays-------*/
    const int_T A   = mxGetNumberOfElements(Y_N_FARVec_p(S));
    const int_T B   = mxGetNumberOfElements(X_N_TtVec_p(S));
    const int_T B1  = mxGetNumberOfElements(X_N_PEQPaVec_p(S));
    const int_T C   = mxGetNumberOfElements(X_N_TtVecTG_p(S));
    
    
    /*---------Define Inputs--------*/
    const real_T *u  = (const real_T*) ssGetInputPortSignal(S,0);
    
    double WIn      = u[0];     /* Input Flow [pps] 	*/
    double htIn     = u[1];     /* enthaply [BTU/lbm] 	*/
    double TtIn     = u[2];     /* Temperature Input [degR] 	*/
    double PtIn     = u[3];     /* Pressure Input [psia] 	*/
    double FARcIn   = u[4];     /* Combusted Fuel to Air Ratio [frac] 	*/
    double PambIn   = u[5];     /* Ambient Pressure [psia] 	*/
    
    real_T *y  = (real_T *)ssGetOutputPortRealSignal(S,0);   /* Output Array */
    
    /*--------Define Constants-------*/
    double choked, Ps, Ts, rhos, V, Test, MN1;
    double CdTh, Cv, Therm_growth, PQPa, PQPaMap, AthroatHot;
    double Rt, gammat, rhot, MN, TsMN1, PsMN1, Woutcalc;
    double WOut, FgOut, NErrorOut;
    double gammatg, gammas, gammasg, MNg, TsMNg, PsMNg, PsMNg_old, erMN_old, erMN;
    double Sin, hsg, hs, htin, rhosg, Rs, Vg;
    double maxIters,tolerance, ncount, PsMNg_new, erthr;
    int maxiter, iter;
    int interpErr = 0;
    
    /* ------- get strings -------------- */
    char * BlkNm;
    int_T buflen;
    int_T status;
    
    /* Get name of block from dialog parameter (string) */
    buflen = mxGetN(BN_p(S))*sizeof(mxChar)+1;
    BlkNm = mxMalloc(buflen);
    status = mxGetString(BN_p(S), BlkNm, buflen);
    
    /* Calc entropy */
    Sin = pt2sc(PtIn, TtIn, FARcIn);
    
    /*-- Compute Input enthalpy --------*/
    
    htin = t2hc(TtIn,FARcIn);
    
    /*  Where gas constant is R = f(FAR), but NOT P & T */
    Rt = interp1Ac(Y_N_FARVec,T_N_RtArray,FARcIn,A,&interpErr);
    if (interpErr == 1 && ssGetIWork(S)[0]==0){
        printf("Warning in %s, Error calculating Rt1. Vector definitions may need to be expanded.\n", BlkNm);
        ssSetIWorkValue(S,0,1);
    }
    Rs = Rt;
    
    /*---- set MN = 1 and calc SS Ps for iteration IC --------*/
    MNg = 1;
    gammatg = interp2Ac(Y_N_FARVec,X_N_TtVec,T_N_MAP_gammaArray,FARcIn,TtIn,A,B,&interpErr);
    if (interpErr == 1 && ssGetIWork(S)[1]==0){
        printf("Warning in %s, Error calculating gammatg. Vector definitions may need to be expanded.\n", BlkNm);
        ssSetIWorkValue(S,1,1);
    }
    TsMNg = TtIn /(1+MNg*MNg*(gammatg-1)/2);
    PsMNg = PtIn*pow((TsMNg/TtIn),(gammatg/(gammatg-1)));
    
    PcalcStat(PtIn, PsMNg, TtIn, htin, FARcIn, Rt, &Sin, &TsMNg, &hsg, &rhosg, &Vg);
    gammasg = interp2Ac(Y_N_FARVec,X_N_TtVec,T_N_MAP_gammaArray,FARcIn,TsMNg,A,B,&interpErr);
    if (interpErr == 1 && ssGetIWork(S)[2]==0){
        printf("Warning in %s, Error calculating gammasg. Vector definitions may need to be expanded.\n", BlkNm);
        ssSetIWorkValue(S,2,1);
    }
    MNg = Vg/sqrt(gammasg*Rs*TsMNg*C_GRAVITY*JOULES_CONST);
    erMN =1 - MNg;
    PsMNg_new = PsMNg + 0.05;
    maxiter = 15;
    iter = 0;
    erthr = 0.001;
    /* if Ps is not close enough to Ps at MN = 1, iterate to find Ps at MN = 1 */
    while (abs_D(erMN) > erthr && iter < maxiter) {
        erMN_old = erMN;
        PsMNg_old = PsMNg;
        if(abs_D(PsMNg - PsMNg_new) < 0.03)
            PsMNg = PsMNg + 0.05;
        else
            PsMNg = PsMNg_new;
        PcalcStat(PtIn, PsMNg, TtIn, htin, FARcIn, Rt, &Sin, &TsMNg, &hsg, &rhosg, &Vg);
        gammasg = interp2Ac(Y_N_FARVec,X_N_TtVec,T_N_MAP_gammaArray,FARcIn,TsMNg,A,B,&interpErr);
        if (interpErr == 1 && ssGetIWork(S)[3]==0){
            printf("Warning in %s, Error calculating iteration gammasg. Vector definitions may need to be expanded.\n", BlkNm);
            ssSetIWorkValue(S,3,1);
        }
        MNg = Vg/sqrt(gammasg*Rs*TsMNg*C_GRAVITY*JOULES_CONST);
        erMN =1 - MNg;
        if (abs_D(erMN) > erthr) {
            /* determine next guess pressure by secant algorithm */
            PsMNg_new = PsMNg - erMN *(PsMNg - PsMNg_old)/(erMN - erMN_old);
        }
        iter = iter + 1;
    }
    if (iter == maxiter && ssGetIWork(S)[4]==0 ){
        printf("Warning in %s, Error calculating Ps at MN = 1. There may be error in output pressure\n", BlkNm);
        ssSetIWorkValue(S,4,1);
    }
    TsMN1 = TsMNg;
    PsMN1 = PsMNg;
    rhos = rhosg;
    hs = hsg;
    gammas = gammasg;
    
    /* Determine if Nozzle is choked  */
    if (PsMN1<PambIn)
        choked = 0;
    else {
        choked = 1;
        
    }
    /* Assumed not choked,  set Ps to ambient pressure and calculate parameters */
    if (choked ==0){
        Ps = PambIn;
        PcalcStat(PtIn, Ps, TtIn, htin, FARcIn, Rt, &Sin, &Ts, &hs, &rhos, &V);
        gammas = interp2Ac(Y_N_FARVec,X_N_TtVec,T_N_MAP_gammaArray,FARcIn,Ts,A,B,&interpErr);
        if (interpErr == 1 && ssGetIWork(S)[5]==0){
            printf("Warning in %s, Error calculating gammas. Vector definitions may need to be expanded.\n", BlkNm);
            ssSetIWorkValue(S,5,1);
        }
        MN = V/sqrt(gammas*Rs*Ts*C_GRAVITY*JOULES_CONST);    
    }
    
    else{  /* Assuming choked, determine static pressure and tempurature */
        Ps = PsMN1;
        Ts = TsMN1;
        MN = 1;
        /*----- calc other values for this Mach number ----*/
        V = MN*sqrt(gammas*Rs*Ts*C_GRAVITY*JOULES_CONST);
    }
    
    /* error('Nozzle Error: Negative Mach number!!') */
    if (MN<0 && ssGetIWork(S)[6]==0){
        printf("Error in %s: negative mach number,  MN = %f. Vector definitions may need to be expanded.\n", BlkNm, MN);
    }
    
    /* Pressure before nozzle/P ambient */
    PQPa = PtIn / PambIn;
    
    /* cacluate Thermal Constants */
    PQPaMap = PQPa;
    
    CdTh = interp1Ac(X_N_PEQPaVec,T_N_CdThArray,PQPaMap,B1,&interpErr);
    if (interpErr == 1 && ssGetIWork(S)[7]==0){
        printf("Warning in %s, Error calculating CdTh. Vector definitions may need to be expanded.\n", BlkNm);
        ssSetIWorkValue(S,7,1);
    }
    Cv = interp1Ac(X_N_PEQPaVec,T_N_CvArray,PQPaMap,B1,&interpErr);
    if (interpErr == 1 && ssGetIWork(S)[8]==0){
        printf("Warning in %s, Error calculating Cv. Vector definitions may need to be expanded.\n", BlkNm);
        ssSetIWorkValue(S,8,1);
    }
    Therm_growth = interp1Ac(X_N_TtVecTG,T_N_TGArray,TtIn,C,&interpErr);
    if (interpErr == 1 && ssGetIWork(S)[9]==0){
        printf("Warning in %s, Error calculating Therm_growth. Vector definitions may need to be expanded.\n", BlkNm);
        ssSetIWorkValue(S,9,1);
    }
    
    
    /* Calculate Flow out of nozzle */
    AthroatHot = Athroat * Therm_growth;
    Woutcalc = (1-flowLoss/100)*AthroatHot*CdTh*rhos*V/C_PSItoPSF;
    WOut = WIn;
    
    Test = rhos;
    
    /*----- calc gross thrust -----------*/
    FgOut = ((WOut/C_GRAVITY) * V * Cv) + ((Ps-PambIn) * Athroat);
    
    /* ----- Compute Normalized Flow Error ----- */
    if (IDes > 0.5 && WDes == 0)
        NErrorOut = 100;
    else if (IDes > 0.5)
        NErrorOut = (WIn - WDes)/WDes;
    else if (WIn == 0) {
        NErrorOut = 100;}
    else {
        NErrorOut = (WIn-Woutcalc)/WIn ;
    }
    /*------Assign output values------------*/
    y[0] = WOut;          /* Outlet Total Flow [pps]	*/
    y[1] = FgOut;          /* Gross Thrust [lbf] */
    y[2] = NErrorOut; /* Normalized Flow Error [frac] */
    y[3] = Test;
    
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
