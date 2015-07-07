/*		T-MATS -- ValvePHY_TMATS.c
 * % *************************************************************************
 * % written by Jeffryes Chapman based on work by J.A. DeCastro
 * % NASA Glenn Research Center, Cleveland, OH
 * % June 6th, 2013
 * %
 * %  This file models a physics based Valve Element for T-MATS simulation
 * % *************************************************************************/
#define S_FUNCTION_NAME  ValvePHY_TMATS
#define S_FUNCTION_LEVEL 2
#include "simstruc.h"
#include "constants_TMATS.h"
#include <math.h>

#define s_V_Ae_vlv_p(S)                 ssGetSFcnParam(S,0)
#define s_V_Ae_byp_p(S)                 ssGetSFcnParam(S,1)
#define s_V_Ae_th_p(S)                  ssGetSFcnParam(S,2)
#define X_V_FAR_vec_p(S)                ssGetSFcnParam(S,3)
#define T_V_Rt_vec_p(S)                 ssGetSFcnParam(S,4)
#define Y_V_Tt_vec_p(S)                 ssGetSFcnParam(S,5)
#define T_V_gamma_array_p(S)            ssGetSFcnParam(S,6)
#define BN_p(S)                         ssGetSFcnParam(S,7)
#define NPARAMS 8

extern double interp2Ac(double kk[], double ll[], double mm[], double nn, double oo,int pp, int qq, int *error);
extern double interp1Ac(double aa[], double bb[], double cc, int ii,int *error);
extern double calc_Pstatic(double aaa, double bbb, double ccc, double ddd, double eee[], double fff[], double ggg[], double hhh[],double kkk, int iii, int jjj);
extern double calc_WvsMN(double jj, double kk, double ll, double mm, double nn, double oo);
extern double calc_PsvsMN(double pp, double qq, double rr);

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
    ssSetInputPortWidth(S, 0, 9);
    ssSetInputPortRequiredContiguous(S, 0, true);
    ssSetInputPortDirectFeedThrough(S, 0, 1);

    if (!ssSetNumOutputPorts(S, 1)) return;
    ssSetOutputPortWidth(S, 0, 3);

    ssSetNumSampleTimes(S, 1);
    ssSetNumRWork(S, 0);
    ssSetNumIWork(S, 3);
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
}
#endif

