#include "types_TMATS.h"
#include "constants_TMATS.h"
#include "functions_TMATS.h"
#include <math.h>

#ifdef MATLAB_MEX_FILE
#include "simstruc.h"
#endif

void StaticCalc_TMATS_body(double *y, const double *u, const StaticCalcStruct* prm)
{
    double WIn      = u[0];     /* Input Flow [pps] 	*/
    double htIn     = u[1];     /* enthaply [BTU/lbm] 	*/
    double TtIn     = u[2];     /* Temperature Input [degR] 	*/
    double PtIn     = u[3];     /* Pressure Input [psia] 	*/
    double FARcIn   = u[4];     /* Combusted Fuel to Air Ratio [frac] 	*/
    
    /*--------Define Constants-------*/
    double PsOut, TsOut, rhosOut, MNOut, AthOut;
    double Sin, htin;
    double Rt, Rs;
    double TsMNg, PsMNg, MNg;
    double Tsg, Psg, Psg_new, Psg_old, Acalc, erA, erA_old;
    double  gammatg, gammasg, hsg, rhosg, Vg;
    double erMN_old, erMN, PsMNg_old, PsMNg_new;
    double erthr;
    int maxiter, iter;
    int interpErr = 0;
    
        
    /* Calc entropy */
    Sin = pt2sc(PtIn, TtIn, FARcIn);
    
    /*-- Compute Input enthalpy --------*/
    
    htin = t2hc(TtIn,FARcIn);
    
    /*  Where gas constant is R = f(FAR), but NOT P & T */
    Rt = interp1Ac(prm->X_FARVec,prm->T_RtArray,FARcIn,prm->A,&interpErr);
    if (interpErr == 1 && *(prm->IWork+Er1)==0){
        #ifdef MATLAB_MEX_FILE
        printf("Warning in %s, Error calculating Rt. Vector definitions may need to be expanded.\n", prm->BlkNm);
        #endif
        *(prm->IWork+Er1) = 1;
    }
    Rs = Rt;
    
    /* Solve for Ts and Ps when MN is known*/
    if (prm->SolveType == 1) {
        /*---- set MN = prm->MNIn and calc SS Ps for iteration IC --------*/
        MNg = prm->MNIn;
        gammatg = interp2Ac(prm->X_FARVec,prm->Y_TtVec,prm->T_gammaArray,FARcIn,TtIn,prm->A,prm->B,&interpErr);
        if (interpErr == 1 && *(prm->IWork+Er2)==0){
            #ifdef MATLAB_MEX_FILE
            printf("Warning in %s, Error calculating gammatg. Vector definitions may need to be expanded.\n", prm->BlkNm);
            #endif
            *(prm->IWork+Er2) = 1;
        }
        TsMNg = TtIn*divby(1+MNg*MNg*(gammatg-1)/2);
        PsMNg = PtIn*powT((TsMNg*divby(TtIn)),(gammatg*divby(gammatg-1)));
        
        PcalcStat(PtIn, PsMNg, TtIn, htin, FARcIn, Rt, &Sin, &TsMNg, &hsg, &rhosg, &Vg);
        gammasg = interp2Ac(prm->X_FARVec,prm->Y_TtVec,prm->T_gammaArray,FARcIn,TsMNg,prm->A,prm->B,&interpErr);
        if (interpErr == 1 && *(prm->IWork+Er2)==0){
            #ifdef MATLAB_MEX_FILE
            printf("Warning in %s, Error calculating gammasg. Vector definitions may need to be expanded.\n", prm->BlkNm);
            #endif
            *(prm->IWork+Er2) = 1;
        }
        MNg = Vg*divby(sqrtT(gammasg*Rs*TsMNg*C_GRAVITY*JOULES_CONST));
        
        if (Vg > 0.0001){
            Acalc = WIn*divby(Vg * rhosg/C_SINtoSFT);}
        else {
            Acalc = 999; /* if velocity is close to zero assume a very large Ath */}
        
        erMN = prm->MNIn - MNg;
        PsMNg_new = PsMNg + 0.05;
        maxiter = 15;
        iter = 0;
        erthr = 0.0001;
        
        /* if Ps is not close enough to Ps at MN = prm->MNIn, iterate to find Ps at MN = prm->MNIn */
        while (fabs(erMN) > erthr && iter < maxiter) {
            erMN_old = erMN;
            PsMNg_old = PsMNg;
            if(fabs(PsMNg - PsMNg_new) < 0.003)
                PsMNg = PsMNg + 0.005;
            else
                PsMNg = PsMNg_new;
            PcalcStat(PtIn, PsMNg, TtIn, htin, FARcIn, Rt, &Sin, &TsMNg, &hsg, &rhosg, &Vg);
            gammasg = interp2Ac(prm->X_FARVec,prm->Y_TtVec,prm->T_gammaArray,FARcIn,TsMNg,prm->A,prm->B,&interpErr);
            if (interpErr == 1 && *(prm->IWork+Er2)==0){
                #ifdef MATLAB_MEX_FILE
                printf("Warning in %s, Error calculating iteration gammasg. Vector definitions may need to be expanded.\n", prm->BlkNm);
                #endif
                *(prm->IWork+Er2) = 1;
            }
            MNg = Vg*divby(sqrtT(gammasg*Rs*TsMNg*C_GRAVITY*JOULES_CONST));
            /* calculated Area */
            if (Vg > 0.0001){
                Acalc = WIn*divby(Vg * rhosg/C_SINtoSFT);}
            else {
                Acalc = 999; /* if velocity is close to zero assume a very large Ath */}
            
            erMN = prm->MNIn - MNg;
            if (fabs(erMN) > erthr) {
                /* determine next guess pressure by secant algorithm */
                PsMNg_new = PsMNg - erMN *(PsMNg - PsMNg_old)*divby(erMN - erMN_old);
            }
            iter = iter + 1;
        }
        if (iter == maxiter && *(prm->IWork+Er3)==0 ){
            #ifdef MATLAB_MEX_FILE
            printf("Warning in %s, Error calculating Ps at MN = prm->MNIn. There may be error in block outputs\n", prm->BlkNm);
            #endif
            *(prm->IWork+Er3) = 1;
        }
        TsOut = TsMNg;
        PsOut = PsMNg;
        rhosOut = rhosg;
        MNOut = prm->MNIn;
        AthOut = Acalc;
    }
    /* Solve for Ts and Ps when Ath is known*/
    else if (prm->SolveType == 0) {
        
        /* guess Psout and calculate an initial Area error */
        MNg = prm->MNIn;
        gammatg = 1.4;
        Tsg = TtIn*divby(1+MNg*MNg*(gammatg-1)/2);
        Psg = PtIn*powT((Tsg*divby(TtIn)),(gammatg*divby(gammatg-1)));
        PcalcStat(PtIn, Psg, TtIn, htin, FARcIn, Rt, &Sin, &Tsg, &hsg, &rhosg, &Vg);
        Acalc = WIn*divby(Vg * rhosg/C_SINtoSFT);
        gammasg = interp2Ac(prm->X_FARVec,prm->Y_TtVec,prm->T_gammaArray,FARcIn,Tsg,prm->A,prm->B,&interpErr);
        if (interpErr == 1 && *(prm->IWork+Er4)==0){
            #ifdef MATLAB_MEX_FILE
            printf("Warning in %s, Error calculating iteration gammasg. Vector definitions may need to be expanded.\n", prm->BlkNm);
            #endif
            *(prm->IWork+Er4) = 1;
        }
        MNg = Vg*divby(sqrtT(gammasg*Rs*Tsg*C_GRAVITY*JOULES_CONST));
        
        /* determine guess error for static pressure iteration */
        erA = (prm->AthroatIn - Acalc)*divby(prm->AthroatIn);
        
        /* determine iteration constants */
        iter = 0;
        maxiter = 1000;
        Psg_new = Psg + 0.05;
        erthr = 0.0001;
        
        while ( fabs(erA) > erthr && iter < maxiter){
            erA_old = erA;
            Psg_old = Psg;
            if(fabs(Psg - Psg_new) < 0.0003) {
                Psg = Psg + 0.0005;
            }
            else {
                Psg = Psg_new;
            }
            /* calculate flow velocity and rhos */
            PcalcStat(PtIn, Psg, TtIn, htin, FARcIn, Rt, &Sin, &Tsg, &hsg, &rhosg, &Vg);
            
            gammasg = interp2Ac(prm->X_FARVec,prm->Y_TtVec,prm->T_gammaArray,FARcIn,Tsg,prm->A,prm->B,&interpErr);
            if (interpErr == 1 && *(prm->IWork+Er4)==0){
                #ifdef MATLAB_MEX_FILE
                printf("Warning in %s, Error calculating iteration gammasg. Vector definitions may need to be expanded.\n", prm->BlkNm);
                #endif
                *(prm->IWork+Er4) = 1;
            }
            
            MNg = Vg*divby(sqrtT(gammasg*Rs*Tsg*C_GRAVITY*JOULES_CONST));
            
            if (Vg > 0.0001) {
                /* calculated Area */
                Acalc = WIn*divby(Vg * rhosg/C_SINtoSFT);
                /*determine error */
                erA = (prm->AthroatIn - Acalc)*divby(prm->AthroatIn);
            }
            else {
                erA = 0;
                Psg = PtIn;
                Tsg = TtIn;
                Acalc = 999;
            }
            if (fabs(erA) > erthr) {
                /* determine next guess pressure by secant algorithm */
                Psg_new = Psg - erA *(Psg - Psg_old)*divby(erA - erA_old);
                /* limit algorthim change */
                if (Psg_new > 1.001*Psg) {
                    Psg_new = 1.002 * Psg;
                }
                else if (Psg_new < 0.999 * Psg) {
                    Psg_new = 0.998 * Psg;
                }
            }
            iter = iter + 1;
        }
        TsOut = Tsg;
        PsOut = Psg;
        rhosOut = rhosg;
        MNOut = MNg;
        AthOut = Acalc;
    }
    else {
        if (*(prm->IWork+Er5)==0 ){
            #ifdef MATLAB_MEX_FILE
            printf("Warning in %s, SolveType_M is not valid. There may be error in block outputs\n", prm->BlkNm);
            #endif
            *(prm->IWork+Er5) = 1;
        }
        TsOut = TtIn;
        PsOut = PtIn;
        rhosOut = 1;
        MNOut = 0;
        AthOut = 100;
    }
    
    
    /*------Assign output values------------*/
    y[0] = TsOut;      /* static Temperature [degR] */
    y[1] = PsOut;      /* static Pressure [psia] */
    y[2] = rhosOut;    /* static rho [lbm/ft3]*/
    y[3] = MNOut;      /* mach number [frac]*/
    y[4] = AthOut;     /* throat area [in^2] */
    
}

