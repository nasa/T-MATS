#ifndef TMATS_FUNCTIONS_H
#define TMATS_FUNCTIONS_H

/*		T-MATS -- functions.h
 * % *************************************************************************
 * % written by Jeffryes Chapman
 * % NASA Glenn Research Center, Cleveland, OH
 * % August 2nd, 2016
 * %
 * %  This is a file that contains various functions for use in the T-MATS C-code
 * % *************************************************************************/

/* functions_TMATS.c */
extern double sqrtT(double A);
extern double divby(double B);
extern double powT(double A, double N);

/* t2hc_TMATS.c */
extern double t2hc(double a, double b);
/* h2tc_TMATS.c */
extern double h2tc(double c, double d);

/* pt2sc_TMATS.c */
extern double pt2sc(double aa, double ba, double ca);
/* sp2tc_TMATS.c */
extern double sp2tc(double da, double ea, double fa);

/* interp1Ac_TMATS.c */
extern double interp1Ac(double a1[], double b1[], double c1, int d1,int *error);
/* interp2Ac_TMATS.c */
extern double interp2Ac(double a2[], double b2[], double c2[], double d2, double e2,int f2, int g2, int *error);
/* interp3Ac_TMATS.c */
extern double interp3Ac(double a3[], double b3[], double c3[], double d3[], double e3, double f3, double g3,int h3, int i3, int j3, int *error);

/* PcalcStat_TMATS.c */
extern void PcalcStat(double A1,double B1,double C1,double D1,double E1,double F1,double *G1,double *H1,double *I1,double *J1,double *K1);

/* functions for PHY valve only */
/*------------------------------------------------- */
/* calc_Pstatic_TMATS.c */
extern double calc_Pstatic(double a1c, double b1c, double c1c, double d1c, double *e1c, double *f1c, double *g1c, double *h1c, double i1c, int j1c, int k1c);
/* calc_WvsMN_TMATS.c */
extern double calc_WvsMN(double a1a, double b1a, double c1a, double d1a, double e1a, double f1a);
/* calc_PsvsMn_TMATS.c */
extern double calc_PsvsMN(double a1b, double b1b, double c1b);
/*-----------------------------------------------*/

#endif  /* TMATS_FUNCTIONS_H */
