/*		T-MATS -- Burner_TMATS.c
 * % *************************************************************************
 * % written by Jeffryes Chapman based on work by R. May and T. Lavelle
 * % NASA Glenn Research Center, Cleveland, OH
 * % February 27th, 2013
 * %
 * %  This file models the Burner Element for T-MATS simulation
 * % *************************************************************************/
#define S_FUNCTION_NAME  Burner_TMATS_new
#define S_FUNCTION_LEVEL 2
#include "simstruc.h"
#include "types_TMATS.h"

#define LHV_p(S)			    ssGetSFcnParam(S,0)
#define dPnormBurner_p(S)		ssGetSFcnParam(S,1)
#define Efficiency_p(S)         ssGetSFcnParam(S,2)
#define LHVEn_p(S)              ssGetSFcnParam(S,3)
#define hFuel_p(S)              ssGetSFcnParam(S,4)
#define NPARAMS 5

// Forward declaration for Burner body of calcs
extern void Burner_TMATS_body(double*, double*, BurnStruct*);

static void mdlInitializeSizes(SimStruct *S)
{
    int i;
    ssSetNumSFcnParams(S, NPARAMS);  /* Number of expected parameters */
    if (ssGetNumSFcnParams(S) != ssGetSFcnParamsCount(S)) {
        /* Return if number of expected != number of actual parameters */
        return;
    }
    
    // Set Block parameters tunable
    // Note: Original version had all parameters set to non-tunable(?)
    for (i = 0; i < NPARAMS; i++)
        ssSetSFcnParamTunable(S, i, 1);  
    
    ssSetNumContStates(S, 0);
    ssSetNumDiscStates(S, 0);
    
    if (!ssSetNumInputPorts(S, 1)) return;
    ssSetInputPortWidth(S, 0, 6);
    ssSetInputPortRequiredContiguous(S, 0, true);
    ssSetInputPortDirectFeedThrough(S, 0, 1);
    
    if (!ssSetNumOutputPorts(S, 1)) return;
    ssSetOutputPortWidth(S, 0, 6);
    
    ssSetNumSampleTimes(S, 1);
    ssSetNumRWork(S, 0);
    ssSetNumIWork(S, 0);
    ssSetNumPWork(S, 0);
    ssSetNumModes(S, 0);
    ssSetNumNonsampledZCs(S, 0);
    
    // Register reserved identifiers to avoid name conflict
    if (ssRTWGenIsCodeGen(S) || ssGetSimMode(S)==SS_SIMMODE_EXTERNAL) {
        // Register reserved identifier for OutputFcnSpec
        ssRegMdlInfo(S, "Burner_TMATS_body", MDL_INFO_ID_RESERVED, 0, 0, ssGetPath(S));
    }
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
    
}
#endif

static void mdlOutputs(SimStruct *S, int_T tid)
{
	// Grab block parameters and place into parameter struct to pass into Burner_TMATS_body()
    BurnStruct burnPrms;
    burnPrms.LHV			 = *mxGetPr(LHV_p(S));
    burnPrms.dPnormBurner   = *mxGetPr(dPnormBurner_p(S));
    burnPrms.Eff            = *mxGetPr(Efficiency_p(S));
    burnPrms.LHVEn          = *mxGetPr(LHVEn_p(S));
    burnPrms.hFuel          = *mxGetPr(hFuel_p(S));
    
    real_T *u  = (real_T*) ssGetInputPortSignal(S,0);      // Inputs
    real_T *y  = (real_T*) ssGetOutputPortRealSignal(S,0); // Outputs
    
    // Call the "body" of the Burner_TMATS code
    Burner_TMATS_body(y, u, &burnPrms);
}

static void mdlTerminate(SimStruct *S)
{
}

#define MDL_RTW                        /* Change to #undef to remove function */
#if defined(MDL_RTW) && (defined(MATLAB_MEX_FILE) || defined(NRT))
/* Function: mdlRTW ============================================================
 * Abstract:
 *    This function is called when Real-Time Workshop is generating the
 *    model.rtw file. In this routine, you can call the following functions
 *    which add fields to the model.rtw file.
 *
 *    Important! Since this s-function has this mdlRTW method, it is required
 *    to have a corresponding .tlc file so as to work with RTW. You will find
 *    the sfun_directlook.tlc in <matlaroot>/toolbox/simulink/blocks/tlc_c/.
 */
static void mdlRTW(SimStruct *S)
{
    real_T LHV          = *mxGetPr(LHV_p(S));
    real_T DPnormBurner = *mxGetPr(dPnormBurner_p(S));
    real_T Eff          = *mxGetPr(Efficiency_p(S));
    real_T LHVEn        = *mxGetPr(LHVEn_p(S));
    real_T hFuel        = *mxGetPr(hFuel_p(S));
    
    if (!ssWriteRTWParameters(S, 5, 
       SSWRITE_VALUE_VECT, "LHV",          "",  LHV,          1,
       SSWRITE_VALUE_VECT, "DPnormBurner", "",  DPnormBurner, 1,
       SSWRITE_VALUE_VECT, "Eff",          "",  Eff,          1,
       SSWRITE_VALUE_VECT, "LHVEn",        "",  LHVEn,        1,
       SSWRITE_VALUE_VECT, "hFuel",        "",  hFuel,        1
       ))
    {
        return;/* An error occurred which will be reported by Simulink */
    }
}

#endif /* MDL_RTW */

#ifdef  MATLAB_MEX_FILE    /* Is this file being compiled as a MEX-file? */
#include "simulink.c"      /* MEX-file interface mechanism */
#else
#include "cg_sfun.h"       /* Code generation registration function */
#endif
/*==================*/
