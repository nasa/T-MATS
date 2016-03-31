/*		T-MATS -- Compressor_TMATS.c
 * % *************************************************************************
 * % written by Jeffryes Chapman based on work by R. May and T. Lavelle
 * % NASA Glenn Research Center, Cleveland, OH
 * % February 27th, 2013
 * %
 * %  This file models the Compressor Element for T-MATS simulation
 * % *************************************************************************/
#define S_FUNCTION_NAME  Compressor_TMATS
#define S_FUNCTION_LEVEL 2
#include "simstruc.h"
#include "constants_TMATS.h"
#include <math.h>

#define s_C_Nc_p(S)             ssGetSFcnParam(S,0)
#define s_C_Wc_p(S)             ssGetSFcnParam(S,1)
#define s_C_PR_p(S)             ssGetSFcnParam(S,2)
#define s_C_Eff_p(S)            ssGetSFcnParam(S,3)
#define Y_C_Map_NcVec_p(S)      ssGetSFcnParam(S,4)
#define X_C_RlineVec_p(S)       ssGetSFcnParam(S,5)
#define T_C_Map_WcArray_p(S)    ssGetSFcnParam(S,6)
#define T_C_Map_PRArray_p(S)    ssGetSFcnParam(S,7)
#define T_C_Map_EffArray_p(S)   ssGetSFcnParam(S,8)
#define FracCusBldht_p(S)       ssGetSFcnParam(S,9)
#define FracCusBldPt_p(S)       ssGetSFcnParam(S,10)
#define FracBldht_p(S)          ssGetSFcnParam(S,11)
#define FracBldPt_p(S)          ssGetSFcnParam(S,12)
#define X_C_Map_WcSurgeVec_p(S) ssGetSFcnParam(S,13)
#define T_C_Map_PRSurgeVec_p(S) ssGetSFcnParam(S,14)
#define IDesign_p(S)            ssGetSFcnParam(S,15)
#define NcDes_p(S)              ssGetSFcnParam(S,16)
#define EffDes_p(S)             ssGetSFcnParam(S,17)
#define PRDes_p(S)              ssGetSFcnParam(S,18)
#define RlineDes_p(S)           ssGetSFcnParam(S,19)
#define CustBldEn_p(S)          ssGetSFcnParam(S,20)
#define FBldEn_p(S)             ssGetSFcnParam(S,21)
#define BN_p(S)                 ssGetSFcnParam(S,22)
#define CustBldNm_p(S)          ssGetSFcnParam(S,23)
#define FracBldNm_p(S)          ssGetSFcnParam(S,24)
#define WcMapCol_p(S)           ssGetSFcnParam(S,25)
#define PRMapCol_p(S)           ssGetSFcnParam(S,26)
#define EffMapCol_p(S)          ssGetSFcnParam(S,27)
#define WcMapRw_p(S)            ssGetSFcnParam(S,28)
#define PRMapRw_p(S)            ssGetSFcnParam(S,29)
#define EffMapRw_p(S)           ssGetSFcnParam(S,30)
#define NPARAMS 31

extern double h2tc(double a, double b);
extern double pt2sc(double c, double d, double e);
extern double sp2tc(double f, double g, double h);
extern double t2hc(double i, double j);
extern double interp1Ac(double aa[], double bb[], double cc, int ii,int *error);
extern double interp2Ac(double kk[], double ll[], double mm[], double nn, double oo,int pp, int qq, int *error);

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
    
    if (!ssSetNumInputPorts(S, 3)) return;
    ssSetInputPortWidth(S, 0, 7);
    ssSetInputPortRequiredContiguous(S, 0, true);
    ssSetInputPortDirectFeedThrough(S, 0, 1);
    
    ssSetInputPortWidth(S, 1, DYNAMICALLY_SIZED);
    ssSetInputPortRequiredContiguous(S, 1, true);
    ssSetInputPortDirectFeedThrough(S, 1, 1);
    
    ssSetInputPortWidth(S, 2, DYNAMICALLY_SIZED);
    ssSetInputPortRequiredContiguous(S, 2, true);
    ssSetInputPortDirectFeedThrough(S, 2, 1);
    
    if (!ssSetNumOutputPorts(S, 3)) return;
    ssSetOutputPortWidth(S, 0, 27);
    ssSetOutputPortWidth(S, 1, DYNAMICALLY_SIZED);
    ssSetOutputPortWidth(S, 2, DYNAMICALLY_SIZED);
    
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
    ssSetOutputPortWidth(S,port,5*inputPortWidth);
}

