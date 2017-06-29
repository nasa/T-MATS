/*
 * Body of calculations for Burner_TMATS S-function
 */

#include "types_TMATS.h"
#include "functions_TMATS.h"

/* For clarity: */
#define WfIn   (u[0])     /* Input Fuel Flow[pps] */
#define WIn    (u[1])     /* Input Flow [pps] */
#define htIn   (u[2])     /* Input enthalpy [BTU/lbm] */
#define TtIn   (u[3])     /* Temperature Input [degR] */
#define PtIn   (u[4])     /* Pressure Input [psia] */
#define FARcIn (u[5])     /* Combusted Fuel to Air Ratio [frac] */

extern double t2hc(double a, double b);
extern double h2tc(double c, double d);

void Burner_TMATS_body(double* y, double* u, BurnStruct* prm)
{
    /* Define constants */
    double htin;
    double htOut, TtOut, PtOut, FARcOut, WOut;
    double Test;
    
     /*-- Compute Input enthalpy (empirical) --------*/
    
    htin = t2hc(TtIn,FARcIn);
    
    /*-- Compute Flow output  --------*/
    
    WOut = WIn + WfIn;     /*Perfect combustion*/
    
    /*-- Compute Input fuel to air ratio --*/
    
    FARcOut = (WIn* FARcIn + WfIn)*divby(WIn*(1-FARcIn));
    
    /*------ Compute enthalpy output ---------*/
    if (prm->LHVEn < 0.5)
        htOut = (WIn*htin + WfIn*prm->hFuel)*divby(WOut);
    else
        htOut = (WIn*htin + WfIn*prm->LHV*prm->Eff)*divby(WOut);
    
    /*------ Compute Temperature output ---------*/
    
    TtOut = h2tc(htOut,FARcOut);
    
    /*------ Compute pressure output ---------*/
    PtOut = (1- prm->dPnormBurner) * PtIn;
    
    Test = htin;
    
    /*------Assign output values------------*/
    y[0] = WOut;     /* Output Air Flow [pps]	*/
    y[1] = htOut; 	  /* Output Enthalpy [BTU/lbm] */
    y[2] = TtOut;     /* Output Temperature  [degR]*/
    y[3] = PtOut;     /* Output Pressure [psia]	*/
    y[4] = FARcOut;   /* Output Combusted Fuel to Air Ratio [frac] */
    y[5] = Test;      /* Output Test Point */
}
