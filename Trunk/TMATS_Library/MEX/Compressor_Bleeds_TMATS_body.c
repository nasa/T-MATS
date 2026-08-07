#include "types_TMATS.h"
#include "constants_TMATS.h"
#include "functions_TMATS.h"
#include <math.h>

#ifdef MATLAB_MEX_FILE
#include "simstruc.h"
#endif

void Compressor_Bleeds_TMATS_body(double* y, double* y1, double* y2, const double* u, const double* Wcust, const double* FracWbld, const CompressorStruct* prm)
{
    double WIn      = u[0];     /* Input Flow [pps] 	*/
    double htIn     = u[1];     /* Input Enthalpy [BTU/lbm] */
    double TtIn     = u[2];     /* Temperature Input [degR] 	*/
    double PtIn     = u[3];     /* Pressure Input [psia] 	*/
    double FARcIn   = u[4];     /* Combusted Fuel to Air Ratio [frac] 	*/
    double htOut    = u[5];     /* Outlet Enthalpy [BTU/lbm] */
    double PtOut    = u[6];     /* Pressure at Outlet [psia] 	*/

    int uWidth1 = prm->CustBldNm;
    int uWidth2 = prm->FracBldNm;

    /*--------Define Constants-------*/
    double WOut, FARcOut, htin;
    double Wbleeds;
    double Wb4bleed, PwrBld;

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

    int i;

    /*-- Compute output Fuel to Air Ratio ---*/
    FARcOut = FARcIn;

    /*-- Compute Input enthalpy --------*/
    htin = t2hc(TtIn,FARcIn);

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
            PtcustOut[i] = PtIn + prm->FracCusBldPt[i]*(PtOut - PtIn); /* calculate customer bleed Total Pressure */
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

    /*------Assign output values port1------------*/
    y[0] = WOut;            /* Outlet Total Flow [pps]	*/
    y[1] = Wbleeds;        /* Bleed flow [pps]*/
    y[2] = PwrBld;         /* Power loss due to bleed [hp] */

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
