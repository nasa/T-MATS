#include "types_TMATS.h"
#include "constants_TMATS.h"
#include "functions_TMATS.h"
#include <math.h>

#ifdef MATLAB_MEX_FILE
#include "simstruc.h"
#endif

void Turbine_PSI_TMATS_body(double *y, const double *u, const double *CoolFlow, const Turbine_PSIStruct* prm)
{
    double WIn      = u[0];     /* Input Flow [pps]	*/
    double htIn     = u[1];     /* input enthalpy [BTU/lbm]*/
    double TtIn     = u[2];     /* Temperature Input [degR]  */
    double PtIn     = u[3];     /* Pressure Input [psia] 	 */
    double FARcIn   = u[4];     /* Compusted Fuel to Air Ratio [frac] */
    double Nmech    = u[5];     /* Mechancial Shaft Speed [rpm]*/
    double psiMapIn = u[6];     /* PSI map [NA] 	 */
    double s_T_Nc   = u[7];     /* Nc map scalar [NA]	*/
    double s_T_Wc   = u[8];     /* Wc map scalar [NA]	*/
    double s_T_PR   = u[9];     /* PR map scalar [NA]	*/
    double s_T_Eff  = u[10];     /* Eff map scalar [NA]	*/
    int    cfWidth  = u[11];     /* Cooling Flow Vector Length */

    /*--------Define Constants-------*/
    double WOut, htOut, TtOut, PtOut, FARcOut, TorqueOut, NErrorOut;
    double WcCalcin, WcMap, theta,delta, Pwrout, PRin, htin;
    double Test, htIdealout, Sout, NcMap, Nc, EffMap, Eff;
    double dHcools1, dHcoolout, Wfcools1, Wfcoolout, Ws1in,hts1in, Tts1in, FARs1in;
    double Ss1in, Wcoolout, Wcools1, PRmapRead;
    double C_Eff, C_PR, C_Nc, C_Wc, TtOutIdeal;
    double WMap, psiMapI, delHtIdealMap, erT, erT_old, Ptoutg, Ptoutg_old;
    double TtOutIdealg, WpqAcrit, WoWMap, Ptoutg_new;

    int interpErr = 0;
    double Wcool[100];
    double htcool[100];
    double Ttcool[100];
    double Ptcool[100];
    double FARcool[100];
    int Vtest, i;

    
    /* Verify input bleed vector is a multiple of 5 */
    Vtest = cfWidth/5;
    if(5*Vtest != cfWidth && prm->CoolFlwEn > 0.5 && *(prm->IWork+Er1)==0){
        #ifdef MATLAB_MEX_FILE
        printf("Error in %s, one or more of the cooling flow input vector eleements is missing(Vector form; 5x1: W,ht,Tt,Pt,FAR)\n",prm->BlkNm);
        #endif
        *(prm->IWork+Er1) = 1;
    }
    else if(prm->BldPosLeng != cfWidth/5 && prm->CoolFlwEn > 0.5 && *(prm->IWork+Er2)==0){
        #ifdef MATLAB_MEX_FILE
        printf("Errorin %s, number of cooling flow inputs does not match the length of the Cooling flow postion vector in the mask\n",prm->BlkNm);
        #endif
        *(prm->IWork+Er2) = 1;
    }

    /* unpack CoolFlow vector */
    for (i = 0; i < cfWidth/5; i++)
    {
        if (prm->CoolFlwEn < 0.5){
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
        if ((prm->T_BldPos[i] > 1 || prm->T_BldPos[i] < 0) && prm->CoolFlwEn > 0.5 && *(prm->IWork+Er3)==0){
            #ifdef MATLAB_MEX_FILE
            printf(" Error in %s, cooling flow postion element %i needs to be defined as a 0 or 1\n",prm->BlkNm,i+1);
            #endif
            *(prm->IWork+Er3) = 1;
        }

        /* calc mass flow for cooling flows */
        Wcools1 = Wcools1 + Wcool[i]*(1-prm->T_BldPos[i]);
        Wcoolout = Wcoolout + Wcool[i];

        /* calc fuel mass flow for cooling flows*/
        Wfcools1 = Wfcools1 + FARcool[i]*Wcool[i]*(1-prm->T_BldPos[i])*divby(1+FARcool[i]);
        Wfcoolout = Wfcoolout + FARcool[i]*Wcool[i]*divby(1+FARcool[i]);
    }
    /*-- Compute Total Flow  --------*/

    Ws1in = WIn + Wcools1;  /* mass flow at station 1 */
    WOut = WIn + Wcoolout;   /* mass flow at turbine exit */

    /*-- Compute Fuel to Air Ratios ---*/

    FARs1in = (FARcIn* WIn*divby(1+FARcIn) + Wfcools1)*divby(WIn*divby(1+FARcIn) + Wcools1- Wfcools1);
    FARcOut = (FARcIn* WIn*divby(1+FARcIn)+ Wfcoolout)*divby(WIn*divby(1+FARcIn) + Wcoolout- Wfcoolout);

    /* calc input enthalpy of cooling flow for stage 1 */
    for (i = 0; i < cfWidth/5; i++)
    {
        /* Compute cooling flow dH at stage 1  */
        dHcools1 = dHcools1 + htcool[i]*Wcool[i]*(1-prm->T_BldPos[i]);
        /* Compute cooling flow dH for the exit of the turbine assuming input htcool = htcoolout for turbine rear bleeds  */
        dHcoolout = dHcoolout + htcool[i]*Wcool[i]*prm->T_BldPos[i];
    }

    /*-- Compute avg enthalpy at stage 1 --------*/
    htin = t2hc(TtIn,FARcIn);
    hts1in = (htin* WIn + dHcools1)*divby(Ws1in);


    /*-- Compute  stage 1 total temp--------*/

    Tts1in = h2tc(hts1in,FARs1in);

    /*-- Compute Stage 1 entropy, assuming PtIn = Pts1in  --------*/
    Ss1in = pt2sc(PtIn,Tts1in,FARs1in);

    /*---- calculate misc. fluid condition related variables --------*/
    delta = PtIn / C_PSTD;
    theta = TtIn / C_TSTD;

    /*------ Calculate corrected speed ---------*/
    Nc = Nmech*divby(sqrtT(theta));

    if(prm->IDes < 0.5)
        C_Nc = Nc*divby(prm->NcDes);
    else
        C_Nc = s_T_Nc;

    NcMap = Nc*divby(C_Nc);

    /* ---- Calculate output entropy ----*/
    Sout = Ss1in;

    /*-- Compute Turbine Efficiency (from Turbine map)  --------*/

    psiMapI = interp2Ac(prm->X_T_PRpsiVec,prm->Y_T_NcpsiVec,prm->T_T_Map_psiArray,psiMapIn,NcMap,prm->B,prm->A,&interpErr);
    if (interpErr == 1 && *(prm->IWork+Er4)==0){
        #ifdef MATLAB_MEX_FILE
        printf("Warning in %s, Error calculating psiMapI. Vector definitions may need to be expanded.\n", prm->BlkNm);
        #endif
        *(prm->IWork+Er4) = 1;
    }
    EffMap = psiMapIn*divby(psiMapI);
    if(prm->IDes < 0.5)
        C_Eff = prm->EffDes*divby(EffMap);
    else
        C_Eff = s_T_Eff;

    Eff = EffMap * C_Eff;

    /* ---- Ideal enthalpy  ----*/
    delHtIdealMap = psiMapI * (Nmech / 60)*(Nmech / 60);
    htIdealout = hts1in - delHtIdealMap * prm->s_T_hi;


    /* ensure enthalpy is >= 0 */
    if(htIdealout < 0)
    {
        htIdealout = 0;
    }

    /* Determine Ideal exit temp */
    TtOutIdeal = h2tc(htIdealout,FARs1in);

    /* Determine starting point for iteration to find PR */
    Ptoutg = PtIn*powT((TtOutIdeal*divby(TtIn)),(prm->gamma_T*divby(prm->gamma_T-1)));
    TtOutIdealg = sp2tc(Sout,Ptoutg,FARs1in);
    erT = 100*fabs(TtOutIdealg - TtOutIdeal)*divby(TtOutIdeal);
    Ptoutg_new = Ptoutg;

    /* iterate to find Ptout when TtOutIdeal guess = TtOutIdeal */
    while (fabs(erT) > 0.05) {
        erT_old = erT;
        Ptoutg_old = Ptoutg;
        if(fabs(Ptoutg - Ptoutg_new) < 0.02)
            Ptoutg = Ptoutg + 0.05;
        else
            Ptoutg = Ptoutg_new;

        Ptoutg = Ptoutg + 0.05;
        TtOutIdealg = sp2tc(Sout,Ptoutg,FARs1in);
        erT = 100*(TtOutIdealg - TtOutIdeal)*divby(TtOutIdeal);
        if (fabs(erT) > 0.05) {
            /* determine next guess pressure by secant algorithm */
            Ptoutg_new = Ptoutg - erT *(Ptoutg - Ptoutg_old)*divby(erT - erT_old);
        }
    }
    PRin = PtIn*divby(Ptoutg);


    /*------ Compute pressure output --------*/
    if(prm->IDes < 0.5)
        C_PR = (PRin - 1)*divby(prm->PRmapDes -1);
    else
        C_PR = s_T_PR;

    PRmapRead = (PRin -1)*divby(C_PR) + 1;

    PtOut = PtIn*divby(PRin);

    /*-- Compute Total Flow input (from Turbine map)  --------*/

    WoWMap = interp2Ac(prm->X_T_PRwowVec,prm->Y_T_NcwowVec,prm->T_T_Map_WoWArray,PRmapRead,NcMap,prm->D,prm->C,&interpErr);
    if (interpErr == 1 && *(prm->IWork+Er5)==0){
        #ifdef MATLAB_MEX_FILE
        printf("Warning in %s, Error calculating WoWMap. Vector definitions may need to be expanded.\n", prm->BlkNm);
        #endif
        *(prm->IWork+Er5) = 1;
    }
    WpqAcrit = sqrtT((prm->gamma_T*C_GRAVITY)*divby(prm->Rt_T*JOULES_CONST))*divby(powT((1+(prm->gamma_T-1)/2),((prm->gamma_T+1)*divby(2*(prm->gamma_T-1)))));
    WMap = WoWMap * WpqAcrit * (PtIn*divby(sqrtT(Tts1in)));
    WcMap = WMap * sqrtT(theta)*divby(delta);
    if(prm->IDes < 0.5)
        C_Wc = Ws1in*sqrtT(theta)*divby(delta)*divby(WcMap);
    else
        C_Wc = s_T_Wc;

    WcCalcin = WcMap * C_Wc;

    /*-Compute power output only takes into account cooling flow that enters at front of engine (stage 1)-*/

    Pwrout = ((hts1in - htIdealout)*Eff)*Ws1in * C_BTU_PER_SECtoHP;

    /* ---- enthalpy output ----*/

    htOut = ((((htIdealout - hts1in)*Eff) + hts1in)*Ws1in + dHcoolout)*divby(WOut);

    /*------ Compute Temperature output (empirical) ---------*/

    TtOut = h2tc(htOut,FARcOut);

    /*----- Compute output Torque to shaft ----*/
    TorqueOut = C_HP_PER_RPMtoFT_LBF * Pwrout*divby(Nmech);

    /* ----- Compute Normalized Flow Error ----- */
    if (prm->IDes < 0.5 && prm->NDes == 0)
        NErrorOut = 100;
    else if (prm->IDes < 0.5)
        NErrorOut = (Nmech - prm->NDes)*divby(prm->NDes);
    else if (Ws1in == 0) {
        NErrorOut = 100;
    }
    else {
        NErrorOut = (Ws1in*sqrtT(theta)*divby(delta)-WcCalcin)*divby(Ws1in*sqrtT(theta)*divby(delta)) ;
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