# define MDL_SET_OUTPUT_PORT_WIDTH
static void mdlSetOutputPortWidth(SimStruct *S, int_T port,
        int_T outputPortWidth)
{
    ssSetInputPortWidth(S,port,outputPortWidth/5);
    ssSetOutputPortWidth(S,port,outputPortWidth);
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
    ssSetIWorkValue(S,Er1,0);
    ssSetIWorkValue(S,Er2,0);
    ssSetIWorkValue(S,Er3,0);
    ssSetIWorkValue(S,Er4,0);
    ssSetIWorkValue(S,Er5,0);
}
#endif

static void mdlOutputs(SimStruct *S, int_T tid)
{
    /*--------Define Parameters-------*/
    const real_T s_C_Nc             = *mxGetPr(s_C_Nc_p(S));
    const real_T s_C_Wc             = *mxGetPr(s_C_Wc_p(S));
    const real_T s_C_PR             = *mxGetPr(s_C_PR_p(S));
    const real_T s_C_Eff            = *mxGetPr(s_C_Eff_p(S));
    const real_T NcDes              = *mxGetPr(NcDes_p(S));
    const real_T PRDes              = *mxGetPr(PRDes_p(S));
    const real_T EffDes             = *mxGetPr(EffDes_p(S));
    const real_T RlineDes           = *mxGetPr(RlineDes_p(S));
    const real_T IDes               = *mxGetPr(IDesign_p(S));
    const real_T CustBldEn          = *mxGetPr(CustBldEn_p(S));
    const real_T FBldEn             = *mxGetPr(FBldEn_p(S));
    const real_T CustBldNm          = *mxGetPr(CustBldNm_p(S));
    const real_T FracBldNm          = *mxGetPr(FracBldNm_p(S));
    
    /* vector & array data */
    const real_T *Y_C_Map_NcVec        = mxGetPr(Y_C_Map_NcVec_p(S));
    const real_T *X_C_RlineVec         = mxGetPr(X_C_RlineVec_p(S));
    const real_T *T_C_Map_WcArray      = mxGetPr(T_C_Map_WcArray_p(S));
    const real_T *T_C_Map_PRArray      = mxGetPr(T_C_Map_PRArray_p(S));
    const real_T *T_C_Map_EffArray     = mxGetPr(T_C_Map_EffArray_p(S));
    
    const real_T *FracCusBldht         = mxGetPr(FracCusBldht_p(S));
    const real_T *FracCusBldPt         = mxGetPr(FracCusBldPt_p(S));
    const real_T *FracBldht            = mxGetPr(FracBldht_p(S));
    const real_T *FracBldPt            = mxGetPr(FracBldPt_p(S));
    
    const real_T *X_C_Map_WcSurgeVec   = mxGetPr(X_C_Map_WcSurgeVec_p(S));
    const real_T *T_C_Map_PRSurgeVec   = mxGetPr(T_C_Map_PRSurgeVec_p(S));
    
    /*------get dimensions of parameter arrays-------*/
    const int_T A   = mxGetNumberOfElements(Y_C_Map_NcVec_p(S));
    const int_T B   = mxGetNumberOfElements(X_C_RlineVec_p(S));
    const int_T C   = mxGetNumberOfElements(X_C_Map_WcSurgeVec_p(S));
    
    const int_T WcMapCol  = *mxGetPr(WcMapCol_p(S));
    const int_T PRMapCol  = *mxGetPr(PRMapCol_p(S));
    const int_T EffMapCol = *mxGetPr(EffMapCol_p(S));
    const int_T WcMapRw   = *mxGetPr(WcMapRw_p(S));
    const int_T PRMapRw   = *mxGetPr(PRMapRw_p(S));
    const int_T EffMapRw  = *mxGetPr(EffMapRw_p(S));
    
    /*---------Define Inputs for input port 1--------*/
    const real_T *u  = (const real_T*) ssGetInputPortSignal(S,0);
    
    double WIn      = u[0];     /* Input Flow [pps] 	*/
    double htIn     = u[1];     /* Input Enthalpy [BTU/lbm] */
    double TtIn     = u[2];     /* Temperature Input [degR] 	*/
    double PtIn     = u[3];     /* Pressure Input [psia] 	*/
    double FARcIn   = u[4];     /* Combusted Fuel to Air Ratio [frac] 	*/
    double Nmech    = u[5];     /* Mechancial Shaft Speed [rpm] 	*/
    double Rline    = u[6];     /* Rline [NA]  */
    
    /*---------Define Inputs for input port 2--------*/
    const real_T *Wcust = ssGetInputPortRealSignal(S, 1);
    int uWidth1 = CustBldNm;
    
    /*---------Define Inputs for input port 3--------*/
    const real_T *FracWbld  = ssGetInputPortSignal(S,2);
    int uWidth2 = FracBldNm;
    
    real_T *y  = (real_T *)ssGetOutputPortRealSignal(S,0);   /* Output Array port 1 */
    real_T *y1  = (real_T *)ssGetOutputPortRealSignal(S,1);   /* Output Array port 2 */
    real_T *y2  = (real_T *)ssGetOutputPortRealSignal(S,2);   /* Output Array port 3 */
    
    /*--------Define Constants-------*/
    double WOut, htOut, TtOut, PtOut, FARcOut, TorqueOut, NErrorOut;
    double C_Nc, C_Wc, C_PR, C_Eff;
    double htin, Sin, Wcin, WcCalcin, WcMap, theta,delta, Pwrout, Wbleeds, Wsumbleed;
    double TtIdealout, htIdealout, Test, Sout, NcMap, Nc, PRMap, PR, EffMap, Eff;
    double Wb4bleed, Pwrb4bleed, PwrBld;
    double SPR, SPRMap, SMavail, SMMap;
    
    /* Define Arrays for bleed calcs */
    int MaxNumberBleeds = 100;
    double WcustOut[500];
    double PtcustOut[500];
    double TtcustOut[500];
    double FARcustOut[500];
    double WbldOut[500];
    double FARbldOut[500];
    double PtbldOut[500];
    double TtbldOut[500];
    double htbldOut[500];
    double htcustOut[500];
    
    int interpErr = 0;
    int i;
    
    /* ------- get strings -------------- */
    char * BlkNm;
    int_T buflen;
    int_T status;
    
    /* Get name of block from dialog parameter (string) */
    buflen = mxGetN(BN_p(S))*sizeof(mxChar)+1;
    BlkNm = mxMalloc(buflen);
    status = mxGetString(BN_p(S), BlkNm, buflen);
    
    
    /*-- Compute output Fuel to Air Ratio ---*/
    FARcOut = FARcIn;
    
    /*-- Compute Input enthalpy --------*/
    
    htin = t2hc(TtIn,FARcIn);
    
    /*-- Compute Input entropy  --------*/
    
    Sin = pt2sc(PtIn,TtIn,FARcIn);
    
    /*---- calculate misc. fluid condition related variables and corrected Flow --*/
    delta = PtIn / C_PSTD;
    theta = TtIn / C_TSTD;
    Wcin = WIn*sqrt(theta)/delta;
    
    /*------ Calculate corrected speed ---------*/
    Nc = Nmech/sqrt(theta);
    if (IDes < 0.5)
        C_Nc = Nc / NcDes ;
    else
        C_Nc = s_C_Nc;
    
    NcMap = Nc / C_Nc;
    
    /*-- Compute Total Flow input (from Compressor map)  --------*/
    
    WcMap = interp2Ac(X_C_RlineVec,Y_C_Map_NcVec,T_C_Map_WcArray,Rline,NcMap,B,A,&interpErr);
    if ((WcMapCol != B || WcMapRw != A) && ssGetIWork(S)[Er1]==0){
        printf("Warning in %s, Error calculating WcMap. Table size does not match axis vector lengths.\n", BlkNm);
        ssSetIWorkValue(S,Er1,1);
    }
    else if (interpErr == 1 && ssGetIWork(S)[Er1]==0){
        printf("Warning in %s, Error calculating WcMap. Vector definitions may need to be expanded.\n", BlkNm);
        ssSetIWorkValue(S,Er1,1);
    }
    
    if (IDes < 0.5)
        C_Wc = Wcin / WcMap;
    else
        C_Wc = s_C_Wc;
    
    WcCalcin = WcMap * C_Wc;
    
    /*-- Compute Pressure Ratio (from Compressor map)  --------*/
    
    PRMap = interp2Ac(X_C_RlineVec,Y_C_Map_NcVec,T_C_Map_PRArray,Rline,NcMap,B,A,&interpErr);
    if ((PRMapCol != B || PRMapRw != A) && ssGetIWork(S)[Er2]==0){
        printf("Warning in %s, Error calculating PRMap. Table size does not match axis vector lengths.\n", BlkNm);
        ssSetIWorkValue(S,Er2,1);
    }
    else if (interpErr == 1 && ssGetIWork(S)[Er2]==0){
        printf("Warning in %s, Error calculating PRMap. Vector definitions may need to be expanded.\n", BlkNm);
        ssSetIWorkValue(S,Er2,1);
    }
    
    if (IDes < 0.5)
        C_PR = (PRDes -1) / (PRMap-1);
    else
        C_PR = s_C_PR;
    
    PR = C_PR*(PRMap - 1) + 1 ;
    
    /*-- Compute Efficiency (from Compressor map) ---*/
    
    EffMap = interp2Ac(X_C_RlineVec,Y_C_Map_NcVec,T_C_Map_EffArray,Rline,NcMap,B,A,&interpErr);
    if ((EffMapCol != B || EffMapRw != A) && ssGetIWork(S)[Er3]==0){
        printf("Warning in %s, Error calculating EffMap. Table size does not match axis vector lengths.\n", BlkNm);
        ssSetIWorkValue(S,Er3,1);
    }
    else if (interpErr == 1 && ssGetIWork(S)[Er3]==0){
        printf("Warning in %s, Error calculating EffMap. Vector definitions may need to be expanded.\n", BlkNm);
        ssSetIWorkValue(S,Er3,1);
    }
    
    if (IDes < 0.5)
        C_Eff = EffDes / EffMap;
    else
        C_Eff = s_C_Eff;
    
    Eff = EffMap * C_Eff;
    
    /*------ Compute pressure output --------*/
    
    PtOut = PtIn*PR;
    
    
    /*------ enthalpy calculations ---------*/
    
    /* ---- Ideal enthalpy ----*/
    Sout = Sin;
    TtIdealout = sp2tc(Sout,PtOut,FARcIn);
    htIdealout = t2hc(TtIdealout,FARcIn);
    
    
    /* ---- Final enthalpy output ----*/
    
    htOut = ((htIdealout - htin)/Eff) + htin;
    
    /*------ Compute Temperature output ---------*/
    
    TtOut = h2tc(htOut,FARcIn);
    
    
    /* initalize Bleed sums components */
    Wbleeds = 0;
    PwrBld = 0;
    
    /* compute customer Bleed components */
    for (i = 0; i < uWidth1; i++)
    {
        /* if customer bleed = 0 or Cust bld is not enabled set outputs to zero */
        if (Wcust[i] == 0 || CustBldEn < 0.5){
            WcustOut[i] = 0;
            htcustOut[i] = 0;
            TtcustOut[i] = 0;
            PtcustOut[i] = 0;
            FARcustOut[i] = 0;
        }
        else {
            /*-- Compute sum of customer Bleed Flow output  --------*/
            Wbleeds = Wbleeds + Wcust[i]; /* add to total bleed value */
            WcustOut[i] = Wcust[i];
            FARcustOut[i] = FARcIn;
            htcustOut[i] = htin + FracCusBldht[i]*(htOut - htin); /* calculate customer bleed enthalpy */
            PtcustOut[i] = PtIn + FracCusBldPt[i]*(PtOut -PtIn); /* calculate customer bleed Total Pressure */
            TtcustOut[i] = h2tc(htcustOut[i],FARcustOut[i]); /* calculate customer bleed Total Temp */
            PwrBld = PwrBld + WcustOut[i]*(htcustOut[i]-htOut)*C_BTU_PER_SECtoHP;  /* calculate customer bleed power */
        }
        if (i > 4*MaxNumberBleeds && ssGetIWork(S)[Er4]==0){
            printf("Error in %s, Number of bleeds in compressor exceeds 100... Array overflow! Reading Bad Data\n", BlkNm);
            ssSetIWorkValue(S,Er4,1);
        }
    }
    
    /*----Disable Fractional bleed when requested----*/
    
    for (i = 0; i < uWidth2; i++)
    {
        if (FracWbld[i] <= 0 || FBldEn < 0.5 ){
            WbldOut[i] = 0;
            htbldOut[i] = 0;
            FARbldOut[i] = 0;
            TtbldOut[i] = 0;
            PtbldOut[i] = 0;
        }
        
        else {
            /*-- Compute sum of Fractional Bleed Flow output  --------*/
            Wbleeds = Wbleeds + FracWbld[i]*WIn; /* add to total bleed value */
            WbldOut[i] = FracWbld[i]*WIn;
            FARbldOut[i] = FARcIn;
            PtbldOut[i] = PtIn + FracBldPt[i]*(PtOut -PtIn); /* calculate  bleed Total Pressure */
            htbldOut[i] = htin + FracBldht[i]*(htOut - htin); /* calculate  bleed enthalpy */
            TtbldOut[i] = h2tc(htbldOut[i],FARbldOut[i]); /* calculate  bleed Total Temp */
            PwrBld = PwrBld + WbldOut[i]*(htbldOut[i]-htOut)*C_BTU_PER_SECtoHP;  /* calculate bleed power */
        }
        if (i > 4*MaxNumberBleeds && ssGetIWork(S)[Er4]==0){
            printf("Error in %s, Number of bleeds in compressor exceeds 100... Array overflow! Reading Bad Data\n", BlkNm);
            ssSetIWorkValue(S,Er4,1);
        }
    }
    
    /*-- Compute Flows  --------*/
    Wb4bleed = WIn;
    WOut = WIn - Wbleeds;
    
    /*------ Compute Powers ---------*/
    
    Pwrb4bleed = Wb4bleed * (htin - htOut) * C_BTU_PER_SECtoHP;
    Pwrout = Pwrb4bleed - PwrBld;
    
    /*----- Compute output Torque to shaft ----*/
    TorqueOut = C_HP_PER_RPMtoFT_LBF * Pwrout/Nmech;
    
    /* ----- Compute Normalized Flow Error ----- */
    if (IDes < 0.5 && Rline == 0)
        NErrorOut = 100;
    else if (IDes < 0.5)
        NErrorOut = (Rline - RlineDes)/Rline;
    else if (WIn == 0)
        NErrorOut = 100;
    else
        NErrorOut = (Wcin - WcCalcin)/Wcin ;
    
    /* Compute Stall Margin */
    SPRMap = interp1Ac(X_C_Map_WcSurgeVec,T_C_Map_PRSurgeVec,WcMap,C,&interpErr);
    if (interpErr == 1 && ssGetIWork(S)[Er5]==0){
        printf("Warning in %s, Error calculating SPR. Vector definitions may need to be expanded.\n", BlkNm);
        ssSetIWorkValue(S,Er5,1);
    }
    SPR = C_PR*(SPRMap - 1) + 1;
    SMavail = (SPR - PR)/PR * 100;
    SMMap = (SPRMap - PRMap)/PRMap * 100;
    
    /* Test variable */
    Test = SPRMap;
    
    /*------Assign output values port1------------*/
    y[0] = WOut;            /* Outlet Total Flow [pps]	*/
    y[1] = htOut;           /* Output Enthalpy [BTU/lbm]	*/
    y[2] = TtOut;           /* Outlet Temperature [degR]	*/
    y[3] = PtOut;           /* Outlet Pressure [psia] 	*/
    y[4] = FARcOut;         /* Exit Combusted Fuel Flow [frac] */
    y[5] = TorqueOut;       /* Outlet Torque [lbf*ft]	*/
    y[6] = NErrorOut;       /* Normalized compressor Error [frac]*/
    y[7] = SMavail;         /* Available Stall Margin [%] */
    y[8] = C_Nc;            /* Corrected shaft speed scalar */
    y[9] = C_Wc;            /* Corrected flow scalar */
    y[10] = C_PR;           /* Pressure Ratio scalar */
    y[11] = C_Eff;          /* Efficiency scalar */
    y[12] = Wcin;           /* Corrected input flow [pps] */
    y[13] = Nc;             /* Corrected speed [rpm]*/
    y[14] = PR;             /* Pressure ratio */
    y[15] = NcMap;          /* Map corrected speed */
    y[16] = WcMap;          /* Map corrected flow */
    y[17] = PRMap;          /* Map pressure ratio */
    y[18] = EffMap;         /* Map efficiency */
    y[19] = SPR;            /* Surge pressure ratio */
    y[20] = Wbleeds;        /* Bleed flow [pps]*/
    y[21] = Pwrb4bleed;     /* Power if there was no bleed [hp]*/
    y[22] = PwrBld;         /* Power loss due to bleed [hp] */
    y[23] = Pwrout;         /* Output power [hp]*/
    y[24] = SMMap;          /* Stall margin calculated from map values [%]*/
    y[25] = SPRMap;         /* Map stall pressure ratio*/
    y[26] = Test;           /* test signal */
    
    /*------Assign output values port2------------*/
    /* Customer or flow based bleed*/
    for (i = 0; i < uWidth1; i++)
    {
        *y1++ = WcustOut[i];
        *y1++ = htcustOut[i];
        *y1++ = TtcustOut[i];
        *y1++ = PtcustOut[i];
        *y1++ = FARcustOut[i];
    }
    
    /*------Assign output values port3------------*/
    /* fractional bleed, typically used for turbine cooling flow */
    for (i = 0; i < uWidth2; i++)
    {
        *y2++ = WbldOut[i];
        *y2++ = htbldOut[i];
        *y2++ = TtbldOut[i];
        *y2++ = PtbldOut[i];
        *y2++ = FARbldOut[i];
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
