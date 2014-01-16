/*		T-MATS -- Turbine_TMATS.c
 * % *************************************************************************
 * % written by Jeffryes Chapman based on work by R. May and T. Lavelle
 * % NASA Glenn Research Center, Cleveland, OH
 * % February 15th, 2013
 * %
 * %  This file models the Turbine Element for T-MATS simulation
 * % *************************************************************************/
#define S_FUNCTION_NAME  Turbine_TMATS
#define S_FUNCTION_LEVEL 2
#include "simstruc.h"
#include "constants_TMATS.h"
#include <math.h>

#define s_T_Nc_p(S)             ssGetSFcnParam(S,0)
#define s_T_PR_p(S)             ssGetSFcnParam(S,1)
#define s_T_Wc_p(S)             ssGetSFcnParam(S,2)
#define s_T_Eff_p(S)            ssGetSFcnParam(S,3)
#define Y_T_NcVec_p(S)          ssGetSFcnParam(S,4)
#define X_T_PRVec_p(S)          ssGetSFcnParam(S,5)
#define T_T_Map_WcArray_p(S)    ssGetSFcnParam(S,6)
#define T_T_Map_EffArray_p(S)   ssGetSFcnParam(S,7)
#define T_BldPos_p(S)           ssGetSFcnParam(S,8)
#define BldPosLeng_p(S)         ssGetSFcnParam(S,9)
#define CoolFlwEn_p(S)          ssGetSFcnParam(S,10)
#define IDesign_p(S)            ssGetSFcnParam(S,11)
#define NcDes_p(S)              ssGetSFcnParam(S,12)
#define EffDes_p(S)             ssGetSFcnParam(S,13)
#define PRmapDes_p(S)           ssGetSFcnParam(S,14)
#define NDes_p(S)               ssGetSFcnParam(S,15)
#define BN_p(S)                 ssGetSFcnParam(S,16)
#define NPARAMS 17

extern double h2tc(double a, double b);
extern double pt2sc(double c, double d, double e);
extern double sp2tc(double f, double g, double h);
extern double t2hc(double i, double j);
extern double interp2Ac(double kk[], double ll[], double mm[], double nn, double oo,int pp, int qq, int *error);



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
    ssSetInputPortWidth(S, 0, 7);
    ssSetInputPortRequiredContiguous(S, 0, true);
    ssSetInputPortDirectFeedThrough(S, 0, 1);
    
    ssSetInputPortWidth(S, 1, DYNAMICALLY_SIZED);
    ssSetInputPortRequiredContiguous(S, 1, true);
    ssSetInputPortDirectFeedThrough(S, 1, 1);
    
    if (!ssSetNumOutputPorts(S, 1)) return;
    ssSetOutputPortWidth(S, 0, 12);
    
    ssSetNumSampleTimes(S, 1);
    ssSetNumRWork(S, 0);
    ssSetNumIWork(S, 5);
    ssSetNumPWork(S, 0);
    ssSetNumModes(S, 0);
    ssSetNumNonsampledZCs(S, 0);
    
    ssSetOptions(S,
            SS_OPTION_WORKS_WITH_CODE_REUSE |
            SS_OPTION_EXCEPTION_FREE_CODE |
            SS_OPTION_USE_TLC_WITH_ACCELERATOR);
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
    ssSetIWorkValue(S,0,0);
    ssSetIWorkValue(S,1,0);
    ssSetIWorkValue(S,2,0);
    ssSetIWorkValue(S,3,0);
    ssSetIWorkValue(S,4,0);
}
#endif