static void mdlOutputs(SimStruct *S, int_T tid)
{
    /*--------Define Parameters-------*/
    const real_T s_V_Ae_vlv                 = *mxGetPr(s_V_Ae_vlv_p(S));
    const real_T s_V_Ae_byp                 = *mxGetPr(s_V_Ae_byp_p(S));
    const real_T s_V_Ae_th                  = *mxGetPr(s_V_Ae_th_p(S));

    /*-------- vector & array data -------*/
    const real_T *X_V_FAR_vec               = mxGetPr(X_V_FAR_vec_p(S));
    const real_T *T_V_Rt_vec                = mxGetPr(T_V_Rt_vec_p(S));
    const real_T *Y_V_Tt_vec                = mxGetPr(Y_V_Tt_vec_p(S));
    const real_T *T_V_gamma_array           = mxGetPr(T_V_gamma_array_p(S));

    /*------get dimensions of parameter arrays-------*/
    const int_T A1  = mxGetNumberOfElements(X_V_FAR_vec_p(S));
    const int_T B1  = mxGetNumberOfElements(Y_V_Tt_vec_p(S));

    /*---------Define Inputs--------*/
    const real_T *u  = (const real_T*) ssGetInputPortSignal(S,0);


    double WbyIn	= u[0];     /* Bypass flow path flow rate [pps] 	*/
    double TtbyIn   = u[1];     /* Bypass Temperature [degR] 	*/
    double PtbyIn   = u[2];     /* Bypass disch. pressure [psia] 	*/
    double FARcbyIn = u[3];     /* Bypass combusted fuel to air ratio [frac] */
    double VlvPosIn	= u[4];     /* Valve Position [frac, 0-1] 	*/
    double WmfpIn	= u[5];     /* Main flow path flow rate [pps] 	*/
    double TtmfpIn	= u[6];     /* Main flow path Temprature [degR] 	*/
    double PtmfpIn	= u[7];     /* Main flow path Pressure Input [psia] 	*/
    double FARcmfpIn= u[8];     /* Main flow path combusted fuel to air ratio [frac] */

    real_T *y  = (real_T *)ssGetOutputPortRealSignal(S,0);  /* Output Array */

    /*--------Define Constants-------*/
    double Ath, Rb, gamb, Cpb, Pe, Me, Tcr_o_Te, Ae_o_Acr, Ath_o_Acr;
    double Mth0, Mth1, Tcr_o_Tth0, Tcr_o_Tth1, Ath_o_Acr0, Ath_o_Acr1, err0, err1, err;
    double Mth, Tcr_o_Tth_it, Ath_o_Acr_it, Tcr_o_T0, Tth_o_T0, Tth, Pth, rhoth, Vth, Wth, Wbyp_noz, Whpc;
    double MthOut, Test, WthOut;

    int interpErr = 0;
    int count;

    /* ------- get strings -------------- */
    char * BlkNm;
    int_T buflen;
    int_T status;

    /* Get name of block from dialog parameter (string) */
    buflen = mxGetN(BN_p(S))*sizeof(mxChar)+1;
    BlkNm = mxMalloc(buflen);
    status = mxGetString(BN_p(S), BlkNm, buflen);

    /* Input validation */
    if ((WbyIn <= 0 || WmfpIn <= 0) && ssGetIWork(S)[0]==0){
        printf("Flow rates must be nonzero !!");
        ssSetIWorkValue(S,0,1);
    }

    if (VlvPosIn > 0){
        if (VlvPosIn < 0.001)
            VlvPosIn = 0.001;
        Ath = VlvPosIn*s_V_Ae_th;      /* throat area [in^2] */

        /* define gas constants for booster discharge air */
        Rb = interp1Ac(X_V_FAR_vec,T_V_Rt_vec,FARcmfpIn,A1,&interpErr);
        if (interpErr == 1 && ssGetIWork(S)[1]==0){
            printf("Warning in %s, Error calculating Rb. Vector definitions may need to be expanded.\n", BlkNm);
            ssSetIWorkValue(S,1,1);
        }
        gamb = interp2Ac(X_V_FAR_vec,Y_V_Tt_vec,T_V_gamma_array,FARcmfpIn,TtmfpIn,A1,B1,&interpErr);
        if (interpErr == 1 && ssGetIWork(S)[2]==0){
            printf("Warning in %s, Error calculating gamb. Vector definitions may need to be expanded.\n", BlkNm);
            ssSetIWorkValue(S,2,1);
        }
        Cpb = Rb*gamb/(gamb-1);

        /* determine static pressure at the exit plane (entering fan); */
        /* assume bypass flow >> bleed flow */
        Pe = calc_Pstatic(PtbyIn,TtbyIn,WbyIn,s_V_Ae_byp,X_V_FAR_vec,T_V_Rt_vec,Y_V_Tt_vec,T_V_gamma_array,FARcbyIn,A1,B1);

        /* compute exit to critical area ratio */
        Me = sqrt(2/(gamb-1)*(pow(Pe/PtmfpIn, (1-gamb)/gamb)-1));
        Tcr_o_Te = (2/(gamb+1))*(1 + 0.5*(gamb-1)*Me*Me);
        Ae_o_Acr = pow(Tcr_o_Te, (gamb+1)/(2*(gamb-1)))/Me;

        /* obtain throat to critical area ratio */
        Ath_o_Acr = Ae_o_Acr*Ath/s_V_Ae_vlv;

        /* determine throat Mach no. iteratively; initialize guesses, errors */
        Mth0 = 0.1;      /* subsonic guess values */
        Mth1 = 0.2;

        Tcr_o_Tth0 = (2/(gamb+1))*(1 + 0.5*(gamb-1)*Mth0*Mth0);
        Tcr_o_Tth1 = (2/(gamb+1))*(1 + 0.5*(gamb-1)*Mth1*Mth1);
        Ath_o_Acr0 = pow(Tcr_o_Tth0, (gamb+1)/(2*(gamb-1)))/Mth0;
        Ath_o_Acr1 = pow(Tcr_o_Tth1, (gamb+1)/(2*(gamb-1)))/Mth1;
        err0 = Ath_o_Acr - Ath_o_Acr0;
        err1 = Ath_o_Acr - Ath_o_Acr1;

        err = 100;
        count = 0;

        while (fabs(err) > 0.002 && (err0 - err1) != 0 && count < 100){

            /* compute new Mach no. guess */
            Mth = Mth0 - err0*(Mth0 - Mth1)/(err0 - err1);
            if (Mth > 1.0)
                Mth = 1.0;

            /* compute error to drive solution towards specified area ratio */
            Tcr_o_Tth_it = (2/(gamb+1))*(1 + 0.5*(gamb-1)*Mth*Mth);
            Ath_o_Acr_it = pow(Tcr_o_Tth_it, (gamb+1)/(2*(gamb-1)))/Mth;
            err = Ath_o_Acr - Ath_o_Acr_it;

            /* propagate errors & guesses */
            Mth1 = Mth0;
            err1 = err0;
            Mth0 = Mth;
            err0 = err;

            count++;
        }

        /* compute throat static pressure, temperature and Mach no.; */
        /* modify if choked */
        Tcr_o_T0 = 2/(gamb+1);
        Tth_o_T0 = 1/(1 + 0.5*(gamb-1)*Mth*Mth);
        if (Tth_o_T0 < Tcr_o_T0)
            Tth_o_T0 = Tcr_o_T0;
        Tth = TtmfpIn*Tth_o_T0;
        Pth = PtmfpIn*pow(Tth_o_T0, gamb/(gamb-1));

        /* recompute the actual flow rate, assume no pressure loss */
        rhoth = Pth*144/(Rb*JOULES_CONST*Tth);      /* [lb/ft^3] */
        Vth = sqrt(2*Cpb*C_GRAVITY*JOULES_CONST*(TtmfpIn - Tth));     /* [ft/s] */
        Wth = rhoth*Ath/144*Vth;        /* [lb/s] */
        Mth = Vth/sqrt(gamb*Rb*C_GRAVITY*JOULES_CONST*Tth);
    }
    else {
        Wth = 0;
        Mth = 0;
    }

    WthOut = Wth;

    Test = WthOut;

    /*------Assign output values------------*/
    y[0] = WthOut;      /* Valve throat flow [pps] */
    y[1] = MthOut;      /* Mach no. at throat */
    y[2] = Test;        /* Output Test Point */

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
