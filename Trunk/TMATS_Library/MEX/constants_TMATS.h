#ifndef TMATS_CONSTANTS_H
#define TMATS_CONSTANTS_H

/*		T-MATS -- constants.h
% *************************************************************************
% written by Jeffryes Chapman bsed on work by Ryan May
% NASA Glenn Research Center, Cleveland, OH
% February 14th, 2013
%
%  This is a file that contains various constants for use in the T-MATS C-code
% *************************************************************************/

/* physical constants */
#define C_PSTD 				    14.695951   /* psia */
#define C_TSTD 				    518.67      /* degR */
#define C_BTU_PER_SECtoHP 		1.414853	/* hp*sec/BTU */
#define C_HP_PER_RPMtoFT_LBF 	5252.113
#define C_BTUtoFT_LBF 			778.169232
#define C_GRAVITY   			32.174049	/* (ft*lbm) / (lbf*sec^2) */
#define JOULES_CONST			778.169233	/* ft*lbf/BTU */
#define C_PSItoPSF              144         /* PSI to PSF */
#define C_SINtoSFT              144         /* in^2 to ft^2 */

/*  other constants */
#define C_GAMMA 				 1.40		/* Generic gamma value */

#endif  /* TMATS_CONSTANTS_H */