static void mdlOutputs(SimStruct *S, int_T tid)
{
    /*--------Define Parameters-------*/
    const real_T s_T_Nc             = *mxGetPr(s_T_Nc_p(S));
    const real_T s_T_PR             = *mxGetPr(s_T_PR_p(S));
    const real_T s_T_Wc             = *mxGetPr(s_T_Wc_p(S));
    const real_T s_T_Eff            = *mxGetPr(s_T_Eff_p(S));
    const real_T NcDes              = *mxGetPr(NcDes_p(S));
    const real_T PRmapDes           = *mxGetPr(PRmapDes_p(S));
    const real_T EffDes             = *mxGetPr(EffDes_p(S));
    const real_T NDes               = *mxGetPr(NDes_p(S));
    const real_T IDes               = *mxGetPr(IDesign_p(S));
    const int_T  BldPosLeng         = *mxGetPr(BldPosLeng_p(S));
    const int_T  CoolFlwEn          = *mxGetPr(CoolFlwEn_p(S));
    
    /* vector & array data */
    const real_T *Y_T_NcVec            = mxGetPr(Y_T_NcVec_p(S));
    const real_T *X_T_PRVec            = mxGetPr(X_T_PRVec_p(S));
    const real_T *T_T_Map_WcArray      = mxGetPr(T_T_Map_WcArray_p(S));
    const real_T *T_T_Map_EffArray     = mxGetPr(T_T_Map_EffArray_p(S));
    const real_T *T_BldPos             = mxGetPr(T_BldPos_p(S));
    
    /*------get dimensions of parameter arrays-------*/
    const int_T A   = mxGetNumberOfElements(Y_T_NcVec_p(S));
    const int_T B   = mxGetNumberOfElements(X_T_PRVec_p(S));
    
    /*---------Define Inputs--------*/
    const real_T *u  = (const real_T*) ssGetInputPortSignal(S,0);
    
    double WIn      = u[0];     /* Input Flow [pps]	*/
    double htIn     = u[1];     /* input enthalpy [BTU/lbm]*/
    double TtIn     = u[2];     /* Temperature Input [degR]  */
    double PtIn     = u[3];     /* Pressure Input [psia] 	 */
    double FARcIn   = u[4];     /* Compusted Fuel to Air Ratio [frac] */
    double Nmech    = u[5];     /* Mechancial Shaft Speed [rpm]*/
    double PRIn     = u[6];     /* Pressure Ratio [NA] 	 */
    
    /*---------Define Inputs for input port 2--------*/
    /* N 5x1 vectors consisting of W, ht, Tt, Pt and FAR, where N is the number of cooling flows */
    const real_T *CoolFlow = ssGetInputPortRealSignal(S, 1);
    int cfWidth = ssGetCurrentInputPortDimensions(S, 1, 0);
    
    real_T *y  = (real_T *)ssGetOutputPortRealSignal(S,0);   /* Output Array */
    
    /*--------Define Constants-------*/
    double WOut, htOut, TtOut, PtOut, FARcOut, TorqueOut, NErrorOut;
    double WcCalcin, WcMap, theta,delta, Pwrout, PRin, htin;
    double TtIdealout, Test, htIdealout, Sout, NcMap, Nc, EffMap, Eff;
    double dHcools1, dHcoolout, Wfcools1, Wfcoolout, Ws1in,hts1in, Tts1in, Pts1in, FARs1in;
    double Ss1in, dHout, Wcoolout, Wcools1, PRmapRead;
    double C_Eff, C_PR, C_Nc, C_Wc;
    
    int interpErr = 0;
    double Wcool[100];
    double htcool[100];
    double Ttcool[100];
    double Ptcool[100];
    double FARcool[100];
    int Vtest, i;
    
    /* ------- get strings -------------- */
    char * BlkNm;
    int_T buflen;
    int_T status;
    
    /* Get name of block from dialog parameter (string) */
    buflen = mxGetN(BN_p(S))*sizeof(mxChar)+1;
    BlkNm = mxMalloc(buflen);
    status = mxGetString(BN_p(S), BlkNm, buflen);
    
    /* Verify input bleed vector is a multiple of 5 */
    Vtest = cfWidth/5;
    if(5*Vtest != cfWidth && CoolFlwEn > 0.5 && ssGetIWork(S)[0]==0){
        printf("Error in %s, one or more of the cooling flow input vector eleements is missing(Vector form; 5x1: W,ht,Tt,Pt,FAR)\n",BlkNm);
        ssSetIWorkValue(S,0,1);
    }
    else if(BldPosLeng != cfWidth/5 && CoolFlwEn > 0.5 && ssGetIWork(S)[1]==0){
        printf("Errorin %s, number of cooling flow inputs does not match the length of the Cooling flow postion vector in the mask\n",BlkNm);
        ssSetIWorkValue(S,1,1);
    }
    
    /* unpack CoolFlow vector */
    for (i = 0; i < cfWidth/5; i++)
    {
        if (CoolFlwEn < 0.5){
            Wcool[i] = 0;
            htcool[i] = 0;
            Ttcool[i] = 0;
            Ptcool[i] = 0;
            FARcool[i] = 0;
        }
        else {
            Wcool[i] = CoolFlow[5*i];
            Ttcool[i] = CoolFlow[5*i+2];
            Ptcool[i] = CoolFlow[5*i+3];
            FARcool[i] = CoolFlow[5*i+4];
            htcool[i] = t2hc(Ttcool[i],FARcool[i]);
        }
    }
    
    /* Initialize cooling flow sum constants */
    
    dHcools1 = 0;   /* enthalpy * mass cooling flow rate at stage 1 of turbine */
    dHcoolout = 0;   /* enthalpy * mass cooling flow rate at exit of turbine */
    Wcools1 = 0;    /* total cooling flow at stage 1 of turbine*/
    Wcoolout = 0;    /* total cooling flow at output of turbine */
    Wfcools1 = 0;  /* combusted fuel flow in cooling at stage 1 of turbine */
    Wfcoolout = 0;  /* combusted fuel flow in cooling at exit of turbine */
    
    /* calc cooling flow constants for stage 1 and output of the turbine */
    for (i = 0; i < cfWidth/5; i++)
    {
        if ((T_BldPos[i] > 1 || T_BldPos[i] < 0) && CoolFlwEn > 0.5 && ssGetIWork(S)[2]==0){
            printf(" Error in %s, cooling flow postion element %i needs to be defined as a 0 or 1\n",BlkNm,i+1);
            ssSetIWorkValue(S,2,1);
        }
        
        /* calc mass flow for cooling flows */
        Wcools1 = Wcools1 + Wcool[i]*(1-T_BldPos[i]);
        Wcoolout = Wcoolout + Wcool[i];
        
        /* calc fuel mass flow for cooling flows*/
        Wfcools1 = Wfcools1 + FARcool[i]*Wcool[i]*(1-T_BldPos[i])/(1+FARcool[i]);
        Wfcoolout = Wfcoolout + FARcool[i]*Wcool[i]/(1+FARcool[i]);
    }
    /*-- Compute Total Flow  --------*/
    
    Ws1in = WIn + Wcools1;  /* mass flow at station 1 */
    WOut = WIn + Wcoolout;   /* mass flow at turbine exit */
    
    /*-- Compute Fuel to Air Ratios ---*/
    
    FARs1in = (FARcIn* WIn/(1+FARcIn) + Wfcools1)/(WIn/(1+FARcIn) + Wcools1- Wfcools1);
    FARcOut = (FARcIn* WIn/(1+FARcIn)+ Wfcoolout)/(WIn/(1+FARcIn) + Wcoolout- Wfcoolout);
    
    /* calc input enthalpy of cooling flow for stage 1 */
    for (i = 0; i < cfWidth/5; i++)
    {
        /* Compute cooling flow dH at stage 1  */
        dHcools1 = dHcools1 + htcool[i]*Wcool[i]*(1-T_BldPos[i]);
        /* Compute cooling flow dH for the exit of the turbine assuming input htcool = htcoolout for turbine rear bleeds  */
        dHcoolout = dHcoolout + htcool[i]*Wcool[i]*T_BldPos[i];
    }
    
    /*-- Compute avg enthalpy at stage 1 --------*/
    htin = t2hc(TtIn,FARcIn);
    hts1in = (htin* WIn + dHcools1)/Ws1in;
    
    /*-- Compute  stage 1 total temp--------*/
    
    Tts1in = h2tc(hts1in,FARs1in);
    
    /*-- Compute Stage 1 entropy, assuming PtIn = Pts1in  --------*/
    Ss1in = pt2sc(PtIn,Tts1in,FARs1in);
    
    /*---- calculate misc. fluid condition related variables --------*/
    delta = PtIn / C_PSTD;
    theta = TtIn / C_TSTD;
    
    /*------ Calculate corrected speed ---------*/
    Nc = Nmech/sqrt(theta);
    
    if(IDes > 0.5)
        C_Nc = NcDes / Nc;
    else
        C_Nc = s_T_Nc;
    
    NcMap = Nc * C_Nc;
    
    /*------ Compute pressure output --------*/
    if(IDes > 0.5)
        C_PR = (PRmapDes -1)/(PRIn - 1);
    else
        C_PR = s_T_PR;
    
    PRmapRead = C_PR*(PRIn -1)+1;
    
    PtOut = PtIn/PRIn;	/* using PR from input */
    
    /*-- Compute Total Flow input (from Turbine map)  --------*/
    
    WcMap = interp2Ac(X_T_PRVec,Y_T_NcVec,T_T_Map_WcArray,PRmapRead,NcMap,B,A,&interpErr);
    if (interpErr == 1 && ssGetIWork(S)[3]==0){
        printf("Warning in %s, Error calculating WcMap. Vector definitions may need to be expanded.\n", BlkNm);
        ssSetIWorkValue(S,3,1);
    }
    if(IDes > 0.5)
        C_Wc = Ws1in*sqrt(theta)/delta / WcMap;
    else
        C_Wc = s_T_Wc;
    
    WcCalcin = WcMap * C_Wc;
    
    /*-- Compute Turbine Efficiency (from Turbine map)  --------*/
    
    EffMap = interp2Ac(X_T_PRVec,Y_T_NcVec,T_T_Map_EffArray,PRmapRead,NcMap,B,A,&interpErr);
    if (interpErr == 1 && ssGetIWork(S)[4]==0){
        printf("Warning in %s, Error calculating EffMap. Vector definitions may need to be expanded.\n", BlkNm);
        ssSetIWorkValue(S,4,1);
    }
    if(IDes > 0.5)
        C_Eff = EffDes / EffMap;
    else
        C_Eff = s_T_Eff;
    
    Eff = EffMap * C_Eff;
    
    
    /*------ enthalpy calculations ---------*/
    /* ---- Ideal enthalpy  ----*/
    Sout = Ss1in;
    TtIdealout = sp2tc(Sout,PtOut,FARcOut);
    htIdealout = t2hc(TtIdealout,FARcOut); /* may need to be updated due to TtIdealout not being correct */
    
    /*-Compute power output only takes into account cooling flow that enters at front of engine (stage 1)-*/
    
    Pwrout = ((hts1in - htIdealout)*Eff)*Ws1in * C_BTU_PER_SECtoHP;
    
    /* ---- enthalpy output ----*/
    
    htOut = ((((htIdealout - hts1in)*Eff) + hts1in)*Ws1in + dHcoolout)/WOut;
    
    /*------ Compute Temperature output (empirical) ---------*/
    
    TtOut = h2tc(htOut,FARcOut);
    
    /*----- Compute output Torque to shaft ----*/
    TorqueOut = C_HP_PER_RPMtoFT_LBF * Pwrout/Nmech;
    
    /* ----- Compute Normalized Flow Error ----- */
    if (IDes > 0.5 && NDes == 0)
        NErrorOut = 100;
    else if (IDes > 0.5)
        NErrorOut = (Nmech - NDes)/NDes;
    else if (Ws1in == 0) {
        NErrorOut = 100;
    }
    else {
        NErrorOut = (Ws1in*sqrt(theta)/delta-WcCalcin)/(Ws1in*sqrt(theta)/delta) ;
    }
    Test = Wcool[0];
    /*------Assign output values------------        */
    y[0] = WOut;            /* Outlet Total Flow [pps]   */
    y[1] = htOut;           /* Outlet Enthalpy [BTU/lbm]*/
    y[2] = TtOut;           /* Outlet Temperature [degR]      */
    y[3] = PtOut;           /* Outlet Pressure  [psia]     */
    y[4] = FARcOut;         /* Outlet Fuel to Air Ratio [NA]	*/
    y[5] = TorqueOut;       /* Torque Output [lbf*ft]       */
    y[6] = NErrorOut;       /* Normalized turbine Error [frac]*/
    y[7] = C_Nc;            /* Corrected Shaft Speed Scalar */
    y[8] = C_Wc;            /* Corrected Flow Scalar */
    y[9] = C_PR;            /* Pressure Ratio Scalar */
    y[10] = C_Eff;          /* Efficiency Scalar */
    y[11] = Test;
    
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
