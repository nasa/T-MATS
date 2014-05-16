/* Include files */

#include "Lib_NM_JacobianCalc_TMATS_sfun.h"
#include "c1_Lib_NM_JacobianCalc_TMATS.h"

/* Type Definitions */

/* Named Constants */

/* Variable Declarations */

/* Variable Definitions */
uint32_T _Lib_NM_JacobianCalc_TMATSMachineNumber_;

/* Function Declarations */

/* Function Definitions */
void Lib_NM_JacobianCalc_TMATS_initializer(void)
{
}

void Lib_NM_JacobianCalc_TMATS_terminator(void)
{
}

/* SFunction Glue Code */
unsigned int sf_Lib_NM_JacobianCalc_TMATS_method_dispatcher(SimStruct
  *simstructPtr, unsigned int chartFileNumber, const char* specsCksum, int_T
  method, void *data)
{
  if (chartFileNumber==1) {
    c1_Lib_NM_JacobianCalc_TMATS_method_dispatcher(simstructPtr, method, data);
    return 1;
  }

  return 0;
}

unsigned int sf_Lib_NM_JacobianCalc_TMATS_process_check_sum_call( int nlhs,
  mxArray * plhs[], int nrhs, const mxArray * prhs[] )
{

#ifdef MATLAB_MEX_FILE

  char commandName[20];
  if (nrhs<1 || !mxIsChar(prhs[0]) )
    return 0;

  /* Possible call to get the checksum */
  mxGetString(prhs[0], commandName,sizeof(commandName)/sizeof(char));
  commandName[(sizeof(commandName)/sizeof(char)-1)] = '\0';
  if (strcmp(commandName,"sf_get_check_sum"))
    return 0;
  plhs[0] = mxCreateDoubleMatrix( 1,4,mxREAL);
  if (nrhs>2 && mxIsChar(prhs[1])) {
    mxGetString(prhs[1], commandName,sizeof(commandName)/sizeof(char));
    commandName[(sizeof(commandName)/sizeof(char)-1)] = '\0';
    if (!strcmp(commandName,"library")) {
      char machineName[100];
      mxGetString(prhs[2], machineName,sizeof(machineName)/sizeof(char));
      machineName[(sizeof(machineName)/sizeof(char)-1)] = '\0';
      if (!strcmp(machineName,"Lib_NM_JacobianCalc_TMATS")) {
        if (nrhs==3) {
          ((real_T *)mxGetPr((plhs[0])))[0] = (real_T)(518200268U);
          ((real_T *)mxGetPr((plhs[0])))[1] = (real_T)(1538895194U);
          ((real_T *)mxGetPr((plhs[0])))[2] = (real_T)(2263191040U);
          ((real_T *)mxGetPr((plhs[0])))[3] = (real_T)(1043040392U);
        } else if (nrhs==4) {
          unsigned int chartFileNumber;
          chartFileNumber = (unsigned int)mxGetScalar(prhs[3]);
          switch (chartFileNumber) {
           case 1:
            {
              extern void sf_c1_Lib_NM_JacobianCalc_TMATS_get_check_sum(mxArray *
                plhs[]);
              sf_c1_Lib_NM_JacobianCalc_TMATS_get_check_sum(plhs);
              break;
            }

           default:
            ((real_T *)mxGetPr((plhs[0])))[0] = (real_T)(0.0);
            ((real_T *)mxGetPr((plhs[0])))[1] = (real_T)(0.0);
            ((real_T *)mxGetPr((plhs[0])))[2] = (real_T)(0.0);
            ((real_T *)mxGetPr((plhs[0])))[3] = (real_T)(0.0);
          }
        } else {
          return 0;
        }
      } else {
        return 0;
      }
    } else {
      return 0;
    }
  } else {
    return 0;
  }

  return 1;

#else

  return 0;

#endif

}

