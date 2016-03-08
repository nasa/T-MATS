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
    ssSetInputPortWidth(S, 0, 8);
    ssSetInputPortRequiredContiguous(S, 0, true);
    ssSetInputPortDirectFeedThrough(S, 0, 1);
    
    if (!ssSetNumOutputPorts(S, 1)) return;
    ssSetOutputPortWidth(S, 0, 17);
    
    ssSetNumSampleTimes(S, 1);
    ssSetNumRWork(S, 0);
    ssSetNumIWork(S, 11);
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
    ssSetIWorkValue(S,10,0);
    ssSetIWorkValue(S,11,0);
    ssSetIWorkValue(S,12,0);
    ssSetIWorkValue(S,13,0);
}
#endif

static void mdlOutputs(SimStruct *S, int_T tid)
{
    
    /*--------parameters defined in S-function block--------*/
    const real_T SwitchType             = *mxGetPr(SwitchType_p(S)); /* Nozzle type */
    const real_T flowLoss               = *mxGetPr(flowLoss_p(S));
    const real_T IDes                   = *mxGetPr(iDesign_p(S));
    const real_T WDes                   = *mxGetPr(WDes_p(S));
    const real_T CfgEn                  = *mxGetPr(CfgEn_p(S));
    
    /* vector & array data */
    const real_T *Y_N_FARVec            = mxGetPr(Y_N_FARVec_p(S));
    const real_T *T_N_RtArray           = mxGetPr(T_N_RtArray_p(S));
    const real_T *X_N_TtVec             = mxGetPr(X_N_TtVec_p(S));
    const real_T *T_N_MAP_gammaArray    = mxGetPr(T_N_MAP_gammaArray_p(S));
    const real_T *X_N_PEQPaVec          = mxGetPr(X_N_PEQPaVec_p(S));
    const real_T *T_N_CdThArray         = mxGetPr(T_N_CdThArray_p(S));
    const real_T *T_N_CvArray           = mxGetPr(T_N_CvArray_p(S));
    const real_T *T_N_CfgArray          = mxGetPr(T_N_CfgArray_p(S));
    const real_T *T_N_TGArray           = mxGetPr(T_N_TGArray_p(S));
    const real_T *X_N_TtVecTG           = mxGetPr(X_N_TtVecTG_p(S));
    
    /*------get dimensions of parameter arrays-------*/
    const int_T A   = mxGetNumberOfElements(Y_N_FARVec_p(S));
    const int_T B   = mxGetNumberOfElements(X_N_TtVec_p(S));
    const int_T B1  = mxGetNumberOfElements(X_N_PEQPaVec_p(S));
    const int_T C   = mxGetNumberOfElements(X_N_TtVecTG_p(S));
    
    
    /*---------Define Inputs--------*/
    const real_T *u  = (const real_T*) ssGetInputPortSignal(S,0);
    
    double WIn       = u[0];     /* Input Flow [pps] 	*/
    double htIn      = u[1];     /* enthaply [BTU/lbm] 	*/
    double TtIn      = u[2];     /* Temperature Input [degR] 	*/
    double PtIn      = u[3];     /* Pressure Input [psia] 	*/
    double FARcIn    = u[4];     /* Combusted Fuel to Air Ratio [frac] 	*/
    double PambIn    = u[5];     /* Ambient Pressure [psia] 	*/
    double AthroatIn = u[6];     /* Throat area [in2] 	*/
    double AexitIn   = u[7];     /* Exit area [in2] 	*/
    
    real_T *y  = (real_T *)ssGetOutputPortRealSignal(S,0);   /* Output Array */
    
    /*--------Define Constants-------*/
    double choked, Ts, rhos, V, Test, MN1, Ptin;
    double CdTh, Cv, Cfg, Therm_growth, PQPa, PQPaMap, AthroatHot;
    double Rt, TsMN1, PsMN1, Woutcalc;
    double WOut, FgOut, NErrorOut, Ath, Vth, Psth, Ax, Vx, Psx, Tsx, gammasx, MNx;
    double gammatg, gammasth,gammasMN1, gammasg, MNg, TsMNg, PsMNg, PsMNg_new, PsMNg_old, VMN1;
    double MNth, Tsth, rhosth, rhosMN1, rhosx;
    double Axcalc, Psxg, Psxg_new, Psxg_old, Exthr;
    double Athcalc, Psthg, Psthg_new, Psthg_old;
    double Sin, hsg, hs, htin, rhosg, Rs, Vg;
    double gammas_s, MN_s, V_s, rhos_s, Ts_s;
    double Ex, Ex_old;
    double erMN_old, erMN, erthr;
    int maxiter, iter, maxiterx, iterx, CDNoz;
    int interpErr = 0;
    
    /* ------- get strings -------------- */
    char * BlkNm;
    int_T buflen;
    int_T status;
    
    /* Get name of block from dialog parameter (string) */
    buflen = mxGetN(BN_p(S))*sizeof(mxChar)+1;
    BlkNm = mxMalloc(buflen);
    status = mxGetString(BN_p(S), BlkNm, buflen);
    
    /* Determine Nozzle Type                  */
    /*SwitchType:                             */
    /*        1: Convergent Nozzle            */
    /*        2: Convergent-Divergent Nozzle  */
    if (SwitchType < 1.5)
        CDNoz = 0;
    else
        CDNoz = 1;
    
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
    
    
    /* Warn if there is the potential for back flow */
    Ptin = PtIn;
    if (Ptin <= PambIn) {
        Ptin = PambIn + 0.1;
        if (ssGetIWork(S)[12]==0){
            printf("Warning in %s, Backflow warning PtIn <= Pamb\n", BlkNm);
            ssSetIWorkValue(S,12,1);
        }
    }
    /* Determine ideal velocity defined by perfect expansion to Pambient */
    PcalcStat(Ptin, PambIn, TtIn, htin, FARcIn, Rt, &Sin, &Ts, &hs, &rhos, &V);
    gammas_s = interp2Ac(Y_N_FARVec,X_N_TtVec,T_N_MAP_gammaArray,FARcIn,Ts,A,B,&interpErr);
    if (interpErr == 1 && ssGetIWork(S)[5]==0){
        printf("Warning in %s, Error calculating gammas. Vector definitions may need to be expanded.\n", BlkNm);
        ssSetIWorkValue(S,5,1);
    }
    MN_s = V/sqrt(gammas_s*Rs*Ts*C_GRAVITY*JOULES_CONST);
    Ts_s = Ts;
    V_s = V;
    rhos_s = rhos;
    
    
    /* Determine if nozzle throat is choked by comparing pressure when MN = 1 to ambient pressure
     * ---- set MN = 1 and calc throat Ps for iteration IC --------*/
    MNg = 1;
    gammatg = interp2Ac(Y_N_FARVec,X_N_TtVec,T_N_MAP_gammaArray,FARcIn,TtIn,A,B,&interpErr);
    if (interpErr == 1 && ssGetIWork(S)[1]==0){
        printf("Warning in %s, Error calculating gammatg. Vector definitions may need to be expanded.\n", BlkNm);
        ssSetIWorkValue(S,1,1);
    }
    /* use isentropic equations for a first cut guess */
    TsMNg = TtIn /(1+MNg*MNg*(gammatg-1)/2);
    PsMNg = Ptin*pow((TsMNg/TtIn),(gammatg/(gammatg-1)));
    
    /* Calculate velcocity and MN using guessed static pressure */
    PcalcStat(Ptin, PsMNg, TtIn, htin, FARcIn, Rt, &Sin, &TsMNg, &hsg, &rhosg, &Vg);
    gammasg = interp2Ac(Y_N_FARVec,X_N_TtVec,T_N_MAP_gammaArray,FARcIn,TsMNg,A,B,&interpErr);
    if (interpErr == 1 && ssGetIWork(S)[2]==0){
        printf("Warning in %s, Error calculating gammasg. Vector definitions may need to be expanded.\n", BlkNm);
        ssSetIWorkValue(S,2,1);
    }
    MNg = Vg/sqrt(gammasg*Rs*TsMNg*C_GRAVITY*JOULES_CONST);
    
    /* determine error based on calculated MN and 1 */
    erMN =1 - MNg;
    
    PsMNg_new = PsMNg + 0.05;
    maxiter = 200;
    iter = 0;
    erthr = 0.001;
    
    /* if Ps is not close enough to Ps at MN = 1, iterate to find Ps at MN = 1 */
    while (fabs(erMN) > erthr && iter < maxiter) {
        erMN_old = erMN;
        PsMNg_old = PsMNg;
        if(fabs(PsMNg - PsMNg_new) < 0.003)
            PsMNg = PsMNg + 0.005;
        else
            PsMNg = PsMNg_new;
        PcalcStat(Ptin, PsMNg, TtIn, htin, FARcIn, Rt, &Sin, &TsMNg, &hsg, &rhosg, &Vg);
        gammasg = interp2Ac(Y_N_FARVec,X_N_TtVec,T_N_MAP_gammaArray,FARcIn,TsMNg,A,B,&interpErr);
        if (interpErr == 1 && ssGetIWork(S)[3]==0){
            printf("Warning in %s, Error calculating iteration gammasg. Vector definitions may need to be expanded.\n", BlkNm);
            ssSetIWorkValue(S,3,1);
        }
        MNg = Vg/sqrt(gammasg*Rs*TsMNg*C_GRAVITY*JOULES_CONST);
        erMN =1 - MNg;
        if (fabs(erMN) > erthr) {
            /* determine next guess pressure by secant algorithm */
            PsMNg_new = PsMNg - erMN *(PsMNg - PsMNg_old)/(erMN - erMN_old);
        }
        iter = iter + 1;
    }
    if (iter == maxiter && ssGetIWork(S)[4]==0 ){
        printf("Warning in %s, Error calculating Ps at MN = 1.\n", BlkNm);
        ssSetIWorkValue(S,4,1);
    }
    /*  MN = 1 parameters */
    TsMN1 = TsMNg;
    PsMN1 = PsMNg;
    rhosMN1 = rhosg;
    gammasMN1 = gammasg;
    VMN1 = Vg;
    
    /* Determine if Nozzle is choked  */
    if (PsMN1<PambIn)
        choked = 0;
    else {
        choked = 1;
    }
    
    /* throat not choked, set Psth to ideal expansion to Pambient values */
    if (choked ==0 && IDes > 0.5){
        Psth = PambIn;
        Tsth = Ts_s;
        Vth = V_s;
        rhosth = rhos_s;
        MNth = MN_s;
    }
    
    else{  /* If nozzle is choked, determine throat parameters based on MN = 1 values */
        Psth = PsMN1;
        Tsth = TsMN1;
        MNth = 1;
        gammasth = interp2Ac(Y_N_FARVec,X_N_TtVec,T_N_MAP_gammaArray,FARcIn,Tsth,A,B,&interpErr);
        if (interpErr == 1 && ssGetIWork(S)[3]==0){
            printf("Warning in %s, Error calculating iteration gammasg. Vector definitions may need to be expanded.\n", BlkNm);
            ssSetIWorkValue(S,3,1);
        }
        Vth = MNth*sqrt(gammasth*Rs*Tsth*C_GRAVITY*JOULES_CONST);
        rhosth = rhosMN1;
    }
    
    /* error('Nozzle Error: Negative Mach number!!') */
    if (MNth<0 && ssGetIWork(S)[6]==0){
        printf("Error in %s: negative throat mach number,  MN = %f.\n", BlkNm, MNth);
        ssSetIWorkValue(S,6,1);
    }
    
    
    /* Pressure before nozzle/P ambient */
    PQPa = Ptin / PambIn;
    
    /* cacluate Thermal Constants */
    PQPaMap = PQPa;
    
    /* look up Flow Coefficient */
    CdTh = interp1Ac(X_N_PEQPaVec,T_N_CdThArray,PQPaMap,B1,&interpErr);
    if (interpErr == 1 && ssGetIWork(S)[7]==0){
        printf("Warning in %s, Error calculating CdTh. Vector definitions may need to be expanded.\n", BlkNm);
        ssSetIWorkValue(S,7,1);
    }
    Therm_growth = interp1Ac(X_N_TtVecTG,T_N_TGArray,TtIn,C,&interpErr);
    if (interpErr == 1 && ssGetIWork(S)[9]==0){
        printf("Warning in %s, Error calculating Therm_growth. Vector definitions may need to be expanded.\n", BlkNm);
        ssSetIWorkValue(S,9,1);
    }
    
    
    /* Determine throat area in^2 */
    if (IDes < 0.5) {
        Ath = WIn * C_PSItoPSF / (Therm_growth *(1-flowLoss/100)*CdTh*rhosth*Vth);
        if (choked == 0 && ssGetIWork(S)[13]==0){
            printf("Warning in %s, Calculating IDes Area with un-choked nozzle.\n", BlkNm);
            ssSetIWorkValue(S,13,1);
        }
    }
    /* if the thoat area is larger then the exit area of a CD nozzle it is a convergent nozzle */
    else if (CDNoz == 1 && AthroatIn > AexitIn) {
        Ath = AexitIn;
        CDNoz = 0;
    }
    else
        Ath = AthroatIn;
    
    
    /* Calculate Flow out of nozzle */
    /* Calculated flow is always determined based on the throat velocity
     * defined at MN = 1 when choked or at the throat area when unchoked */
    AthroatHot = Ath * Therm_growth;
    Woutcalc = (1-flowLoss/100)*AthroatHot*CdTh*rhosth*Vth/C_PSItoPSF;
    
    /* Initialize variables, will only be used with CD nozzle */
    /* Psx will be used as guess to calculate Area error */
    Psxg = PambIn;
    Psx = Psth;
    Tsx = Tsth;
    MNx = MNth;
    Vx = Vth;
    Ax = Ath;
    
    /* if the nozzle is choked and convergent-divergent calculate values after the nozzle throat */
    if (CDNoz == 1 && choked == 1) {
        
        /* Generate exit area */
        if (IDes < 0.5)
            Ax = WIn * C_PSItoPSF / (Therm_growth *(1-flowLoss/100)*CdTh*rhos_s*V_s);
        else /* Use calculated area value when using Cv method  */
            Ax = AexitIn;
        
        /* start iteration to find Psx */
        Psxg = PambIn;
        PcalcStat(Ptin, Psxg, TtIn, htin, FARcIn, Rt, &Sin, &Ts, &hs, &rhos, &V);
        Axcalc = WIn/(V * rhos/C_SINtoSFT); /* Will not be used for the Cfg method */
        
        Ex = fabs((Ax - Axcalc)/Ax);
        /* iterate to find static pressure, calculated area should be close to actual area */
        maxiterx = 200;
        iterx = 0;
        Psxg_new = Psxg + 0.05;
        Exthr = 0.0001;
        while ( fabs(Ex) > Exthr && iterx < maxiter) {
            Ex_old = Ex;
            Psxg_old = Psxg;
            if (fabs(Psxg - Psxg_new) < 0.0003)
                Psxg = Psxg + 0.0005;
            else
                Psxg = Psxg_new;
            
            /* calculate flow velocity and rhos */
            PcalcStat(Ptin, Psxg, TtIn, htin, FARcIn, Rt, &Sin, &Ts, &hs, &rhos, &V);
            /* calculated Area */
            Axcalc = WIn/(V * rhos/C_SINtoSFT);
            /*determine error */
            Ex = (Ax - Axcalc)/Ax;
            if (fabs(Ex) > Exthr) {
                /* determine next guess pressure by secant algorithm */
                Psxg_new = Psxg - Ex *(Psxg - Psxg_old)/(Ex - Ex_old);
                /* limit algorthim change */
                if (Psxg_new > 1.1*Psxg) {
                    Psxg_new = 1.1 * Psxg;
                }
                else if (Psxg_new < 0.9 * Psxg) {
                    Psxg_new = 0.9 * Psxg;
                }
            }
            iterx = iterx + 1;
        }
        if (iterx == maxiterx && ssGetIWork(S)[11]==0 ){
            printf("Warning in %s, Error calculating Ps at exit.\n", BlkNm);
            ssSetIWorkValue(S,11,1);
        }
        /* Collect data from the expansion to exit area: */
        Tsx = Ts;
        Vx = V;
        Psx = Psxg;
        rhosx = rhos;
        gammasx = interp2Ac(Y_N_FARVec,X_N_TtVec,T_N_MAP_gammaArray,FARcIn,Ts,A,B,&interpErr);
        if (interpErr == 1 && ssGetIWork(S)[5]==0){
            printf("Warning in %s, Error calculating gammas. Vector definitions may need to be expanded.\n", BlkNm);
            ssSetIWorkValue(S,5,1);
        }
        MNx = Vx/sqrt(gammasx*Rs*Tsx*C_GRAVITY*JOULES_CONST);
    }
    
    /* error('Nozzle Error: Negative Mach number!!') */
    if (MNx<0 && ssGetIWork(S)[6]==0){
        printf("Error in %s: negative exit mach number,  MN = %f.\n", BlkNm, MNx);
        ssSetIWorkValue(S,6,1);
    }
    
    WOut = WIn;
    Test = Psx;
    
    /* look up Thrust and velocity coefficients */
    if (CfgEn < 0.5){
        Cv = interp1Ac(X_N_PEQPaVec,T_N_CvArray,PQPaMap,B1,&interpErr);
        if (interpErr == 1 && ssGetIWork(S)[8]==0){
            printf("Warning in %s, Error calculating Cv. Vector definitions may need to be expanded.\n", BlkNm);
            ssSetIWorkValue(S,8,1);
        }
        Cfg = 1;
    }
    else {
        Cfg = interp1Ac(X_N_PEQPaVec,T_N_CfgArray,PQPaMap,B1,&interpErr);
        if (interpErr == 1 && ssGetIWork(S)[8]==0){
            printf("Warning in %s, Error calculating Cfg. Vector definitions may need to be expanded.\n", BlkNm);
            ssSetIWorkValue(S,8,1);
        }
        Cv = 1;
    }
    
    /*----- calc gross thrust -----------*/
    if (CDNoz ==1){
        if (CfgEn > 0.5)
            FgOut = (WOut/C_GRAVITY)*V_s*Cfg;
        else
            FgOut = (WOut/C_GRAVITY)*Vx*Cv + (Psx-PambIn)*Ax;
    }
    else {
        if (CfgEn > 0.5)
            FgOut = (WOut/C_GRAVITY)*V_s*Cfg;
        else
            FgOut = (WOut/C_GRAVITY)*Vth*Cv + (Psth-PambIn)*Ath;
    }
    /* ----- Compute Normalized Flow Error ----- */
    if (IDes < 0.5 && WDes == 0)
        NErrorOut = 100;
    else if (IDes < 0.5)
        NErrorOut = (WIn - WDes)/WDes;
    else if (WIn == 0) {
        NErrorOut = 100;}
    else {
        NErrorOut = (WIn-Woutcalc)/WIn ;
    }
    /*------Assign output values------------*/
    y[0] = WOut;          /* Outlet Total Flow [pps]	*/
    y[1] = FgOut;         /* Gross Thrust [lbf] */
    y[2] = NErrorOut;     /* Normalized Flow Error [frac] */
    y[3] = Ath;           /* Throat Area [in^2] */
    y[4] = Ax;            /* Exit Area [in^2] */
    y[5] = Psth;          /* Throat static pressure [psi] */
    y[6] = Tsth;          /* Throat static temperature [R] */
    y[7] = MNth;          /* Throat Mach Number */
    y[8] = Vth;           /* Throat velocity */
    y[9] = Psx;           /* Exit static pressure [psi]*/
    y[10] = Tsx;          /* Exit static temperature [R]*/
    y[11] = MNx;          /* Exit Mach Number */
    y[12] = Vx;           /* Exit velocity */
    y[13] = Woutcalc;     /* Calculated mass flow [pps] */
    y[14] = choked ;      /* nozzle is choked at the throat */
    y[15] = V_s;          /* ideal velocity expanded to ambient */
    y[16] = Test;
    
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
