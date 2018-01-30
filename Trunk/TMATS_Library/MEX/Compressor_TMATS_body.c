#include "types_TMATS.h"
#include "constants_TMATS.h"
#include "functions_TMATS.h"
#include <math.h>

#ifdef MATLAB_MEX_FILE
#include "simstruc.h"
#endif

void Compressor_TMATS_body(double* y, double* y1, double* y2, const double* u, const double* Wcust, const double* FracWbld, const CompressorStruct* prm)
{
    double WIn      = u[0];     /* Input Flow [pps] 	*/
    double htIn     = u[1];     /* Input Enthalpy [BTU/lbm] */
    double TtIn     = u[2];     /* Temperature Input [degR] 	*/
    double PtIn     = u[3];     /* Pressure Input [psia] 	*/
    double FARcIn   = u[4];     /* Combusted Fuel to Air Ratio [frac] 	*/
    double Nmech    = u[5];     /* Mechancial Shaft Speed [rpm] 	*/
    double Rline    = u[6];     /* Rline [NA]  */
    double Alpha    = u[7];     /* Alpha [NA]  */
    double s_C_Nc   = u[8];     /* Nc map scalar [NA]  */
    double s_C_Wc   = u[9];     /* Wc map scalar [NA] */
    double s_C_PR   = u[10];    /* PR map scalar [NA]  */
    double s_C_Eff  = u[11];    /* Eff map scalar [NA]  */

    int uWidth1 = prm->CustBldNm;
    int uWidth2 = prm->FracBldNm;

    /*--------Define Constants-------*/
    double WOut, htOut, TtOut, PtOut, FARcOut, TorqueOut, NErrorOut;
    double C_Nc, C_Wc, C_PR, C_Eff;
    double htin, Sin, Wcin, WcCalcin, WcMap, theta,delta, Pwrout, Wbleeds;
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

    double SMWcVec[500];
    double SMPRVec[500];
    
    int i;
    
    // Variables for iterative search to find Rline stall
    int interpErr = 0;
    int iterations;
    double RlineErr, RlineErrOld, RlineGuess;
    double RlineGuessBounds[2];
    double WcMapTemp, PRMapTemp, SPRMapTemp;

    /*-- Compute output Fuel to Air Ratio ---*/
    FARcOut = FARcIn;

    /*-- Compute Input enthalpy --------*/

    htin = t2hc(TtIn,FARcIn);

    /*-- Compute Input entropy  --------*/

    Sin = pt2sc(PtIn,TtIn,FARcIn);

    /*---- calculate misc. fluid condition related variables and corrected Flow --*/
    delta = PtIn / C_PSTD;
    theta = TtIn / C_TSTD;
    Wcin = WIn*sqrtT(theta)*divby(delta);

    /*------ Calculate corrected speed ---------*/
    Nc = Nmech*divby(sqrtT(theta));
    if (prm->IDes < 0.5)
        C_Nc = Nc *divby(prm->NcDes) ;
    else
        C_Nc = s_C_Nc;

    NcMap = Nc *divby(C_Nc);

    /*-- Compute Total Flow input (from Compressor map)  --------*/
    if(prm->C > 1)
        WcMap = interp3Ac(prm->X_C_RlineVec,prm->Y_C_Map_NcVec,prm->Z_C_AlphaVec,prm->T_C_Map_WcArray,Rline,NcMap,Alpha,prm->B,prm->A,prm->C,&interpErr);
    else
        WcMap = interp2Ac(prm->X_C_RlineVec,prm->Y_C_Map_NcVec,prm->T_C_Map_WcArray,Rline,NcMap,prm->B,prm->A,&interpErr);

    if ((prm->WcMapCol != prm->B || prm->WcMapRw != prm->A || prm->WcMapLay !=prm->C) && *(prm->IWork+Er1)==0){
        #ifdef MATLAB_MEX_FILE
        printf("Warning in %s, Error calculating WcMap. Table size does not match axis vector lengths.\n", prm->BlkNm);
        #endif
        *(prm->IWork+Er1) = 1;
    }
    else if (interpErr == 1 && *(prm->IWork+Er1)==0){
        #ifdef MATLAB_MEX_FILE
        printf("Warning in %s, Error calculating WcMap. Vector definitions may need to be expanded.\n", prm->BlkNm);
        #endif
        *(prm->IWork+Er1) = 1;
    }

    if (prm->IDes < 0.5)
        C_Wc = Wcin*divby(WcMap);
    else
        C_Wc = s_C_Wc;

    WcCalcin = WcMap * C_Wc;

    /*-- Compute Pressure Ratio (from Compressor map)  --------*/
    if(prm->C > 1)
        PRMap = interp3Ac(prm->X_C_RlineVec,prm->Y_C_Map_NcVec,prm->Z_C_AlphaVec,prm->T_C_Map_PRArray,Rline,NcMap,Alpha,prm->B,prm->A,prm->C,&interpErr);
    else
        PRMap = interp2Ac(prm->X_C_RlineVec,prm->Y_C_Map_NcVec,prm->T_C_Map_PRArray,Rline,NcMap,prm->B,prm->A,&interpErr);

    if ((prm->PRMapCol != prm->B || prm->PRMapRw != prm->A || prm->PRMapLay !=prm->C) && *(prm->IWork+Er2)==0){
        #ifdef MATLAB_MEX_FILE
        printf("Warning in %s, Error calculating PRMap. Table size does not match axis vector lengths.\n", prm->BlkNm);
        #endif
        *(prm->IWork+Er2) = 1;
    }
    else if (interpErr == 1 && *(prm->IWork+Er2)==0){
        #ifdef MATLAB_MEX_FILE
        printf("Warning in %s, Error calculating PRMap. Vector definitions may need to be expanded.\n", prm->BlkNm);
        #endif
        *(prm->IWork+Er2) = 1;
    }

    if (prm->IDes < 0.5)
        C_PR = (prm->PRDes -1)*divby(PRMap-1);
    else
        C_PR = s_C_PR;

    PR = C_PR*(PRMap - 1) + 1 ;

    /*-- Compute Efficiency (from Compressor map) ---*/
    if(prm->C > 1)
        EffMap = interp3Ac(prm->X_C_RlineVec,prm->Y_C_Map_NcVec,prm->Z_C_AlphaVec,prm->T_C_Map_EffArray,Rline,NcMap,Alpha,prm->B,prm->A,prm->C,&interpErr);
    else
        EffMap = interp2Ac(prm->X_C_RlineVec,prm->Y_C_Map_NcVec,prm->T_C_Map_EffArray,Rline,NcMap,prm->B,prm->A,&interpErr);

    if ((prm->EffMapCol != prm->B || prm->EffMapRw != prm->A || prm->EffMapLay !=prm->C) && *(prm->IWork+Er3)==0){
        #ifdef MATLAB_MEX_FILE
        printf("Warning in %s, Error calculating EffMap. Table size does not match axis vector lengths.\n", prm->BlkNm);
        #endif
        *(prm->IWork+Er3) = 1;
    }
    else if (interpErr == 1 && *(prm->IWork+Er3)==0){
        #ifdef MATLAB_MEX_FILE
        printf("Warning in %s, Error calculating EffMap. Vector definitions may need to be expanded.\n", prm->BlkNm);
        #endif
        *(prm->IWork+Er3) = 1;
    }

    if (prm->IDes < 0.5)
        C_Eff = prm->EffDes*divby(EffMap);
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

    htOut = ((htIdealout - htin)*divby(Eff)) + htin;

    /*------ Compute Temperature output ---------*/

    TtOut = h2tc(htOut,FARcIn);


    /* initalize Bleed sums components */
    Wbleeds = 0;
    PwrBld = 0;

    /* compute customer Bleed components */
    for (i = 0; i < uWidth1; i++)
    {
        /* if customer bleed = 0 or Cust bld is not enabled set outputs to zero */
        if (Wcust[i] == 0 || prm->CustBldEn < 0.5){
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
            htcustOut[i] = htin + prm->FracCusBldht[i]*(htOut - htin); /* calculate customer bleed enthalpy */
            PtcustOut[i] = PtIn + prm->FracCusBldPt[i]*(PtOut -PtIn); /* calculate customer bleed Total Pressure */
            TtcustOut[i] = h2tc(htcustOut[i],FARcustOut[i]); /* calculate customer bleed Total Temp */
            PwrBld = PwrBld + WcustOut[i]*(htcustOut[i]-htOut)*C_BTU_PER_SECtoHP;  /* calculate customer bleed power */
        }
        if (i > 4*MaxNumberBleeds && *(prm->IWork+Er4)==0){
            #ifdef MATLAB_MEX_FILE
            printf("Error in %s, Number of bleeds in compressor exceeds 100... Array overflow! Reading Bad Data\n", prm->BlkNm);
            #endif
            *(prm->IWork+Er4) = 1;
        }
    }

    /*----Disable Fractional bleed when requested----*/

    for (i = 0; i < uWidth2; i++)
    {
        if (FracWbld[i] <= 0 || prm->FBldEn < 0.5 ){
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
            PtbldOut[i] = PtIn + prm->FracBldPt[i]*(PtOut -PtIn); /* calculate  bleed Total Pressure */
            htbldOut[i] = htin + prm->FracBldht[i]*(htOut - htin); /* calculate  bleed enthalpy */
            TtbldOut[i] = h2tc(htbldOut[i],FARbldOut[i]); /* calculate  bleed Total Temp */
            PwrBld = PwrBld + WbldOut[i]*(htbldOut[i]-htOut)*C_BTU_PER_SECtoHP;  /* calculate bleed power */
        }
        if (i > 4*MaxNumberBleeds && *(prm->IWork+Er4)==0){
            #ifdef MATLAB_MEX_FILE
            printf("Error in %s, Number of bleeds in compressor exceeds 100... Array overflow! Reading Bad Data\n", prm->BlkNm);
            #endif
            *(prm->IWork+Er4) = 1;
        }
    }

    /*-- Compute Flows  --------*/
    Wb4bleed = WIn;
    WOut = WIn - Wbleeds;

    /*------ Compute Powers ---------*/

    Pwrb4bleed = Wb4bleed * (htin - htOut) * C_BTU_PER_SECtoHP;
    Pwrout = Pwrb4bleed - PwrBld;

    /*----- Compute output Torque to shaft ----*/
    TorqueOut = C_HP_PER_RPMtoFT_LBF * Pwrout*divby(Nmech);

    /* ----- Compute Normalized Flow Error ----- */
    if (prm->IDes < 0.5 && Rline == 0)
        NErrorOut = 100;
    else if (prm->IDes < 0.5)
        NErrorOut = (Rline - prm->RlineDes)*divby(Rline);
    else if (WIn == 0)
        NErrorOut = 100;
    else
        NErrorOut = (Wcin - WcCalcin)*divby(Wcin);

    /* Compute Stall Margin */
    if (prm->C > 1){
        /* Define 1-prm->D surge margin vectors based on alpha */
        for (i = 0; i < prm->D/prm->C; i++){
            SMWcVec[i] = interp1Ac(prm->Z_C_AlphaVec, prm->X_C_Map_WcSurgeVec + prm->C*i, Alpha,prm->C, &interpErr);
            if (interpErr == 1 && *(prm->IWork+Er5)==0){
                #ifdef MATLAB_MEX_FILE
                printf("Warning in %s, Error calculating 1D SMWcVec. Vector definitions may need to be adjusted.\n", prm->BlkNm);
                #endif
                *(prm->IWork+Er5) = 1;
            }
            SMPRVec[i] = interp1Ac(prm->Z_C_AlphaVec, prm->T_C_Map_PRSurgeVec + prm->C*i, Alpha,prm->C, &interpErr);
            if (interpErr == 1 && *(prm->IWork+Er5)==0){
                #ifdef MATLAB_MEX_FILE
                printf("Warning in %s, Error calculating 1D SMPRVec. Vector definitions may need to be adjusted.\n", prm->BlkNm);
                #endif
                *(prm->IWork+Er5) = 1;
            }
        }
        SPRMap = interp1Ac(SMWcVec, SMPRVec,WcMap,prm->D/prm->C,&interpErr);
        if (interpErr == 1 && *(prm->IWork+Er5)==0){
            #ifdef MATLAB_MEX_FILE
            printf("Warning in %s, Error calculating 2D SPR. Vector definitions may need to be adjusted.\n", prm->BlkNm);
            #endif
            *(prm->IWork+Er5) = 1;
        }
    }
    else
        SPRMap = interp1Ac(prm->X_C_Map_WcSurgeVec,prm->T_C_Map_PRSurgeVec,WcMap,prm->D,&interpErr);
        
    if (interpErr == 1 && *(prm->IWork+Er5)==0){
        #ifdef MATLAB_MEX_FILE
        printf("Warning in %s, Error calculating SPR. Vector definitions may need to be expanded.\n", prm->BlkNm);
        #endif
        *(prm->IWork+Er5) = 1;
    }
    SPR = C_PR*(SPRMap - 1) + 1;
    
    // If SMN calculation desired instead of SMW (via checkbox in mask)...
    if (prm->SMNEn > 0.5)
    {
        // Iterative stall r-line finder, via binary search.
        // Should not take many iterations since this is an easy search.
        // Assumes RlineVec is monotonically increasing and that the stall
        // line is a function with an inverse.
        iterations = 20;
        RlineErr = 1000;
        RlineGuessBounds[0] = prm->X_C_RlineVec[0];
        RlineGuessBounds[1] = prm->X_C_RlineVec[prm->B - 1];
        while ( ((iterations--) > 0) && abs(RlineErr) > 0.01)
        {
            // Take a guess at the stall R-line for current speed, use the
            // middle of our current search range (which narrows as we go)
            RlineGuess = (RlineGuessBounds[0] + RlineGuessBounds[1]) / 2;
            // Look up the Wc and PR at current Nc, and guessed R-line.
            if(prm->C > 1)
                WcMapTemp = interp3Ac(prm->X_C_RlineVec,prm->Y_C_Map_NcVec,prm->Z_C_AlphaVec,prm->T_C_Map_WcArray,RlineGuess,NcMap,Alpha,prm->B,prm->A,prm->C,&interpErr);
            else
                WcMapTemp = interp2Ac(prm->X_C_RlineVec,prm->Y_C_Map_NcVec,prm->T_C_Map_WcArray,RlineGuess,NcMap,prm->B,prm->A,&interpErr);
            if(prm->C > 1)
                PRMapTemp = interp3Ac(prm->X_C_RlineVec,prm->Y_C_Map_NcVec,prm->Z_C_AlphaVec,prm->T_C_Map_PRArray,RlineGuess,NcMap,Alpha,prm->B,prm->A,prm->C,&interpErr);
            else
                PRMapTemp = interp2Ac(prm->X_C_RlineVec,prm->Y_C_Map_NcVec,prm->T_C_Map_PRArray,RlineGuess,NcMap,prm->B,prm->A,&interpErr);
            
            // Compute the stall pressure ratio of the guess point.
            // Take the difference between that stall PR and the PR of the
            // guess point. As the guess point gets closer to the stall
            // line, this difference will get closer to zero. Once we're
            // there, the guess value for Rline will be the stall Rline.
            if (prm->C > 1)
            {
                SPRMapTemp = interp1Ac(SMWcVec, SMPRVec,WcMapTemp,prm->D/prm->C,&interpErr);
                if (interpErr == 1 && *(prm->IWork+Er5)==0){
                    #ifdef MATLAB_MEX_FILE
                    printf("Warning in %s, Error calculating 2D SPR for SMN solver. Vector definitions may need to be expanded.\n", prm->BlkNm);
                    #endif
                    *(prm->IWork+Er5) = 1;
                }
            }
            else
            {
                SPRMapTemp = interp1Ac(prm->X_C_Map_WcSurgeVec,prm->T_C_Map_PRSurgeVec,WcMapTemp,prm->D,&interpErr);
                if (interpErr == 1 && *(prm->IWork+Er5)==0){
                    #ifdef MATLAB_MEX_FILE
                    printf("Warning in %s, Error calculating SPR for SMN solver. Vector definitions may need to be expanded.\n", prm->BlkNm);
                    #endif
                    *(prm->IWork+Er5) = 1;
                }
            }
            RlineErr = (SPRMapTemp-PRMapTemp) / SPRMapTemp;
            
            // If this error is greater than zero, we are below stall.
            // So we'll want to decrease Rline (to get closer to stall)
            // Therefore, we'll want to shrink upper search bounds.
            if (RlineErr > 0)
            {
                // Search between current guess Rline and lower bound.
                RlineGuessBounds[1] = RlineGuess;
            }
            // Otherwise, error is positive, so we are past stall.
            // So we'll want to increase Rline (to move away from stall)
            // Therefore, we'll want to shrink lower search bounds.
            else
            {
                // Search between current guess Rline and upper bound.
                RlineGuessBounds[0] = RlineGuess;
            }
        }
        if (iterations <= 0)
        {
            #ifdef MATLAB_MEX_FILE
            printf("Warning in %s, SM solver could not converge.\n", prm->BlkNm);
            #endif
            *(prm->IWork+Er5) = 2;
        }
        // Ok, so our iterative search process should have driven us up
        // our current speed line, up to the stall line.
        // If converged, we'll have the stall Wc and Pr in the temp vars.
        // Stall margin at constant speed
        SMMap = ((WcMap/WcMapTemp) / (PRMap/PRMapTemp) - 1.0) * 100.;
        // Ok, to compute the SMavail number, just scale the unscaled map
        // values for stall PR and Wc and use those.
        WcMapTemp = C_Wc*WcMapTemp;
        PRMapTemp = C_PR*(PRMapTemp - 1) + 1;
        SMavail = ((WcCalcin/WcMapTemp) / (PR/PRMapTemp) - 1.0) * 100.;
    }
    // Else, we're calculating normal SMW.
    else
    {
        SMavail = (SPR - PR)*divby(PR) * 100;
        SMMap = (SPRMap - PRMap)*divby(PRMap) * 100;
    }
    
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
