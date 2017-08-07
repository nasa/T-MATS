#include "types_TMATS.h"
#include "constants_TMATS.h"
#include "functions_TMATS.h"
#include <math.h>

#ifdef MATLAB_MEX_FILE
#include "simstruc.h"
#endif

void Mixer_TMATS_body(double* y, const double* u, const MixerStruct* prm)
{
    double W1In     = u[0];     /* Input 1 Flow [pps]                     */
    double ht1In    = u[1];     /* Input 1 enthalpy [BTU/lbm]             */
    double Tt1In	= u[2];     /* Temperature Input 1 [degR]             */
    double Pt1In	= u[3];     /* Pressure Input 1 [psia]                */
    double FARc1In  = u[4];     /* Combusted Fuel to Air Ratio 1 [frac]   */
    double W2In     = u[5];     /* Input 1 Flow [pps]                     */
    double ht2In	= u[6];     /* Input 1 enthalpy [BTU/lbm]             */
    double Tt2In	= u[7];     /* Temperature Input 1 [degR]             */
    double Pt2In	= u[8];     /* Pressure Input 1 [psia]                */
    double FARc2In  = u[9];     /* Combusted Fuel to Air Ratio 1 [frac]   */
    
    /*--------Define Constants-------*/
    double WOut, FARcOut, TtOut, Aphyout, PtOut;
    double Test;
    double E1, E2, Eout1, Eout2, A1calc, A2calc, Aoutcalc, Ps1g, Ps2g, Psoutg;
    double Rt1, ht1in, Ps1, S1in, Ts1, hs1, rhos1, V1, Ps1g_new, Ps1g_old, E1_old, MN1g, gammat1g, Ts1g;
    double Rt2, ht2in, Ps2, S2in, Ts2, hs2, rhos2, V2, Ps2g_new, Ps2g_old, E2_old, MN2g, gammat2g, Ts2g;
    double erthr, gammatg, gammasg, TsMNg, PsMNg,PsMNg_new, PsMNg_old, erMN, erMN_old;
    double Rtout, htOut, Psout, Sout, Tsout, hsout, rhosout, Vout, Psoutg_new, Psoutg_old, Eout1_old, Eout2_old, MNoutg, gammatoutg, Tsoutg;
    double E1thr, E2thr, Eout1thr, Eout2thr, Ptoutg, Ptoutg_old, Ptoutg_new, Impulseoutputg, ImpulseMixed, NErr;
    double Aphy1, Aphy2;
    double Psp, Pss, Ttp, Tts, Ptp, Pts, Tsp, Tss, rhosp, rhoss, Vp, Vs, Ap, As, hsp, hss, htp, hts;
    double Ws, Wp, MNg, Sin;
    double FARp, FARs, Rtp, Rts, Rsp, Rss;
    double rhosg, Vg, hsg;
    int iter, iter1, iter2, iter3a, iter3b, maxiter;
    int interpErr = 0;
    
    /* calculate output flow */
    WOut = W1In + W2In;
    /* calculate output fuel to air ratio */
    FARcOut = (FARc1In * W1In + FARc2In *W2In)*divby(WOut);
    
    /* determine gas characteristics */
    /*  Where gas constant is R = f(FAR), but NOT P & T */
    Rt1 = interp1Ac(prm->Y_M_FARVec,prm->T_M_RtArray,FARc1In,prm->A,&interpErr);
    if (interpErr == 1 && *(prm->IWork+Er1)==0){
        #ifdef MATLAB_MEX_FILE
        printf("Warning in %s, Error calculating Rt1. Vector definitions may need to be expanded.\n", prm->BlkNm);
        #endif
        *(prm->IWork+Er1) = 1;
    }
    Rt2 = interp1Ac(prm->Y_M_FARVec,prm->T_M_RtArray,FARc2In,prm->A,&interpErr);
    if (interpErr == 1 && *(prm->IWork+Er2)==0){
        #ifdef MATLAB_MEX_FILE
        printf("Warning in %s, Error calculating Rt2. Vector definitions may need to be expanded.\n", prm->BlkNm);
        #endif
        *(prm->IWork+Er2) = 1;
    }
    Rtout = interp1Ac(prm->Y_M_FARVec,prm->T_M_RtArray,FARcOut,prm->A,&interpErr);
    if (interpErr == 1 && *(prm->IWork+Er3)==0){
        #ifdef MATLAB_MEX_FILE
        printf("Warning in %s, Error calculating Rtout. Vector definitions may need to be expanded.\n", prm->BlkNm);
        #endif
        *(prm->IWork+Er3) = 1;
    }
    
    /* determine calculated enthalpy for input flows */
    ht1in = t2hc(Tt1In,FARc1In);
    ht2in = t2hc(Tt2In,FARc2In);
    /* calculate total enthalpy */
    htOut = (W1In*ht1in + W2In*ht2in)*divby(WOut);
    
    /* calculate total output temperature */
    TtOut = h2tc(htOut, FARcOut);
    
    maxiter = 20;
    if (prm->IDes < 0.5){
        
        /* set primary and secondary definition via switch primary-secondary */
        if (prm->SWPS < 0.5){
            /* primary inflow */
            Wp  = W1In;
            htp = ht1in;
            Ttp = Tt1In;
            Ptp = Pt1In;
            FARp = FARc1In;
            Rtp = Rt1;
            Rsp = Rtp;
            /* secondary inflow */
            Ws  = W2In;
            hts = ht2in;
            Tts = Tt2In;
            Pts = Pt2In;
            FARs = FARc2In;
            Rts = Rt2;}
        else {
            /* primary inflow */
            Wp  = W2In;
            htp = ht2in;
            Ttp = Tt2In;
            Ptp = Pt2In;
            FARp = FARc2In;
            Rtp = Rt2;
            Rsp = Rtp;
            /* secondary inflow*/
            Ws  = W1In;
            hts = ht1in;
            Tts = Tt1In;
            Pts = Pt1In;
            FARs = FARc1In;
            Rts = Rt1;}
        
        /* Determine primary flow Area and Ps */
        MNg = prm->MNp;
        gammatg = interp2Ac(prm->Y_M_FARVec,prm->X_M_TVec,prm->T_M_gammaArray,FARp,Ttp,prm->A,prm->B,&interpErr);
        if (interpErr == 1 && *(prm->IWork+Er4)==0){
            #ifdef MATLAB_MEX_FILE
            printf("Warning in %s, Error calculating gammatg. Vector definitions may need to be expanded.\n", prm->BlkNm);
            #endif
            *(prm->IWork+Er4) = 1;
        }
        TsMNg = Ttp*divby(1+MNg*MNg*(gammatg-1)/2);
        PsMNg = Ptp*powT((TsMNg*divby(Ttp)),(gammatg*divby(gammatg-1)));
        
        PcalcStat(Ptp, PsMNg, Ttp, htp, FARp, Rtp, &Sin, &TsMNg, &hsg, &rhosg, &Vg);
        gammasg = interp2Ac(prm->Y_M_FARVec,prm->X_M_TVec,prm->T_M_gammaArray,FARp,TsMNg,prm->A,prm->B,&interpErr);
        if (interpErr == 1 && *(prm->IWork+Er4)==0){
            #ifdef MATLAB_MEX_FILE
            printf("Warning in %s, Error calculating gammasg. Vector definitions may need to be expanded.\n", prm->BlkNm);
            #endif
            *(prm->IWork+Er4) = 1;
        }
        MNg = Vg*divby(sqrtT(gammasg*Rsp*TsMNg*C_GRAVITY*JOULES_CONST));
        
        erMN =prm->MNp - MNg;
        PsMNg_new = PsMNg + 0.05;
        iter = 0;
        erthr = 0.0001;
        /* if Ps is not close enough to Ps at MN = prm->MNp, iterate to find Ps at MN = prm->MNp */
        while (fabs(erMN) > erthr && iter < maxiter) {
            erMN_old = erMN;
            PsMNg_old = PsMNg;
            if(fabs(PsMNg - PsMNg_new) < 0.03)
                PsMNg = PsMNg + 0.05;
            else
                PsMNg = PsMNg_new;

            PcalcStat(Ptp, PsMNg, Ttp, htp, FARp, Rtp, &Sin, &TsMNg, &hsg, &rhosg, &Vg);
            gammasg = interp2Ac(prm->Y_M_FARVec,prm->X_M_TVec,prm->T_M_gammaArray,FARp,TsMNg,prm->A,prm->B,&interpErr);
            if (interpErr == 1 && *(prm->IWork+Er5)==0){
                #ifdef MATLAB_MEX_FILE
                printf("Warning in %s, Error calculating iteration gammasg. Vector definitions may need to be expanded.\n", prm->BlkNm);
                #endif
                *(prm->IWork+Er5) = 1;
            }
            if (Vg <= 0) {
                Vg = 0.00001;
                if(iter >= maxiter && *(prm->IWork+Er6)==0 ){
                    #ifdef MATLAB_MEX_FILE
                    printf("Warning in %s, Primary flow velocity is zero\n", prm->BlkNm);
                    #endif
                    *(prm->IWork+Er6) = 1;
                }
            }
            MNg = Vg*divby(sqrtT(gammasg*Rsp*TsMNg*C_GRAVITY*JOULES_CONST));
            erMN = prm->MNp - MNg;
            if (fabs(erMN) > erthr) {
                /* determine next guess pressure by secant algorithm */
                PsMNg_new = PsMNg - erMN *(PsMNg - PsMNg_old)*divby(erMN - erMN_old);
            }
            iter = iter + 1;
        }
        if (iter >= maxiter && *(prm->IWork+Er7)==0 ){
            #ifdef MATLAB_MEX_FILE
            printf("Warning in %s, Error calculating Ps at MN = prm->MNp. There may be error in output pressure\n", prm->BlkNm);
            #endif
            *(prm->IWork+Er7) = 1;
        }
        Ap = Wp*divby(Vg * rhosg/C_SINtoSFT);
        Vp = Vg;
        Tsp = TsMNg;
        Psp = PsMNg;
        rhosp = rhosg;
        hsp = hsg;
        /* calculate secondary flow area */
        Pss = Psp;
        PcalcStat(Pts, Pss, Tts, hts, FARs, Rts, &Sin, &TsMNg, &hsg, &rhosg, &Vg);
        if (Vg <= 0) {
            Vg = 0.00001;
            if(iter >= maxiter && *(prm->IWork+Er8)==0 ){
                #ifdef MATLAB_MEX_FILE
                printf("Warning in %s, Secondary flow velocity is zero\n", prm->BlkNm);
                #endif
                *(prm->IWork+Er8) = 1;
            }
        }
        
        MNg = Vg*divby(sqrtT(1.4*Rts*TsMNg*C_GRAVITY*JOULES_CONST));
        
        As = Ws*divby((Vg * rhosg/C_SINtoSFT));
        Vs = Vg;
        Tss = TsMNg;
        Pss = PsMNg;
        rhoss = rhosg;
        hss = hsg;
        
        if (prm->SWPS < 0.5){
            Aphy1 = Ap;
            V1 = Vp;
            Ps1 = Psp;
            Aphy2 = As;
            V2 = Vs;
            Ps2 = Pss;
        }
        else {
            Aphy2 = Ap;
            V2 = Vp;
            Ps2 = Psp;
            Aphy1 = As;
            V1 = Vs;
            Ps1 = Pss;
        }
    }
    else {
        /* Set areas to input parameter values */
        Aphy1 = prm->Aphy1In;
        Aphy2 = prm->Aphy2In;
        /* start iteration to find Ps1in */
        /* guess Ps1 and calculate an initial Area error */
        MN1g = 0.3;
        gammat1g = 1.4;
        Ts1g = Tt1In*divby(1+MN1g*MN1g*(gammat1g-1)/2);
        Ps1g = Pt1In*powT((Ts1g*divby(Tt1In)),(gammat1g*divby(gammat1g-1)));
        PcalcStat(Pt1In, Ps1g, Tt1In, ht1in, FARc1In, Rt1, &S1in, &Ts1, &hs1, &rhos1, &V1);
        A1calc = W1In*divby(V1 * rhos1/C_SINtoSFT);
        E1 = fabs((Aphy1 - A1calc)*divby(Aphy1));
        
        /* iterate to find static pressure, calculated area should be close to actual area */
        iter1 = 0;
        Ps1g_new = Ps1g + 0.05;
        E1thr = 0.001;
        while ( fabs(E1) > E1thr && iter1 < maxiter) {
            E1_old = E1;
            Ps1g_old = Ps1g;
            if(fabs(Ps1g - Ps1g_new) < 0.03) {
                Ps1g = Ps1g + 0.05;
            }
            else {
                Ps1g = Ps1g_new;
            }
            /* calculate flow velocity and rhos */
            PcalcStat(Pt1In, Ps1g, Tt1In, ht1in, FARc1In, Rt1, &S1in, &Ts1, &hs1, &rhos1, &V1);
            
            if (V1 <= 0) {
                V1 = 0.00001;
                if(iter >= maxiter && *(prm->IWork+Er9)==0 ){
                    #ifdef MATLAB_MEX_FILE
                    printf("Warning in %s, Input 1 flow velocity is zero\n", prm->BlkNm);
                    #endif
                    *(prm->IWork+Er9) = 1;
                }
            }
            
            /* calculated Area */
            A1calc = W1In*divby(V1 * rhos1/C_SINtoSFT);
            /*determine error */
            E1 = (Aphy1 - A1calc)*divby(Aphy1);
            if (fabs(E1) > E1thr) {
                /* determine next guess pressure by secant algorithm */
                Ps1g_new = Ps1g - E1 *(Ps1g - Ps1g_old)*divby(E1 - E1_old);
                /* limit algorthim change */
                if (Ps1g_new > 1.1*Ps1g) {
                    Ps1g_new = 1.1 * Ps1g;
                }
                else if (Ps1g_new < 0.9 * Ps1g) {
                    Ps1g_new = 0.9 * Ps1g;
                }
            }
            iter1 = iter1 + 1;
        }
        Ps1 = Ps1g;
        if (iter1 >= maxiter && *(prm->IWork+Er10)==0){
            #ifdef MATLAB_MEX_FILE
            printf("Warning in %s, unable to caluclate Ps1 within allowed iterations, PtOut may contain high error\n", prm->BlkNm);
            #endif
            *(prm->IWork+Er10) = 1;
        }
        /* end Ps1in iteration */
        
        /* start iteration to find Ps2in */
        /* guess Ps2 and calculate an initial Area error */
        MN2g = 0.3;
        gammat2g = 1.4;
        Ts2g = Tt2In*divby(1+MN2g*MN2g*(gammat2g-1)/2);
        Ps2g = Pt2In*powT((Ts2g*divby(Tt2In)),(gammat2g*divby(gammat2g-1)));
        PcalcStat(Pt2In, Ps2g, Tt2In, ht2in, FARc2In, Rt2, &S2in, &Ts2, &hs2, &rhos2, &V2);
        A2calc = W2In*divby(V2 * rhos2/C_SINtoSFT);
        E2 = fabs((Aphy2 - A2calc)*divby(Aphy2));
        
        /* iterate to find static pressure, calculated area should be close to actual area */
        iter2 = 0;
        Ps2g_new = Ps2g + 0.05;
        E2thr = E1thr;
        while ( fabs(E2) > E2thr  && iter2 < maxiter) {
            E2_old = E2;
            Ps2g_old = Ps2g;
            if(fabs(Ps2g - Ps2g_new) < 0.03) {
                Ps2g = Ps2g + 0.05;
            }
            else {
                Ps2g = Ps2g_new;
            }
            /* calculate flow velocity and rhos */
            PcalcStat(Pt2In, Ps2g, Tt2In, ht2in, FARc2In, Rt2, &S2in, &Ts2, &hs2, &rhos2, &V2);
            if (V2 <= 0) {
                V2 = 0.00001;
                if(iter >= maxiter && *(prm->IWork+Er11)==0 ){
                    #ifdef MATLAB_MEX_FILE
                    printf("Warning in %s, Input 2 flow velocity is zero\n", prm->BlkNm);
                    #endif
                    *(prm->IWork+Er11) = 1;
                }
            }
            /* calculated Area */
            A2calc = W2In*divby(V2 * rhos2/C_SINtoSFT);
            /*determine error */
            E2 = (Aphy2 - A2calc)*divby(Aphy2);
            
            if (fabs(E2) > E2thr) {
                /* determine next guess pressure by secant algorithm */
                Ps2g_new = Ps2g - E2 *(Ps2g - Ps2g_old)*divby(E2 - E2_old);
                /* limit algorthim change */
                if (Ps2g_new > 1.1*Ps2g) {
                    Ps2g_new = 1.1 * Ps2g;
                }
                else if (Ps2g_new < 0.9 * Ps2g) {
                    Ps2g_new = 0.9 * Ps2g;
                }
            }
            iter2 = iter2 + 1;
        }
        Ps2 = Ps2g;
        if (iter1 >= maxiter && *(prm->IWork+Er12)==0){
            #ifdef MATLAB_MEX_FILE
            printf("Warning in %s, unable to caluclate Ps2 within allowed iterations, PtOut may contain high error\n", prm->BlkNm);
            #endif
            *(prm->IWork+Er12) = 1;
        }
        /* end Ps2 iteration */
        
        
    } /* end off-Design caulcation of Ps1, Ps2, Aphy1, Aphy2 ,V1, and V2 */
    
    
    if ((Ps1 > Pt1In || Ps2 > Pt2In) && *(prm->IWork+Er13)==0 ){
        #ifdef MATLAB_MEX_FILE
        printf("Warning in %s, Error calculating input static pressures\n", prm->BlkNm);
        #endif
        *(prm->IWork+Er13) = 1;
    }
    
    /* determine area */
    Aphyout = Aphy1 + Aphy2;
    
    /* start iteration to find Psout */
    /* guess a Ptout */
    Ptoutg = (W1In* Pt1In + W2In * Pt2In)*divby(WOut);
    /* guess Psout and calculate an initial Area error */
    MNoutg = 0.3;
    gammatoutg = 1.4;
    Tsoutg = TtOut*divby(1+MNoutg*MNoutg*(gammatoutg-1)/2);
    Psoutg = Ptoutg*powT((Tsoutg*divby(TtOut)),(gammatoutg*divby(gammatoutg-1)));
    PcalcStat(Ptoutg, Psoutg, TtOut, htOut, FARcOut, Rtout, &Sout, &Tsout, &hsout, &rhosout, &Vout);
    Aoutcalc = WOut*divby(Vout * rhosout/C_SINtoSFT);
    /* determine guess error for static pressure iteration */
    Eout2 = fabs((Aphyout - Aoutcalc)*divby(Aphyout));
    
    /* determine guess error for total pressure iteration */
    
    ImpulseMixed = prm->s_M_Imp1*(Ps1*Aphy1 + W1In * V1 * prm->s_M_V1/C_GRAVITY) + prm->s_M_Imp2*(Ps2 * Aphy2 + W2In * V2 * prm->s_M_V2/ C_GRAVITY);
    Impulseoutputg = Psoutg * Aphyout + WOut * Vout/C_GRAVITY;
    Eout1 = fabs((ImpulseMixed - Impulseoutputg)*divby(ImpulseMixed));
    /* determine iteration constants */
    iter3a = 0;
    iter3b = 0;
    Ptoutg_new = Ptoutg + 0.05;
    Psoutg_new = Psoutg + 0.05;
    Eout1thr = 0.001;
    Eout2thr = 0.001;
    /* iterate to find total pressure, calculated from errors in Impulse  */
    while ( (fabs(Eout1) > Eout1thr || fabs(Eout2) > Eout2thr) && iter3a < maxiter) {
        Eout1_old = Eout1;
        Ptoutg_old = Ptoutg;
        if(fabs(Ptoutg - Ptoutg_new) < 0.03) {
            Ptoutg = Ptoutg + 0.05;
        }
        else {
            Ptoutg = Ptoutg_new;
        }
        
        
        /* iterate to find static pressure, calculated area should be close to actual area */
        do{
            Eout2_old = Eout2;
            Psoutg_old = Psoutg;
            if(fabs(Psoutg - Psoutg_new) < 0.03) {
                Psoutg = Psoutg + 0.05;
            }
            else {
                Psoutg = Psoutg_new;
            }
            /* calculate flow velocity and rhos */
            PcalcStat(Ptoutg, Psoutg, TtOut, htOut, FARcOut, Rtout, &Sout, &Tsout, &hsout, &rhosout, &Vout);
            if (Vout <= 0) {
                Vout = 0.00001;
                if(iter >= maxiter && *(prm->IWork+Er14)==0 ){
                    #ifdef MATLAB_MEX_FILE
                    printf("Warning in %s, Output flow velocity is zero\n", prm->BlkNm);
                    #endif
                    *(prm->IWork+Er14) = 1;
                }
            }
            /* calculated Area */
            Aoutcalc = WOut*divby(Vout * rhosout/C_SINtoSFT);
            /*determine error */
            Eout2 = (Aphyout - Aoutcalc)*divby(Aphyout);
            if (fabs(Eout2) > Eout2thr) {
                /* determine next guess pressure by secant algorithm */
                Psoutg_new = Psoutg - Eout2 *(Psoutg - Psoutg_old)*divby(Eout2 - Eout2_old);
                /* limit algorthim change */
                if (Psoutg_new > 1.05*Psoutg) {
                    Psoutg_new = 1.05 * Psoutg;
                }
                else if (Psoutg_new < 0.95 * Psoutg) {
                    Psoutg_new = 0.95 * Psoutg;
                }
            }
            iter3b = iter3b + 1;
        } while ( fabs(Eout2) > Eout2thr && iter3b < maxiter);
        
        /* verify Ps is <= Pt */
        if (Psoutg > Ptoutg){
            Psoutg = Ptoutg;
            if (*(prm->IWork+Er15)==0){
                #ifdef MATLAB_MEX_FILE
                printf ("Warning in %s, Error calculating Psout\n", prm->BlkNm);
                #endif
                *(prm->IWork+Er15) = 1;
            }
        }
        /* end Psout iteration for current Ptout guess */
        
        /* calculate impulse output based on current Pt guess */
        Impulseoutputg = Psoutg * Aphyout + WOut * Vout/C_GRAVITY;
        /* calculate impulse error */
        Eout1 = fabs((ImpulseMixed - Impulseoutputg)*divby(ImpulseMixed));
        if (fabs(Eout1) > Eout1thr) {
            /* determine next guess pressure by secant algorithm */
            Ptoutg_new = Ptoutg - Eout1 *(Ptoutg - Ptoutg_old)*divby(Eout1 - Eout1_old);
            /* limit algorthim change */
            if (Ptoutg_new > 1.1*Ptoutg) {
                Ptoutg_new = 1.1 * Ptoutg;
            }
            else if (Ptoutg_new < 0.9 * Ptoutg) {
                Ptoutg_new = 0.9 * Ptoutg;
            }
        }
        
        iter3a = iter3a + 1;
    }
    /* end PtOut iteration */
    Psout = Psoutg;
    PtOut = Ptoutg;
    
    if (iter3a >= maxiter && *(prm->IWork+Er16)==0){
        #ifdef MATLAB_MEX_FILE
        printf("Error in %s, unable to calculate PtOut\n", prm->BlkNm);
        #endif
        *(prm->IWork+Er16) = 1;
    }
    
    if (iter3b >= maxiter && *(prm->IWork+Er17)==0){
        #ifdef MATLAB_MEX_FILE
        printf("Warning in %s, unable to caluclate PsOut, PtOut may contain high error\n", prm->BlkNm);
        #endif
        *(prm->IWork+Er17) = 1;
    }
    
    /* Compute normalized error */
    if (prm->IDes < 0.5)
        NErr = (W1In*divby(W2In) -prm->BPRdes)*divby(W1In*divby(W2In));
    else
        NErr = (Ps1 - Ps2)*divby(Ps1);
    
    /*------Assign output values------------*/
    y[0] = WOut;      /* Output Air Flow [pps]						*/
    y[1] = htOut; 	  /* Output Enthalpy [BTU/lbm] 					*/
    y[2] = TtOut;     /* Output Temperature  [degR]					*/
    y[3] = PtOut;     /* Output Pressure [psia]						*/
    y[4] = FARcOut;   /* Output Combusted Fuel to Air Ratio [frac] 	*/
    y[5] = NErr;      /* Normalized Static Pressure Error           */
    y[6] = Aphy1;     /* Area of input 1 [in^2]                     */
    y[7] = Aphy2;     /* Area of input 2 [in^2]                     */
    
}
