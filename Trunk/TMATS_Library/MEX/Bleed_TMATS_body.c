#include "types_TMATS.h"
#include "constants_TMATS.h"
#include "functions_TMATS.h"
#include <math.h>

#ifdef MATLAB_MEX_FILE
#include "simstruc.h"
#endif

void Bleed_TMATS_body(double *y, double *y1, const double *u, const double *BldInFlow, const BleedStruct* prm)
{
    double WIn      = u[0];     /* Input Flow [pps] 	*/
    double htIn     = u[1];     /* Input Enthalpy [BTU/lbm] */
    double TtIn     = u[2];     /* Temperature Input [degR] 	*/
    double PtIn     = u[3];     /* Pressure [psia] 	*/
    double FARIn    = u[4];     /* combusted Fuel to Air Ratio [frac] 	*/
    double BldInLen = u[5];     /* Length of Bleed Vector input */
    
    /*--------Define Constants-------*/
    double Wfin, htin, Test;
    double WOut, htOut, TtOut, PtOut, FAROut, WbldoutTotal;
    double Ws1,hts1,Tts1,Pts1,FARs1, Wfs1;
    double dHblds1, dHbldout, Wblds1, Wbldout, Wfblds1, Wfbldout;
    int Vtest;
    int i;
    
    /* define arrays for bleed outputs */
    /* output bleeds */
    double WbldOut[100];
    double FARbldOut[100];
    double PtbldOut[100];
    double TtbldOut[100];
    double htbldOut[100];
    /* input bleeds */
    double Wbld[100];
    double FARbld[100];
    double Ptbld[100];
    double Ttbld[100];
    double htbld[100];
    
        
    /* Verify input bleed vector is a multiple of 5 */
    Vtest = BldInLen/5;
    if(5*Vtest != BldInLen && *(prm->IWork+Er1)==0){
        #ifdef MATLAB_MEX_FILE
        printf("Error in %s, one or more of the cooling flow input vector elements is missing(Vector form; 5x1: W,ht,Tt,Pt,FAR)\n",prm->BlkNm);
        #endif
        *(prm->IWork+Er1) = 1;
    }
    
    /* unpack BleedFlow vector */
    for (i = 0; i < BldInLen/5; i++)
    {
            Wbld[i] = BldInFlow[5*i];
            Ttbld[i] = BldInFlow[5*i+2];
            Ptbld[i] = BldInFlow[5*i+3];
            FARbld[i] = BldInFlow[5*i+4];
            htbld[i] = t2hc(Ttbld[i],FARbld[i]);
    }
    /* Initialize cooling flow sum constants */
    
    dHblds1 = 0;   /* enthalpy * mass cooling flow rate at stage 1 */
    Wblds1 = 0;    /* total cooling flow at stage 1*/
    Wfblds1 = 0;  /* combusted fuel flow in cooling at stage */
    
    /* calc station 1 bleed parameters */
    for (i = 0; i < BldInLen/5; i++)
    {
        Wblds1 = Wblds1 + Wbld[i];
        Wfblds1 = Wfblds1 + FARbld[i]*Wbld[i]*divby(1+FARbld[i]);
        dHblds1 = dHblds1 + htbld[i]*Wbld[i];
    }
    /* calc station 1 flow parameters */
    Ws1 = Wblds1 + WIn;
    Wfin = FARIn * WIn*divby(1+FARIn);
    Wfs1 = Wfblds1 + Wfin;
    FARs1 = Wfs1*divby(Ws1 - Wfs1);
    
    htin = t2hc(TtIn,FARIn);
    hts1 = (htin* WIn + dHblds1)*divby(Ws1);
    
    Tts1 = h2tc(hts1,FARs1);
    Pts1 = PtIn;
    
    
    htOut = hts1;
    TtOut = Tts1;
    PtOut = Pts1;
    FAROut = FARs1;
    
    WbldoutTotal = 0;
    for (i = 0; i < prm->WfracLen; i++)
    {
        WbldOut[i] = prm->Wfrac[i]*WIn;
        WbldoutTotal = WbldoutTotal +WbldOut[i];
    }
    
    WOut = Ws1 - WbldoutTotal;
    
    Test = PtIn;
    
    /*------Assign output values------------*/
    y[0] = WOut;            /* Outlet Total Flow [pps]   */
    y[1] = htOut;           /* Outlet Enthalpy [BTU/lbm]*/
    y[2] = TtOut;           /* Outlet Temperature [degR]      */
    y[3] = PtOut;           /* Outlet Pressure  [psia]     */
    y[4] = FAROut;          /* Outlet Fuel to Air Ratio [NA]	*/
    y[5] = Test;
        /*------Assign output values port2------------*/
    /* fractional bleed, typically used for turbine cooling flow */
    for (i = 0; i < prm->WfracLen; i++)
    {
        *y1++ = WbldOut[i];
        *y1++ = htOut;
        *y1++ = TtOut;
        *y1++ = PtOut;
        *y1++ = FAROut;
    }
    
}
