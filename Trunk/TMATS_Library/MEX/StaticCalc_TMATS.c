/*		T-MATS -- StaticCalc_TMATS.c
 * % *************************************************************************
 * % written by Jeffryes Chapman
 * % NASA Glenn Research Center, Cleveland, OH
 * % Feb 24, 2014
 * %
 * %  This file caclulates static Pressure and Temperature based on current
 * %  conditions and assumptions in throat area or MN.
 * % *************************************************************************/
#define S_FUNCTION_NAME  StaticCalc_TMATS
#define S_FUNCTION_LEVEL 2
#include "simstruc.h"
#include "constants_TMATS.h"
#include "functions_TMATS.h"
#include <math.h>

#define AthroatIn_p(S)              ssGetSFcnParam(S,0)
#define MNIn_p(S)                   ssGetSFcnParam(S,1)
#define SolveType_p(S)              ssGetSFcnParam(S,2)
#define X_FARVec_p(S)               ssGetSFcnParam(S,3)
#define T_RtArray_p(S)              ssGetSFcnParam(S,4)
#define Y_TtVec_p(S)                ssGetSFcnParam(S,5)
#define T_gammaArray_p(S)           ssGetSFcnParam(S,6)
#define BN_p(S)                     ssGetSFcnParam(S,7)
#define NPARAMS 8

/* create enumeration for Iwork */
typedef enum {Er1=0, Er2 , Er3 , Er4 , Er5 , NUM_IWORK}IWorkIdx;

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
    ssSetOutputPortWidth(S, 0, 5);
    
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
    ssSetIWorkValue(S,Er1,0);
    ssSetIWorkValue(S,Er2,0);
    ssSetIWorkValue(S,Er3,0);
    ssSetIWorkValue(S,Er4,0);
    ssSetIWorkValue(S,Er5,0);
}
#endif

