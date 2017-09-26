#include "types_TMATS.h"
#include "constants_TMATS.h"
#include "functions_TMATS.h"
#include <math.h>

#ifdef MATLAB_MEX_FILE
#include "simstruc.h"
#endif

void ValvePHY_TMATS_body(double *y, const double *u, const ValvePHYStruct* prm)
{
    double WbyIn	= u[0];     /* Bypass flow path flow rate [pps] 	*/
    double TtbyIn   = u[1];     /* Bypass Temperature [degR] 	*/
    double PtbyIn   = u[2];     /* Bypass disch. pressure [psia] 	*/
    double FARcbyIn = u[3];     /* Bypass combusted fuel to air ratio [frac] */
    double VlvPosIn	= u[4];     /* Valve Position [frac, 0-1] 	*/
    double WmfpIn	= u[5];     /* Main flow path flow rate [pps] 	*/
    double TtmfpIn	= u[6];     /* Main flow path Temprature [degR] 	*/
    double PtmfpIn	= u[7];     /* Main flow path Pressure Input [psia] 	*/
    double FARcmfpIn= u[8];     /* Main flow path combusted fuel to air ratio [frac] */


    /*--------Define Constants-------*/
    double Ath, Rb, gamb, Cpb, Pe, Me, Tcr_o_Te, Ae_o_Acr, Ath_o_Acr;
    double Mth0, Mth1, Tcr_o_Tth0, Tcr_o_Tth1, Ath_o_Acr0, Ath_o_Acr1, err0, err1, err;
    double Mth, Tcr_o_Tth_it, Ath_o_Acr_it, Tcr_o_T0, Tth_o_T0, Tth, Pth, rhoth, Vth, Wth, Wbyp_noz, Whpc;
    double MthOut, Test, WthOut;

    int interpErr = 0;
    int count;

    
    /* Input validation */
    if ((WbyIn <= 0 || WmfpIn <= 0) && *(prm->IWork+Er1)==0){
        #ifdef MATLAB_MEX_FILE
        printf("Flow rates must be nonzero !!");
        #endif
        *(prm->IWork+Er1) = 1;
    }

    if (VlvPosIn > 0){
        if (VlvPosIn < 0.001)
            VlvPosIn = 0.001;
        Ath = VlvPosIn*prm->s_V_Ae_th;      /* throat area [in^2] */

        /* define gas constants for booster discharge air */
        Rb = interp1Ac(prm->X_V_FAR_vec,prm->T_V_Rt_vec,FARcmfpIn,prm->A1,&interpErr);
        if (interpErr == 1 && *(prm->IWork+Er2)==0){
            #ifdef MATLAB_MEX_FILE
            printf("Warning in %s, Error calculating Rb. Vector definitions may need to be expanded.\n", prm->BlkNm);
            #endif
            *(prm->IWork+Er2) = 1;
        }
        gamb = interp2Ac(prm->X_V_FAR_vec,prm->Y_V_Tt_vec,prm->T_V_gamma_array,FARcmfpIn,TtmfpIn,prm->A1,prm->B1,&interpErr);
        if (interpErr == 1 && *(prm->IWork+Er3)==0){
            #ifdef MATLAB_MEX_FILE
            printf("Warning in %s, Error calculating gamb. Vector definitions may need to be expanded.\n", prm->BlkNm);
            #endif
            *(prm->IWork+Er3) = 1;
        }
        Cpb = Rb*gamb*divby(gamb-1);

        /* determine static pressure at the exit plane (entering fan); */
        /* assume bypass flow >> bleed flow */
        Pe = calc_Pstatic(PtbyIn,TtbyIn,WbyIn,prm->s_V_Ae_byp,prm->X_V_FAR_vec,prm->T_V_Rt_vec,prm->Y_V_Tt_vec,prm->T_V_gamma_array,FARcbyIn,prm->A1,prm->B1);

        /* compute exit to critical area ratio */
        Me = sqrtT(2*divby(gamb-1)*(powT(Pe*divby(PtmfpIn), (1-gamb)*divby(gamb))-1));
        Tcr_o_Te = (2*divby(gamb+1))*(1 + 0.5*(gamb-1)*Me*Me);
        Ae_o_Acr = powT(Tcr_o_Te, (gamb+1)*divby(2*(gamb-1)))*divby(Me);
        
        /* obtain throat to critical area ratio */
        Ath_o_Acr = Ae_o_Acr*Ath*divby(prm->s_V_Ae_vlv);
        
        /* determine throat Mach no. iteratively; initialize guesses, errors */
        Mth0 = 0.1;      /* subsonic guess values */
        Mth1 = 0.2;

        Tcr_o_Tth0 = (2*divby(gamb+1))*(1 + 0.5*(gamb-1)*Mth0*Mth0);
        Tcr_o_Tth1 = (2*divby(gamb+1))*(1 + 0.5*(gamb-1)*Mth1*Mth1);
        Ath_o_Acr0 = powT(Tcr_o_Tth0, (gamb+1)*divby(2*(gamb-1)))*divby(Mth0);
        Ath_o_Acr1 = powT(Tcr_o_Tth1, (gamb+1)*divby(2*(gamb-1)))*divby(Mth1);
        err0 = Ath_o_Acr - Ath_o_Acr0;
        err1 = Ath_o_Acr - Ath_o_Acr1;
        err = 100;
        count = 0;

        while (fabs(err) > 0.002 && (err0 - err1) != 0 && count < 100){

            /* compute new Mach no. guess */
            Mth = Mth0 - err0*(Mth0 - Mth1)*divby(err0 - err1);
            if (Mth > 1.0)
                Mth = 1.0;

            /* compute error to drive solution towards specified area ratio */
            Tcr_o_Tth_it = (2*divby(gamb+1))*(1 + 0.5*(gamb-1)*Mth*Mth);
            Ath_o_Acr_it = powT(Tcr_o_Tth_it, (gamb+1)*divby(2*(gamb-1)))*divby(Mth);
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
        Tcr_o_T0 = 2*divby(gamb+1);
        Tth_o_T0 = 1*divby(1 + 0.5*(gamb-1)*Mth*Mth);
        if (Tth_o_T0 < Tcr_o_T0)
            Tth_o_T0 = Tcr_o_T0;
        Tth = TtmfpIn*Tth_o_T0;
        Pth = PtmfpIn*powT(Tth_o_T0, gamb*divby(gamb-1));

        /* recompute the actual flow rate, assume no pressure loss */
        rhoth = Pth*144*divby(Rb*JOULES_CONST*Tth);      /* [lb/ft^3] */
        Vth = sqrtT(2*Cpb*C_GRAVITY*JOULES_CONST*(TtmfpIn - Tth));     /* [ft/s] */
        Wth = rhoth*Ath/144*Vth;        /* [lb/s] */
        Mth = Vth*divby(sqrtT(gamb*Rb*C_GRAVITY*JOULES_CONST*Tth));
    }
    else {
        Wth = 0;
        Mth = 0;
    }

    WthOut = Wth;

    Test = Vth;

    /*------Assign output values------------*/
    y[0] = WthOut;      /* Valve throat flow [pps] */
    y[1] = MthOut;      /* Mach no. at throat */
    y[2] = Test;        /* Output Test Point */

}
