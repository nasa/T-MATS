/*       /NASA/C code/sp2tc.c                  		       
*														   
* Computes temperature as function of entropy & pressure  
*														   
* 	    D K Frederick & Javad Sanati
*---------------------------------------------------------
*   Using option 3 of therm.f as basis for Matlab code    
*---------------------------------------------------------
*/

#include <stdio.h>
#include <math.h>

double sp2tc(double S, double P, double fa)
{
	/*--------Define Arrays----------*/
	double ITAB[60] = {1,1,1,2,3,3,4,4,4,5,5,5,5,6,6,6,6,6,7,7,7,7,7,7,
		 8,8,8,8,8,8,9,9,9,9,9,9,10,10,10,10,10,10,10,10,
		 11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11};
	double TTTAB[11] = {300,400,500,700,1000,1400,1900,2500,3100,3700,4500};
	double APAIR[11] = {4.229854000000e1,4.429218000000e1,4.584067000000e1,
         4.818303000000e1,5.070949000000e1,5.318950000000e1,
         5.556056000000e1,5.779398000000e1,5.960312000000e1,
         6.112402000000e1,6.283719000000e1};
	double BPAIR[11] = {2.305525000000e-2,1.732687898835e-2,1.390113404660e-2,
         9.984237743689e-3,7.194810211645e-3,5.398110354070e-3,
         4.191444392129e-3,3.318644249749e-3,2.746778608874e-3,
         2.344441314755e-3,1.960623219312e-3};
	double CPAIR[11] = {-3.628178988352e-5,-2.100192023298e-5,
         -1.325552918449e-5,-6.328952330080e-6,-2.969139443401e-6,
         -1.522610200538e-6,-8.907217233446e-7,-5.639451806208e-7,
         -3.891642208381e-7,-2.813979360273e-7,-1.983746832756e-7};
	double DPAIR[11] = {5.093289883514e-8,2.582130349498e-8,1.154429475734e-8,
         3.733125429643e-9,1.205441035719e-9,4.212589847953e-10,
         1.815425237354e-10,9.710053321261e-11,5.987015822823e-11,
         3.459302197988e-11,2.445751324881e-11};
	double APSTOC[11] = {4.208565000000e1,4.414325000000e1,4.576019000000e1,
         4.824312000000e1,5.096532000000e1,5.367114000000e1,
         5.628635000000e1,5.877705000000e1,6.081328000000e1,
         6.253552000000e1,6.448296000000e1};
	double BPSTOC[11] = {2.361110000000e-2,1.800764547338e-2,1.459451810646e-2,
         1.067795041447e-2,7.806395767972e-3,5.922535863503e-3,
         4.648916404273e-3,3.720736784998e-3,3.102786455737e-3,
         2.660467392055e-3,2.231936896092e-3};
	double CPSTOC[11] = {-3.50184547338e-5,-2.10160905323e-5,-1.31151831369e-5,
         -6.467655323031e-6,-3.104193498621e-6,-1.605456262554e-6,
         -9.417826559062e-7,-6.051833762187e-7,-4.247338392160e-7,
         -3.124646002534e-7,-2.231985197004e-7};
	double DPSTOC[11] = {4.667454733830e-8,2.633635798468e-8,1.107921302316e-8,
         3.737179804900e-9,1.248947696722e-9,4.424490710984e-10,
         1.869995998264e-10,1.002497427793e-10,6.237179942365e-11,
         3.719420023041e-11,2.698623281668e-11};

	double zmea, zmsp, tmlsr, zmwtr, tmls, rcas, Stol, Sg, temp, 
                    phiea, dl, phisp, phig, Sg1, Tg1, dTdS, T, Tg;
	int jj, Jmax, Tindex, Tindex2, it;

	if (fa == 0){
		zmea = 4.7642;	/* moles of unburned air */
		zmsp = 0;		/* moles of oxygen */
		tmlsr = 0.2098988288;
		zmwtr = 0.0345194683;
	}
	else{
		zmea = 4.7642-fa*69.69056873;        /* eqn 35 */
		zmsp = fa*74.411931335;              /* eqn 38 */
		tmls = 4.7642+fa*4.721362582;        /* eqn 39 */
		zmwtr = tmls/(138.0148721*(1+fa)); /* eqn 42 */
		tmlsr = 1/tmls;
	}
	rcas = 1.98587*zmwtr;

	/*---- guess starting temperature ----------------*/
	Tg = 1000;
	Jmax = 10;
	Stol = 1e-4;
	jj = 0;
	Sg = 1e3;

    /*Don Simon- Added logic to ensure that at least two iterations occur*/
    /* while (fabs(S - Sg) >= Stol && jj < Jmax){ */
    while ((fabs(S - Sg) >= Stol && jj < Jmax) || (jj < 2)){

		/*--- temperature index is integer part of temp/100 ----*/

		temp = 0.01 * (Tg - fmod(Tg,100));
		if (temp > 1)
			Tindex2 = temp;
		else
			Tindex2 = 1;
		if (Tindex2 < 60)
			Tindex = Tindex2;
		else
			Tindex = 60;

		it = ITAB[Tindex - 1] - 1;
		dl = Tg - TTTAB[it];
		phiea =((DPAIR[it]*dl + CPAIR[it])*dl + BPAIR[it])*dl + APAIR[it];
		if (fa > 0)
			phisp = ((DPSTOC[it]*dl + CPSTOC[it])*dl + BPSTOC[it])*dl 
                                    + APSTOC[it];
		else
			phisp = 0;

		phig =(phisp*zmsp + phiea*zmea)*tmlsr;

		/*----- calc entropy so can use it to adjust temp value ---*/
		Sg = (phig*0.5035576347-23.0258509)*rcas - 0.1841304 
                            - rcas * log(P/14.696);

		/*---- iterate temperature until calculated entropy ... */
		/*                       agrees with specified value ---*/
		if (jj==0){
			Sg1 = Sg;
			Tg1 = Tg;
			Tg = Tg1 + 50;
		}
		else{
			dTdS = (Tg - Tg1)/(Sg - Sg1);
			Tg1 = Tg;
			Sg1 = Sg;
			Tg = Tg1 + (S - Sg1)*dTdS;
		}
		++jj;
      /*  printf("values of Sg & Tg are %f %f\n",Sg,Tg);  */
	}
	T = Tg;
	return T;
}