unsigned int sf_Lib_NM_JacobianCalc_TMATS_autoinheritance_info( int nlhs,
  mxArray * plhs[], int nrhs, const mxArray * prhs[] )
{

#ifdef MATLAB_MEX_FILE

  char commandName[32];
  char aiChksum[64];
  if (nrhs<3 || !mxIsChar(prhs[0]) )
    return 0;

  /* Possible call to get the autoinheritance_info */
  mxGetString(prhs[0], commandName,sizeof(commandName)/sizeof(char));
  commandName[(sizeof(commandName)/sizeof(char)-1)] = '\0';
  if (strcmp(commandName,"get_autoinheritance_info"))
    return 0;
  mxGetString(prhs[2], aiChksum,sizeof(aiChksum)/sizeof(char));
  aiChksum[(sizeof(aiChksum)/sizeof(char)-1)] = '\0';

  {
    unsigned int chartFileNumber;
    chartFileNumber = (unsigned int)mxGetScalar(prhs[1]);
    switch (chartFileNumber) {
     case 1:
      {
        if (strcmp(aiChksum, "efKQxlHHeoxsEVy7c9BNnE") == 0) {
          extern mxArray
            *sf_c1_Lib_NM_JacobianCalc_TMATS_get_autoinheritance_info(void);
          plhs[0] = sf_c1_Lib_NM_JacobianCalc_TMATS_get_autoinheritance_info();
          break;
        }

        plhs[0] = mxCreateDoubleMatrix(0,0,mxREAL);
        break;
      }

     default:
      plhs[0] = mxCreateDoubleMatrix(0,0,mxREAL);
    }
  }

  return 1;

#else

  return 0;

#endif

}

unsigned int sf_Lib_NM_JacobianCalc_TMATS_get_eml_resolved_functions_info( int
  nlhs, mxArray * plhs[], int nrhs, const mxArray * prhs[] )
{

#ifdef MATLAB_MEX_FILE

  char commandName[64];
  if (nrhs<2 || !mxIsChar(prhs[0]))
    return 0;

  /* Possible call to get the get_eml_resolved_functions_info */
  mxGetString(prhs[0], commandName,sizeof(commandName)/sizeof(char));
  commandName[(sizeof(commandName)/sizeof(char)-1)] = '\0';
  if (strcmp(commandName,"get_eml_resolved_functions_info"))
    return 0;

  {
    unsigned int chartFileNumber;
    chartFileNumber = (unsigned int)mxGetScalar(prhs[1]);
    switch (chartFileNumber) {
     case 1:
      {
        extern const mxArray
          *sf_c1_Lib_NM_JacobianCalc_TMATS_get_eml_resolved_functions_info(void);
        mxArray *persistentMxArray = (mxArray *)
          sf_c1_Lib_NM_JacobianCalc_TMATS_get_eml_resolved_functions_info();
        plhs[0] = mxDuplicateArray(persistentMxArray);
        mxDestroyArray(persistentMxArray);
        break;
      }

     default:
      plhs[0] = mxCreateDoubleMatrix(0,0,mxREAL);
    }
  }

  return 1;

#else

  return 0;

#endif

}

void Lib_NM_JacobianCalc_TMATS_debug_initialize(void)
{
  _Lib_NM_JacobianCalc_TMATSMachineNumber_ = sf_debug_initialize_machine(
    "Lib_NM_JacobianCalc_TMATS","sfun",1,1,0,0,0);
  sf_debug_set_machine_event_thresholds(_Lib_NM_JacobianCalc_TMATSMachineNumber_,
    0,0);
  sf_debug_set_machine_data_thresholds(_Lib_NM_JacobianCalc_TMATSMachineNumber_,
    0);
}

void Lib_NM_JacobianCalc_TMATS_register_exported_symbols(SimStruct* S)
{
}

static mxArray* sRtwOptimizationInfoStruct= NULL;
mxArray* load_Lib_NM_JacobianCalc_TMATS_optimization_info(void)
{
  if (sRtwOptimizationInfoStruct==NULL) {
    sRtwOptimizationInfoStruct = sf_load_rtw_optimization_info(
      "Lib_NM_JacobianCalc_TMATS", "turbofan");
    mexMakeArrayPersistent(sRtwOptimizationInfoStruct);
  }

  return(sRtwOptimizationInfoStruct);
}

void unload_Lib_NM_JacobianCalc_TMATS_optimization_info(void)
{
  if (sRtwOptimizationInfoStruct!=NULL) {
    mxDestroyArray(sRtwOptimizationInfoStruct);
    sRtwOptimizationInfoStruct = NULL;
  }
}
