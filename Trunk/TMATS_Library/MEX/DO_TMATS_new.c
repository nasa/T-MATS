/*		T-MATS -- DO_TMATS.c
 * % *************************************************************************
 * % written by Jeffryes Chapman
 * % NASA Glenn Research Center, Cleveland, OH
 * % July 10, 2015
 * %
 * %  Writes Data to an output file
 * % *************************************************************************/
#define S_FUNCTION_NAME  DO_TMATS_new
#define S_FUNCTION_LEVEL 2
#include "stdio.h"
#include "stdlib.h"
#include "simstruc.h"
#include <math.h>

#define FN_p(S)              ssGetSFcnParam(S,0)
#define NPARAMS 1

static void mdlInitializeSizes(SimStruct *S)
{
    int i;
    ssSetNumSFcnParams(S, NPARAMS);  /* Number of expected parameters */
    if (ssGetNumSFcnParams(S) != ssGetSFcnParamsCount(S)) {
        /* Return if number of expected != number of actual parameters */
        return;
    }
    
    for (i = 0; i < NPARAMS; i++)
        ssSetSFcnParamTunable(S, i, 0);
    
    ssSetNumContStates(S, 0);
    ssSetNumDiscStates(S, 0);
    
    if (!ssSetNumInputPorts(S, 1)) return;
    ssSetInputPortWidth(S, 0, DYNAMICALLY_SIZED);
    ssSetInputPortRequiredContiguous(S, 0, true);
    ssSetInputPortDirectFeedThrough(S, 0, 1);
    
    ssSetNumSampleTimes(S, 1);
    ssSetNumRWork(S, 0);
    ssSetNumIWork(S, 0);
    ssSetNumPWork(S, 0);
    ssSetNumModes(S, 0);
    ssSetNumNonsampledZCs(S, 0);
    
    ssSetOptions(S, SS_OPTION_WORKS_WITH_CODE_REUSE |
                    SS_OPTION_USE_TLC_WITH_ACCELERATOR);
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
    /*---------Define Inputs--------*/
    const real_T *VecIn = ssGetInputPortRealSignal(S, 0);
    
    /* ------- get strings -------------- */
    double Val;
    char * FileNm;
    FILE * fp;
    int Width, i;
    int_T buflen;
    int_T status;
    
    /* Get File Name from dialog parameter (string) */
    buflen = mxGetN(FN_p(S))*sizeof(mxChar)+1;
    FileNm = mxMalloc(buflen);
    status = mxGetString(FN_p(S), FileNm, buflen);
    
    
    Width = VecIn[0];
    fp = fopen(FileNm,"w");
    for (i = 1; i <= Width; i++){
        
        Val = VecIn[i];
        if (Val >= 99999)
            Val = 99999;
        else if (Val <= -99999)
            Val = -99999;
        
        fprintf(fp,"%f\n",Val);
    }
    
    fclose(fp);
}

static void mdlTerminate(SimStruct *S)
{
}

#if defined(MATLAB_MEX_FILE)
#define MDL_RTW
/* Function: mdlRTW ===========================================================
 * Abstract:
 *      Write out the single "gain" parameter for this block.
 *      Write out the signs parameter setting for this block.
 */
static void mdlRTW(SimStruct *S)
{
    
    char_T * FileNm;
    int_T Width, i;
    int_T buflen;
    int_T status;
    
    /* Get File Name from dialog parameter (string) */
    buflen = mxGetN(ssGetSFcnParam(S,0))*sizeof(mxChar)+1;
    FileNm = mxMalloc(buflen);
    if (!FileNm) {
        ssSetErrorStatus(S,"Memory allocation error in mdlRTW");
        return;
    }
    
    status = mxGetString(FN_p(S), FileNm, buflen);
    ssWriteRTWParamSettings(S, 1, SSWRITE_VALUE_STR, "FileNm", FileNm, buflen);
}
#endif /* MDL_RTW */

#ifdef  MATLAB_MEX_FILE    /* Is this file being compiled as a MEX-file? */
#include "simulink.c"      /* MEX-file interface mechanism */
#else
#include "cg_sfun.h"       /* Code generation registration function */
#endif
/*==================*/
