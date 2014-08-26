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
#include <math.h>
#include "constants_TMATS.h"
#include "funcs_TMATS.h"

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

extern double t2hc(double a, double b);
extern double h2tc(double c, double d);
extern double pt2sc(double e, double f, double g);
extern double interp1Ac(double aa[], double bb[], double cc, int ii,int *error);
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
    ssSetInputPortWidth(S, 0, 10);
    ssSetInputPortRequiredContiguous(S, 0, true);
    ssSetInputPortDirectFeedThrough(S, 0, 1);
    
    if (!ssSetNumOutputPorts(S, 1)) return;
    ssSetOutputPortWidth(S, 0, 8);
    
    ssSetNumSampleTimes(S, 1);
    ssSetNumRWork(S, 0);
    ssSetNumIWork(S, 8);
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
    ssSetIWorkValue(S,14,0);
    ssSetIWorkValue(S,15,0);
    ssSetIWorkValue(S,16,0);
    ssSetIWorkValue(S,17,0);
}
#endif

static void mdlOutputs(SimStruct *S, int_T tid)
{
    /*--------Define Parameters-------*/
    const real_T Aphy1In                   = *mxGetPr(Aphy1In_p(S));
    const real_T Aphy2In                   = *mxGetPr(Aphy2In_p(S));
    const real_T s_M_Imp1                  = *mxGetPr(s_M_Imp1_p(S));
    const real_T s_M_Imp2                  = *mxGetPr(s_M_Imp2_p(S));
    const real_T s_M_V1                    = *mxGetPr(s_M_V1_p(S));
    const real_T s_M_V2                    = *mxGetPr(s_M_V2_p(S));
    const real_T BPRdes                    = *mxGetPr(BPRdes_p(S));
    const real_T IDes                      = *mxGetPr(IDes_p(S));
    const real_T SWPS                      = *mxGetPr(SWPS_p(S));
    const real_T MNp                       = *mxGetPr(MNp_p(S));
    
    /*-------- vector & array data -------*/
    const real_T *Y_M_FARVec		= mxGetPr(Y_M_FARVec_p(S));
    const real_T *X_M_TVec          = mxGetPr(X_M_TVec_p(S));
    const real_T *T_M_RtArray       = mxGetPr(T_M_RtArray_p(S));
    const real_T *T_M_gammaArray    = mxGetPr(T_M_gammaArray_p(S));
    
    
    
    
    /*------get dimensions of parameter arrays-------*/
    const int_T A   = mxGetNumberOfElements(Y_M_FARVec_p(S));
    const int_T B   = mxGetNumberOfElements(X_M_TVec_p(S));
    
    /*---------Define Inputs--------*/
    const real_T *u  = (const real_T*) ssGetInputPortSignal(S,0);
    
    double W1In     = u[0];     /* Input 1 Flow [pps]                     */
    double ht1In    = u[1];     /* Input 1 enthalpy [BTU/lbm]             */
    double Tt1In	= u[2];     /* Temperature Input 1 [degR]             */
    double Pt1In	= u[3];     /* Pressure Input 1 [psia]                */
    double FARc1In  = u[4];     /* Combusted Fuel to Air Ratio 1 [frac]   */
    double W2In     = u[5];     /* Input 1 Flow [pps]                     */
    double ht2In	= u[6];     /* Input 1 enthalpy [BTU/lbm]             */
    double Tt2In	= u[7];     /* Temperature Input 1 [degR]             */
    double Pt2In	= u[8];     /* Pressure Input 1 [psia]                */
    double FARc2In  = u[9];     /* Combusted Fuel to Air Ratio 1 [frac]   */
    
    
    real_T *y  = (real_T *)ssGetOutputPortRealSignal(S,0);  /* Output Array */
    
    /*--------Define Constants-------*/
    double WOut, FARcOut, TtOut, Aphyout, PtOut;
    double Test;
    double E1, E2, Eout1, Eout2, A1calc, A2calc, Aoutcalc, Ps1g, Ps2g, Psoutg;
    double Rt1, ht1in, Ps1, S1in, Ts1, hs1, rhos1, V1, Ps1g_new, Ps1g_old, E1_old, MN1g, gammat1g, Ts1g;
    double Rt2, ht2in, Ps2, S2in, Ts2, hs2, rhos2, V2, Ps2g_new, Ps2g_old, E2_old, MN2g, gammat2g, Ts2g;
    double erthr, gammatg, gammasg, TsMNg, PsMNg,PsMNg_new, PsMNg_old, erMN, erMN_old;
    double Rtout, htOut, Psout, Sout, Tsout, hsout, rhosout, Vout, Psoutg_new, Psoutg_old, Eout1_old, Eout2_old, MNoutg, gammatoutg, Tsoutg;
    double E1thr, E2thr, Eout1thr, Eout2thr, Ptoutg, Ptoutg_old, Ptoutg_new, Impulseoutputg, ImpulseMixed, NErr;
    double Aphy1, Aphy2;
    double Psp, Pss, Ttp, Tts, Ptp, Pts, Tsp, Tss, rhosp, rhoss, Vp, Vs, Ap, As, hsp, hss, htp, hts;
    double Ws, Wp, MNg, Sin;
    double FARp, FARs, Rtp, Rts, Rsp, Rss;
    double rhosg, Vg, hsg;
    int iter, iter1, iter2, iter3a, iter3b, maxiter;
    int interpErr = 0;
    
    
    
    /* ------- get strings -------------- */
    char * BlkNm;
    int_T buflen;
    int_T status;
    
    /* Get name of block from dialog parameter (string) */
    buflen = mxGetN(BN_p(S))*sizeof(mxChar)+1;
    BlkNm = mxMalloc(buflen);
    status = mxGetString(BN_p(S), BlkNm, buflen);
    
    /* calculate output flow */
    WOut = W1In + W2In;
    /* calculate output fuel to air ratio */
    FARcOut = (FARc1In * W1In + FARc2In *W2In)/WOut;
    
    /* determine gas characteristics */
    /*  Where gas constant is R = f(FAR), but NOT P & T */
    Rt1 = interp1Ac(Y_M_FARVec,T_M_RtArray,FARc1In,A,&interpErr);
    if (interpErr == 1 && ssGetIWork(S)[0]==0){
        printf("Warning in %s, Error calculating Rt1. Vector definitions may need to be expanded.\n", BlkNm);
        ssSetIWorkValue(S,0,1);
    }
    Rt2 = interp1Ac(Y_M_FARVec,T_M_RtArray,FARc2In,A,&interpErr);
    if (interpErr == 1 && ssGetIWork(S)[1]==0){
        printf("Warning in %s, Error calculating Rt2. Vector definitions may need to be expanded.\n", BlkNm);
        ssSetIWorkValue(S,1,1);
    }
    Rtout = interp1Ac(Y_M_FARVec,T_M_RtArray,FARcOut,A,&interpErr);
    if (interpErr == 1 && ssGetIWork(S)[2]==0){
        printf("Warning in %s, Error calculating Rtout. Vector definitions may need to be expanded.\n", BlkNm);
        ssSetIWorkValue(S,2,1);
    }
    
    /* determine calculated enthalpy for input flows */
    ht1in = t2hc(Tt1In,FARc1In);
    ht2in = t2hc(Tt2In,FARc2In);
    /* calculate total enthalpy */
    htOut = (W1In*ht1in + W2In*ht2in)/WOut;
    
    /* calculate total output temperature */
    TtOut = h2tc(htOut, FARcOut);
    
    maxiter = 20;
    if (IDes < 0.5){
        
        /* set primary and secondary definition via switch primary-secondary */
        if (SWPS < 0.5){
            /* primary inflow */
            Wp  = W1In;
            htp = ht1in;
            Ttp = Tt1In;
            Ptp = Pt1In;
            FARp = FARc1In;
            Rtp = Rt1;
            Rsp = Rtp;
            /* secondary inflow */
            Ws  = W2In;
            hts = ht2in;
            Tts = Tt2In;
            Pts = Pt2In;
            FARs = FARc2In;
            Rts = Rt2;}
        else {
            /* primary inflow */
            Wp  = W2In;
            htp = ht2in;
            Ttp = Tt2In;
            Ptp = Pt2In;
            FARp = FARc2In;
            Rtp = Rt2;
            Rsp = Rtp;
            /* secondary inflow*/
            Ws  = W1In;
            hts = ht1in;
            Tts = Tt1In;
            Pts = Pt1In;
            FARs = FARc1In;
            Rts = Rt1;}
        
        /* Determine primary flow Area and Ps */
        MNg = MNp;
        gammatg = interp2Ac(Y_M_FARVec,X_M_TVec,T_M_gammaArray,FARp,Ttp,A,B,&interpErr);
        if (interpErr == 1 && ssGetIWork(S)[3]==0){
            printf("Warning in %s, Error calculating gammatg. Vector definitions may need to be expanded.\n", BlkNm);
            ssSetIWorkValue(S,3,1);
        }
        TsMNg = Ttp /(1+MNg*MNg*(gammatg-1)/2);
        PsMNg = Ptp*pow((TsMNg/Ttp),(gammatg/(gammatg-1)));
        
        PcalcStat(Ptp, PsMNg, Ttp, htp, FARp, Rtp, &Sin, &TsMNg, &hsg, &rhosg, &Vg);
        gammasg = interp2Ac(Y_M_FARVec,X_M_TVec,T_M_gammaArray,FARp,TsMNg,A,B,&interpErr);
        if (interpErr == 1 && ssGetIWork(S)[4]==0){
            printf("Warning in %s, Error calculating gammasg. Vector definitions may need to be expanded.\n", BlkNm);
            ssSetIWorkValue(S,4,1);
        }
        MNg = Vg/sqrt(gammasg*Rsp*TsMNg*C_GRAVITY*JOULES_CONST);
        
        erMN =MNp - MNg;
        PsMNg_new = PsMNg + 0.05;
        iter = 0;
        erthr = 0.0001;
        /* if Ps is not close enough to Ps at MN = MNp, iterate to find Ps at MN = MNp */
        while (abs_D(erMN) > erthr && iter < maxiter) {
            erMN_old = erMN;
            PsMNg_old = PsMNg;
            if(abs_D(PsMNg - PsMNg_new) < 0.03)
                PsMNg = PsMNg + 0.05;
            else
                PsMNg = PsMNg_new;

            PcalcStat(Ptp, PsMNg, Ttp, htp, FARp, Rtp, &Sin, &TsMNg, &hsg, &rhosg, &Vg);
            gammasg = interp2Ac(Y_M_FARVec,X_M_TVec,T_M_gammaArray,FARp,TsMNg,A,B,&interpErr);
            if (interpErr == 1 && ssGetIWork(S)[5]==0){
                printf("Warning in %s, Error calculating iteration gammasg. Vector definitions may need to be expanded.\n", BlkNm);
                ssSetIWorkValue(S,5,1);
            }
            if (Vg <= 0) {
                Vg = 0.00001;
                if(iter >= maxiter && ssGetIWork(S)[6]==0 ){
                    printf("Warning in %s, Primary flow velocity is zero\n", BlkNm);
                    ssSetIWorkValue(S,6,1);
                }
            }
            MNg = Vg/sqrt(gammasg*Rsp*TsMNg*C_GRAVITY*JOULES_CONST);
            erMN = MNp - MNg;
            if (abs_D(erMN) > erthr) {
                /* determine next guess pressure by secant algorithm */
                PsMNg_new = PsMNg - erMN *(PsMNg - PsMNg_old)/(erMN - erMN_old);
            }
            iter = iter + 1;
        }
        if (iter >= maxiter && ssGetIWork(S)[7]==0 ){
            printf("Warning in %s, Error calculating Ps at MN = MNp. There may be error in output pressure\n", BlkNm);
            ssSetIWorkValue(S,7,1);
        }
        Ap = Wp/(Vg * rhosg/C_SINtoSFT);
        Vp = Vg;
        Tsp = TsMNg;
        Psp = PsMNg;
        rhosp = rhosg;
        hsp = hsg;
        /* calculate secondary flow area */
        Pss = Psp;
        PcalcStat(Pts, Pss, Tts, hts, FARs, Rts, &Sin, &TsMNg, &hsg, &rhosg, &Vg);
        if (Vg <= 0) {
            Vg = 0.00001;
            if(iter >= maxiter && ssGetIWork(S)[8]==0 ){
                printf("Warning in %s, Secondary flow velocity is zero\n", BlkNm);
                ssSetIWorkValue(S,8,1);
            }
        }
        
        MNg = Vg/sqrt(1.4*Rts*TsMNg*C_GRAVITY*JOULES_CONST);
        
        As = Ws/(Vg * rhosg/C_SINtoSFT);
        Vs = Vg;
        Tss = TsMNg;
        Pss = PsMNg;
        rhoss = rhosg;
        hss = hsg;
        
        if (SWPS < 0.5){
            Aphy1 = Ap;
            V1 = Vp;
            Ps1 = Psp;
            Aphy2 = As;
            V2 = Vs;
            Ps2 = Pss;
        }
        else {
            Aphy2 = Ap;
            V2 = Vp;
            Ps2 = Psp;
            Aphy1 = As;
            V1 = Vs;
            Ps1 = Pss;
        }
    }
    else {
        /* Set areas to input parameter values */
        Aphy1 = Aphy1In;
        Aphy2 = Aphy2In;
        /* start iteration to find Ps1in */
        /* guess Ps1 and calculate an initial Area error */
        MN1g = 0.3;
        gammat1g = 1.4;
        Ts1g = Tt1In /(1+MN1g*MN1g*(gammat1g-1)/2);
        Ps1g = Pt1In*pow((Ts1g/Tt1In),(gammat1g/(gammat1g-1)));
        PcalcStat(Pt1In, Ps1g, Tt1In, ht1in, FARc1In, Rt1, &S1in, &Ts1, &hs1, &rhos1, &V1);
        A1calc = W1In/(V1 * rhos1/C_SINtoSFT);
        E1 = abs_D((Aphy1 - A1calc)/Aphy1);
        
        /* iterate to find static pressure, calculated area should be close to actual area */
        iter1 = 0;
        Ps1g_new = Ps1g + 0.05;
        E1thr = 0.001;
        while ( abs_D(E1) > E1thr && iter1 < maxiter) {
            E1_old = E1;
            Ps1g_old = Ps1g;
            if(abs_D(Ps1g - Ps1g_new) < 0.03) {
                Ps1g = Ps1g + 0.05;
            }
            else {
                Ps1g = Ps1g_new;
            }
            /* calculate flow velocity and rhos */
            PcalcStat(Pt1In, Ps1g, Tt1In, ht1in, FARc1In, Rt1, &S1in, &Ts1, &hs1, &rhos1, &V1);
            
            if (V1 <= 0) {
                V1 = 0.00001;
                if(iter >= maxiter && ssGetIWork(S)[9]==0 ){
                    printf("Warning in %s, Input 1 flow velocity is zero\n", BlkNm);
                    ssSetIWorkValue(S,9,1);
                }
            }
            
            /* calculated Area */
            A1calc = W1In/(V1 * rhos1/C_SINtoSFT);
            /*determine error */
            E1 = (Aphy1 - A1calc)/Aphy1;
            if (abs_D(E1) > E1thr) {
                /* determine next guess pressure by secant algorithm */
                Ps1g_new = Ps1g - E1 *(Ps1g - Ps1g_old)/(E1 - E1_old);
                /* limit algorthim change */
                if (Ps1g_new > 1.1*Ps1g) {
                    Ps1g_new = 1.1 * Ps1g;
                }
                else if (Ps1g_new < 0.9 * Ps1g) {
                    Ps1g_new = 0.9 * Ps1g;
                }
            }
            iter1 = iter1 + 1;
        }
        Ps1 = Ps1g;
        if (iter1 >= maxiter && ssGetIWork(S)[10]==0){
            printf("Warning in %s, unable to caluclate Ps1 within allowed iterations, PtOut may contain high error\n", BlkNm);
            ssSetIWorkValue(S,10,1);
        }
        /* end Ps1in iteration */
        
        /* start iteration to find Ps2in */
        /* guess Ps2 and calculate an initial Area error */
        MN2g = 0.3;
        gammat2g = 1.4;
        Ts2g = Tt2In /(1+MN2g*MN2g*(gammat2g-1)/2);
        Ps2g = Pt2In*pow((Ts2g/Tt2In),(gammat2g/(gammat2g-1)));
        PcalcStat(Pt2In, Ps2g, Tt2In, ht2in, FARc2In, Rt2, &S2in, &Ts2, &hs2, &rhos2, &V2);
        A2calc = W2In/(V2 * rhos2/C_SINtoSFT);
        E2 = abs_D((Aphy2 - A2calc)/Aphy2);
        
        /* iterate to find static pressure, calculated area should be close to actual area */
        iter2 = 0;
        Ps2g_new = Ps2g + 0.05;
        E2thr = E1thr;
        while ( abs_D(E2) > E2thr  && iter2 < maxiter) {
            E2_old = E2;
            Ps2g_old = Ps2g;
            if(abs_D(Ps2g - Ps2g_new) < 0.03) {
                Ps2g = Ps2g + 0.05;
            }
            else {
                Ps2g = Ps2g_new;
            }
            /* calculate flow velocity and rhos */
            PcalcStat(Pt2In, Ps2g, Tt2In, ht2in, FARc2In, Rt2, &S2in, &Ts2, &hs2, &rhos2, &V2);
            if (V2 <= 0) {
                V2 = 0.00001;
                if(iter >= maxiter && ssGetIWork(S)[11]==0 ){
                    printf("Warning in %s, Input 2 flow velocity is zero\n", BlkNm);
                    ssSetIWorkValue(S,11,1);
                }
            }
            /* calculated Area */
            A2calc = W2In/(V2 * rhos2/C_SINtoSFT);
            /*determine error */
            E2 = (Aphy2 - A2calc)/Aphy2;
            
            if (abs_D(E2) > E2thr) {
                /* determine next guess pressure by secant algorithm */
                Ps2g_new = Ps2g - E2 *(Ps2g - Ps2g_old)/(E2 - E2_old);
                /* limit algorthim change */
                if (Ps2g_new > 1.1*Ps2g) {
                    Ps2g_new = 1.1 * Ps2g;
                }
                else if (Ps2g_new < 0.9 * Ps2g) {
                    Ps2g_new = 0.9 * Ps2g;
                }
            }
            iter2 = iter2 + 1;
        }
        Ps2 = Ps2g;
        if (iter1 >= maxiter && ssGetIWork(S)[12]==0){
            printf("Warning in %s, unable to caluclate Ps2 within allowed iterations, PtOut may contain high error\n", BlkNm);
            ssSetIWorkValue(S,12,1);
        }
        /* end Ps2 iteration */
        
        
    } /* end off-Design caulcation of Ps1, Ps2, Aphy1, Aphy2 ,V1, and V2 */
    
    
    if ((Ps1 > Pt1In || Ps2 > Pt2In) && ssGetIWork(S)[13]==0 ){
        printf("Warning in %s, Error calculating input static pressures\n", BlkNm);
        ssSetIWorkValue(S,13,1);
    }
    
    /* determine area */
    Aphyout = Aphy1 + Aphy2;
    
    /* start iteration to find Psout */
    /* guess a Ptout */
    Ptoutg = (W1In* Pt1In + W2In * Pt2In) / WOut;
    /* guess Psout and calculate an initial Area error */
    MNoutg = 0.3;
    gammatoutg = 1.4;
    Tsoutg = TtOut /(1+MNoutg*MNoutg*(gammatoutg-1)/2);
    Psoutg = Ptoutg*pow((Tsoutg/TtOut),(gammatoutg/(gammatoutg-1)));
    PcalcStat(Ptoutg, Psoutg, TtOut, htOut, FARcOut, Rtout, &Sout, &Tsout, &hsout, &rhosout, &Vout);
    Aoutcalc = WOut/(Vout * rhosout/C_SINtoSFT);
    /* determine guess error for static pressure iteration */
    Eout2 = abs_D((Aphyout - Aoutcalc)/Aphyout);
    
    /* determine guess error for total pressure iteration */
    
    ImpulseMixed = s_M_Imp1*(Ps1*Aphy1 + W1In * V1 * s_M_V1/C_GRAVITY) + s_M_Imp2*(Ps2 * Aphy2 + W2In * V2 * s_M_V2/ C_GRAVITY);
    Impulseoutputg = Psoutg * Aphyout + WOut * Vout/C_GRAVITY;
    Eout1 = abs_D((ImpulseMixed - Impulseoutputg)/ ImpulseMixed);
    /* determine iteration constants */
    iter3a = 0;
    iter3b = 0;
    Ptoutg_new = Ptoutg + 0.05;
    Psoutg_new = Psoutg + 0.05;
    Eout1thr = 0.001;
    Eout2thr = 0.001;
    /* iterate to find total pressure, calculated from errors in Impulse  */
    while ( (abs_D(Eout1) > Eout1thr || abs_D(Eout2) > Eout2thr) && iter3a < maxiter) {
        Eout1_old = Eout1;
        Ptoutg_old = Ptoutg;
        if(abs_D(Ptoutg - Ptoutg_new) < 0.03) {
            Ptoutg = Ptoutg + 0.05;
        }
        else {
            Ptoutg = Ptoutg_new;
        }
        
        
        /* iterate to find static pressure, calculated area should be close to actual area */
        do{
            Eout2_old = Eout2;
            Psoutg_old = Psoutg;
            if(abs_D(Psoutg - Psoutg_new) < 0.03) {
                Psoutg = Psoutg + 0.05;
            }
            else {
                Psoutg = Psoutg_new;
            }
            /* calculate flow velocity and rhos */
            PcalcStat(Ptoutg, Psoutg, TtOut, htOut, FARcOut, Rtout, &Sout, &Tsout, &hsout, &rhosout, &Vout);
            if (Vout <= 0) {
                Vout = 0.00001;
                if(iter >= maxiter && ssGetIWork(S)[14]==0 ){
                    printf("Warning in %s, Output flow velocity is zero\n", BlkNm);
                    ssSetIWorkValue(S,14,1);
                }
            }
            /* calculated Area */
            Aoutcalc = WOut/(Vout * rhosout/C_SINtoSFT);
            /*determine error */
            Eout2 = (Aphyout - Aoutcalc)/Aphyout;
            if (abs_D(Eout2) > Eout2thr) {
                /* determine next guess pressure by secant algorithm */
                Psoutg_new = Psoutg - Eout2 *(Psoutg - Psoutg_old)/(Eout2 - Eout2_old);
                /* limit algorthim change */
                if (Psoutg_new > 1.05*Psoutg) {
                    Psoutg_new = 1.05 * Psoutg;
                }
                else if (Psoutg_new < 0.95 * Psoutg) {
                    Psoutg_new = 0.95 * Psoutg;
                }
            }
            iter3b = iter3b + 1;
        } while ( abs_D(Eout2) > Eout2thr && iter3b < maxiter);
        
        /* verify Ps is <= Pt */
        if (Psoutg > Ptoutg){
            Psoutg = Ptoutg;
            if (ssGetIWork(S)[15]==0){
                printf ("Warning in %s, Error calculating Psout\n", BlkNm);
                ssSetIWorkValue(S,15,1);
            }
        }
        /* end Psout iteration for current Ptout guess */
        
        /* calculate impulse output based on current Pt guess */
        Impulseoutputg = Psoutg * Aphyout + WOut * Vout/C_GRAVITY;
        /* calculate impulse error */
        Eout1 = abs_D((ImpulseMixed - Impulseoutputg)/ ImpulseMixed);
        if (abs_D(Eout1) > Eout1thr) {
            /* determine next guess pressure by secant algorithm */
            Ptoutg_new = Ptoutg - Eout1 *(Ptoutg - Ptoutg_old)/(Eout1 - Eout1_old);
            /* limit algorthim change */
            if (Ptoutg_new > 1.1*Ptoutg) {
                Ptoutg_new = 1.1 * Ptoutg;
            }
            else if (Ptoutg_new < 0.9 * Ptoutg) {
                Ptoutg_new = 0.9 * Ptoutg;
            }
        }
        
        iter3a = iter3a + 1;
    }
    /* end PtOut iteration */
    Psout = Psoutg;
    PtOut = Ptoutg;
    
    if (iter3a >= maxiter && ssGetIWork(S)[16]==0){
        printf("Error in %s, unable to calculate PtOut\n", BlkNm);
        ssSetIWorkValue(S,16,1);
    }
    
    if (iter3b >= maxiter && ssGetIWork(S)[17]==0){
        printf("Warning in %s, unable to caluclate PsOut, PtOut may contain high error\n", BlkNm);
        ssSetIWorkValue(S,17,1);
    }
    
    /* Compute normalized error */
    if (IDes < 0.5)
        NErr = (W1In/W2In -BPRdes)/(W1In/W2In);
    else
        NErr = (Ps1 - Ps2)/Ps1;
    
    /*------Assign output values------------*/
    y[0] = WOut;      /* Output Air Flow [pps]						*/
    y[1] = htOut; 	  /* Output Enthalpy [BTU/lbm] 					*/
    y[2] = TtOut;     /* Output Temperature  [degR]					*/
    y[3] = PtOut;     /* Output Pressure [psia]						*/
    y[4] = FARcOut;   /* Output Combusted Fuel to Air Ratio [frac] 	*/
    y[5] = NErr;      /* Normalized Static Pressure Error           */
    y[6] = Aphy1;     /* Area of input 1 [in^2]                     */
    y[7] = Aphy2;     /* Area of input 2 [in^2]                     */
    
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
