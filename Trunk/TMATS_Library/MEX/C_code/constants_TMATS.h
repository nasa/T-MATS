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

/* configuration constants */
const int PRINT_WARNING = 1;   /* 0 = do not print interp warnings, 1=print interp warnings */

/* physical constants */
const double C_PSTD 				= 14.695951;	/* psia */
const double C_TSTD 				= 518.67;		/* degR */
const double C_BTU_PER_SECtoHP 		= 1.414853;		/* hp*sec/BTU */
const double C_HP_PER_RPMtoFT_LBF 	= 5252.113;
const double C_BTUtoFT_LBF 			= 778.169232;
const double C_GRAVITY   			= 32.174049;	/* (ft*lbm) / (lbf*sec^2) */
const double JOULES_CONST			= 778.169233;	/* ft*lbf/BTU */
const double C_HPtoFT_LBF_PER_SEC	= 550.0;
const double C_PSItoPSF             = 144;          /* PSI to PSF */
const double C_SINtoSFT             = 144;          /* in^2 to ft^2 */

/*  other constants */
const double C_MACH1 				= 49.020;		/* speed of sound = C_MACH1 * sqrt(Ts) */
const double C_GAMMA 				= 1.40;			/* use this constant value for now ?? */

#endif  /* TMATS_CONSTANTS_H */
