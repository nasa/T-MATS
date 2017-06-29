
#include "constants_TMATS.h"
#include "types_TMATS.h"
#include "functions_TMATS.h"
#include <math.h>

void Nozzle_TMATS_body(double* y, const double* u, NozzleStruct* prm)
{
    double WIn       = u[0];     /* Input Flow [pps] 	*/
    double htIn      = u[1];     /* enthaply [BTU/lbm] 	*/
    double TtIn      = u[2];     /* Temperature Input [degR] 	*/
    double PtIn      = u[3];     /* Pressure Input [psia] 	*/
    double FARcIn    = u[4];     /* Combusted Fuel to Air Ratio [frac] 	*/
    double PambIn    = u[5];     /* Ambient Pressure [psia] 	*/
    double AthroatIn = u[6];     /* Throat area [in2] 	*/
    double AexitIn   = u[7];     /* Exit area [in2] 	*/
    
    /*--------Define Constants-------*/
    double choked, Ts, rhos, V, Test, MN1, Ptin;
    double CdTh, Cv, Cfg, Therm_growth, PQPa, PQPaMap, AthroatHot;
    double Rt, TsMN1, PsMN1, Woutcalc;
    double WOut, FgOut, NErrorOut, Ath, Vth, Psth, Ax, Vx, Psx, Tsx, gammasx, MNx;
    double gammatg, gammasth,gammasMN1, gammasg, MNg, TsMNg, PsMNg, PsMNg_new, PsMNg_old, VMN1;
    double MNth, Tsth, rhosth, rhosMN1, rhosx;
    double Axcalc, Psxg, Psxg_new, Psxg_old, Exthr;
    double Athcalc, Psthg, Psthg_new, Psthg_old;
    double Sin, hsg, hs, htin, rhosg, Rs, Vg;
    double gammas_s, MN_s, V_s, rhos_s, Ts_s;
    double Ex, Ex_old;
    double erMN_old, erMN, erthr;
    int maxiter, iter, maxiterx, iterx, CDNoz;
    int interpErr = 0;
    
    /* Determine Nozzle Type                  */
    /*SwitchType:                             */
    /*        1: Convergent Nozzle            */
    /*        2: Convergent-Divergent Nozzle  */
    if (prm->SwitchType < 1.5)
        CDNoz = 0;
    else
        CDNoz = 1;
    
    /* Calc entropy */
    Sin = pt2sc(PtIn, TtIn, FARcIn);
    
    /*-- Compute Input enthalpy --------*/
    
    htin = t2hc(TtIn,FARcIn);
    
    /*  Where gas constant is R = f(FAR), but NOT P & T */
    Rt = interp1Ac(prm->Y_N_FARVec,prm->T_N_RtArray,FARcIn,prm->A,&interpErr);
    Rs = Rt;
    
    /* Warn if there is the potential for back flow */
    Ptin = PtIn;
    if (Ptin <= PambIn) {
        Ptin = PambIn + 0.1;
    }
    /* Determine ideal velocity defined by perfect expansion to Pambient */
    PcalcStat(Ptin, PambIn, TtIn, htin, FARcIn, Rt, &Sin, &Ts, &hs, &rhos, &V);
    gammas_s = interp2Ac(prm->Y_N_FARVec,prm->X_N_TtVec,prm->T_N_MAP_gammaArray,FARcIn,Ts,prm->A,prm->B,&interpErr);
    MN_s = V*divby(sqrtT(gammas_s*Rs*Ts*C_GRAVITY*JOULES_CONST));
    Ts_s = Ts;
    V_s = V;
    rhos_s = rhos;
    
    
    /* Determine if nozzle throat is choked by comparing pressure when MN = 1 to ambient pressure
     * ---- set MN = 1 and calc throat Ps for iteration IC --------*/
    MNg = 1;
    gammatg = interp2Ac(prm->Y_N_FARVec,prm->X_N_TtVec,prm->T_N_MAP_gammaArray,FARcIn,TtIn,prm->A,prm->B,&interpErr);
    /* use isentropic equations for a first cut guess */
    TsMNg = TtIn*divby(1+MNg*MNg*(gammatg-1)/2);
    PsMNg = Ptin*powT((TsMNg*divby(TtIn)),(gammatg*divby(gammatg-1)));
    
    /* Calculate velcocity and MN using guessed static pressure */
    PcalcStat(Ptin, PsMNg, TtIn, htin, FARcIn, Rt, &Sin, &TsMNg, &hsg, &rhosg, &Vg);
    gammasg = interp2Ac(prm->Y_N_FARVec,prm->X_N_TtVec,prm->T_N_MAP_gammaArray,FARcIn,TsMNg,prm->A,prm->B,&interpErr);
    MNg = Vg*divby(sqrtT(gammasg*Rs*TsMNg*C_GRAVITY*JOULES_CONST));
    
    /* determine error based on calculated MN and 1 */
    erMN =1 - MNg;
    
    PsMNg_new = PsMNg + 0.05;
    maxiter = 200;
    iter = 0;
    erthr = 0.001;
    
    /* if Ps is not close enough to Ps at MN = 1, iterate to find Ps at MN = 1 */
    while (fabs(erMN) > erthr && iter < maxiter) {
        erMN_old = erMN;
        PsMNg_old = PsMNg;
        if(fabs(PsMNg - PsMNg_new) < 0.003)
            PsMNg = PsMNg + 0.005;
        else
            PsMNg = PsMNg_new;
        PcalcStat(Ptin, PsMNg, TtIn, htin, FARcIn, Rt, &Sin, &TsMNg, &hsg, &rhosg, &Vg);
        gammasg = interp2Ac(prm->Y_N_FARVec,prm->X_N_TtVec,prm->T_N_MAP_gammaArray,FARcIn,TsMNg,prm->A,prm->B,&interpErr);
        MNg = Vg*divby(sqrtT(gammasg*Rs*TsMNg*C_GRAVITY*JOULES_CONST));
        erMN =1 - MNg;
        if (fabs(erMN) > erthr) {
            /* determine next guess pressure by secant algorithm */
            PsMNg_new = PsMNg - erMN *(PsMNg - PsMNg_old)*divby(erMN - erMN_old);
        }
        iter = iter + 1;
    }
    /*  MN = 1 parameters */
    TsMN1 = TsMNg;
    PsMN1 = PsMNg;
    rhosMN1 = rhosg;
    gammasMN1 = gammasg;
    VMN1 = Vg;
    
    /* Determine if Nozzle is choked  */
    if (PsMN1<PambIn)
        choked = 0;
    else {
        choked = 1;
    }
    
    /* throat not choked, set Psth to ideal expansion to Pambient values */
    if (choked ==0){
        Psth = PambIn;
        Tsth = Ts_s;
        Vth = V_s;
        rhosth = rhos_s;
        MNth = MN_s;
    }
    
    else{  /* If nozzle is choked, determine throat parameters based on MN = 1 values */
        Psth = PsMN1;
        Tsth = TsMN1;
        MNth = 1;
        gammasth = interp2Ac(prm->Y_N_FARVec,prm->X_N_TtVec,prm->T_N_MAP_gammaArray,FARcIn,Tsth,prm->A,prm->B,&interpErr);
        Vth = MNth*sqrtT(gammasth*Rs*Tsth*C_GRAVITY*JOULES_CONST);
        rhosth = rhosMN1;
    }

    /* Pressure before nozzle/P ambient */
    PQPa = Ptin*divby(PambIn);
    
    /* cacluate Thermal Constants */
    PQPaMap = PQPa;
    
    /* look up Flow Coefficient */
    CdTh = interp1Ac(prm->X_N_PEQPaVec,prm->T_N_CdThArray,PQPaMap,prm->B1,&interpErr);
    Therm_growth = interp1Ac(prm->X_N_TtVecTG,prm->T_N_TGArray,TtIn,prm->C,&interpErr);
    
    
    /* Determine throat area in^2 */
    if (prm->IDes < 0.5) {
        Ath = WIn * C_PSItoPSF*divby(Therm_growth *(1-prm->flowLoss/100)*CdTh*rhosth*Vth);
    }
    /* if the thoat area is larger then the exit area of a CD nozzle it is a convergent nozzle */
    else if (CDNoz == 1 && AthroatIn > AexitIn) {
        Ath = AexitIn;
        CDNoz = 0;
    }
    else
        Ath = AthroatIn;
    
    
    /* Calculate Flow out of nozzle */
    /* Calculated flow is always determined based on the throat velocity
     * defined at MN = 1 when choked or at the throat area when unchoked */
    AthroatHot = Ath * Therm_growth;
    Woutcalc = (1-prm->flowLoss/100)*AthroatHot*CdTh*rhosth*Vth/C_PSItoPSF;
    
    /* Initialize variables, will only be used with CD nozzle */
    /* Psx will be used as guess to calculate Area error */
    Psxg = PambIn;
    Psx = Psth;
    Tsx = Tsth;
    MNx = MNth;
    Vx = Vth;
    Ax = Ath;
    
    /* if the nozzle is choked and convergent-divergent calculate values after the nozzle throat */
    if (CDNoz == 1 && choked == 1) {
        
        /* Generate exit area */
        if (prm->IDes < 0.5)
            Ax = WIn * C_PSItoPSF*divby(Therm_growth *(1-prm->flowLoss/100)*CdTh*rhos_s*V_s);
        else /* Use calculated area value when using Cv method  */
            Ax = AexitIn;
        
        /* start iteration to find Psx */
        Psxg = PambIn;
        PcalcStat(Ptin, Psxg, TtIn, htin, FARcIn, Rt, &Sin, &Ts, &hs, &rhos, &V);
        Axcalc = WIn*divby(V * rhos/C_SINtoSFT); /* Will not be used for the Cfg method */
        
        Ex = fabs((Ax - Axcalc)*divby(Ax));
        /* iterate to find static pressure, calculated area should be close to actual area */
        maxiterx = 200;
        iterx = 0;
        Psxg_new = Psxg + 0.05;
        Exthr = 0.0001;
        while ( fabs(Ex) > Exthr && iterx < maxiter) {
            Ex_old = Ex;
            Psxg_old = Psxg;
            if (fabs(Psxg - Psxg_new) < 0.0003)
                Psxg = Psxg + 0.0005;
            else
                Psxg = Psxg_new;
            
            /* calculate flow velocity and rhos */
            PcalcStat(Ptin, Psxg, TtIn, htin, FARcIn, Rt, &Sin, &Ts, &hs, &rhos, &V);
            /* calculated Area */
            Axcalc = WIn*divby(V * rhos/C_SINtoSFT);
            /*determine error */
            Ex = (Ax - Axcalc)*divby(Ax);
            if (fabs(Ex) > Exthr) {
                /* determine next guess pressure by secant algorithm */
                Psxg_new = Psxg - Ex *(Psxg - Psxg_old)*divby(Ex - Ex_old);
                /* limit algorthim change */
                if (Psxg_new > 1.1*Psxg) {
                    Psxg_new = 1.1 * Psxg;
                }
                else if (Psxg_new < 0.9 * Psxg) {
                    Psxg_new = 0.9 * Psxg;
                }
            }
            iterx = iterx + 1;
        }
        /* Collect data from the expansion to exit area: */
        Tsx = Ts;
        Vx = V;
        Psx = Psxg;
        rhosx = rhos;
        gammasx = interp2Ac(prm->Y_N_FARVec,prm->X_N_TtVec,prm->T_N_MAP_gammaArray,FARcIn,Ts,prm->A,prm->B,&interpErr);
        MNx = Vx*divby(sqrtT(gammasx*Rs*Tsx*C_GRAVITY*JOULES_CONST));
    }
    
    WOut = WIn;
    Test = Psx;
    
    /* look up Thrust and velocity coefficients */
    if (prm->CfgEn < 0.5){
        Cv = interp1Ac(prm->X_N_PEQPaVec,prm->T_N_CvArray,PQPaMap,prm->B1,&interpErr);
        Cfg = 1;
    }
    else {
        Cfg = interp1Ac(prm->X_N_PEQPaVec,prm->T_N_CfgArray,PQPaMap,prm->B1,&interpErr);
        Cv = 1;
    }
    
    /*----- calc gross thrust -----------*/
    if (CDNoz ==1){
        if (prm->CfgEn > 0.5)
            FgOut = (WOut/C_GRAVITY)*V_s*Cfg;
        else
            FgOut = (WOut/C_GRAVITY)*Vx*Cv + (Psx-PambIn)*Ax;
    }
    else {
        if (prm->CfgEn > 0.5)
            FgOut = (WOut/C_GRAVITY)*V_s*Cfg;
        else
            FgOut = (WOut/C_GRAVITY)*Vth*Cv + (Psth-PambIn)*Ath;
    }
    /* ----- Compute Normalized Flow Error ----- */
    if (prm->IDes < 0.5 && prm->WDes == 0)
        NErrorOut = 100;
    else if (prm->IDes < 0.5)
        NErrorOut = (WIn - prm->WDes)*divby(prm->WDes);
    else if (WIn == 0) {
        NErrorOut = 100;}
    else {
        NErrorOut = (WIn-Woutcalc)*divby(WIn);
    }
    /*------Assign output values------------*/
    y[0] = WOut;          /* Outlet Total Flow [pps]	*/
    y[1] = FgOut;         /* Gross Thrust [lbf] */
    y[2] = NErrorOut;     /* Normalized Flow Error [frac] */
    y[3] = Ath;           /* Throat Area [in^2] */
    y[4] = Ax;            /* Exit Area [in^2] */
    y[5] = Psth;          /* Throat static pressure [psi] */
    y[6] = Tsth;          /* Throat static temperature [R] */
    y[7] = MNth;          /* Throat Mach Number */
    y[8] = Vth;           /* Throat velocity */
    y[9] = Psx;           /* Exit static pressure [psi]*/
    y[10] = Tsx;          /* Exit static temperature [R]*/
    y[11] = MNx;          /* Exit Mach Number */
    y[12] = Vx;           /* Exit velocity */
    y[13] = Woutcalc;     /* Calculated mass flow [pps] */
    y[14] = choked ;      /* nozzle is choked at the throat */
    y[15] = V_s;          /* ideal velocity expanded to ambient */
    y[16] = Test;
    
}
