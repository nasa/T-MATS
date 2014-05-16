#ifndef __Lib_NM_JacobianCalc_TMATS_sfun_h__
#define __Lib_NM_JacobianCalc_TMATS_sfun_h__

/* Include files */
#define S_FUNCTION_NAME                sf_sfun
#include "sfc_sf.h"
#include "sfc_mex.h"
#include "rtwtypes.h"
#include "sfcdebug.h"
#define rtInf                          (mxGetInf())
#define rtMinusInf                     (-(mxGetInf()))
#define rtNaN                          (mxGetNaN())
#define rtIsNaN(X)                     ((int)mxIsNaN(X))
#define rtIsInf(X)                     ((int)mxIsInf(X))

/* Type Definitions */

/* Named Constants */

/* Variable Declarations */
extern uint32_T _Lib_NM_JacobianCalc_TMATSMachineNumber_;
extern real_T _sfTime_;

/* Variable Definitions */

/* Function Declarations */
extern void Lib_NM_JacobianCalc_TMATS_initializer(void);
extern void Lib_NM_JacobianCalc_TMATS_terminator(void);

/* Function Definitions */

/* We load infoStruct for rtw_optimation_info on demand in mdlSetWorkWidths and
   free it immediately in mdlStart. Given that this is machine-wide as
   opposed to chart specific, we use NULL check to make sure it gets loaded
   and unloaded once per machine even though the  methods mdlSetWorkWidths/mdlStart
   are chart/instance specific. The following methods abstract this out. */
extern mxArray* load_Lib_NM_JacobianCalc_TMATS_optimization_info(void);
extern void unload_Lib_NM_JacobianCalc_TMATS_optimization_info(void);

#endif
