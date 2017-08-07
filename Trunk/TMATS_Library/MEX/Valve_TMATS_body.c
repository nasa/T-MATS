#include "types_TMATS.h"
#include "functions_TMATS.h"
#include <math.h>

#ifdef MATLAB_MEX_FILE
#include "simstruc.h"
#endif

void Valve_TMATS_body(double* y, const double* u, const ValveStruct* prm)
{
    double PtbyIn   = u[0];     /* Bypass disch. pressure [psia] 	*/
    double VlvPosIn	= u[1];     /* Valve Position [frac, 0-1] 	*/
    double WmfpIn	= u[2];     /* Main flow path flow rate [pps] 	*/
    double TtmfpIn	= u[3];     /* Main flow path Temprature [degR] 	*/
    double PtmfpIn	= u[4];     /* Main flow path Pressure Input [psia] 	*/

    /*--------Define Constants-------*/
    double ValveFrac, ValvePR, WthOut, Valve_active_Ae;
    double bleedFlxCr, Test;

    int interpErr = 0;
    
    /* Compute Valve open fraction */
    ValveFrac = (VlvPosIn-prm->VlvdeadZone)*divby(prm->VlvfullyOpen-prm->VlvdeadZone);

    /* ratio of total pressures at inlet and exit of bleed line */
    ValvePR = PtmfpIn*divby(PtbyIn);

    /* Determine flow properties of the valve */
    if ((ValveFrac <= 0) || (ValvePR <= 1.0))	/* dead zone or one-way valve */
    {

        WthOut = 0;                 /* Valve throat flow [pps] */
    }
    else	/* the valve is open and flow is moving */
    {
        /*------ Compute Active Area ---------*/
        Valve_active_Ae = ValveFrac*prm->Valve_Ae;

        /* compute corrected flow based on pressure ratio */
        bleedFlxCr = interp1Ac(prm->X_V_PRVec,prm->T_V_WcVec,ValvePR,prm->A,&interpErr);
        if (interpErr == 1 && *(prm->IWork+Er1)==0){
            #ifdef MATLAB_MEX_FILE
            printf("Warning in %s, Error calculating bleedFlxCr. Vector definitions may need to be expanded.\n", prm->BlkNm);
            #endif
            *(prm->IWork+Er1) = 1;
        }
        /*------ Compute Air flow through valve ---------*/
        WthOut = bleedFlxCr*PtmfpIn*divby(sqrtT(TtmfpIn))*Valve_active_Ae;  /* Valve throat flow [pps] */
        if (WthOut > WmfpIn) /* Flow check */
        {
            WthOut = WmfpIn;
        }
    }

    Test = WthOut;

    /*------Assign output values------------*/
    y[0] = WthOut;      /* Valve throat flow [pps] */
    y[1] = Test;        /* Output Test Point */

}
