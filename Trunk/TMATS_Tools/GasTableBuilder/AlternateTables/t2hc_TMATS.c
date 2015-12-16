/*	T-MATS -- t2hc_TMATS.c */

/* Stream 1 Name: N2, O2, AR, */
/* Stream 1 Species: 0.754700, 0.232000, 0.012800, */
/* Stream 2 Name: CH2, CH, */
/* Stream 2 Species: 0.922189, 0.077811, */

#include <stdio.h>
#include <stdlib.h>
#include "simstruc.h"
#include <math.h>
double t2hc(double Temperature, double FAR)
{
    double tg; 
    int interpErr = 0;
    double zi, yi, xi;
	double inc_X, inc_Y, inc_Z;
	int ii, jj;
	int i, j;
	int Lxy;
	double v00, v10, v01, v11, P;
	double xx, yy, zz;
	
	int errValue = 0;
    /*generate vector length constants */

const int Lx = 18;
const int Ly = 6;

/* X or Temperature axis definition */
const double X[18] = {500,700,900,1100,1300,1500,1700,1900,2100,2300,2500,2700
,2900,3100,3300,3500,3700,3900};

/* Y or fractional axis definition */
const double Y[6] = {0.000000,0.010000,0.020000,0.030000,0.040000,0.050000};

/* M or 2D lookup table of Enthalpy: X,Y */
const double M[108] = {-8.768045,39.382776,88.238046,138.020864,188.945309,241.156804,294.675002,349.340735,404.954538,461.470906,518.877629,577.179389,636.393507,696.549599,757.695690,819.912478,883.336092,948.187872   /* entries 1,1 to 18,1 */
,-4301669.733167,-153.162069,-103.737199,-53.296711,-1.644960,51.353973,105.716032,161.280888,217.844988,275.355350,333.794491,393.167509,453.501947,514.852277,577.311696,641.033613,706.263860,796.381744   /* entries 1,2 to 18,2 */
,-4301669.733167,-345.706914,-295.712444,-244.614286,-192.235231,-138.448890,-83.243107,-26.779625,30.733211,89.233138,148.693350,209.111407,270.511436,332.951964,396.542906,461.475184,528.064970,642.809841   /* entries 1,3 to 18,3 */
,-4301669.733167,-538.251759,-487.687689,-435.931860,-382.825508,-328.251797,-272.202465,-214.840907,-156.380744,-96.894371,-36.419372,25.031987,87.477126,150.974571,215.647086,281.721515,349.588137,488.874571   /* entries 1,4 to 18,4 */
,-4301669.733167,-730.796603,-679.662934,-627.249435,-573.415793,-518.054771,-461.162151,-402.903334,-343.497864,-283.029335,-221.547688,-159.077189,-95.609678,-31.088643,34.622343,101.793890,170.910938,334.767562   /* entries 1,5 to 18,5 */
,-4301669.733167,-923.341448,-871.638179,-818.567010,-764.006093,-707.857868,-650.122422,-590.967775,-530.620505,-469.177131,-406.702355,-343.235361,-278.779757,-213.280438,-146.577565,-78.327659,-7.891776,180.776407   /* entries 1,6 to 18,6 */
};

/* Temperature */
xi = Temperature;
/* FAR */
yi = FAR;

	/*********************************/
	/*check for out of bounds errors */
	/*********************************/

	/* Check to see if zi is outside of X vector */
	if (xi < X[0]){
        errValue = 1;
		xi = X[0];
    }
    else if (xi > X[Lx-1]){
        errValue = 1;
		xi = X[Lx-1];
    }
	else if (!(xi >= X[0]))
	{
		/* xi must be NaN to have failed by < X[0]  and >= X[0] */
		errValue = 1;
		xi = X[0];
	}

	/* Check to see if zi is outside of Y vector */
	if (yi < Y[0]){
        errValue = 1;
		yi = Y[0];
    }
    else if (yi > Y[Ly-1]){
        errValue = 1;
		yi = Y[Ly-1];
    }
	else if (!(yi >= Y[0]))
	{
		/* yi must be NaN to have failed by < Y[0]  and >= Y[0] */
		errValue = 1;
		yi = Y[0];
	}

    /* Search for starting point */
	i = Lx-2;
	j = Ly-2;
	while (i >= 0){
		if (xi >= X[i]){
			ii = i;
			break;
		}
		else
			i = i - 1;
    }

	while (j >= 0){
		if (yi >= Y[j]){
			jj = j;
			break;
		}
		else
			j = j - 1;
    }


	/*-------------------------------------------------------------- */

	Lxy = Lx*Ly;

	/* JL
	Find the values at the vertices of the bounding cube

	Assume M is structured as follows:
	First increment x values with others constant
	Then increment y once, and then increment x values
	When done with y values, finally increment z once
	Repeat until z is done */
	v00 = M[jj*Lx + ii];
	v10 = M[jj*Lx + (ii+1)];
	v01 = M[(jj+1)*Lx + ii];
	v11 = M[(jj+1)*Lx + (ii+1)];

	/* Normalize/translate coords xi,yi so that they go from 0 to 1 within bounding square */
	xx = (xi-X[ii])/(X[ii+1]-X[ii]);
	yy = (yi-Y[jj])/(Y[jj+1]-Y[jj]);
	/* 2D interpolation */
	P = v00*(1-xx)*(1-yy) + v10*xx*(1-yy) + v01*(1-xx)*yy + v11*xx*yy;

        if (errValue == 1)
            printf("Warning in Temperature to Enthalpy lookup, lookup tables should be expanded\n");

    errValue = 0;

return P;
}
