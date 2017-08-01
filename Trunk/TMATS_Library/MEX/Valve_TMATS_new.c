/*		T-MATS -- Valve_TMATS.c
 * % *************************************************************************
 * % written by Jeffryes Chapman based on work by R. May and Ananda Himansu
 * % NASA Glenn Research Center, Cleveland, OH
 * % March 1st, 2013
 * %
 * %  This file models the Valve Element for T-MATS simulation
 * % *************************************************************************/
#define S_FUNCTION_NAME	 Valve_TMATS_new
#define S_FUNCTION_LEVEL 2
#include "simstruc.h"
#include "types_TMATS.h"

#define VlvfullyOpen_p(S)			ssGetSFcnParam(S,0)
#define VlvdeadZone_p(S)			ssGetSFcnParam(S,1)
#define Valve_Ae_p(S)				ssGetSFcnParam(S,2)
#define X_V_PRVec_p(S)				ssGetSFcnParam(S,3)
#define T_V_WcVec_p(S)				ssGetSFcnParam(S,4)
#define BN_p(S)						ssGetSFcnParam(S,5)
#define NPARAMS 6
#define NERRORS 1

extern void Valve_TMATS_body(real_T*, const real_T*, const ValveStruct*);

static void mdlInitializeSizes(SimStruct *S)
{
	int i;
	ssSetNumSFcnParams(S, NPARAMS);	 /* Number of expected parameters */
	if (ssGetNumSFcnParams(S) != ssGetSFcnParamsCount(S)) {
		/* Return if number of expected != number of actual parameters */
		return;
	}

	for (i = 0; i < NPARAMS; i++) {
        if (i != 5) {
            ssSetSFcnParamTunable(S, i, 1);
        } else {
            ssSetSFcnParamTunable(S, i, 0);
        }
    }

	ssSetNumContStates(S, 0);
	ssSetNumDiscStates(S, 0);

	if (!ssSetNumInputPorts(S, 1)) return;
	ssSetInputPortWidth(S, 0, 5);
	ssSetInputPortRequiredContiguous(S, 0, true);
	ssSetInputPortDirectFeedThrough(S, 0, 1);

	if (!ssSetNumOutputPorts(S, 1)) return;
	ssSetOutputPortWidth(S, 0, 2);

	ssSetNumSampleTimes(S, 1);
	ssSetNumRWork(S, 0);
	ssSetNumIWork(S, NERRORS);
	ssSetNumPWork(S, 0);
	ssSetNumModes(S, 0);
	ssSetNumNonsampledZCs(S, 0);

}

static void mdlInitializeSampleTimes(SimStruct *S)
{
	ssSetSampleTime(S, 0, INHERITED_SAMPLE_TIME);
	ssSetOffsetTime(S, 0, 0.0);
	ssSetModelReferenceSampleTimeDefaultInheritance(S);
}

#define MDL_START
#if defined(MDL_START)
static void mdlStart(SimStruct *S)
{
	/* initialize print error variables */
	ssSetIWorkValue(S,Er1,0);
}
#endif

static void mdlOutputs(SimStruct *S, int_T tid)
{
	/* Input vector */
	const real_T *u	 = (const real_T*) ssGetInputPortSignal(S,0);
	
	/* Output vector */
	real_T *y  = (real_T *)ssGetOutputPortRealSignal(S,0);
	
	/* Block name buffer length and string read status */
	char * BlkNm;
	int_T buflen;
	int_T status;
	
	/* Block mask parameter struct */
	ValveStruct valvePrms;
	valvePrms.VlvfullyOpen			= *mxGetPr(VlvfullyOpen_p(S));
	valvePrms.VlvdeadZone			= *mxGetPr(VlvdeadZone_p(S));
	valvePrms.Valve_Ae				 = *mxGetPr(Valve_Ae_p(S));
	/* Vector & array data */
	valvePrms.X_V_PRVec			= mxGetPr(X_V_PRVec_p(S));
	valvePrms.T_V_WcVec			 = mxGetPr(T_V_WcVec_p(S));
	/* Dimensions of parameter arrays */
	valvePrms.A	  = mxGetNumberOfElements(X_V_PRVec_p(S));

	/* Get name of block from dialog parameter (string) */
	buflen = mxGetN(BN_p(S))*sizeof(mxChar)+1;
	valvePrms.BlkNm = mxMalloc(buflen);
	status = mxGetString(BN_p(S), valvePrms.BlkNm, buflen);
	
	/* Integer work vector for error codes */
	valvePrms.IWork = ssGetIWork(S);

	Valve_TMATS_body(y, u, &valvePrms);
}

static void mdlTerminate(SimStruct *S)
{
}

#define MDL_RTW						   /* Change to #undef to remove function */
#if defined(MDL_RTW) && (defined(MATLAB_MEX_FILE) || defined(NRT))
/* Function: mdlRTW ============================================================
 * Abstract:
 *	  This function is called when Real-Time Workshop is generating the
 *	  model.rtw file. In this routine, you can call the following functions
 *	  which add fields to the model.rtw file.
 *
 *	  Important! Since this s-function has this mdlRTW method, it is required
 *	  to have a corresponding .tlc file so as to work with RTW. You will find
 *	  the sfun_directlook.tlc in <matlaroot>/toolbox/simulink/blocks/tlc_c/.
 */
static void mdlRTW(SimStruct *S)
{
	real_T* VlvfullyOpen	    = mxGetPr(VlvfullyOpen_p(S));
	real_T* VlvdeadZone			= mxGetPr(VlvdeadZone_p(S));
	real_T* Valve_Ae			= mxGetPr(Valve_Ae_p(S));
	real_T* X_V_PRVec			= mxGetPr(X_V_PRVec_p(S));
	real_T* T_V_WcVec			= mxGetPr(T_V_WcVec_p(S));
	
	if (!ssWriteRTWParameters(S, 5,
        SSWRITE_VALUE_VECT, "VlvfullyOpen",   "",  VlvfullyOpen,   1,
        SSWRITE_VALUE_VECT, "VlvdeadZone",    "",  VlvdeadZone,    1,
        SSWRITE_VALUE_VECT, "Valve_Ae",       "",  Valve_Ae,       1,
        SSWRITE_VALUE_VECT, "X_V_PRVec",      "",  X_V_PRVec,      mxGetNumberOfElements(X_V_PRVec_p(S)),
        SSWRITE_VALUE_VECT, "T_V_WcVec",      "",  T_V_WcVec,      mxGetNumberOfElements(T_V_WcVec_p(S))
		))
	{
		return;/* An error occurred which will be reported by Simulink */
	}
}

#endif /* MDL_RTW */


#ifdef	MATLAB_MEX_FILE	   /* Is this file being compiled as a MEX-file? */
#include "simulink.c"	   /* MEX-file interface mechanism */
#else
#include "cg_sfun.h"	   /* Code generation registration function */
#endif
/*==================*/