static void mdlOutputs(SimStruct *S, int_T tid)
{
    
    /*--------parameters defined in S-function block--------*/
    const real_T AthroatIn              = *mxGetPr(AthroatIn_p(S));  /* input throat area (sq-in) */
    const real_T MNIn                   = *mxGetPr(MNIn_p(S));       /* input throat area (sq-in), note will use as initial guess when calculting based on Ath */
    const int_T SolveType               = *mxGetPr(SolveType_p(S));  /* 0-solve based on Ath, 1-solve based on MNIn*/
    
    /*-------- vector & array data -------*/
    const real_T *X_FARVec            = mxGetPr(X_FARVec_p(S));
    const real_T *T_RtArray           = mxGetPr(T_RtArray_p(S));
    const real_T *Y_TtVec             = mxGetPr(Y_TtVec_p(S));
    const real_T *T_gammaArray    = mxGetPr(T_gammaArray_p(S));
    
    
    /*------get dimensions of parameter arrays-------*/
    const int_T A   = mxGetNumberOfElements(X_FARVec_p(S));
    const int_T B   = mxGetNumberOfElements(Y_TtVec_p(S));
    
    /*---------Define Inputs--------*/
    const real_T *u  = (const real_T*) ssGetInputPortSignal(S,0);
    
    double WIn      = u[0];     /* Input Flow [pps] 	*/
    double htIn     = u[1];     /* enthaply [BTU/lbm] 	*/
    double TtIn     = u[2];     /* Temperature Input [degR] 	*/
    double PtIn     = u[3];     /* Pressure Input [psia] 	*/
    double FARcIn   = u[4];     /* Combusted Fuel to Air Ratio [frac] 	*/
    
    real_T *y  = (real_T *)ssGetOutputPortRealSignal(S,0);  /* Output Array */
    
    /*--------Define Constants-------*/
    double PsOut, TsOut, rhosOut, MNOut, AthOut;
    double Sin, htin;
    double Rt, Rs;
    double TsMNg, PsMNg, MNg;
    double Tsg, Psg, Psg_new, Psg_old, Acalc, erA, erA_old;
    double  gammatg, gammasg, hsg, rhosg, Vg;
    double erMN_old, erMN, PsMNg_old, PsMNg_new;
    double erthr;
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
    Rt = interp1Ac(X_FARVec,T_RtArray,FARcIn,A,&interpErr);
    if (interpErr == 1 && ssGetIWork(S)[Er1]==0){
        printf("Warning in %s, Error calculating Rt. Vector definitions may need to be expanded.\n", BlkNm);
        ssSetIWorkValue(S,Er1,1);
    }
    Rs = Rt;
    
    /* Solve for Ts and Ps when MN is known*/
    if (SolveType == 1) {
        /*---- set MN = MNIn and calc SS Ps for iteration IC --------*/
        MNg = MNIn;
        gammatg = interp2Ac(X_FARVec,Y_TtVec,T_gammaArray,FARcIn,TtIn,A,B,&interpErr);
        if (interpErr == 1 && ssGetIWork(S)[Er2]==0){
            printf("Warning in %s, Error calculating gammatg. Vector definitions may need to be expanded.\n", BlkNm);
            ssSetIWorkValue(S,Er2,1);
        }
        TsMNg = TtIn*divby(1+MNg*MNg*(gammatg-1)/2);
        PsMNg = PtIn*powT((TsMNg*divby(TtIn)),(gammatg*divby(gammatg-1)));
        
        PcalcStat(PtIn, PsMNg, TtIn, htin, FARcIn, Rt, &Sin, &TsMNg, &hsg, &rhosg, &Vg);
        gammasg = interp2Ac(X_FARVec,Y_TtVec,T_gammaArray,FARcIn,TsMNg,A,B,&interpErr);
        if (interpErr == 1 && ssGetIWork(S)[Er2]==0){
            printf("Warning in %s, Error calculating gammasg. Vector definitions may need to be expanded.\n", BlkNm);
            ssSetIWorkValue(S,Er2,1);
        }
        MNg = Vg*divby(sqrtT(gammasg*Rs*TsMNg*C_GRAVITY*JOULES_CONST));
        
        if (Vg > 0.0001){
            Acalc = WIn*divby(Vg * rhosg/C_SINtoSFT);}
        else {
            Acalc = 999; /* if velocity is close to zero assume a very large Ath */}
        
        erMN = MNIn - MNg;
        PsMNg_new = PsMNg + 0.05;
        maxiter = 15;
        iter = 0;
        erthr = 0.0001;
        
        /* if Ps is not close enough to Ps at MN = MNIn, iterate to find Ps at MN = MNIn */
        while (fabs(erMN) > erthr && iter < maxiter) {
            erMN_old = erMN;
            PsMNg_old = PsMNg;
            if(fabs(PsMNg - PsMNg_new) < 0.003)
                PsMNg = PsMNg + 0.005;
            else
                PsMNg = PsMNg_new;
            PcalcStat(PtIn, PsMNg, TtIn, htin, FARcIn, Rt, &Sin, &TsMNg, &hsg, &rhosg, &Vg);
            gammasg = interp2Ac(X_FARVec,Y_TtVec,T_gammaArray,FARcIn,TsMNg,A,B,&interpErr);
            if (interpErr == 1 && ssGetIWork(S)[Er2]==0){
                printf("Warning in %s, Error calculating iteration gammasg. Vector definitions may need to be expanded.\n", BlkNm);
                ssSetIWorkValue(S,Er2,1);
            }
            MNg = Vg*divby(sqrtT(gammasg*Rs*TsMNg*C_GRAVITY*JOULES_CONST));
            /* calculated Area */
            if (Vg > 0.0001){
                Acalc = WIn*divby(Vg * rhosg/C_SINtoSFT);}
            else {
                Acalc = 999; /* if velocity is close to zero assume a very large Ath */}
            
            erMN = MNIn - MNg;
            if (fabs(erMN) > erthr) {
                /* determine next guess pressure by secant algorithm */
                PsMNg_new = PsMNg - erMN *(PsMNg - PsMNg_old)*divby(erMN - erMN_old);
            }
            iter = iter + 1;
        }
        if (iter == maxiter && ssGetIWork(S)[Er3]==0 ){
            printf("Warning in %s, Error calculating Ps at MN = MNIn. There may be error in block outputs\n", BlkNm);
            ssSetIWorkValue(S,Er3,1);
        }
        TsOut = TsMNg;
        PsOut = PsMNg;
        rhosOut = rhosg;
        MNOut = MNIn;
        AthOut = Acalc;
    }
    /* Solve for Ts and Ps when Ath is known*/
    else if (SolveType == 0) {
        
        /* guess Psout and calculate an initial Area error */
        MNg = MNIn;
        gammatg = 1.4;
        Tsg = TtIn*divby(1+MNg*MNg*(gammatg-1)/2);
        Psg = PtIn*powT((Tsg*divby(TtIn)),(gammatg*divby(gammatg-1)));
        PcalcStat(PtIn, Psg, TtIn, htin, FARcIn, Rt, &Sin, &Tsg, &hsg, &rhosg, &Vg);
        Acalc = WIn*divby(Vg * rhosg/C_SINtoSFT);
        gammasg = interp2Ac(X_FARVec,Y_TtVec,T_gammaArray,FARcIn,Tsg,A,B,&interpErr);
        if (interpErr == 1 && ssGetIWork(S)[Er4]==0){
            printf("Warning in %s, Error calculating iteration gammasg. Vector definitions may need to be expanded.\n", BlkNm);
            ssSetIWorkValue(S,Er4,1);
        }
        MNg = Vg*divby(sqrtT(gammasg*Rs*Tsg*C_GRAVITY*JOULES_CONST));
        
        /* determine guess error for static pressure iteration */
        erA = (AthroatIn - Acalc)*divby(AthroatIn);
        
        /* determine iteration constants */
        iter = 0;
        maxiter = 1000;
        Psg_new = Psg + 0.05;
        erthr = 0.0001;
        
        while ( fabs(erA) > erthr && iter < maxiter){
            erA_old = erA;
            Psg_old = Psg;
            if(fabs(Psg - Psg_new) < 0.0003) {
                Psg = Psg + 0.0005;
            }
            else {
                Psg = Psg_new;
            }
            /* calculate flow velocity and rhos */
            PcalcStat(PtIn, Psg, TtIn, htin, FARcIn, Rt, &Sin, &Tsg, &hsg, &rhosg, &Vg);
            
            gammasg = interp2Ac(X_FARVec,Y_TtVec,T_gammaArray,FARcIn,Tsg,A,B,&interpErr);
            if (interpErr == 1 && ssGetIWork(S)[Er4]==0){
                printf("Warning in %s, Error calculating iteration gammasg. Vector definitions may need to be expanded.\n", BlkNm);
                ssSetIWorkValue(S,Er4,1);
            }
            
            MNg = Vg*divby(sqrtT(gammasg*Rs*Tsg*C_GRAVITY*JOULES_CONST));
            
            if (Vg > 0.0001) {
                /* calculated Area */
                Acalc = WIn*divby(Vg * rhosg/C_SINtoSFT);
                /*determine error */
                erA = (AthroatIn - Acalc)*divby(AthroatIn);
            }
            else {
                erA = 0;
                Psg = PtIn;
                Tsg = TtIn;
                Acalc = 999;
            }
            if (fabs(erA) > erthr) {
                /* determine next guess pressure by secant algorithm */
                Psg_new = Psg - erA *(Psg - Psg_old)*divby(erA - erA_old);
                /* limit algorthim change */
                if (Psg_new > 1.001*Psg) {
                    Psg_new = 1.002 * Psg;
                }
                else if (Psg_new < 0.999 * Psg) {
                    Psg_new = 0.998 * Psg;
                }
            }
            iter = iter + 1;
        }
        TsOut = Tsg;
        PsOut = Psg;
        rhosOut = rhosg;
        MNOut = MNg;
        AthOut = Acalc;
    }
    else {
        if (ssGetIWork(S)[Er5]==0 ){
            printf("Warning in %s, SolveType_M is not valid. There may be error in block outputs\n", BlkNm);
            ssSetIWorkValue(S,Er5,1);
        }
        TsOut = TtIn;
        PsOut = PtIn;
        rhosOut = 1;
        MNOut = 0;
        AthOut = 100;
    }
    
    
    /*------Assign output values------------*/
    y[0] = TsOut;      /* static Temperature [degR] */
    y[1] = PsOut;      /* static Pressure [psia] */
    y[2] = rhosOut;    /* static rho [lbm/ft3]*/
    y[3] = MNOut;      /* mach number [frac]*/
    y[4] = AthOut;     /* throat area [in^2] */
    
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
