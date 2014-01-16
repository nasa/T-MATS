/* Include files */

#include "blascompat32.h"
#include "TMATS_NumericalMethods_sfun.h"
#include "c1_TMATS_NumericalMethods.h"
#include "mwmathutil.h"
#define CHARTINSTANCE_CHARTNUMBER      (chartInstance->chartNumber)
#define CHARTINSTANCE_INSTANCENUMBER   (chartInstance->instanceNumber)
#include "TMATS_NumericalMethods_sfun_debug_macros.h"

/* Type Definitions */

/* Named Constants */
#define CALL_EVENT                     (-1)

/* Variable Declarations */

/* Variable Definitions */
static const char * c1_debug_family_names[15] = { "i", "a", "b", "nargin",
  "nargout", "IterationIn", "y_xpIn", "PerIn", "XoIn", "JStateIn", "y_StateIn",
  "XpOut", "InvJOut", "JOut", "y_StateOut" };

/* Function Declarations */
static void initialize_c1_TMATS_NumericalMethods
  (SFc1_TMATS_NumericalMethodsInstanceStruct *chartInstance);
static void initialize_params_c1_TMATS_NumericalMethods
  (SFc1_TMATS_NumericalMethodsInstanceStruct *chartInstance);
static void enable_c1_TMATS_NumericalMethods
  (SFc1_TMATS_NumericalMethodsInstanceStruct *chartInstance);
static void disable_c1_TMATS_NumericalMethods
  (SFc1_TMATS_NumericalMethodsInstanceStruct *chartInstance);
static void c1_update_debugger_state_c1_TMATS_NumericalMethods
  (SFc1_TMATS_NumericalMethodsInstanceStruct *chartInstance);
static const mxArray *get_sim_state_c1_TMATS_NumericalMethods
  (SFc1_TMATS_NumericalMethodsInstanceStruct *chartInstance);
static void set_sim_state_c1_TMATS_NumericalMethods
  (SFc1_TMATS_NumericalMethodsInstanceStruct *chartInstance, const mxArray
   *c1_st);
static void finalize_c1_TMATS_NumericalMethods
  (SFc1_TMATS_NumericalMethodsInstanceStruct *chartInstance);
static void sf_c1_TMATS_NumericalMethods
  (SFc1_TMATS_NumericalMethodsInstanceStruct *chartInstance);
static void initSimStructsc1_TMATS_NumericalMethods
  (SFc1_TMATS_NumericalMethodsInstanceStruct *chartInstance);
static void init_script_number_translation(uint32_T c1_machineNumber, uint32_T
  c1_chartNumber);
static const mxArray *c1_sf_marshallOut(void *chartInstanceVoid, void *c1_inData);
static void c1_emlrt_marshallIn(SFc1_TMATS_NumericalMethodsInstanceStruct
  *chartInstance, const mxArray *c1_y_StateOut, const char_T *c1_identifier,
  real_T c1_y[90]);
static void c1_b_emlrt_marshallIn(SFc1_TMATS_NumericalMethodsInstanceStruct
  *chartInstance, const mxArray *c1_u, const emlrtMsgIdentifier *c1_parentId,
  real_T c1_y[90]);
static void c1_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c1_mxArrayInData, const char_T *c1_varName, void *c1_outData);
static const mxArray *c1_b_sf_marshallOut(void *chartInstanceVoid, void
  *c1_inData);
static void c1_c_emlrt_marshallIn(SFc1_TMATS_NumericalMethodsInstanceStruct
  *chartInstance, const mxArray *c1_JOut, const char_T *c1_identifier, real_T
  c1_y[81]);
static void c1_d_emlrt_marshallIn(SFc1_TMATS_NumericalMethodsInstanceStruct
  *chartInstance, const mxArray *c1_u, const emlrtMsgIdentifier *c1_parentId,
  real_T c1_y[81]);
static void c1_b_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c1_mxArrayInData, const char_T *c1_varName, void *c1_outData);
static const mxArray *c1_c_sf_marshallOut(void *chartInstanceVoid, void
  *c1_inData);
static void c1_e_emlrt_marshallIn(SFc1_TMATS_NumericalMethodsInstanceStruct
  *chartInstance, const mxArray *c1_XpOut, const char_T *c1_identifier, real_T
  c1_y[9]);
static void c1_f_emlrt_marshallIn(SFc1_TMATS_NumericalMethodsInstanceStruct
  *chartInstance, const mxArray *c1_u, const emlrtMsgIdentifier *c1_parentId,
  real_T c1_y[9]);
static void c1_c_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c1_mxArrayInData, const char_T *c1_varName, void *c1_outData);
static const mxArray *c1_d_sf_marshallOut(void *chartInstanceVoid, void
  *c1_inData);
static const mxArray *c1_e_sf_marshallOut(void *chartInstanceVoid, void
  *c1_inData);
static real_T c1_g_emlrt_marshallIn(SFc1_TMATS_NumericalMethodsInstanceStruct
  *chartInstance, const mxArray *c1_u, const emlrtMsgIdentifier *c1_parentId);
static void c1_d_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c1_mxArrayInData, const char_T *c1_varName, void *c1_outData);
static void c1_info_helper(c1_ResolvedFunctionInfo c1_info[124]);
static void c1_b_info_helper(c1_ResolvedFunctionInfo c1_info[124]);
static void c1_invNxN(SFc1_TMATS_NumericalMethodsInstanceStruct *chartInstance,
                      real_T c1_x[81], real_T c1_y[81]);
static void c1_realmin(SFc1_TMATS_NumericalMethodsInstanceStruct *chartInstance);
static void c1_eps(SFc1_TMATS_NumericalMethodsInstanceStruct *chartInstance);
static void c1_eml_matlab_zgetrf(SFc1_TMATS_NumericalMethodsInstanceStruct
  *chartInstance, real_T c1_A[81], real_T c1_b_A[81], int32_T c1_ipiv[9],
  int32_T *c1_info);
static void c1_check_forloop_overflow_error
  (SFc1_TMATS_NumericalMethodsInstanceStruct *chartInstance);
static void c1_b_check_forloop_overflow_error
  (SFc1_TMATS_NumericalMethodsInstanceStruct *chartInstance, boolean_T
   c1_overflow);
static void c1_eml_xger(SFc1_TMATS_NumericalMethodsInstanceStruct *chartInstance,
  int32_T c1_m, int32_T c1_n, real_T c1_alpha1, int32_T c1_ix0, int32_T c1_iy0,
  real_T c1_A[81], int32_T c1_ia0, real_T c1_b_A[81]);
static void c1_eml_xtrsm(SFc1_TMATS_NumericalMethodsInstanceStruct
  *chartInstance, real_T c1_A[81], real_T c1_B[81], real_T c1_b_B[81]);
static real_T c1_norm(SFc1_TMATS_NumericalMethodsInstanceStruct *chartInstance,
                      real_T c1_x[81]);
static void c1_eml_warning(SFc1_TMATS_NumericalMethodsInstanceStruct
  *chartInstance);
static void c1_b_eml_warning(SFc1_TMATS_NumericalMethodsInstanceStruct
  *chartInstance, char_T c1_varargin_2[14]);
static void c1_h_emlrt_marshallIn(SFc1_TMATS_NumericalMethodsInstanceStruct
  *chartInstance, const mxArray *c1_sprintf, const char_T *c1_identifier, char_T
  c1_y[14]);
static void c1_i_emlrt_marshallIn(SFc1_TMATS_NumericalMethodsInstanceStruct
  *chartInstance, const mxArray *c1_u, const emlrtMsgIdentifier *c1_parentId,
  char_T c1_y[14]);
static const mxArray *c1_f_sf_marshallOut(void *chartInstanceVoid, void
  *c1_inData);
static int32_T c1_j_emlrt_marshallIn(SFc1_TMATS_NumericalMethodsInstanceStruct
  *chartInstance, const mxArray *c1_u, const emlrtMsgIdentifier *c1_parentId);
static void c1_e_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c1_mxArrayInData, const char_T *c1_varName, void *c1_outData);
static uint8_T c1_k_emlrt_marshallIn(SFc1_TMATS_NumericalMethodsInstanceStruct
  *chartInstance, const mxArray *c1_b_is_active_c1_TMATS_NumericalMethods, const
  char_T *c1_identifier);
static uint8_T c1_l_emlrt_marshallIn(SFc1_TMATS_NumericalMethodsInstanceStruct
  *chartInstance, const mxArray *c1_u, const emlrtMsgIdentifier *c1_parentId);
static void c1_b_eml_matlab_zgetrf(SFc1_TMATS_NumericalMethodsInstanceStruct
  *chartInstance, real_T c1_A[81], int32_T c1_ipiv[9], int32_T *c1_info);
static void c1_b_eml_xger(SFc1_TMATS_NumericalMethodsInstanceStruct
  *chartInstance, int32_T c1_m, int32_T c1_n, real_T c1_alpha1, int32_T c1_ix0,
  int32_T c1_iy0, real_T c1_A[81], int32_T c1_ia0);
static void c1_b_eml_xtrsm(SFc1_TMATS_NumericalMethodsInstanceStruct
  *chartInstance, real_T c1_A[81], real_T c1_B[81]);
static void init_dsm_address_info(SFc1_TMATS_NumericalMethodsInstanceStruct
  *chartInstance);

/* Function Definitions */
static void initialize_c1_TMATS_NumericalMethods
  (SFc1_TMATS_NumericalMethodsInstanceStruct *chartInstance)
{
  chartInstance->c1_sfEvent = CALL_EVENT;
  _sfTime_ = (real_T)ssGetT(chartInstance->S);
  chartInstance->c1_is_active_c1_TMATS_NumericalMethods = 0U;
}

static void initialize_params_c1_TMATS_NumericalMethods
  (SFc1_TMATS_NumericalMethodsInstanceStruct *chartInstance)
{
}

static void enable_c1_TMATS_NumericalMethods
  (SFc1_TMATS_NumericalMethodsInstanceStruct *chartInstance)
{
  _sfTime_ = (real_T)ssGetT(chartInstance->S);
}

static void disable_c1_TMATS_NumericalMethods
  (SFc1_TMATS_NumericalMethodsInstanceStruct *chartInstance)
{
  _sfTime_ = (real_T)ssGetT(chartInstance->S);
}

static void c1_update_debugger_state_c1_TMATS_NumericalMethods
  (SFc1_TMATS_NumericalMethodsInstanceStruct *chartInstance)
{
}

static const mxArray *get_sim_state_c1_TMATS_NumericalMethods
  (SFc1_TMATS_NumericalMethodsInstanceStruct *chartInstance)
{
  const mxArray *c1_st;
  const mxArray *c1_y = NULL;
  int32_T c1_i0;
  real_T c1_u[81];
  const mxArray *c1_b_y = NULL;
  int32_T c1_i1;
  real_T c1_b_u[81];
  const mxArray *c1_c_y = NULL;
  int32_T c1_i2;
  real_T c1_c_u[9];
  const mxArray *c1_d_y = NULL;
  int32_T c1_i3;
  real_T c1_d_u[90];
  const mxArray *c1_e_y = NULL;
  uint8_T c1_hoistedGlobal;
  uint8_T c1_e_u;
  const mxArray *c1_f_y = NULL;
  real_T (*c1_y_StateOut)[90];
  real_T (*c1_XpOut)[9];
  real_T (*c1_JOut)[81];
  real_T (*c1_InvJOut)[81];
  c1_y_StateOut = (real_T (*)[90])ssGetOutputPortSignal(chartInstance->S, 4);
  c1_JOut = (real_T (*)[81])ssGetOutputPortSignal(chartInstance->S, 3);
  c1_InvJOut = (real_T (*)[81])ssGetOutputPortSignal(chartInstance->S, 2);
  c1_XpOut = (real_T (*)[9])ssGetOutputPortSignal(chartInstance->S, 1);
  c1_st = NULL;
  c1_st = NULL;
  c1_y = NULL;
  sf_mex_assign(&c1_y, sf_mex_createcellarray(5), FALSE);
  for (c1_i0 = 0; c1_i0 < 81; c1_i0++) {
    c1_u[c1_i0] = (*c1_InvJOut)[c1_i0];
  }

  c1_b_y = NULL;
  sf_mex_assign(&c1_b_y, sf_mex_create("y", c1_u, 0, 0U, 1U, 0U, 2, 9, 9), FALSE);
  sf_mex_setcell(c1_y, 0, c1_b_y);
  for (c1_i1 = 0; c1_i1 < 81; c1_i1++) {
    c1_b_u[c1_i1] = (*c1_JOut)[c1_i1];
  }

  c1_c_y = NULL;
  sf_mex_assign(&c1_c_y, sf_mex_create("y", c1_b_u, 0, 0U, 1U, 0U, 2, 9, 9),
                FALSE);
  sf_mex_setcell(c1_y, 1, c1_c_y);
  for (c1_i2 = 0; c1_i2 < 9; c1_i2++) {
    c1_c_u[c1_i2] = (*c1_XpOut)[c1_i2];
  }

  c1_d_y = NULL;
  sf_mex_assign(&c1_d_y, sf_mex_create("y", c1_c_u, 0, 0U, 1U, 0U, 2, 1, 9),
                FALSE);
  sf_mex_setcell(c1_y, 2, c1_d_y);
  for (c1_i3 = 0; c1_i3 < 90; c1_i3++) {
    c1_d_u[c1_i3] = (*c1_y_StateOut)[c1_i3];
  }

  c1_e_y = NULL;
  sf_mex_assign(&c1_e_y, sf_mex_create("y", c1_d_u, 0, 0U, 1U, 0U, 2, 9, 10),
                FALSE);
  sf_mex_setcell(c1_y, 3, c1_e_y);
  c1_hoistedGlobal = chartInstance->c1_is_active_c1_TMATS_NumericalMethods;
  c1_e_u = c1_hoistedGlobal;
  c1_f_y = NULL;
  sf_mex_assign(&c1_f_y, sf_mex_create("y", &c1_e_u, 3, 0U, 0U, 0U, 0), FALSE);
  sf_mex_setcell(c1_y, 4, c1_f_y);
  sf_mex_assign(&c1_st, c1_y, FALSE);
  return c1_st;
}

static void set_sim_state_c1_TMATS_NumericalMethods
  (SFc1_TMATS_NumericalMethodsInstanceStruct *chartInstance, const mxArray
   *c1_st)
{
  const mxArray *c1_u;
  real_T c1_dv0[81];
  int32_T c1_i4;
  real_T c1_dv1[81];
  int32_T c1_i5;
  real_T c1_dv2[9];
  int32_T c1_i6;
  real_T c1_dv3[90];
  int32_T c1_i7;
  real_T (*c1_InvJOut)[81];
  real_T (*c1_JOut)[81];
  real_T (*c1_y_StateOut)[90];
  real_T (*c1_XpOut)[9];
  c1_y_StateOut = (real_T (*)[90])ssGetOutputPortSignal(chartInstance->S, 4);
  c1_JOut = (real_T (*)[81])ssGetOutputPortSignal(chartInstance->S, 3);
  c1_InvJOut = (real_T (*)[81])ssGetOutputPortSignal(chartInstance->S, 2);
  c1_XpOut = (real_T (*)[9])ssGetOutputPortSignal(chartInstance->S, 1);
  chartInstance->c1_doneDoubleBufferReInit = TRUE;
  c1_u = sf_mex_dup(c1_st);
  c1_c_emlrt_marshallIn(chartInstance, sf_mex_dup(sf_mex_getcell(c1_u, 0)),
                        "InvJOut", c1_dv0);
  for (c1_i4 = 0; c1_i4 < 81; c1_i4++) {
    (*c1_InvJOut)[c1_i4] = c1_dv0[c1_i4];
  }

  c1_c_emlrt_marshallIn(chartInstance, sf_mex_dup(sf_mex_getcell(c1_u, 1)),
                        "JOut", c1_dv1);
  for (c1_i5 = 0; c1_i5 < 81; c1_i5++) {
    (*c1_JOut)[c1_i5] = c1_dv1[c1_i5];
  }

  c1_e_emlrt_marshallIn(chartInstance, sf_mex_dup(sf_mex_getcell(c1_u, 2)),
                        "XpOut", c1_dv2);
  for (c1_i6 = 0; c1_i6 < 9; c1_i6++) {
    (*c1_XpOut)[c1_i6] = c1_dv2[c1_i6];
  }

  c1_emlrt_marshallIn(chartInstance, sf_mex_dup(sf_mex_getcell(c1_u, 3)),
                      "y_StateOut", c1_dv3);
  for (c1_i7 = 0; c1_i7 < 90; c1_i7++) {
    (*c1_y_StateOut)[c1_i7] = c1_dv3[c1_i7];
  }

  chartInstance->c1_is_active_c1_TMATS_NumericalMethods = c1_k_emlrt_marshallIn
    (chartInstance, sf_mex_dup(sf_mex_getcell(c1_u, 4)),
     "is_active_c1_TMATS_NumericalMethods");
  sf_mex_destroy(&c1_u);
  c1_update_debugger_state_c1_TMATS_NumericalMethods(chartInstance);
  sf_mex_destroy(&c1_st);
}

static void finalize_c1_TMATS_NumericalMethods
  (SFc1_TMATS_NumericalMethodsInstanceStruct *chartInstance)
{
}

static void sf_c1_TMATS_NumericalMethods
  (SFc1_TMATS_NumericalMethodsInstanceStruct *chartInstance)
{
  int32_T c1_i8;
  int32_T c1_i9;
  int32_T c1_i10;
  int32_T c1_i11;
  int32_T c1_i12;
  int32_T c1_i13;
  int32_T c1_i14;
  int32_T c1_i15;
  int32_T c1_i16;
  real_T c1_hoistedGlobal;
  real_T c1_IterationIn;
  int32_T c1_i17;
  real_T c1_y_xpIn[9];
  int32_T c1_i18;
  real_T c1_PerIn[9];
  int32_T c1_i19;
  real_T c1_XoIn[9];
  int32_T c1_i20;
  real_T c1_JStateIn[81];
  int32_T c1_i21;
  real_T c1_y_StateIn[90];
  uint32_T c1_debug_family_var_map[15];
  real_T c1_i;
  real_T c1_a;
  real_T c1_b;
  real_T c1_nargin = 6.0;
  real_T c1_nargout = 4.0;
  real_T c1_XpOut[9];
  real_T c1_InvJOut[81];
  real_T c1_JOut[81];
  real_T c1_y_StateOut[90];
  int32_T c1_i22;
  int32_T c1_i23;
  int32_T c1_i24;
  int32_T c1_i25;
  int32_T c1_b_i;
  int32_T c1_i26;
  int32_T c1_i27;
  real_T c1_A[9];
  real_T c1_B;
  real_T c1_y;
  real_T c1_b_y;
  int32_T c1_i28;
  int32_T c1_c_i;
  int32_T c1_i29;
  int32_T c1_i30;
  real_T c1_x[81];
  int32_T c1_i31;
  real_T c1_b_x[81];
  real_T c1_dv4[81];
  int32_T c1_i32;
  int32_T c1_i33;
  real_T c1_xinv[81];
  int32_T c1_i34;
  real_T c1_c_x[81];
  real_T c1_n1x;
  int32_T c1_i35;
  real_T c1_b_xinv[81];
  real_T c1_n1xinv;
  real_T c1_b_a;
  real_T c1_b_b;
  real_T c1_c_y;
  real_T c1_rc;
  real_T c1_d_x;
  boolean_T c1_c_b;
  real_T c1_e_x;
  int32_T c1_i36;
  static char_T c1_cv0[8] = { '%', '%', '%', 'd', '.', '%', 'd', 'e' };

  char_T c1_u[8];
  const mxArray *c1_d_y = NULL;
  real_T c1_b_u;
  const mxArray *c1_e_y = NULL;
  real_T c1_c_u;
  const mxArray *c1_f_y = NULL;
  real_T c1_d_u;
  const mxArray *c1_g_y = NULL;
  char_T c1_str[14];
  int32_T c1_i37;
  char_T c1_b_str[14];
  int32_T c1_i38;
  int32_T c1_i39;
  int32_T c1_i40;
  int32_T c1_i41;
  real_T (*c1_b_XpOut)[9];
  real_T (*c1_b_y_StateOut)[90];
  real_T (*c1_b_JOut)[81];
  real_T (*c1_b_InvJOut)[81];
  real_T *c1_b_IterationIn;
  real_T (*c1_b_y_StateIn)[90];
  real_T (*c1_b_JStateIn)[81];
  real_T (*c1_b_XoIn)[9];
  real_T (*c1_b_PerIn)[9];
  real_T (*c1_b_y_xpIn)[9];
  boolean_T guard1 = FALSE;
  boolean_T guard2 = FALSE;
  boolean_T guard3 = FALSE;
  boolean_T guard4 = FALSE;
  boolean_T guard5 = FALSE;
  boolean_T guard6 = FALSE;
  c1_b_y_StateOut = (real_T (*)[90])ssGetOutputPortSignal(chartInstance->S, 4);
  c1_b_y_StateIn = (real_T (*)[90])ssGetInputPortSignal(chartInstance->S, 5);
  c1_b_JStateIn = (real_T (*)[81])ssGetInputPortSignal(chartInstance->S, 4);
  c1_b_JOut = (real_T (*)[81])ssGetOutputPortSignal(chartInstance->S, 3);
  c1_b_InvJOut = (real_T (*)[81])ssGetOutputPortSignal(chartInstance->S, 2);
  c1_b_XoIn = (real_T (*)[9])ssGetInputPortSignal(chartInstance->S, 3);
  c1_b_PerIn = (real_T (*)[9])ssGetInputPortSignal(chartInstance->S, 2);
  c1_b_y_xpIn = (real_T (*)[9])ssGetInputPortSignal(chartInstance->S, 1);
  c1_b_XpOut = (real_T (*)[9])ssGetOutputPortSignal(chartInstance->S, 1);
  c1_b_IterationIn = (real_T *)ssGetInputPortSignal(chartInstance->S, 0);
  _sfTime_ = (real_T)ssGetT(chartInstance->S);
  _SFD_CC_CALL(CHART_ENTER_SFUNCTION_TAG, 0U, chartInstance->c1_sfEvent);
  _SFD_DATA_RANGE_CHECK(*c1_b_IterationIn, 0U);
  for (c1_i8 = 0; c1_i8 < 9; c1_i8++) {
    _SFD_DATA_RANGE_CHECK((*c1_b_XpOut)[c1_i8], 1U);
  }

  for (c1_i9 = 0; c1_i9 < 9; c1_i9++) {
    _SFD_DATA_RANGE_CHECK((*c1_b_y_xpIn)[c1_i9], 2U);
  }

  for (c1_i10 = 0; c1_i10 < 9; c1_i10++) {
    _SFD_DATA_RANGE_CHECK((*c1_b_PerIn)[c1_i10], 3U);
  }

  for (c1_i11 = 0; c1_i11 < 9; c1_i11++) {
    _SFD_DATA_RANGE_CHECK((*c1_b_XoIn)[c1_i11], 4U);
  }

  for (c1_i12 = 0; c1_i12 < 81; c1_i12++) {
    _SFD_DATA_RANGE_CHECK((*c1_b_InvJOut)[c1_i12], 5U);
  }

  for (c1_i13 = 0; c1_i13 < 81; c1_i13++) {
    _SFD_DATA_RANGE_CHECK((*c1_b_JOut)[c1_i13], 6U);
  }

  for (c1_i14 = 0; c1_i14 < 81; c1_i14++) {
    _SFD_DATA_RANGE_CHECK((*c1_b_JStateIn)[c1_i14], 7U);
  }

  for (c1_i15 = 0; c1_i15 < 90; c1_i15++) {
    _SFD_DATA_RANGE_CHECK((*c1_b_y_StateIn)[c1_i15], 8U);
  }

  for (c1_i16 = 0; c1_i16 < 90; c1_i16++) {
    _SFD_DATA_RANGE_CHECK((*c1_b_y_StateOut)[c1_i16], 9U);
  }

  chartInstance->c1_sfEvent = CALL_EVENT;
  _SFD_CC_CALL(CHART_ENTER_DURING_FUNCTION_TAG, 0U, chartInstance->c1_sfEvent);
  c1_hoistedGlobal = *c1_b_IterationIn;
  c1_IterationIn = c1_hoistedGlobal;
  for (c1_i17 = 0; c1_i17 < 9; c1_i17++) {
    c1_y_xpIn[c1_i17] = (*c1_b_y_xpIn)[c1_i17];
  }

  for (c1_i18 = 0; c1_i18 < 9; c1_i18++) {
    c1_PerIn[c1_i18] = (*c1_b_PerIn)[c1_i18];
  }

  for (c1_i19 = 0; c1_i19 < 9; c1_i19++) {
    c1_XoIn[c1_i19] = (*c1_b_XoIn)[c1_i19];
  }

  for (c1_i20 = 0; c1_i20 < 81; c1_i20++) {
    c1_JStateIn[c1_i20] = (*c1_b_JStateIn)[c1_i20];
  }

  for (c1_i21 = 0; c1_i21 < 90; c1_i21++) {
    c1_y_StateIn[c1_i21] = (*c1_b_y_StateIn)[c1_i21];
  }

  sf_debug_symbol_scope_push_eml(0U, 15U, 15U, c1_debug_family_names,
    c1_debug_family_var_map);
  sf_debug_symbol_scope_add_eml_importable(&c1_i, 0U, c1_e_sf_marshallOut,
    c1_d_sf_marshallIn);
  sf_debug_symbol_scope_add_eml_importable(&c1_a, 1U, c1_e_sf_marshallOut,
    c1_d_sf_marshallIn);
  sf_debug_symbol_scope_add_eml_importable(&c1_b, 2U, c1_e_sf_marshallOut,
    c1_d_sf_marshallIn);
  sf_debug_symbol_scope_add_eml_importable(&c1_nargin, 3U, c1_e_sf_marshallOut,
    c1_d_sf_marshallIn);
  sf_debug_symbol_scope_add_eml_importable(&c1_nargout, 4U, c1_e_sf_marshallOut,
    c1_d_sf_marshallIn);
  sf_debug_symbol_scope_add_eml(&c1_IterationIn, 5U, c1_e_sf_marshallOut);
  sf_debug_symbol_scope_add_eml(c1_y_xpIn, 6U, c1_d_sf_marshallOut);
  sf_debug_symbol_scope_add_eml(c1_PerIn, 7U, c1_d_sf_marshallOut);
  sf_debug_symbol_scope_add_eml(c1_XoIn, 8U, c1_d_sf_marshallOut);
  sf_debug_symbol_scope_add_eml(c1_JStateIn, 9U, c1_b_sf_marshallOut);
  sf_debug_symbol_scope_add_eml(c1_y_StateIn, 10U, c1_sf_marshallOut);
  sf_debug_symbol_scope_add_eml_importable(c1_XpOut, 11U, c1_c_sf_marshallOut,
    c1_c_sf_marshallIn);
  sf_debug_symbol_scope_add_eml_importable(c1_InvJOut, 12U, c1_b_sf_marshallOut,
    c1_b_sf_marshallIn);
  sf_debug_symbol_scope_add_eml_importable(c1_JOut, 13U, c1_b_sf_marshallOut,
    c1_b_sf_marshallIn);
  sf_debug_symbol_scope_add_eml_importable(c1_y_StateOut, 14U, c1_sf_marshallOut,
    c1_sf_marshallIn);
  CV_EML_FCN(0, 0);
  _SFD_EML_CALL(0U, chartInstance->c1_sfEvent, 4);
  c1_i = c1_IterationIn;
  _SFD_EML_CALL(0U, chartInstance->c1_sfEvent, 5);
  for (c1_i22 = 0; c1_i22 < 9; c1_i22++) {
    c1_XpOut[c1_i22] = c1_XoIn[c1_i22];
  }

  _SFD_EML_CALL(0U, chartInstance->c1_sfEvent, 6);
  for (c1_i23 = 0; c1_i23 < 81; c1_i23++) {
    c1_InvJOut[c1_i23] = 0.0;
  }

  _SFD_EML_CALL(0U, chartInstance->c1_sfEvent, 7);
  for (c1_i24 = 0; c1_i24 < 81; c1_i24++) {
    c1_JOut[c1_i24] = c1_JStateIn[c1_i24];
  }

  _SFD_EML_CALL(0U, chartInstance->c1_sfEvent, 8);
  for (c1_i25 = 0; c1_i25 < 90; c1_i25++) {
    c1_y_StateOut[c1_i25] = c1_y_StateIn[c1_i25];
  }

  _SFD_EML_CALL(0U, chartInstance->c1_sfEvent, 10);
  c1_a = 1.0;
  _SFD_EML_CALL(0U, chartInstance->c1_sfEvent, 12);
  c1_b = c1_a + 1.0;
  _SFD_EML_CALL(0U, chartInstance->c1_sfEvent, 15);
  guard6 = FALSE;
  if (CV_EML_COND(0, 1, 0, c1_i > c1_a)) {
    if (CV_EML_COND(0, 1, 1, c1_i <= 9.0 + c1_a)) {
      CV_EML_MCDC(0, 1, 0, TRUE);
      CV_EML_IF(0, 1, 0, TRUE);
      _SFD_EML_CALL(0U, chartInstance->c1_sfEvent, 16);
      c1_XpOut[_SFD_EML_ARRAY_BOUNDS_CHECK("XpOut", (int32_T)_SFD_INTEGER_CHECK(
        "i-a", c1_i - c1_a), 1, 9, 1, 0) - 1] =
        c1_XpOut[_SFD_EML_ARRAY_BOUNDS_CHECK("XpOut", (int32_T)
        _SFD_INTEGER_CHECK("i-a", c1_i - c1_a), 1, 9, 1, 0) - 1] +
        c1_PerIn[_SFD_EML_ARRAY_BOUNDS_CHECK("PerIn", (int32_T)
        _SFD_INTEGER_CHECK("i - a", c1_i - c1_a), 1, 9, 1, 0) - 1];
    } else {
      guard6 = TRUE;
    }
  } else {
    guard6 = TRUE;
  }

  if (guard6 == TRUE) {
    CV_EML_MCDC(0, 1, 0, FALSE);
    CV_EML_IF(0, 1, 0, FALSE);
  }

  _SFD_EML_CALL(0U, chartInstance->c1_sfEvent, 20);
  CV_EML_IF(0, 1, 1, FALSE);
  _SFD_EML_CALL(0U, chartInstance->c1_sfEvent, 25);
  guard5 = FALSE;
  if (CV_EML_COND(0, 1, 2, c1_i > c1_a)) {
    if (CV_EML_COND(0, 1, 3, c1_i <= (9.0 + c1_a) + 1.0)) {
      CV_EML_MCDC(0, 1, 1, TRUE);
      CV_EML_IF(0, 1, 3, TRUE);
      _SFD_EML_CALL(0U, chartInstance->c1_sfEvent, 26);
      c1_b_i = _SFD_EML_ARRAY_BOUNDS_CHECK("y_StateOut", (int32_T)
        _SFD_INTEGER_CHECK("i - a", c1_i - c1_a), 1, 10, 2, 0) - 1;
      for (c1_i26 = 0; c1_i26 < 9; c1_i26++) {
        c1_y_StateOut[c1_i26 + 9 * c1_b_i] = c1_y_xpIn[c1_i26];
      }
    } else {
      guard5 = TRUE;
    }
  } else {
    guard5 = TRUE;
  }

  if (guard5 == TRUE) {
    CV_EML_MCDC(0, 1, 1, FALSE);
    CV_EML_IF(0, 1, 3, FALSE);
  }

  _SFD_EML_CALL(0U, chartInstance->c1_sfEvent, 30);
  guard1 = FALSE;
  if (CV_EML_COND(0, 1, 4, c1_i > c1_b)) {
    if (CV_EML_COND(0, 1, 5, c1_i <= 9.0 + c1_b)) {
      CV_EML_MCDC(0, 1, 2, TRUE);
      CV_EML_IF(0, 1, 4, TRUE);
      _SFD_EML_CALL(0U, chartInstance->c1_sfEvent, 31);
      CV_EML_IF(0, 1, 5, FALSE);
      _SFD_EML_CALL(0U, chartInstance->c1_sfEvent, 34);
      for (c1_i27 = 0; c1_i27 < 9; c1_i27++) {
        c1_A[c1_i27] = c1_y_xpIn[c1_i27] - c1_y_StateOut[c1_i27];
      }

      c1_B = c1_PerIn[_SFD_EML_ARRAY_BOUNDS_CHECK("PerIn", (int32_T)
        _SFD_INTEGER_CHECK("i - b", c1_i - c1_b), 1, 9, 1, 0) - 1];
      c1_y = c1_B;
      c1_b_y = c1_y;
      for (c1_i28 = 0; c1_i28 < 9; c1_i28++) {
        c1_A[c1_i28] /= c1_b_y;
      }

      c1_c_i = _SFD_EML_ARRAY_BOUNDS_CHECK("JOut", (int32_T)_SFD_INTEGER_CHECK(
        "i - b", c1_i - c1_b), 1, 9, 2, 0) - 1;
      for (c1_i29 = 0; c1_i29 < 9; c1_i29++) {
        c1_JOut[c1_i29 + 9 * c1_c_i] = c1_A[c1_i29];
      }

      _SFD_EML_CALL(0U, chartInstance->c1_sfEvent, 36);
      if (CV_EML_IF(0, 1, 6, c1_i == 9.0 + c1_b)) {
        _SFD_EML_CALL(0U, chartInstance->c1_sfEvent, 37);
        for (c1_i30 = 0; c1_i30 < 81; c1_i30++) {
          c1_x[c1_i30] = c1_JOut[c1_i30];
        }

        for (c1_i31 = 0; c1_i31 < 81; c1_i31++) {
          c1_b_x[c1_i31] = c1_x[c1_i31];
        }

        c1_invNxN(chartInstance, c1_b_x, c1_dv4);
        for (c1_i32 = 0; c1_i32 < 81; c1_i32++) {
          c1_InvJOut[c1_i32] = c1_dv4[c1_i32];
        }

        for (c1_i33 = 0; c1_i33 < 81; c1_i33++) {
          c1_xinv[c1_i33] = c1_InvJOut[c1_i33];
        }

        for (c1_i34 = 0; c1_i34 < 81; c1_i34++) {
          c1_c_x[c1_i34] = c1_x[c1_i34];
        }

        c1_n1x = c1_norm(chartInstance, c1_c_x);
        for (c1_i35 = 0; c1_i35 < 81; c1_i35++) {
          c1_b_xinv[c1_i35] = c1_xinv[c1_i35];
        }

        c1_n1xinv = c1_norm(chartInstance, c1_b_xinv);
        c1_b_a = c1_n1x;
        c1_b_b = c1_n1xinv;
        c1_c_y = c1_b_a * c1_b_b;
        c1_rc = 1.0 / c1_c_y;
        guard2 = FALSE;
        guard3 = FALSE;
        if (c1_n1x == 0.0) {
          guard3 = TRUE;
        } else if (c1_n1xinv == 0.0) {
          guard3 = TRUE;
        } else if (c1_rc == 0.0) {
          guard2 = TRUE;
        } else {
          c1_d_x = c1_rc;
          c1_c_b = muDoubleScalarIsNaN(c1_d_x);
          guard4 = FALSE;
          if (c1_c_b) {
            guard4 = TRUE;
          } else {
            c1_eps(chartInstance);
            if (c1_rc < 2.2204460492503131E-16) {
              guard4 = TRUE;
            }
          }

          if (guard4 == TRUE) {
            c1_e_x = c1_rc;
            for (c1_i36 = 0; c1_i36 < 8; c1_i36++) {
              c1_u[c1_i36] = c1_cv0[c1_i36];
            }

            c1_d_y = NULL;
            sf_mex_assign(&c1_d_y, sf_mex_create("y", c1_u, 10, 0U, 1U, 0U, 2, 1,
              8), FALSE);
            c1_b_u = 14.0;
            c1_e_y = NULL;
            sf_mex_assign(&c1_e_y, sf_mex_create("y", &c1_b_u, 0, 0U, 0U, 0U, 0),
                          FALSE);
            c1_c_u = 6.0;
            c1_f_y = NULL;
            sf_mex_assign(&c1_f_y, sf_mex_create("y", &c1_c_u, 0, 0U, 0U, 0U, 0),
                          FALSE);
            c1_d_u = c1_e_x;
            c1_g_y = NULL;
            sf_mex_assign(&c1_g_y, sf_mex_create("y", &c1_d_u, 0, 0U, 0U, 0U, 0),
                          FALSE);
            c1_h_emlrt_marshallIn(chartInstance, sf_mex_call_debug("sprintf", 1U,
              2U, 14, sf_mex_call_debug("sprintf", 1U, 3U, 14, c1_d_y, 14,
              c1_e_y, 14, c1_f_y), 14, c1_g_y), "sprintf", c1_str);
            for (c1_i37 = 0; c1_i37 < 14; c1_i37++) {
              c1_b_str[c1_i37] = c1_str[c1_i37];
            }

            c1_b_eml_warning(chartInstance, c1_b_str);
          }
        }

        if (guard3 == TRUE) {
          guard2 = TRUE;
        }

        if (guard2 == TRUE) {
          c1_eml_warning(chartInstance);
        }
      }
    } else {
      guard1 = TRUE;
    }
  } else {
    guard1 = TRUE;
  }

  if (guard1 == TRUE) {
    CV_EML_MCDC(0, 1, 2, FALSE);
    CV_EML_IF(0, 1, 4, FALSE);
  }

  _SFD_EML_CALL(0U, chartInstance->c1_sfEvent, -37);
  sf_debug_symbol_scope_pop();
  for (c1_i38 = 0; c1_i38 < 9; c1_i38++) {
    (*c1_b_XpOut)[c1_i38] = c1_XpOut[c1_i38];
  }

  for (c1_i39 = 0; c1_i39 < 81; c1_i39++) {
    (*c1_b_InvJOut)[c1_i39] = c1_InvJOut[c1_i39];
  }

  for (c1_i40 = 0; c1_i40 < 81; c1_i40++) {
    (*c1_b_JOut)[c1_i40] = c1_JOut[c1_i40];
  }

  for (c1_i41 = 0; c1_i41 < 90; c1_i41++) {
    (*c1_b_y_StateOut)[c1_i41] = c1_y_StateOut[c1_i41];
  }

  _SFD_CC_CALL(EXIT_OUT_OF_FUNCTION_TAG, 0U, chartInstance->c1_sfEvent);
  sf_debug_check_for_state_inconsistency(_TMATS_NumericalMethodsMachineNumber_,
    chartInstance->chartNumber, chartInstance->instanceNumber);
}

static void initSimStructsc1_TMATS_NumericalMethods
  (SFc1_TMATS_NumericalMethodsInstanceStruct *chartInstance)
{
}

static void init_script_number_translation(uint32_T c1_machineNumber, uint32_T
  c1_chartNumber)
{
}

static const mxArray *c1_sf_marshallOut(void *chartInstanceVoid, void *c1_inData)
{
  const mxArray *c1_mxArrayOutData = NULL;
  int32_T c1_i42;
  int32_T c1_i43;
  int32_T c1_i44;
  real_T c1_b_inData[90];
  int32_T c1_i45;
  int32_T c1_i46;
  int32_T c1_i47;
  real_T c1_u[90];
  const mxArray *c1_y = NULL;
  SFc1_TMATS_NumericalMethodsInstanceStruct *chartInstance;
  chartInstance = (SFc1_TMATS_NumericalMethodsInstanceStruct *)chartInstanceVoid;
  c1_mxArrayOutData = NULL;
  c1_i42 = 0;
  for (c1_i43 = 0; c1_i43 < 10; c1_i43++) {
    for (c1_i44 = 0; c1_i44 < 9; c1_i44++) {
      c1_b_inData[c1_i44 + c1_i42] = (*(real_T (*)[90])c1_inData)[c1_i44 +
        c1_i42];
    }

    c1_i42 += 9;
  }

  c1_i45 = 0;
  for (c1_i46 = 0; c1_i46 < 10; c1_i46++) {
    for (c1_i47 = 0; c1_i47 < 9; c1_i47++) {
      c1_u[c1_i47 + c1_i45] = c1_b_inData[c1_i47 + c1_i45];
    }

    c1_i45 += 9;
  }

  c1_y = NULL;
  sf_mex_assign(&c1_y, sf_mex_create("y", c1_u, 0, 0U, 1U, 0U, 2, 9, 10), FALSE);
  sf_mex_assign(&c1_mxArrayOutData, c1_y, FALSE);
  return c1_mxArrayOutData;
}

static void c1_emlrt_marshallIn(SFc1_TMATS_NumericalMethodsInstanceStruct
  *chartInstance, const mxArray *c1_y_StateOut, const char_T *c1_identifier,
  real_T c1_y[90])
{
  emlrtMsgIdentifier c1_thisId;
  c1_thisId.fIdentifier = c1_identifier;
  c1_thisId.fParent = NULL;
  c1_b_emlrt_marshallIn(chartInstance, sf_mex_dup(c1_y_StateOut), &c1_thisId,
                        c1_y);
  sf_mex_destroy(&c1_y_StateOut);
}

static void c1_b_emlrt_marshallIn(SFc1_TMATS_NumericalMethodsInstanceStruct
  *chartInstance, const mxArray *c1_u, const emlrtMsgIdentifier *c1_parentId,
  real_T c1_y[90])
{
  real_T c1_dv5[90];
  int32_T c1_i48;
  sf_mex_import(c1_parentId, sf_mex_dup(c1_u), c1_dv5, 1, 0, 0U, 1, 0U, 2, 9, 10);
  for (c1_i48 = 0; c1_i48 < 90; c1_i48++) {
    c1_y[c1_i48] = c1_dv5[c1_i48];
  }

  sf_mex_destroy(&c1_u);
}

static void c1_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c1_mxArrayInData, const char_T *c1_varName, void *c1_outData)
{
  const mxArray *c1_y_StateOut;
  const char_T *c1_identifier;
  emlrtMsgIdentifier c1_thisId;
  real_T c1_y[90];
  int32_T c1_i49;
  int32_T c1_i50;
  int32_T c1_i51;
  SFc1_TMATS_NumericalMethodsInstanceStruct *chartInstance;
  chartInstance = (SFc1_TMATS_NumericalMethodsInstanceStruct *)chartInstanceVoid;
  c1_y_StateOut = sf_mex_dup(c1_mxArrayInData);
  c1_identifier = c1_varName;
  c1_thisId.fIdentifier = c1_identifier;
  c1_thisId.fParent = NULL;
  c1_b_emlrt_marshallIn(chartInstance, sf_mex_dup(c1_y_StateOut), &c1_thisId,
                        c1_y);
  sf_mex_destroy(&c1_y_StateOut);
  c1_i49 = 0;
  for (c1_i50 = 0; c1_i50 < 10; c1_i50++) {
    for (c1_i51 = 0; c1_i51 < 9; c1_i51++) {
      (*(real_T (*)[90])c1_outData)[c1_i51 + c1_i49] = c1_y[c1_i51 + c1_i49];
    }

    c1_i49 += 9;
  }

  sf_mex_destroy(&c1_mxArrayInData);
}

static const mxArray *c1_b_sf_marshallOut(void *chartInstanceVoid, void
  *c1_inData)
{
  const mxArray *c1_mxArrayOutData = NULL;
  int32_T c1_i52;
  int32_T c1_i53;
  int32_T c1_i54;
  real_T c1_b_inData[81];
  int32_T c1_i55;
  int32_T c1_i56;
  int32_T c1_i57;
  real_T c1_u[81];
  const mxArray *c1_y = NULL;
  SFc1_TMATS_NumericalMethodsInstanceStruct *chartInstance;
  chartInstance = (SFc1_TMATS_NumericalMethodsInstanceStruct *)chartInstanceVoid;
  c1_mxArrayOutData = NULL;
  c1_i52 = 0;
  for (c1_i53 = 0; c1_i53 < 9; c1_i53++) {
    for (c1_i54 = 0; c1_i54 < 9; c1_i54++) {
      c1_b_inData[c1_i54 + c1_i52] = (*(real_T (*)[81])c1_inData)[c1_i54 +
        c1_i52];
    }

    c1_i52 += 9;
  }

  c1_i55 = 0;
  for (c1_i56 = 0; c1_i56 < 9; c1_i56++) {
    for (c1_i57 = 0; c1_i57 < 9; c1_i57++) {
      c1_u[c1_i57 + c1_i55] = c1_b_inData[c1_i57 + c1_i55];
    }

    c1_i55 += 9;
  }

  c1_y = NULL;
  sf_mex_assign(&c1_y, sf_mex_create("y", c1_u, 0, 0U, 1U, 0U, 2, 9, 9), FALSE);
  sf_mex_assign(&c1_mxArrayOutData, c1_y, FALSE);
  return c1_mxArrayOutData;
}

static void c1_c_emlrt_marshallIn(SFc1_TMATS_NumericalMethodsInstanceStruct
  *chartInstance, const mxArray *c1_JOut, const char_T *c1_identifier, real_T
  c1_y[81])
{
  emlrtMsgIdentifier c1_thisId;
  c1_thisId.fIdentifier = c1_identifier;
  c1_thisId.fParent = NULL;
  c1_d_emlrt_marshallIn(chartInstance, sf_mex_dup(c1_JOut), &c1_thisId, c1_y);
  sf_mex_destroy(&c1_JOut);
}

static void c1_d_emlrt_marshallIn(SFc1_TMATS_NumericalMethodsInstanceStruct
  *chartInstance, const mxArray *c1_u, const emlrtMsgIdentifier *c1_parentId,
  real_T c1_y[81])
{
  real_T c1_dv6[81];
  int32_T c1_i58;
  sf_mex_import(c1_parentId, sf_mex_dup(c1_u), c1_dv6, 1, 0, 0U, 1, 0U, 2, 9, 9);
  for (c1_i58 = 0; c1_i58 < 81; c1_i58++) {
    c1_y[c1_i58] = c1_dv6[c1_i58];
  }

  sf_mex_destroy(&c1_u);
}

static void c1_b_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c1_mxArrayInData, const char_T *c1_varName, void *c1_outData)
{
  const mxArray *c1_JOut;
  const char_T *c1_identifier;
  emlrtMsgIdentifier c1_thisId;
  real_T c1_y[81];
  int32_T c1_i59;
  int32_T c1_i60;
  int32_T c1_i61;
  SFc1_TMATS_NumericalMethodsInstanceStruct *chartInstance;
  chartInstance = (SFc1_TMATS_NumericalMethodsInstanceStruct *)chartInstanceVoid;
  c1_JOut = sf_mex_dup(c1_mxArrayInData);
  c1_identifier = c1_varName;
  c1_thisId.fIdentifier = c1_identifier;
  c1_thisId.fParent = NULL;
  c1_d_emlrt_marshallIn(chartInstance, sf_mex_dup(c1_JOut), &c1_thisId, c1_y);
  sf_mex_destroy(&c1_JOut);
  c1_i59 = 0;
  for (c1_i60 = 0; c1_i60 < 9; c1_i60++) {
    for (c1_i61 = 0; c1_i61 < 9; c1_i61++) {
      (*(real_T (*)[81])c1_outData)[c1_i61 + c1_i59] = c1_y[c1_i61 + c1_i59];
    }

    c1_i59 += 9;
  }

  sf_mex_destroy(&c1_mxArrayInData);
}

static const mxArray *c1_c_sf_marshallOut(void *chartInstanceVoid, void
  *c1_inData)
{
  const mxArray *c1_mxArrayOutData = NULL;
  int32_T c1_i62;
  real_T c1_b_inData[9];
  int32_T c1_i63;
  real_T c1_u[9];
  const mxArray *c1_y = NULL;
  SFc1_TMATS_NumericalMethodsInstanceStruct *chartInstance;
  chartInstance = (SFc1_TMATS_NumericalMethodsInstanceStruct *)chartInstanceVoid;
  c1_mxArrayOutData = NULL;
  for (c1_i62 = 0; c1_i62 < 9; c1_i62++) {
    c1_b_inData[c1_i62] = (*(real_T (*)[9])c1_inData)[c1_i62];
  }

  for (c1_i63 = 0; c1_i63 < 9; c1_i63++) {
    c1_u[c1_i63] = c1_b_inData[c1_i63];
  }

  c1_y = NULL;
  sf_mex_assign(&c1_y, sf_mex_create("y", c1_u, 0, 0U, 1U, 0U, 2, 1, 9), FALSE);
  sf_mex_assign(&c1_mxArrayOutData, c1_y, FALSE);
  return c1_mxArrayOutData;
}

static void c1_e_emlrt_marshallIn(SFc1_TMATS_NumericalMethodsInstanceStruct
  *chartInstance, const mxArray *c1_XpOut, const char_T *c1_identifier, real_T
  c1_y[9])
{
  emlrtMsgIdentifier c1_thisId;
  c1_thisId.fIdentifier = c1_identifier;
  c1_thisId.fParent = NULL;
  c1_f_emlrt_marshallIn(chartInstance, sf_mex_dup(c1_XpOut), &c1_thisId, c1_y);
  sf_mex_destroy(&c1_XpOut);
}

static void c1_f_emlrt_marshallIn(SFc1_TMATS_NumericalMethodsInstanceStruct
  *chartInstance, const mxArray *c1_u, const emlrtMsgIdentifier *c1_parentId,
  real_T c1_y[9])
{
  real_T c1_dv7[9];
  int32_T c1_i64;
  sf_mex_import(c1_parentId, sf_mex_dup(c1_u), c1_dv7, 1, 0, 0U, 1, 0U, 2, 1, 9);
  for (c1_i64 = 0; c1_i64 < 9; c1_i64++) {
    c1_y[c1_i64] = c1_dv7[c1_i64];
  }

  sf_mex_destroy(&c1_u);
}

static void c1_c_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c1_mxArrayInData, const char_T *c1_varName, void *c1_outData)
{
  const mxArray *c1_XpOut;
  const char_T *c1_identifier;
  emlrtMsgIdentifier c1_thisId;
  real_T c1_y[9];
  int32_T c1_i65;
  SFc1_TMATS_NumericalMethodsInstanceStruct *chartInstance;
  chartInstance = (SFc1_TMATS_NumericalMethodsInstanceStruct *)chartInstanceVoid;
  c1_XpOut = sf_mex_dup(c1_mxArrayInData);
  c1_identifier = c1_varName;
  c1_thisId.fIdentifier = c1_identifier;
  c1_thisId.fParent = NULL;
  c1_f_emlrt_marshallIn(chartInstance, sf_mex_dup(c1_XpOut), &c1_thisId, c1_y);
  sf_mex_destroy(&c1_XpOut);
  for (c1_i65 = 0; c1_i65 < 9; c1_i65++) {
    (*(real_T (*)[9])c1_outData)[c1_i65] = c1_y[c1_i65];
  }

  sf_mex_destroy(&c1_mxArrayInData);
}

static const mxArray *c1_d_sf_marshallOut(void *chartInstanceVoid, void
  *c1_inData)
{
  const mxArray *c1_mxArrayOutData = NULL;
  int32_T c1_i66;
  real_T c1_b_inData[9];
  int32_T c1_i67;
  real_T c1_u[9];
  const mxArray *c1_y = NULL;
  SFc1_TMATS_NumericalMethodsInstanceStruct *chartInstance;
  chartInstance = (SFc1_TMATS_NumericalMethodsInstanceStruct *)chartInstanceVoid;
  c1_mxArrayOutData = NULL;
  for (c1_i66 = 0; c1_i66 < 9; c1_i66++) {
    c1_b_inData[c1_i66] = (*(real_T (*)[9])c1_inData)[c1_i66];
  }

  for (c1_i67 = 0; c1_i67 < 9; c1_i67++) {
    c1_u[c1_i67] = c1_b_inData[c1_i67];
  }

  c1_y = NULL;
  sf_mex_assign(&c1_y, sf_mex_create("y", c1_u, 0, 0U, 1U, 0U, 1, 9), FALSE);
  sf_mex_assign(&c1_mxArrayOutData, c1_y, FALSE);
  return c1_mxArrayOutData;
}

static const mxArray *c1_e_sf_marshallOut(void *chartInstanceVoid, void
  *c1_inData)
{
  const mxArray *c1_mxArrayOutData = NULL;
  real_T c1_u;
  const mxArray *c1_y = NULL;
  SFc1_TMATS_NumericalMethodsInstanceStruct *chartInstance;
  chartInstance = (SFc1_TMATS_NumericalMethodsInstanceStruct *)chartInstanceVoid;
  c1_mxArrayOutData = NULL;
  c1_u = *(real_T *)c1_inData;
  c1_y = NULL;
  sf_mex_assign(&c1_y, sf_mex_create("y", &c1_u, 0, 0U, 0U, 0U, 0), FALSE);
  sf_mex_assign(&c1_mxArrayOutData, c1_y, FALSE);
  return c1_mxArrayOutData;
}

static real_T c1_g_emlrt_marshallIn(SFc1_TMATS_NumericalMethodsInstanceStruct
  *chartInstance, const mxArray *c1_u, const emlrtMsgIdentifier *c1_parentId)
{
  real_T c1_y;
  real_T c1_d0;
  sf_mex_import(c1_parentId, sf_mex_dup(c1_u), &c1_d0, 1, 0, 0U, 0, 0U, 0);
  c1_y = c1_d0;
  sf_mex_destroy(&c1_u);
  return c1_y;
}

static void c1_d_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c1_mxArrayInData, const char_T *c1_varName, void *c1_outData)
{
  const mxArray *c1_nargout;
  const char_T *c1_identifier;
  emlrtMsgIdentifier c1_thisId;
  real_T c1_y;
  SFc1_TMATS_NumericalMethodsInstanceStruct *chartInstance;
  chartInstance = (SFc1_TMATS_NumericalMethodsInstanceStruct *)chartInstanceVoid;
  c1_nargout = sf_mex_dup(c1_mxArrayInData);
  c1_identifier = c1_varName;
  c1_thisId.fIdentifier = c1_identifier;
  c1_thisId.fParent = NULL;
  c1_y = c1_g_emlrt_marshallIn(chartInstance, sf_mex_dup(c1_nargout), &c1_thisId);
  sf_mex_destroy(&c1_nargout);
  *(real_T *)c1_outData = c1_y;
  sf_mex_destroy(&c1_mxArrayInData);
}

const mxArray *sf_c1_TMATS_NumericalMethods_get_eml_resolved_functions_info(void)
{
  const mxArray *c1_nameCaptureInfo;
  c1_ResolvedFunctionInfo c1_info[124];
  const mxArray *c1_m0 = NULL;
  int32_T c1_i68;
  c1_ResolvedFunctionInfo *c1_r0;
  c1_nameCaptureInfo = NULL;
  c1_nameCaptureInfo = NULL;
  c1_info_helper(c1_info);
  c1_b_info_helper(c1_info);
  sf_mex_assign(&c1_m0, sf_mex_createstruct("nameCaptureInfo", 1, 124), FALSE);
  for (c1_i68 = 0; c1_i68 < 124; c1_i68++) {
    c1_r0 = &c1_info[c1_i68];
    sf_mex_addfield(c1_m0, sf_mex_create("nameCaptureInfo", c1_r0->context, 15,
      0U, 0U, 0U, 2, 1, strlen(c1_r0->context)), "context", "nameCaptureInfo",
                    c1_i68);
    sf_mex_addfield(c1_m0, sf_mex_create("nameCaptureInfo", c1_r0->name, 15, 0U,
      0U, 0U, 2, 1, strlen(c1_r0->name)), "name", "nameCaptureInfo", c1_i68);
    sf_mex_addfield(c1_m0, sf_mex_create("nameCaptureInfo", c1_r0->dominantType,
      15, 0U, 0U, 0U, 2, 1, strlen(c1_r0->dominantType)), "dominantType",
                    "nameCaptureInfo", c1_i68);
    sf_mex_addfield(c1_m0, sf_mex_create("nameCaptureInfo", c1_r0->resolved, 15,
      0U, 0U, 0U, 2, 1, strlen(c1_r0->resolved)), "resolved", "nameCaptureInfo",
                    c1_i68);
    sf_mex_addfield(c1_m0, sf_mex_create("nameCaptureInfo", &c1_r0->fileTimeLo,
      7, 0U, 0U, 0U, 0), "fileTimeLo", "nameCaptureInfo", c1_i68);
    sf_mex_addfield(c1_m0, sf_mex_create("nameCaptureInfo", &c1_r0->fileTimeHi,
      7, 0U, 0U, 0U, 0), "fileTimeHi", "nameCaptureInfo", c1_i68);
    sf_mex_addfield(c1_m0, sf_mex_create("nameCaptureInfo", &c1_r0->mFileTimeLo,
      7, 0U, 0U, 0U, 0), "mFileTimeLo", "nameCaptureInfo", c1_i68);
    sf_mex_addfield(c1_m0, sf_mex_create("nameCaptureInfo", &c1_r0->mFileTimeHi,
      7, 0U, 0U, 0U, 0), "mFileTimeHi", "nameCaptureInfo", c1_i68);
  }

  sf_mex_assign(&c1_nameCaptureInfo, c1_m0, FALSE);
  sf_mex_emlrtNameCapturePostProcessR2012a(&c1_nameCaptureInfo);
  return c1_nameCaptureInfo;
}

static void c1_info_helper(c1_ResolvedFunctionInfo c1_info[124])
{
  c1_info[0].context = "";
  c1_info[0].name = "length";
  c1_info[0].dominantType = "double";
  c1_info[0].resolved =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elmat/length.m";
  c1_info[0].fileTimeLo = 1303167806U;
  c1_info[0].fileTimeHi = 0U;
  c1_info[0].mFileTimeLo = 0U;
  c1_info[0].mFileTimeHi = 0U;
  c1_info[1].context = "";
  c1_info[1].name = "mrdivide";
  c1_info[1].dominantType = "double";
  c1_info[1].resolved =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/ops/mrdivide.p";
  c1_info[1].fileTimeLo = 1342832544U;
  c1_info[1].fileTimeHi = 0U;
  c1_info[1].mFileTimeLo = 1319751566U;
  c1_info[1].mFileTimeHi = 0U;
  c1_info[2].context =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/ops/mrdivide.p";
  c1_info[2].name = "rdivide";
  c1_info[2].dominantType = "double";
  c1_info[2].resolved =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/ops/rdivide.m";
  c1_info[2].fileTimeLo = 1286840444U;
  c1_info[2].fileTimeHi = 0U;
  c1_info[2].mFileTimeLo = 0U;
  c1_info[2].mFileTimeHi = 0U;
  c1_info[3].context = "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/ops/rdivide.m";
  c1_info[3].name = "eml_div";
  c1_info[3].dominantType = "double";
  c1_info[3].resolved =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_div.m";
  c1_info[3].fileTimeLo = 1313369410U;
  c1_info[3].fileTimeHi = 0U;
  c1_info[3].mFileTimeLo = 0U;
  c1_info[3].mFileTimeHi = 0U;
  c1_info[4].context = "";
  c1_info[4].name = "inv";
  c1_info[4].dominantType = "double";
  c1_info[4].resolved = "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/matfun/inv.m";
  c1_info[4].fileTimeLo = 1305339600U;
  c1_info[4].fileTimeHi = 0U;
  c1_info[4].mFileTimeLo = 0U;
  c1_info[4].mFileTimeHi = 0U;
  c1_info[5].context =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/matfun/inv.m!invNxN";
  c1_info[5].name = "eml_index_class";
  c1_info[5].dominantType = "";
  c1_info[5].resolved =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_index_class.m";
  c1_info[5].fileTimeLo = 1323192178U;
  c1_info[5].fileTimeHi = 0U;
  c1_info[5].mFileTimeLo = 0U;
  c1_info[5].mFileTimeHi = 0U;
  c1_info[6].context =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/matfun/inv.m!invNxN";
  c1_info[6].name = "eml_xgetrf";
  c1_info[6].dominantType = "double";
  c1_info[6].resolved =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/lapack/eml_xgetrf.m";
  c1_info[6].fileTimeLo = 1286840406U;
  c1_info[6].fileTimeHi = 0U;
  c1_info[6].mFileTimeLo = 0U;
  c1_info[6].mFileTimeHi = 0U;
  c1_info[7].context =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/lapack/eml_xgetrf.m";
  c1_info[7].name = "eml_lapack_xgetrf";
  c1_info[7].dominantType = "double";
  c1_info[7].resolved =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/lapack/internal/eml_lapack_xgetrf.m";
  c1_info[7].fileTimeLo = 1286840410U;
  c1_info[7].fileTimeHi = 0U;
  c1_info[7].mFileTimeLo = 0U;
  c1_info[7].mFileTimeHi = 0U;
  c1_info[8].context =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/lapack/internal/eml_lapack_xgetrf.m";
  c1_info[8].name = "eml_matlab_zgetrf";
  c1_info[8].dominantType = "double";
  c1_info[8].resolved =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/lapack/matlab/eml_matlab_zgetrf.m";
  c1_info[8].fileTimeLo = 1302710594U;
  c1_info[8].fileTimeHi = 0U;
  c1_info[8].mFileTimeLo = 0U;
  c1_info[8].mFileTimeHi = 0U;
  c1_info[9].context =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/lapack/matlab/eml_matlab_zgetrf.m";
  c1_info[9].name = "realmin";
  c1_info[9].dominantType = "char";
  c1_info[9].resolved =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elmat/realmin.m";
  c1_info[9].fileTimeLo = 1307672842U;
  c1_info[9].fileTimeHi = 0U;
  c1_info[9].mFileTimeLo = 0U;
  c1_info[9].mFileTimeHi = 0U;
  c1_info[10].context =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elmat/realmin.m";
  c1_info[10].name = "eml_realmin";
  c1_info[10].dominantType = "char";
  c1_info[10].resolved =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_realmin.m";
  c1_info[10].fileTimeLo = 1307672844U;
  c1_info[10].fileTimeHi = 0U;
  c1_info[10].mFileTimeLo = 0U;
  c1_info[10].mFileTimeHi = 0U;
  c1_info[11].context =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_realmin.m";
  c1_info[11].name = "eml_float_model";
  c1_info[11].dominantType = "char";
  c1_info[11].resolved =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_float_model.m";
  c1_info[11].fileTimeLo = 1326749596U;
  c1_info[11].fileTimeHi = 0U;
  c1_info[11].mFileTimeLo = 0U;
  c1_info[11].mFileTimeHi = 0U;
  c1_info[12].context =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/lapack/matlab/eml_matlab_zgetrf.m";
  c1_info[12].name = "eps";
  c1_info[12].dominantType = "char";
  c1_info[12].resolved = "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elmat/eps.m";
  c1_info[12].fileTimeLo = 1326749596U;
  c1_info[12].fileTimeHi = 0U;
  c1_info[12].mFileTimeLo = 0U;
  c1_info[12].mFileTimeHi = 0U;
  c1_info[13].context = "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elmat/eps.m";
  c1_info[13].name = "eml_is_float_class";
  c1_info[13].dominantType = "char";
  c1_info[13].resolved =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_is_float_class.m";
  c1_info[13].fileTimeLo = 1286840382U;
  c1_info[13].fileTimeHi = 0U;
  c1_info[13].mFileTimeLo = 0U;
  c1_info[13].mFileTimeHi = 0U;
  c1_info[14].context = "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elmat/eps.m";
  c1_info[14].name = "eml_eps";
  c1_info[14].dominantType = "char";
  c1_info[14].resolved =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_eps.m";
  c1_info[14].fileTimeLo = 1326749596U;
  c1_info[14].fileTimeHi = 0U;
  c1_info[14].mFileTimeLo = 0U;
  c1_info[14].mFileTimeHi = 0U;
  c1_info[15].context =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_eps.m";
  c1_info[15].name = "eml_float_model";
  c1_info[15].dominantType = "char";
  c1_info[15].resolved =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_float_model.m";
  c1_info[15].fileTimeLo = 1326749596U;
  c1_info[15].fileTimeHi = 0U;
  c1_info[15].mFileTimeLo = 0U;
  c1_info[15].mFileTimeHi = 0U;
  c1_info[16].context =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/lapack/matlab/eml_matlab_zgetrf.m";
  c1_info[16].name = "min";
  c1_info[16].dominantType = "coder.internal.indexInt";
  c1_info[16].resolved =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/datafun/min.m";
  c1_info[16].fileTimeLo = 1311276918U;
  c1_info[16].fileTimeHi = 0U;
  c1_info[16].mFileTimeLo = 0U;
  c1_info[16].mFileTimeHi = 0U;
  c1_info[17].context =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/datafun/min.m";
  c1_info[17].name = "eml_min_or_max";
  c1_info[17].dominantType = "char";
  c1_info[17].resolved =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_min_or_max.m";
  c1_info[17].fileTimeLo = 1334093090U;
  c1_info[17].fileTimeHi = 0U;
  c1_info[17].mFileTimeLo = 0U;
  c1_info[17].mFileTimeHi = 0U;
  c1_info[18].context =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_min_or_max.m!eml_bin_extremum";
  c1_info[18].name = "eml_scalar_eg";
  c1_info[18].dominantType = "coder.internal.indexInt";
  c1_info[18].resolved =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_scalar_eg.m";
  c1_info[18].fileTimeLo = 1286840396U;
  c1_info[18].fileTimeHi = 0U;
  c1_info[18].mFileTimeLo = 0U;
  c1_info[18].mFileTimeHi = 0U;
  c1_info[19].context =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_min_or_max.m!eml_bin_extremum";
  c1_info[19].name = "eml_scalexp_alloc";
  c1_info[19].dominantType = "coder.internal.indexInt";
  c1_info[19].resolved =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_scalexp_alloc.m";
  c1_info[19].fileTimeLo = 1330630034U;
  c1_info[19].fileTimeHi = 0U;
  c1_info[19].mFileTimeLo = 0U;
  c1_info[19].mFileTimeHi = 0U;
  c1_info[20].context =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_min_or_max.m!eml_bin_extremum";
  c1_info[20].name = "eml_index_class";
  c1_info[20].dominantType = "";
  c1_info[20].resolved =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_index_class.m";
  c1_info[20].fileTimeLo = 1323192178U;
  c1_info[20].fileTimeHi = 0U;
  c1_info[20].mFileTimeLo = 0U;
  c1_info[20].mFileTimeHi = 0U;
  c1_info[21].context =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_min_or_max.m!eml_scalar_bin_extremum";
  c1_info[21].name = "eml_scalar_eg";
  c1_info[21].dominantType = "coder.internal.indexInt";
  c1_info[21].resolved =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_scalar_eg.m";
  c1_info[21].fileTimeLo = 1286840396U;
  c1_info[21].fileTimeHi = 0U;
  c1_info[21].mFileTimeLo = 0U;
  c1_info[21].mFileTimeHi = 0U;
  c1_info[22].context =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/lapack/matlab/eml_matlab_zgetrf.m";
  c1_info[22].name = "colon";
  c1_info[22].dominantType = "double";
  c1_info[22].resolved = "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/ops/colon.m";
  c1_info[22].fileTimeLo = 1311276918U;
  c1_info[22].fileTimeHi = 0U;
  c1_info[22].mFileTimeLo = 0U;
  c1_info[22].mFileTimeHi = 0U;
  c1_info[23].context = "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/ops/colon.m";
  c1_info[23].name = "floor";
  c1_info[23].dominantType = "double";
  c1_info[23].resolved =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elfun/floor.m";
  c1_info[23].fileTimeLo = 1286840342U;
  c1_info[23].fileTimeHi = 0U;
  c1_info[23].mFileTimeLo = 0U;
  c1_info[23].mFileTimeHi = 0U;
  c1_info[24].context =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elfun/floor.m";
  c1_info[24].name = "eml_scalar_floor";
  c1_info[24].dominantType = "double";
  c1_info[24].resolved =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elfun/eml_scalar_floor.m";
  c1_info[24].fileTimeLo = 1286840326U;
  c1_info[24].fileTimeHi = 0U;
  c1_info[24].mFileTimeLo = 0U;
  c1_info[24].mFileTimeHi = 0U;
  c1_info[25].context =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/ops/colon.m!checkrange";
  c1_info[25].name = "intmin";
  c1_info[25].dominantType = "char";
  c1_info[25].resolved =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elmat/intmin.m";
  c1_info[25].fileTimeLo = 1311276918U;
  c1_info[25].fileTimeHi = 0U;
  c1_info[25].mFileTimeLo = 0U;
  c1_info[25].mFileTimeHi = 0U;
  c1_info[26].context =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/ops/colon.m!checkrange";
  c1_info[26].name = "intmax";
  c1_info[26].dominantType = "char";
  c1_info[26].resolved =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elmat/intmax.m";
  c1_info[26].fileTimeLo = 1311276916U;
  c1_info[26].fileTimeHi = 0U;
  c1_info[26].mFileTimeLo = 0U;
  c1_info[26].mFileTimeHi = 0U;
  c1_info[27].context =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/ops/colon.m!eml_integer_colon_dispatcher";
  c1_info[27].name = "intmin";
  c1_info[27].dominantType = "char";
  c1_info[27].resolved =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elmat/intmin.m";
  c1_info[27].fileTimeLo = 1311276918U;
  c1_info[27].fileTimeHi = 0U;
  c1_info[27].mFileTimeLo = 0U;
  c1_info[27].mFileTimeHi = 0U;
  c1_info[28].context =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/ops/colon.m!eml_integer_colon_dispatcher";
  c1_info[28].name = "intmax";
  c1_info[28].dominantType = "char";
  c1_info[28].resolved =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elmat/intmax.m";
  c1_info[28].fileTimeLo = 1311276916U;
  c1_info[28].fileTimeHi = 0U;
  c1_info[28].mFileTimeLo = 0U;
  c1_info[28].mFileTimeHi = 0U;
  c1_info[29].context =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/ops/colon.m!eml_integer_colon_dispatcher";
  c1_info[29].name = "eml_isa_uint";
  c1_info[29].dominantType = "coder.internal.indexInt";
  c1_info[29].resolved =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_isa_uint.m";
  c1_info[29].fileTimeLo = 1286840384U;
  c1_info[29].fileTimeHi = 0U;
  c1_info[29].mFileTimeLo = 0U;
  c1_info[29].mFileTimeHi = 0U;
  c1_info[30].context =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/ops/colon.m!integer_colon_length_nonnegd";
  c1_info[30].name = "eml_unsigned_class";
  c1_info[30].dominantType = "char";
  c1_info[30].resolved =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_unsigned_class.m";
  c1_info[30].fileTimeLo = 1323192180U;
  c1_info[30].fileTimeHi = 0U;
  c1_info[30].mFileTimeLo = 0U;
  c1_info[30].mFileTimeHi = 0U;
  c1_info[31].context =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_unsigned_class.m";
  c1_info[31].name = "eml_index_class";
  c1_info[31].dominantType = "";
  c1_info[31].resolved =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_index_class.m";
  c1_info[31].fileTimeLo = 1323192178U;
  c1_info[31].fileTimeHi = 0U;
  c1_info[31].mFileTimeLo = 0U;
  c1_info[31].mFileTimeHi = 0U;
  c1_info[32].context =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/ops/colon.m!integer_colon_length_nonnegd";
  c1_info[32].name = "eml_index_class";
  c1_info[32].dominantType = "";
  c1_info[32].resolved =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_index_class.m";
  c1_info[32].fileTimeLo = 1323192178U;
  c1_info[32].fileTimeHi = 0U;
  c1_info[32].mFileTimeLo = 0U;
  c1_info[32].mFileTimeHi = 0U;
  c1_info[33].context =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/ops/colon.m!integer_colon_length_nonnegd";
  c1_info[33].name = "intmax";
  c1_info[33].dominantType = "char";
  c1_info[33].resolved =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elmat/intmax.m";
  c1_info[33].fileTimeLo = 1311276916U;
  c1_info[33].fileTimeHi = 0U;
  c1_info[33].mFileTimeLo = 0U;
  c1_info[33].mFileTimeHi = 0U;
  c1_info[34].context =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/ops/colon.m!integer_colon_length_nonnegd";
  c1_info[34].name = "eml_isa_uint";
  c1_info[34].dominantType = "coder.internal.indexInt";
  c1_info[34].resolved =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_isa_uint.m";
  c1_info[34].fileTimeLo = 1286840384U;
  c1_info[34].fileTimeHi = 0U;
  c1_info[34].mFileTimeLo = 0U;
  c1_info[34].mFileTimeHi = 0U;
  c1_info[35].context =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/ops/colon.m!integer_colon_length_nonnegd";
  c1_info[35].name = "eml_index_plus";
  c1_info[35].dominantType = "double";
  c1_info[35].resolved =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_index_plus.m";
  c1_info[35].fileTimeLo = 1286840378U;
  c1_info[35].fileTimeHi = 0U;
  c1_info[35].mFileTimeLo = 0U;
  c1_info[35].mFileTimeHi = 0U;
  c1_info[36].context =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_index_plus.m";
  c1_info[36].name = "eml_index_class";
  c1_info[36].dominantType = "";
  c1_info[36].resolved =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_index_class.m";
  c1_info[36].fileTimeLo = 1323192178U;
  c1_info[36].fileTimeHi = 0U;
  c1_info[36].mFileTimeLo = 0U;
  c1_info[36].mFileTimeHi = 0U;
  c1_info[37].context =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/ops/colon.m!eml_signed_integer_colon";
  c1_info[37].name = "eml_int_forloop_overflow_check";
  c1_info[37].dominantType = "";
  c1_info[37].resolved =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_int_forloop_overflow_check.m";
  c1_info[37].fileTimeLo = 1332186672U;
  c1_info[37].fileTimeHi = 0U;
  c1_info[37].mFileTimeLo = 0U;
  c1_info[37].mFileTimeHi = 0U;
  c1_info[38].context =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_int_forloop_overflow_check.m!eml_int_forloop_overflow_check_helper";
  c1_info[38].name = "intmax";
  c1_info[38].dominantType = "char";
  c1_info[38].resolved =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elmat/intmax.m";
  c1_info[38].fileTimeLo = 1311276916U;
  c1_info[38].fileTimeHi = 0U;
  c1_info[38].mFileTimeLo = 0U;
  c1_info[38].mFileTimeHi = 0U;
  c1_info[39].context =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/lapack/matlab/eml_matlab_zgetrf.m";
  c1_info[39].name = "eml_index_class";
  c1_info[39].dominantType = "";
  c1_info[39].resolved =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_index_class.m";
  c1_info[39].fileTimeLo = 1323192178U;
  c1_info[39].fileTimeHi = 0U;
  c1_info[39].mFileTimeLo = 0U;
  c1_info[39].mFileTimeHi = 0U;
  c1_info[40].context =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/lapack/matlab/eml_matlab_zgetrf.m";
  c1_info[40].name = "eml_index_plus";
  c1_info[40].dominantType = "double";
  c1_info[40].resolved =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_index_plus.m";
  c1_info[40].fileTimeLo = 1286840378U;
  c1_info[40].fileTimeHi = 0U;
  c1_info[40].mFileTimeLo = 0U;
  c1_info[40].mFileTimeHi = 0U;
  c1_info[41].context =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/lapack/matlab/eml_matlab_zgetrf.m";
  c1_info[41].name = "eml_int_forloop_overflow_check";
  c1_info[41].dominantType = "";
  c1_info[41].resolved =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_int_forloop_overflow_check.m";
  c1_info[41].fileTimeLo = 1332186672U;
  c1_info[41].fileTimeHi = 0U;
  c1_info[41].mFileTimeLo = 0U;
  c1_info[41].mFileTimeHi = 0U;
  c1_info[42].context =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/lapack/matlab/eml_matlab_zgetrf.m";
  c1_info[42].name = "eml_index_minus";
  c1_info[42].dominantType = "double";
  c1_info[42].resolved =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_index_minus.m";
  c1_info[42].fileTimeLo = 1286840378U;
  c1_info[42].fileTimeHi = 0U;
  c1_info[42].mFileTimeLo = 0U;
  c1_info[42].mFileTimeHi = 0U;
  c1_info[43].context =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_index_minus.m";
  c1_info[43].name = "eml_index_class";
  c1_info[43].dominantType = "";
  c1_info[43].resolved =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_index_class.m";
  c1_info[43].fileTimeLo = 1323192178U;
  c1_info[43].fileTimeHi = 0U;
  c1_info[43].mFileTimeLo = 0U;
  c1_info[43].mFileTimeHi = 0U;
  c1_info[44].context =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/lapack/matlab/eml_matlab_zgetrf.m";
  c1_info[44].name = "eml_index_minus";
  c1_info[44].dominantType = "coder.internal.indexInt";
  c1_info[44].resolved =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_index_minus.m";
  c1_info[44].fileTimeLo = 1286840378U;
  c1_info[44].fileTimeHi = 0U;
  c1_info[44].mFileTimeLo = 0U;
  c1_info[44].mFileTimeHi = 0U;
  c1_info[45].context =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/lapack/matlab/eml_matlab_zgetrf.m";
  c1_info[45].name = "eml_index_times";
  c1_info[45].dominantType = "coder.internal.indexInt";
  c1_info[45].resolved =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_index_times.m";
  c1_info[45].fileTimeLo = 1286840380U;
  c1_info[45].fileTimeHi = 0U;
  c1_info[45].mFileTimeLo = 0U;
  c1_info[45].mFileTimeHi = 0U;
  c1_info[46].context =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_index_times.m";
  c1_info[46].name = "eml_index_class";
  c1_info[46].dominantType = "";
  c1_info[46].resolved =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_index_class.m";
  c1_info[46].fileTimeLo = 1323192178U;
  c1_info[46].fileTimeHi = 0U;
  c1_info[46].mFileTimeLo = 0U;
  c1_info[46].mFileTimeHi = 0U;
  c1_info[47].context =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/lapack/matlab/eml_matlab_zgetrf.m";
  c1_info[47].name = "eml_index_plus";
  c1_info[47].dominantType = "coder.internal.indexInt";
  c1_info[47].resolved =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_index_plus.m";
  c1_info[47].fileTimeLo = 1286840378U;
  c1_info[47].fileTimeHi = 0U;
  c1_info[47].mFileTimeLo = 0U;
  c1_info[47].mFileTimeHi = 0U;
  c1_info[48].context =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/lapack/matlab/eml_matlab_zgetrf.m";
  c1_info[48].name = "eml_ixamax";
  c1_info[48].dominantType = "double";
  c1_info[48].resolved =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/blas/eml_ixamax.m";
  c1_info[48].fileTimeLo = 1299098370U;
  c1_info[48].fileTimeHi = 0U;
  c1_info[48].mFileTimeLo = 0U;
  c1_info[48].mFileTimeHi = 0U;
  c1_info[49].context =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/blas/eml_ixamax.m";
  c1_info[49].name = "eml_blas_inline";
  c1_info[49].dominantType = "";
  c1_info[49].resolved =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/blas/eml_blas_inline.m";
  c1_info[49].fileTimeLo = 1299098368U;
  c1_info[49].fileTimeHi = 0U;
  c1_info[49].mFileTimeLo = 0U;
  c1_info[49].mFileTimeHi = 0U;
  c1_info[50].context =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/blas/external/eml_blas_ixamax.m!below_threshold";
  c1_info[50].name = "length";
  c1_info[50].dominantType = "double";
  c1_info[50].resolved =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elmat/length.m";
  c1_info[50].fileTimeLo = 1303167806U;
  c1_info[50].fileTimeHi = 0U;
  c1_info[50].mFileTimeLo = 0U;
  c1_info[50].mFileTimeHi = 0U;
  c1_info[51].context =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elmat/length.m!intlength";
  c1_info[51].name = "eml_index_class";
  c1_info[51].dominantType = "";
  c1_info[51].resolved =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_index_class.m";
  c1_info[51].fileTimeLo = 1323192178U;
  c1_info[51].fileTimeHi = 0U;
  c1_info[51].mFileTimeLo = 0U;
  c1_info[51].mFileTimeHi = 0U;
  c1_info[52].context =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/blas/external/eml_blas_ixamax.m";
  c1_info[52].name = "eml_index_class";
  c1_info[52].dominantType = "";
  c1_info[52].resolved =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_index_class.m";
  c1_info[52].fileTimeLo = 1323192178U;
  c1_info[52].fileTimeHi = 0U;
  c1_info[52].mFileTimeLo = 0U;
  c1_info[52].mFileTimeHi = 0U;
  c1_info[53].context =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/blas/external/eml_blas_ixamax.m";
  c1_info[53].name = "eml_refblas_ixamax";
  c1_info[53].dominantType = "double";
  c1_info[53].resolved =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/blas/refblas/eml_refblas_ixamax.m";
  c1_info[53].fileTimeLo = 1299098370U;
  c1_info[53].fileTimeHi = 0U;
  c1_info[53].mFileTimeLo = 0U;
  c1_info[53].mFileTimeHi = 0U;
  c1_info[54].context =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/blas/refblas/eml_refblas_ixamax.m";
  c1_info[54].name = "eml_index_class";
  c1_info[54].dominantType = "";
  c1_info[54].resolved =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_index_class.m";
  c1_info[54].fileTimeLo = 1323192178U;
  c1_info[54].fileTimeHi = 0U;
  c1_info[54].mFileTimeLo = 0U;
  c1_info[54].mFileTimeHi = 0U;
  c1_info[55].context =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/blas/refblas/eml_refblas_ixamax.m";
  c1_info[55].name = "eml_xcabs1";
  c1_info[55].dominantType = "double";
  c1_info[55].resolved =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/blas/eml_xcabs1.m";
  c1_info[55].fileTimeLo = 1286840306U;
  c1_info[55].fileTimeHi = 0U;
  c1_info[55].mFileTimeLo = 0U;
  c1_info[55].mFileTimeHi = 0U;
  c1_info[56].context =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/blas/eml_xcabs1.m";
  c1_info[56].name = "abs";
  c1_info[56].dominantType = "double";
  c1_info[56].resolved = "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elfun/abs.m";
  c1_info[56].fileTimeLo = 1286840294U;
  c1_info[56].fileTimeHi = 0U;
  c1_info[56].mFileTimeLo = 0U;
  c1_info[56].mFileTimeHi = 0U;
  c1_info[57].context = "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elfun/abs.m";
  c1_info[57].name = "eml_scalar_abs";
  c1_info[57].dominantType = "double";
  c1_info[57].resolved =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elfun/eml_scalar_abs.m";
  c1_info[57].fileTimeLo = 1286840312U;
  c1_info[57].fileTimeHi = 0U;
  c1_info[57].mFileTimeLo = 0U;
  c1_info[57].mFileTimeHi = 0U;
  c1_info[58].context =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/blas/refblas/eml_refblas_ixamax.m";
  c1_info[58].name = "eml_int_forloop_overflow_check";
  c1_info[58].dominantType = "";
  c1_info[58].resolved =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_int_forloop_overflow_check.m";
  c1_info[58].fileTimeLo = 1332186672U;
  c1_info[58].fileTimeHi = 0U;
  c1_info[58].mFileTimeLo = 0U;
  c1_info[58].mFileTimeHi = 0U;
  c1_info[59].context =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/blas/refblas/eml_refblas_ixamax.m";
  c1_info[59].name = "eml_index_plus";
  c1_info[59].dominantType = "coder.internal.indexInt";
  c1_info[59].resolved =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_index_plus.m";
  c1_info[59].fileTimeLo = 1286840378U;
  c1_info[59].fileTimeHi = 0U;
  c1_info[59].mFileTimeLo = 0U;
  c1_info[59].mFileTimeHi = 0U;
  c1_info[60].context =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/lapack/matlab/eml_matlab_zgetrf.m";
  c1_info[60].name = "eml_xswap";
  c1_info[60].dominantType = "double";
  c1_info[60].resolved =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/blas/eml_xswap.m";
  c1_info[60].fileTimeLo = 1299098378U;
  c1_info[60].fileTimeHi = 0U;
  c1_info[60].mFileTimeLo = 0U;
  c1_info[60].mFileTimeHi = 0U;
  c1_info[61].context =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/blas/eml_xswap.m";
  c1_info[61].name = "eml_blas_inline";
  c1_info[61].dominantType = "";
  c1_info[61].resolved =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/blas/eml_blas_inline.m";
  c1_info[61].fileTimeLo = 1299098368U;
  c1_info[61].fileTimeHi = 0U;
  c1_info[61].mFileTimeLo = 0U;
  c1_info[61].mFileTimeHi = 0U;
  c1_info[62].context =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/blas/external/eml_blas_xswap.m";
  c1_info[62].name = "eml_index_class";
  c1_info[62].dominantType = "";
  c1_info[62].resolved =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_index_class.m";
  c1_info[62].fileTimeLo = 1323192178U;
  c1_info[62].fileTimeHi = 0U;
  c1_info[62].mFileTimeLo = 0U;
  c1_info[62].mFileTimeHi = 0U;
  c1_info[63].context =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/blas/external/eml_blas_xswap.m";
  c1_info[63].name = "eml_refblas_xswap";
  c1_info[63].dominantType = "double";
  c1_info[63].resolved =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/blas/refblas/eml_refblas_xswap.m";
  c1_info[63].fileTimeLo = 1299098386U;
  c1_info[63].fileTimeHi = 0U;
  c1_info[63].mFileTimeLo = 0U;
  c1_info[63].mFileTimeHi = 0U;
}

static void c1_b_info_helper(c1_ResolvedFunctionInfo c1_info[124])
{
  c1_info[64].context =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/blas/refblas/eml_refblas_xswap.m";
  c1_info[64].name = "eml_index_class";
  c1_info[64].dominantType = "";
  c1_info[64].resolved =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_index_class.m";
  c1_info[64].fileTimeLo = 1323192178U;
  c1_info[64].fileTimeHi = 0U;
  c1_info[64].mFileTimeLo = 0U;
  c1_info[64].mFileTimeHi = 0U;
  c1_info[65].context =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/blas/refblas/eml_refblas_xswap.m";
  c1_info[65].name = "abs";
  c1_info[65].dominantType = "coder.internal.indexInt";
  c1_info[65].resolved = "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elfun/abs.m";
  c1_info[65].fileTimeLo = 1286840294U;
  c1_info[65].fileTimeHi = 0U;
  c1_info[65].mFileTimeLo = 0U;
  c1_info[65].mFileTimeHi = 0U;
  c1_info[66].context = "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elfun/abs.m";
  c1_info[66].name = "eml_scalar_abs";
  c1_info[66].dominantType = "coder.internal.indexInt";
  c1_info[66].resolved =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elfun/eml_scalar_abs.m";
  c1_info[66].fileTimeLo = 1286840312U;
  c1_info[66].fileTimeHi = 0U;
  c1_info[66].mFileTimeLo = 0U;
  c1_info[66].mFileTimeHi = 0U;
  c1_info[67].context =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/blas/refblas/eml_refblas_xswap.m";
  c1_info[67].name = "eml_int_forloop_overflow_check";
  c1_info[67].dominantType = "";
  c1_info[67].resolved =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_int_forloop_overflow_check.m";
  c1_info[67].fileTimeLo = 1332186672U;
  c1_info[67].fileTimeHi = 0U;
  c1_info[67].mFileTimeLo = 0U;
  c1_info[67].mFileTimeHi = 0U;
  c1_info[68].context =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/blas/refblas/eml_refblas_xswap.m";
  c1_info[68].name = "eml_index_plus";
  c1_info[68].dominantType = "coder.internal.indexInt";
  c1_info[68].resolved =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_index_plus.m";
  c1_info[68].fileTimeLo = 1286840378U;
  c1_info[68].fileTimeHi = 0U;
  c1_info[68].mFileTimeLo = 0U;
  c1_info[68].mFileTimeHi = 0U;
  c1_info[69].context =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/lapack/matlab/eml_matlab_zgetrf.m";
  c1_info[69].name = "eml_div";
  c1_info[69].dominantType = "double";
  c1_info[69].resolved =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_div.m";
  c1_info[69].fileTimeLo = 1313369410U;
  c1_info[69].fileTimeHi = 0U;
  c1_info[69].mFileTimeLo = 0U;
  c1_info[69].mFileTimeHi = 0U;
  c1_info[70].context =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/lapack/matlab/eml_matlab_zgetrf.m";
  c1_info[70].name = "eml_xgeru";
  c1_info[70].dominantType = "double";
  c1_info[70].resolved =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/blas/eml_xgeru.m";
  c1_info[70].fileTimeLo = 1299098374U;
  c1_info[70].fileTimeHi = 0U;
  c1_info[70].mFileTimeLo = 0U;
  c1_info[70].mFileTimeHi = 0U;
  c1_info[71].context =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/blas/eml_xgeru.m";
  c1_info[71].name = "eml_blas_inline";
  c1_info[71].dominantType = "";
  c1_info[71].resolved =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/blas/eml_blas_inline.m";
  c1_info[71].fileTimeLo = 1299098368U;
  c1_info[71].fileTimeHi = 0U;
  c1_info[71].mFileTimeLo = 0U;
  c1_info[71].mFileTimeHi = 0U;
  c1_info[72].context =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/blas/eml_xgeru.m";
  c1_info[72].name = "eml_xger";
  c1_info[72].dominantType = "double";
  c1_info[72].resolved =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/blas/eml_xger.m";
  c1_info[72].fileTimeLo = 1299098374U;
  c1_info[72].fileTimeHi = 0U;
  c1_info[72].mFileTimeLo = 0U;
  c1_info[72].mFileTimeHi = 0U;
  c1_info[73].context =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/blas/eml_xger.m";
  c1_info[73].name = "eml_blas_inline";
  c1_info[73].dominantType = "";
  c1_info[73].resolved =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/blas/eml_blas_inline.m";
  c1_info[73].fileTimeLo = 1299098368U;
  c1_info[73].fileTimeHi = 0U;
  c1_info[73].mFileTimeLo = 0U;
  c1_info[73].mFileTimeHi = 0U;
  c1_info[74].context =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/blas/external/eml_blas_xger.m!below_threshold";
  c1_info[74].name = "intmax";
  c1_info[74].dominantType = "char";
  c1_info[74].resolved =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elmat/intmax.m";
  c1_info[74].fileTimeLo = 1311276916U;
  c1_info[74].fileTimeHi = 0U;
  c1_info[74].mFileTimeLo = 0U;
  c1_info[74].mFileTimeHi = 0U;
  c1_info[75].context =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/blas/external/eml_blas_xger.m!below_threshold";
  c1_info[75].name = "min";
  c1_info[75].dominantType = "double";
  c1_info[75].resolved =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/datafun/min.m";
  c1_info[75].fileTimeLo = 1311276918U;
  c1_info[75].fileTimeHi = 0U;
  c1_info[75].mFileTimeLo = 0U;
  c1_info[75].mFileTimeHi = 0U;
  c1_info[76].context =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_min_or_max.m!eml_bin_extremum";
  c1_info[76].name = "eml_scalar_eg";
  c1_info[76].dominantType = "double";
  c1_info[76].resolved =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_scalar_eg.m";
  c1_info[76].fileTimeLo = 1286840396U;
  c1_info[76].fileTimeHi = 0U;
  c1_info[76].mFileTimeLo = 0U;
  c1_info[76].mFileTimeHi = 0U;
  c1_info[77].context =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_min_or_max.m!eml_bin_extremum";
  c1_info[77].name = "eml_scalexp_alloc";
  c1_info[77].dominantType = "double";
  c1_info[77].resolved =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_scalexp_alloc.m";
  c1_info[77].fileTimeLo = 1330630034U;
  c1_info[77].fileTimeHi = 0U;
  c1_info[77].mFileTimeLo = 0U;
  c1_info[77].mFileTimeHi = 0U;
  c1_info[78].context =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_min_or_max.m!eml_scalar_bin_extremum";
  c1_info[78].name = "eml_scalar_eg";
  c1_info[78].dominantType = "double";
  c1_info[78].resolved =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_scalar_eg.m";
  c1_info[78].fileTimeLo = 1286840396U;
  c1_info[78].fileTimeHi = 0U;
  c1_info[78].mFileTimeLo = 0U;
  c1_info[78].mFileTimeHi = 0U;
  c1_info[79].context =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/blas/external/eml_blas_xger.m!below_threshold";
  c1_info[79].name = "mtimes";
  c1_info[79].dominantType = "double";
  c1_info[79].resolved =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/ops/mtimes.m";
  c1_info[79].fileTimeLo = 1289541292U;
  c1_info[79].fileTimeHi = 0U;
  c1_info[79].mFileTimeLo = 0U;
  c1_info[79].mFileTimeHi = 0U;
  c1_info[80].context =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/blas/external/eml_blas_xger.m";
  c1_info[80].name = "eml_index_class";
  c1_info[80].dominantType = "";
  c1_info[80].resolved =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_index_class.m";
  c1_info[80].fileTimeLo = 1323192178U;
  c1_info[80].fileTimeHi = 0U;
  c1_info[80].mFileTimeLo = 0U;
  c1_info[80].mFileTimeHi = 0U;
  c1_info[81].context =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/blas/external/eml_blas_xger.m";
  c1_info[81].name = "eml_refblas_xger";
  c1_info[81].dominantType = "double";
  c1_info[81].resolved =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/blas/refblas/eml_refblas_xger.m";
  c1_info[81].fileTimeLo = 1299098376U;
  c1_info[81].fileTimeHi = 0U;
  c1_info[81].mFileTimeLo = 0U;
  c1_info[81].mFileTimeHi = 0U;
  c1_info[82].context =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/blas/refblas/eml_refblas_xger.m";
  c1_info[82].name = "eml_refblas_xgerx";
  c1_info[82].dominantType = "char";
  c1_info[82].resolved =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/blas/refblas/eml_refblas_xgerx.m";
  c1_info[82].fileTimeLo = 1299098378U;
  c1_info[82].fileTimeHi = 0U;
  c1_info[82].mFileTimeLo = 0U;
  c1_info[82].mFileTimeHi = 0U;
  c1_info[83].context =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/blas/refblas/eml_refblas_xgerx.m";
  c1_info[83].name = "eml_index_class";
  c1_info[83].dominantType = "";
  c1_info[83].resolved =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_index_class.m";
  c1_info[83].fileTimeLo = 1323192178U;
  c1_info[83].fileTimeHi = 0U;
  c1_info[83].mFileTimeLo = 0U;
  c1_info[83].mFileTimeHi = 0U;
  c1_info[84].context =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/blas/refblas/eml_refblas_xgerx.m";
  c1_info[84].name = "abs";
  c1_info[84].dominantType = "coder.internal.indexInt";
  c1_info[84].resolved = "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elfun/abs.m";
  c1_info[84].fileTimeLo = 1286840294U;
  c1_info[84].fileTimeHi = 0U;
  c1_info[84].mFileTimeLo = 0U;
  c1_info[84].mFileTimeHi = 0U;
  c1_info[85].context =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/blas/refblas/eml_refblas_xgerx.m";
  c1_info[85].name = "eml_index_minus";
  c1_info[85].dominantType = "double";
  c1_info[85].resolved =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_index_minus.m";
  c1_info[85].fileTimeLo = 1286840378U;
  c1_info[85].fileTimeHi = 0U;
  c1_info[85].mFileTimeLo = 0U;
  c1_info[85].mFileTimeHi = 0U;
  c1_info[86].context =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/blas/refblas/eml_refblas_xgerx.m";
  c1_info[86].name = "eml_int_forloop_overflow_check";
  c1_info[86].dominantType = "";
  c1_info[86].resolved =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_int_forloop_overflow_check.m";
  c1_info[86].fileTimeLo = 1332186672U;
  c1_info[86].fileTimeHi = 0U;
  c1_info[86].mFileTimeLo = 0U;
  c1_info[86].mFileTimeHi = 0U;
  c1_info[87].context =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/blas/refblas/eml_refblas_xgerx.m";
  c1_info[87].name = "eml_index_plus";
  c1_info[87].dominantType = "double";
  c1_info[87].resolved =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_index_plus.m";
  c1_info[87].fileTimeLo = 1286840378U;
  c1_info[87].fileTimeHi = 0U;
  c1_info[87].mFileTimeLo = 0U;
  c1_info[87].mFileTimeHi = 0U;
  c1_info[88].context =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/blas/refblas/eml_refblas_xgerx.m";
  c1_info[88].name = "eml_index_plus";
  c1_info[88].dominantType = "coder.internal.indexInt";
  c1_info[88].resolved =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_index_plus.m";
  c1_info[88].fileTimeLo = 1286840378U;
  c1_info[88].fileTimeHi = 0U;
  c1_info[88].mFileTimeLo = 0U;
  c1_info[88].mFileTimeHi = 0U;
  c1_info[89].context =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/matfun/inv.m!invNxN";
  c1_info[89].name = "eml_ipiv2perm";
  c1_info[89].dominantType = "coder.internal.indexInt";
  c1_info[89].resolved =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_ipiv2perm.m";
  c1_info[89].fileTimeLo = 1286840382U;
  c1_info[89].fileTimeHi = 0U;
  c1_info[89].mFileTimeLo = 0U;
  c1_info[89].mFileTimeHi = 0U;
  c1_info[90].context =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_ipiv2perm.m";
  c1_info[90].name = "colon";
  c1_info[90].dominantType = "double";
  c1_info[90].resolved = "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/ops/colon.m";
  c1_info[90].fileTimeLo = 1311276918U;
  c1_info[90].fileTimeHi = 0U;
  c1_info[90].mFileTimeLo = 0U;
  c1_info[90].mFileTimeHi = 0U;
  c1_info[91].context =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_ipiv2perm.m";
  c1_info[91].name = "eml_index_class";
  c1_info[91].dominantType = "";
  c1_info[91].resolved =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_index_class.m";
  c1_info[91].fileTimeLo = 1323192178U;
  c1_info[91].fileTimeHi = 0U;
  c1_info[91].mFileTimeLo = 0U;
  c1_info[91].mFileTimeHi = 0U;
  c1_info[92].context =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_ipiv2perm.m";
  c1_info[92].name = "coder.internal.indexIntRelop";
  c1_info[92].dominantType = "";
  c1_info[92].resolved =
    "[IXE]$matlabroot$/toolbox/shared/coder/coder/+coder/+internal/indexIntRelop.m";
  c1_info[92].fileTimeLo = 1326749922U;
  c1_info[92].fileTimeHi = 0U;
  c1_info[92].mFileTimeLo = 0U;
  c1_info[92].mFileTimeHi = 0U;
  c1_info[93].context =
    "[IXE]$matlabroot$/toolbox/shared/coder/coder/+coder/+internal/indexIntRelop.m!float_class_contains_indexIntClass";
  c1_info[93].name = "eml_float_model";
  c1_info[93].dominantType = "char";
  c1_info[93].resolved =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_float_model.m";
  c1_info[93].fileTimeLo = 1326749596U;
  c1_info[93].fileTimeHi = 0U;
  c1_info[93].mFileTimeLo = 0U;
  c1_info[93].mFileTimeHi = 0U;
  c1_info[94].context =
    "[IXE]$matlabroot$/toolbox/shared/coder/coder/+coder/+internal/indexIntRelop.m!is_signed_indexIntClass";
  c1_info[94].name = "intmin";
  c1_info[94].dominantType = "char";
  c1_info[94].resolved =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elmat/intmin.m";
  c1_info[94].fileTimeLo = 1311276918U;
  c1_info[94].fileTimeHi = 0U;
  c1_info[94].mFileTimeLo = 0U;
  c1_info[94].mFileTimeHi = 0U;
  c1_info[95].context =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/matfun/inv.m!invNxN";
  c1_info[95].name = "eml_int_forloop_overflow_check";
  c1_info[95].dominantType = "";
  c1_info[95].resolved =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_int_forloop_overflow_check.m";
  c1_info[95].fileTimeLo = 1332186672U;
  c1_info[95].fileTimeHi = 0U;
  c1_info[95].mFileTimeLo = 0U;
  c1_info[95].mFileTimeHi = 0U;
  c1_info[96].context =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/matfun/inv.m!invNxN";
  c1_info[96].name = "eml_index_plus";
  c1_info[96].dominantType = "double";
  c1_info[96].resolved =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_index_plus.m";
  c1_info[96].fileTimeLo = 1286840378U;
  c1_info[96].fileTimeHi = 0U;
  c1_info[96].mFileTimeLo = 0U;
  c1_info[96].mFileTimeHi = 0U;
  c1_info[97].context =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/matfun/inv.m!invNxN";
  c1_info[97].name = "mtimes";
  c1_info[97].dominantType = "double";
  c1_info[97].resolved =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/ops/mtimes.m";
  c1_info[97].fileTimeLo = 1289541292U;
  c1_info[97].fileTimeHi = 0U;
  c1_info[97].mFileTimeLo = 0U;
  c1_info[97].mFileTimeHi = 0U;
  c1_info[98].context =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/matfun/inv.m!invNxN";
  c1_info[98].name = "eml_scalar_eg";
  c1_info[98].dominantType = "double";
  c1_info[98].resolved =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_scalar_eg.m";
  c1_info[98].fileTimeLo = 1286840396U;
  c1_info[98].fileTimeHi = 0U;
  c1_info[98].mFileTimeLo = 0U;
  c1_info[98].mFileTimeHi = 0U;
  c1_info[99].context =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/matfun/inv.m!invNxN";
  c1_info[99].name = "eml_xtrsm";
  c1_info[99].dominantType = "char";
  c1_info[99].resolved =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/blas/eml_xtrsm.m";
  c1_info[99].fileTimeLo = 1299098378U;
  c1_info[99].fileTimeHi = 0U;
  c1_info[99].mFileTimeLo = 0U;
  c1_info[99].mFileTimeHi = 0U;
  c1_info[100].context =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/blas/eml_xtrsm.m";
  c1_info[100].name = "eml_blas_inline";
  c1_info[100].dominantType = "";
  c1_info[100].resolved =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/blas/eml_blas_inline.m";
  c1_info[100].fileTimeLo = 1299098368U;
  c1_info[100].fileTimeHi = 0U;
  c1_info[100].mFileTimeLo = 0U;
  c1_info[100].mFileTimeHi = 0U;
  c1_info[101].context =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/blas/external/eml_blas_xtrsm.m!below_threshold";
  c1_info[101].name = "mtimes";
  c1_info[101].dominantType = "double";
  c1_info[101].resolved =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/ops/mtimes.m";
  c1_info[101].fileTimeLo = 1289541292U;
  c1_info[101].fileTimeHi = 0U;
  c1_info[101].mFileTimeLo = 0U;
  c1_info[101].mFileTimeHi = 0U;
  c1_info[102].context =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/blas/external/eml_blas_xtrsm.m";
  c1_info[102].name = "eml_index_class";
  c1_info[102].dominantType = "";
  c1_info[102].resolved =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_index_class.m";
  c1_info[102].fileTimeLo = 1323192178U;
  c1_info[102].fileTimeHi = 0U;
  c1_info[102].mFileTimeLo = 0U;
  c1_info[102].mFileTimeHi = 0U;
  c1_info[103].context =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/blas/external/eml_blas_xtrsm.m";
  c1_info[103].name = "eml_scalar_eg";
  c1_info[103].dominantType = "double";
  c1_info[103].resolved =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_scalar_eg.m";
  c1_info[103].fileTimeLo = 1286840396U;
  c1_info[103].fileTimeHi = 0U;
  c1_info[103].mFileTimeLo = 0U;
  c1_info[103].mFileTimeHi = 0U;
  c1_info[104].context =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/blas/external/eml_blas_xtrsm.m";
  c1_info[104].name = "eml_refblas_xtrsm";
  c1_info[104].dominantType = "char";
  c1_info[104].resolved =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/blas/refblas/eml_refblas_xtrsm.m";
  c1_info[104].fileTimeLo = 1299098386U;
  c1_info[104].fileTimeHi = 0U;
  c1_info[104].mFileTimeLo = 0U;
  c1_info[104].mFileTimeHi = 0U;
  c1_info[105].context =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/blas/refblas/eml_refblas_xtrsm.m";
  c1_info[105].name = "eml_scalar_eg";
  c1_info[105].dominantType = "double";
  c1_info[105].resolved =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_scalar_eg.m";
  c1_info[105].fileTimeLo = 1286840396U;
  c1_info[105].fileTimeHi = 0U;
  c1_info[105].mFileTimeLo = 0U;
  c1_info[105].mFileTimeHi = 0U;
  c1_info[106].context =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/blas/refblas/eml_refblas_xtrsm.m";
  c1_info[106].name = "eml_index_minus";
  c1_info[106].dominantType = "double";
  c1_info[106].resolved =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_index_minus.m";
  c1_info[106].fileTimeLo = 1286840378U;
  c1_info[106].fileTimeHi = 0U;
  c1_info[106].mFileTimeLo = 0U;
  c1_info[106].mFileTimeHi = 0U;
  c1_info[107].context =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/blas/refblas/eml_refblas_xtrsm.m";
  c1_info[107].name = "eml_index_class";
  c1_info[107].dominantType = "";
  c1_info[107].resolved =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_index_class.m";
  c1_info[107].fileTimeLo = 1323192178U;
  c1_info[107].fileTimeHi = 0U;
  c1_info[107].mFileTimeLo = 0U;
  c1_info[107].mFileTimeHi = 0U;
  c1_info[108].context =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/blas/refblas/eml_refblas_xtrsm.m";
  c1_info[108].name = "eml_int_forloop_overflow_check";
  c1_info[108].dominantType = "";
  c1_info[108].resolved =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_int_forloop_overflow_check.m";
  c1_info[108].fileTimeLo = 1332186672U;
  c1_info[108].fileTimeHi = 0U;
  c1_info[108].mFileTimeLo = 0U;
  c1_info[108].mFileTimeHi = 0U;
  c1_info[109].context =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/blas/refblas/eml_refblas_xtrsm.m";
  c1_info[109].name = "eml_index_times";
  c1_info[109].dominantType = "coder.internal.indexInt";
  c1_info[109].resolved =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_index_times.m";
  c1_info[109].fileTimeLo = 1286840380U;
  c1_info[109].fileTimeHi = 0U;
  c1_info[109].mFileTimeLo = 0U;
  c1_info[109].mFileTimeHi = 0U;
  c1_info[110].context =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/blas/refblas/eml_refblas_xtrsm.m";
  c1_info[110].name = "eml_index_plus";
  c1_info[110].dominantType = "coder.internal.indexInt";
  c1_info[110].resolved =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_index_plus.m";
  c1_info[110].fileTimeLo = 1286840378U;
  c1_info[110].fileTimeHi = 0U;
  c1_info[110].mFileTimeLo = 0U;
  c1_info[110].mFileTimeHi = 0U;
  c1_info[111].context =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_int_forloop_overflow_check.m!eml_int_forloop_overflow_check_helper";
  c1_info[111].name = "intmin";
  c1_info[111].dominantType = "char";
  c1_info[111].resolved =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elmat/intmin.m";
  c1_info[111].fileTimeLo = 1311276918U;
  c1_info[111].fileTimeHi = 0U;
  c1_info[111].mFileTimeLo = 0U;
  c1_info[111].mFileTimeHi = 0U;
  c1_info[112].context =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/blas/refblas/eml_refblas_xtrsm.m";
  c1_info[112].name = "eml_div";
  c1_info[112].dominantType = "double";
  c1_info[112].resolved =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_div.m";
  c1_info[112].fileTimeLo = 1313369410U;
  c1_info[112].fileTimeHi = 0U;
  c1_info[112].mFileTimeLo = 0U;
  c1_info[112].mFileTimeHi = 0U;
  c1_info[113].context =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/matfun/inv.m!checkcond";
  c1_info[113].name = "norm";
  c1_info[113].dominantType = "double";
  c1_info[113].resolved =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/matfun/norm.m";
  c1_info[113].fileTimeLo = 1336543694U;
  c1_info[113].fileTimeHi = 0U;
  c1_info[113].mFileTimeLo = 0U;
  c1_info[113].mFileTimeHi = 0U;
  c1_info[114].context =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/matfun/norm.m!mat1norm";
  c1_info[114].name = "abs";
  c1_info[114].dominantType = "double";
  c1_info[114].resolved =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elfun/abs.m";
  c1_info[114].fileTimeLo = 1286840294U;
  c1_info[114].fileTimeHi = 0U;
  c1_info[114].mFileTimeLo = 0U;
  c1_info[114].mFileTimeHi = 0U;
  c1_info[115].context =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/matfun/norm.m!mat1norm";
  c1_info[115].name = "isnan";
  c1_info[115].dominantType = "double";
  c1_info[115].resolved =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elmat/isnan.m";
  c1_info[115].fileTimeLo = 1286840360U;
  c1_info[115].fileTimeHi = 0U;
  c1_info[115].mFileTimeLo = 0U;
  c1_info[115].mFileTimeHi = 0U;
  c1_info[116].context =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/matfun/norm.m!mat1norm";
  c1_info[116].name = "eml_guarded_nan";
  c1_info[116].dominantType = "char";
  c1_info[116].resolved =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_guarded_nan.m";
  c1_info[116].fileTimeLo = 1286840376U;
  c1_info[116].fileTimeHi = 0U;
  c1_info[116].mFileTimeLo = 0U;
  c1_info[116].mFileTimeHi = 0U;
  c1_info[117].context =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_guarded_nan.m";
  c1_info[117].name = "eml_is_float_class";
  c1_info[117].dominantType = "char";
  c1_info[117].resolved =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_is_float_class.m";
  c1_info[117].fileTimeLo = 1286840382U;
  c1_info[117].fileTimeHi = 0U;
  c1_info[117].mFileTimeLo = 0U;
  c1_info[117].mFileTimeHi = 0U;
  c1_info[118].context =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/matfun/inv.m!checkcond";
  c1_info[118].name = "mtimes";
  c1_info[118].dominantType = "double";
  c1_info[118].resolved =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/ops/mtimes.m";
  c1_info[118].fileTimeLo = 1289541292U;
  c1_info[118].fileTimeHi = 0U;
  c1_info[118].mFileTimeLo = 0U;
  c1_info[118].mFileTimeHi = 0U;
  c1_info[119].context =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/matfun/inv.m!checkcond";
  c1_info[119].name = "eml_warning";
  c1_info[119].dominantType = "char";
  c1_info[119].resolved =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_warning.m";
  c1_info[119].fileTimeLo = 1286840402U;
  c1_info[119].fileTimeHi = 0U;
  c1_info[119].mFileTimeLo = 0U;
  c1_info[119].mFileTimeHi = 0U;
  c1_info[120].context =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/matfun/inv.m!checkcond";
  c1_info[120].name = "isnan";
  c1_info[120].dominantType = "double";
  c1_info[120].resolved =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elmat/isnan.m";
  c1_info[120].fileTimeLo = 1286840360U;
  c1_info[120].fileTimeHi = 0U;
  c1_info[120].mFileTimeLo = 0U;
  c1_info[120].mFileTimeHi = 0U;
  c1_info[121].context =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/matfun/inv.m!checkcond";
  c1_info[121].name = "eps";
  c1_info[121].dominantType = "char";
  c1_info[121].resolved =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/elmat/eps.m";
  c1_info[121].fileTimeLo = 1326749596U;
  c1_info[121].fileTimeHi = 0U;
  c1_info[121].mFileTimeLo = 0U;
  c1_info[121].mFileTimeHi = 0U;
  c1_info[122].context =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/matfun/inv.m!checkcond";
  c1_info[122].name = "eml_flt2str";
  c1_info[122].dominantType = "double";
  c1_info[122].resolved =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_flt2str.m";
  c1_info[122].fileTimeLo = 1309472796U;
  c1_info[122].fileTimeHi = 0U;
  c1_info[122].mFileTimeLo = 0U;
  c1_info[122].mFileTimeHi = 0U;
  c1_info[123].context =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_flt2str.m";
  c1_info[123].name = "char";
  c1_info[123].dominantType = "double";
  c1_info[123].resolved =
    "[ILXE]$matlabroot$/toolbox/eml/lib/matlab/strfun/char.m";
  c1_info[123].fileTimeLo = 1319751568U;
  c1_info[123].fileTimeHi = 0U;
  c1_info[123].mFileTimeLo = 0U;
  c1_info[123].mFileTimeHi = 0U;
}

static void c1_invNxN(SFc1_TMATS_NumericalMethodsInstanceStruct *chartInstance,
                      real_T c1_x[81], real_T c1_y[81])
{
  int32_T c1_i69;
  int32_T c1_info;
  int32_T c1_ipiv[9];
  int32_T c1_i70;
  int32_T c1_p[9];
  int32_T c1_k;
  real_T c1_b_k;
  int32_T c1_ipk;
  int32_T c1_a;
  real_T c1_b;
  int32_T c1_b_a;
  real_T c1_b_b;
  int32_T c1_idx;
  real_T c1_flt;
  boolean_T c1_b_p;
  int32_T c1_pipk;
  int32_T c1_c_k;
  int32_T c1_d_k;
  int32_T c1_c;
  int32_T c1_e_k;
  boolean_T c1_overflow;
  int32_T c1_j;
  int32_T c1_b_j;
  int32_T c1_c_a;
  int32_T c1_i71;
  boolean_T c1_b_overflow;
  int32_T c1_i;
  int32_T c1_b_i;
  real_T c1_d_a;
  real_T c1_c_b;
  real_T c1_b_y;
  int32_T c1_i72;
  real_T c1_b_x[81];
  for (c1_i69 = 0; c1_i69 < 81; c1_i69++) {
    c1_y[c1_i69] = 0.0;
  }

  c1_b_eml_matlab_zgetrf(chartInstance, c1_x, c1_ipiv, &c1_info);
  for (c1_i70 = 0; c1_i70 < 9; c1_i70++) {
    c1_p[c1_i70] = 1 + c1_i70;
  }

  for (c1_k = 0; c1_k < 8; c1_k++) {
    c1_b_k = 1.0 + (real_T)c1_k;
    c1_ipk = c1_ipiv[_SFD_EML_ARRAY_BOUNDS_CHECK("", (int32_T)_SFD_INTEGER_CHECK
      ("", c1_b_k), 1, 9, 1, 0) - 1];
    c1_a = c1_ipk;
    c1_b = c1_b_k;
    c1_b_a = c1_a;
    c1_b_b = c1_b;
    c1_idx = c1_b_a;
    c1_flt = c1_b_b;
    c1_b_p = ((real_T)c1_idx > c1_flt);
    if (c1_b_p) {
      c1_pipk = c1_p[_SFD_EML_ARRAY_BOUNDS_CHECK("", (int32_T)_SFD_INTEGER_CHECK
        ("", (real_T)c1_ipk), 1, 9, 1, 0) - 1];
      c1_p[_SFD_EML_ARRAY_BOUNDS_CHECK("", (int32_T)_SFD_INTEGER_CHECK("",
        (real_T)c1_ipk), 1, 9, 1, 0) - 1] = c1_p[_SFD_EML_ARRAY_BOUNDS_CHECK("",
        (int32_T)_SFD_INTEGER_CHECK("", c1_b_k), 1, 9, 1, 0) - 1];
      c1_p[_SFD_EML_ARRAY_BOUNDS_CHECK("", (int32_T)_SFD_INTEGER_CHECK("",
        c1_b_k), 1, 9, 1, 0) - 1] = c1_pipk;
    }
  }

  c1_check_forloop_overflow_error(chartInstance);
  for (c1_c_k = 1; c1_c_k < 10; c1_c_k++) {
    c1_d_k = c1_c_k;
    c1_c = c1_p[_SFD_EML_ARRAY_BOUNDS_CHECK("", (int32_T)_SFD_INTEGER_CHECK("",
      (real_T)c1_d_k), 1, 9, 1, 0) - 1];
    c1_y[(_SFD_EML_ARRAY_BOUNDS_CHECK("", (int32_T)_SFD_INTEGER_CHECK("",
            (real_T)c1_d_k), 1, 9, 1, 0) + 9 * (_SFD_EML_ARRAY_BOUNDS_CHECK("",
            (int32_T)_SFD_INTEGER_CHECK("", (real_T)c1_c), 1, 9, 2, 0) - 1)) - 1]
      = 1.0;
    c1_e_k = c1_d_k;
    c1_overflow = FALSE;
    c1_b_check_forloop_overflow_error(chartInstance, c1_overflow);
    for (c1_j = c1_e_k; c1_j < 10; c1_j++) {
      c1_b_j = c1_j;
      if (c1_y[(_SFD_EML_ARRAY_BOUNDS_CHECK("", (int32_T)_SFD_INTEGER_CHECK("",
             (real_T)c1_b_j), 1, 9, 1, 0) + 9 * (_SFD_EML_ARRAY_BOUNDS_CHECK("",
             (int32_T)_SFD_INTEGER_CHECK("", (real_T)c1_c), 1, 9, 2, 0) - 1)) -
          1] != 0.0) {
        c1_c_a = c1_b_j;
        c1_i71 = c1_c_a;
        c1_b_overflow = FALSE;
        c1_b_check_forloop_overflow_error(chartInstance, c1_b_overflow);
        for (c1_i = c1_i71 + 1; c1_i < 10; c1_i++) {
          c1_b_i = c1_i;
          c1_d_a = c1_y[(_SFD_EML_ARRAY_BOUNDS_CHECK("", (int32_T)
            _SFD_INTEGER_CHECK("", (real_T)c1_b_j), 1, 9, 1, 0) + 9 *
                         (_SFD_EML_ARRAY_BOUNDS_CHECK("", (int32_T)
            _SFD_INTEGER_CHECK("", (real_T)c1_c), 1, 9, 2, 0) - 1)) - 1];
          c1_c_b = c1_x[(_SFD_EML_ARRAY_BOUNDS_CHECK("", (int32_T)
            _SFD_INTEGER_CHECK("", (real_T)c1_b_i), 1, 9, 1, 0) + 9 *
                         (_SFD_EML_ARRAY_BOUNDS_CHECK("", (int32_T)
            _SFD_INTEGER_CHECK("", (real_T)c1_b_j), 1, 9, 2, 0) - 1)) - 1];
          c1_b_y = c1_d_a * c1_c_b;
          c1_y[(_SFD_EML_ARRAY_BOUNDS_CHECK("", (int32_T)_SFD_INTEGER_CHECK("",
                  (real_T)c1_b_i), 1, 9, 1, 0) + 9 *
                (_SFD_EML_ARRAY_BOUNDS_CHECK("", (int32_T)_SFD_INTEGER_CHECK("",
                   (real_T)c1_c), 1, 9, 2, 0) - 1)) - 1] = c1_y
            [(_SFD_EML_ARRAY_BOUNDS_CHECK("", (int32_T)_SFD_INTEGER_CHECK("",
                (real_T)c1_b_i), 1, 9, 1, 0) + 9 * (_SFD_EML_ARRAY_BOUNDS_CHECK(
                "", (int32_T)_SFD_INTEGER_CHECK("", (real_T)c1_c), 1, 9, 2, 0) -
               1)) - 1] - c1_b_y;
        }
      }
    }
  }

  for (c1_i72 = 0; c1_i72 < 81; c1_i72++) {
    c1_b_x[c1_i72] = c1_x[c1_i72];
  }

  c1_b_eml_xtrsm(chartInstance, c1_b_x, c1_y);
}

static void c1_realmin(SFc1_TMATS_NumericalMethodsInstanceStruct *chartInstance)
{
}

static void c1_eps(SFc1_TMATS_NumericalMethodsInstanceStruct *chartInstance)
{
}

static void c1_eml_matlab_zgetrf(SFc1_TMATS_NumericalMethodsInstanceStruct
  *chartInstance, real_T c1_A[81], real_T c1_b_A[81], int32_T c1_ipiv[9],
  int32_T *c1_info)
{
  int32_T c1_i73;
  for (c1_i73 = 0; c1_i73 < 81; c1_i73++) {
    c1_b_A[c1_i73] = c1_A[c1_i73];
  }

  c1_b_eml_matlab_zgetrf(chartInstance, c1_b_A, c1_ipiv, c1_info);
}

static void c1_check_forloop_overflow_error
  (SFc1_TMATS_NumericalMethodsInstanceStruct *chartInstance)
{
}

static void c1_b_check_forloop_overflow_error
  (SFc1_TMATS_NumericalMethodsInstanceStruct *chartInstance, boolean_T
   c1_overflow)
{
  int32_T c1_i74;
  static char_T c1_cv1[34] = { 'C', 'o', 'd', 'e', 'r', ':', 't', 'o', 'o', 'l',
    'b', 'o', 'x', ':', 'i', 'n', 't', '_', 'f', 'o', 'r', 'l', 'o', 'o', 'p',
    '_', 'o', 'v', 'e', 'r', 'f', 'l', 'o', 'w' };

  char_T c1_u[34];
  const mxArray *c1_y = NULL;
  int32_T c1_i75;
  static char_T c1_cv2[23] = { 'c', 'o', 'd', 'e', 'r', '.', 'i', 'n', 't', 'e',
    'r', 'n', 'a', 'l', '.', 'i', 'n', 'd', 'e', 'x', 'I', 'n', 't' };

  char_T c1_b_u[23];
  const mxArray *c1_b_y = NULL;
  if (!c1_overflow) {
  } else {
    for (c1_i74 = 0; c1_i74 < 34; c1_i74++) {
      c1_u[c1_i74] = c1_cv1[c1_i74];
    }

    c1_y = NULL;
    sf_mex_assign(&c1_y, sf_mex_create("y", c1_u, 10, 0U, 1U, 0U, 2, 1, 34),
                  FALSE);
    for (c1_i75 = 0; c1_i75 < 23; c1_i75++) {
      c1_b_u[c1_i75] = c1_cv2[c1_i75];
    }

    c1_b_y = NULL;
    sf_mex_assign(&c1_b_y, sf_mex_create("y", c1_b_u, 10, 0U, 1U, 0U, 2, 1, 23),
                  FALSE);
    sf_mex_call_debug("error", 0U, 1U, 14, sf_mex_call_debug("message", 1U, 2U,
      14, c1_y, 14, c1_b_y));
  }
}

static void c1_eml_xger(SFc1_TMATS_NumericalMethodsInstanceStruct *chartInstance,
  int32_T c1_m, int32_T c1_n, real_T c1_alpha1, int32_T c1_ix0, int32_T c1_iy0,
  real_T c1_A[81], int32_T c1_ia0, real_T c1_b_A[81])
{
  int32_T c1_i76;
  for (c1_i76 = 0; c1_i76 < 81; c1_i76++) {
    c1_b_A[c1_i76] = c1_A[c1_i76];
  }

  c1_b_eml_xger(chartInstance, c1_m, c1_n, c1_alpha1, c1_ix0, c1_iy0, c1_b_A,
                c1_ia0);
}

static void c1_eml_xtrsm(SFc1_TMATS_NumericalMethodsInstanceStruct
  *chartInstance, real_T c1_A[81], real_T c1_B[81], real_T c1_b_B[81])
{
  int32_T c1_i77;
  int32_T c1_i78;
  real_T c1_b_A[81];
  for (c1_i77 = 0; c1_i77 < 81; c1_i77++) {
    c1_b_B[c1_i77] = c1_B[c1_i77];
  }

  for (c1_i78 = 0; c1_i78 < 81; c1_i78++) {
    c1_b_A[c1_i78] = c1_A[c1_i78];
  }

  c1_b_eml_xtrsm(chartInstance, c1_b_A, c1_b_B);
}

static real_T c1_norm(SFc1_TMATS_NumericalMethodsInstanceStruct *chartInstance,
                      real_T c1_x[81])
{
  real_T c1_y;
  int32_T c1_j;
  real_T c1_b_j;
  real_T c1_s;
  int32_T c1_i;
  real_T c1_b_i;
  real_T c1_b_x;
  real_T c1_c_x;
  real_T c1_b_y;
  real_T c1_d_x;
  boolean_T c1_b;
  boolean_T exitg1;
  c1_y = 0.0;
  c1_j = 0;
  exitg1 = FALSE;
  while ((exitg1 == FALSE) && (c1_j < 9)) {
    c1_b_j = 1.0 + (real_T)c1_j;
    c1_s = 0.0;
    for (c1_i = 0; c1_i < 9; c1_i++) {
      c1_b_i = 1.0 + (real_T)c1_i;
      c1_b_x = c1_x[(_SFD_EML_ARRAY_BOUNDS_CHECK("", (int32_T)_SFD_INTEGER_CHECK
                      ("", c1_b_i), 1, 9, 1, 0) + 9 *
                     (_SFD_EML_ARRAY_BOUNDS_CHECK("", (int32_T)
        _SFD_INTEGER_CHECK("", c1_b_j), 1, 9, 2, 0) - 1)) - 1];
      c1_c_x = c1_b_x;
      c1_b_y = muDoubleScalarAbs(c1_c_x);
      c1_s += c1_b_y;
    }

    c1_d_x = c1_s;
    c1_b = muDoubleScalarIsNaN(c1_d_x);
    if (c1_b) {
      c1_y = rtNaN;
      exitg1 = TRUE;
    } else {
      if (c1_s > c1_y) {
        c1_y = c1_s;
      }

      c1_j++;
    }
  }

  return c1_y;
}

static void c1_eml_warning(SFc1_TMATS_NumericalMethodsInstanceStruct
  *chartInstance)
{
  int32_T c1_i79;
  static char_T c1_varargin_1[27] = { 'C', 'o', 'd', 'e', 'r', ':', 'M', 'A',
    'T', 'L', 'A', 'B', ':', 's', 'i', 'n', 'g', 'u', 'l', 'a', 'r', 'M', 'a',
    't', 'r', 'i', 'x' };

  char_T c1_u[27];
  const mxArray *c1_y = NULL;
  for (c1_i79 = 0; c1_i79 < 27; c1_i79++) {
    c1_u[c1_i79] = c1_varargin_1[c1_i79];
  }

  c1_y = NULL;
  sf_mex_assign(&c1_y, sf_mex_create("y", c1_u, 10, 0U, 1U, 0U, 2, 1, 27), FALSE);
  sf_mex_call_debug("warning", 0U, 1U, 14, sf_mex_call_debug("message", 1U, 1U,
    14, c1_y));
}

static void c1_b_eml_warning(SFc1_TMATS_NumericalMethodsInstanceStruct
  *chartInstance, char_T c1_varargin_2[14])
{
  int32_T c1_i80;
  static char_T c1_varargin_1[33] = { 'C', 'o', 'd', 'e', 'r', ':', 'M', 'A',
    'T', 'L', 'A', 'B', ':', 'i', 'l', 'l', 'C', 'o', 'n', 'd', 'i', 't', 'i',
    'o', 'n', 'e', 'd', 'M', 'a', 't', 'r', 'i', 'x' };

  char_T c1_u[33];
  const mxArray *c1_y = NULL;
  int32_T c1_i81;
  char_T c1_b_u[14];
  const mxArray *c1_b_y = NULL;
  for (c1_i80 = 0; c1_i80 < 33; c1_i80++) {
    c1_u[c1_i80] = c1_varargin_1[c1_i80];
  }

  c1_y = NULL;
  sf_mex_assign(&c1_y, sf_mex_create("y", c1_u, 10, 0U, 1U, 0U, 2, 1, 33), FALSE);
  for (c1_i81 = 0; c1_i81 < 14; c1_i81++) {
    c1_b_u[c1_i81] = c1_varargin_2[c1_i81];
  }

  c1_b_y = NULL;
  sf_mex_assign(&c1_b_y, sf_mex_create("y", c1_b_u, 10, 0U, 1U, 0U, 2, 1, 14),
                FALSE);
  sf_mex_call_debug("warning", 0U, 1U, 14, sf_mex_call_debug("message", 1U, 2U,
    14, c1_y, 14, c1_b_y));
}

static void c1_h_emlrt_marshallIn(SFc1_TMATS_NumericalMethodsInstanceStruct
  *chartInstance, const mxArray *c1_sprintf, const char_T *c1_identifier, char_T
  c1_y[14])
{
  emlrtMsgIdentifier c1_thisId;
  c1_thisId.fIdentifier = c1_identifier;
  c1_thisId.fParent = NULL;
  c1_i_emlrt_marshallIn(chartInstance, sf_mex_dup(c1_sprintf), &c1_thisId, c1_y);
  sf_mex_destroy(&c1_sprintf);
}

static void c1_i_emlrt_marshallIn(SFc1_TMATS_NumericalMethodsInstanceStruct
  *chartInstance, const mxArray *c1_u, const emlrtMsgIdentifier *c1_parentId,
  char_T c1_y[14])
{
  char_T c1_cv3[14];
  int32_T c1_i82;
  sf_mex_import(c1_parentId, sf_mex_dup(c1_u), c1_cv3, 1, 10, 0U, 1, 0U, 2, 1,
                14);
  for (c1_i82 = 0; c1_i82 < 14; c1_i82++) {
    c1_y[c1_i82] = c1_cv3[c1_i82];
  }

  sf_mex_destroy(&c1_u);
}

static const mxArray *c1_f_sf_marshallOut(void *chartInstanceVoid, void
  *c1_inData)
{
  const mxArray *c1_mxArrayOutData = NULL;
  int32_T c1_u;
  const mxArray *c1_y = NULL;
  SFc1_TMATS_NumericalMethodsInstanceStruct *chartInstance;
  chartInstance = (SFc1_TMATS_NumericalMethodsInstanceStruct *)chartInstanceVoid;
  c1_mxArrayOutData = NULL;
  c1_u = *(int32_T *)c1_inData;
  c1_y = NULL;
  sf_mex_assign(&c1_y, sf_mex_create("y", &c1_u, 6, 0U, 0U, 0U, 0), FALSE);
  sf_mex_assign(&c1_mxArrayOutData, c1_y, FALSE);
  return c1_mxArrayOutData;
}

static int32_T c1_j_emlrt_marshallIn(SFc1_TMATS_NumericalMethodsInstanceStruct
  *chartInstance, const mxArray *c1_u, const emlrtMsgIdentifier *c1_parentId)
{
  int32_T c1_y;
  int32_T c1_i83;
  sf_mex_import(c1_parentId, sf_mex_dup(c1_u), &c1_i83, 1, 6, 0U, 0, 0U, 0);
  c1_y = c1_i83;
  sf_mex_destroy(&c1_u);
  return c1_y;
}

static void c1_e_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c1_mxArrayInData, const char_T *c1_varName, void *c1_outData)
{
  const mxArray *c1_b_sfEvent;
  const char_T *c1_identifier;
  emlrtMsgIdentifier c1_thisId;
  int32_T c1_y;
  SFc1_TMATS_NumericalMethodsInstanceStruct *chartInstance;
  chartInstance = (SFc1_TMATS_NumericalMethodsInstanceStruct *)chartInstanceVoid;
  c1_b_sfEvent = sf_mex_dup(c1_mxArrayInData);
  c1_identifier = c1_varName;
  c1_thisId.fIdentifier = c1_identifier;
  c1_thisId.fParent = NULL;
  c1_y = c1_j_emlrt_marshallIn(chartInstance, sf_mex_dup(c1_b_sfEvent),
    &c1_thisId);
  sf_mex_destroy(&c1_b_sfEvent);
  *(int32_T *)c1_outData = c1_y;
  sf_mex_destroy(&c1_mxArrayInData);
}

static uint8_T c1_k_emlrt_marshallIn(SFc1_TMATS_NumericalMethodsInstanceStruct
  *chartInstance, const mxArray *c1_b_is_active_c1_TMATS_NumericalMethods, const
  char_T *c1_identifier)
{
  uint8_T c1_y;
  emlrtMsgIdentifier c1_thisId;
  c1_thisId.fIdentifier = c1_identifier;
  c1_thisId.fParent = NULL;
  c1_y = c1_l_emlrt_marshallIn(chartInstance, sf_mex_dup
    (c1_b_is_active_c1_TMATS_NumericalMethods), &c1_thisId);
  sf_mex_destroy(&c1_b_is_active_c1_TMATS_NumericalMethods);
  return c1_y;
}

static uint8_T c1_l_emlrt_marshallIn(SFc1_TMATS_NumericalMethodsInstanceStruct
  *chartInstance, const mxArray *c1_u, const emlrtMsgIdentifier *c1_parentId)
{
  uint8_T c1_y;
  uint8_T c1_u0;
  sf_mex_import(c1_parentId, sf_mex_dup(c1_u), &c1_u0, 1, 3, 0U, 0, 0U, 0);
  c1_y = c1_u0;
  sf_mex_destroy(&c1_u);
  return c1_y;
}

static void c1_b_eml_matlab_zgetrf(SFc1_TMATS_NumericalMethodsInstanceStruct
  *chartInstance, real_T c1_A[81], int32_T c1_ipiv[9], int32_T *c1_info)
{
  int32_T c1_i84;
  int32_T c1_j;
  int32_T c1_b_j;
  int32_T c1_a;
  int32_T c1_jm1;
  int32_T c1_b;
  int32_T c1_mmj;
  int32_T c1_b_a;
  int32_T c1_c;
  int32_T c1_b_b;
  int32_T c1_jj;
  int32_T c1_c_a;
  int32_T c1_jp1j;
  int32_T c1_d_a;
  int32_T c1_b_c;
  int32_T c1_n;
  int32_T c1_ix0;
  int32_T c1_b_n;
  int32_T c1_b_ix0;
  int32_T c1_c_n;
  int32_T c1_c_ix0;
  int32_T c1_idxmax;
  int32_T c1_ix;
  real_T c1_x;
  real_T c1_b_x;
  real_T c1_c_x;
  real_T c1_y;
  real_T c1_d_x;
  real_T c1_e_x;
  real_T c1_b_y;
  real_T c1_smax;
  int32_T c1_d_n;
  int32_T c1_c_b;
  int32_T c1_d_b;
  boolean_T c1_overflow;
  int32_T c1_k;
  int32_T c1_b_k;
  int32_T c1_e_a;
  real_T c1_f_x;
  real_T c1_g_x;
  real_T c1_h_x;
  real_T c1_c_y;
  real_T c1_i_x;
  real_T c1_j_x;
  real_T c1_d_y;
  real_T c1_s;
  int32_T c1_f_a;
  int32_T c1_jpiv_offset;
  int32_T c1_g_a;
  int32_T c1_e_b;
  int32_T c1_jpiv;
  int32_T c1_h_a;
  int32_T c1_f_b;
  int32_T c1_c_c;
  int32_T c1_g_b;
  int32_T c1_jrow;
  int32_T c1_i_a;
  int32_T c1_h_b;
  int32_T c1_jprow;
  int32_T c1_d_ix0;
  int32_T c1_iy0;
  int32_T c1_e_ix0;
  int32_T c1_b_iy0;
  int32_T c1_f_ix0;
  int32_T c1_c_iy0;
  int32_T c1_b_ix;
  int32_T c1_iy;
  int32_T c1_c_k;
  real_T c1_temp;
  int32_T c1_j_a;
  int32_T c1_k_a;
  int32_T c1_b_jp1j;
  int32_T c1_l_a;
  int32_T c1_d_c;
  int32_T c1_m_a;
  int32_T c1_i_b;
  int32_T c1_i85;
  int32_T c1_n_a;
  int32_T c1_j_b;
  int32_T c1_o_a;
  int32_T c1_k_b;
  boolean_T c1_b_overflow;
  int32_T c1_i;
  int32_T c1_b_i;
  real_T c1_k_x;
  real_T c1_e_y;
  real_T c1_z;
  int32_T c1_l_b;
  int32_T c1_e_c;
  int32_T c1_p_a;
  int32_T c1_f_c;
  int32_T c1_q_a;
  int32_T c1_g_c;
  int32_T c1_m;
  int32_T c1_e_n;
  int32_T c1_g_ix0;
  int32_T c1_d_iy0;
  int32_T c1_ia0;
  real_T c1_d1;
  c1_realmin(chartInstance);
  c1_eps(chartInstance);
  for (c1_i84 = 0; c1_i84 < 9; c1_i84++) {
    c1_ipiv[c1_i84] = 1 + c1_i84;
  }

  *c1_info = 0;
  c1_check_forloop_overflow_error(chartInstance);
  for (c1_j = 1; c1_j < 9; c1_j++) {
    c1_b_j = c1_j;
    c1_a = c1_b_j - 1;
    c1_jm1 = c1_a;
    c1_b = c1_b_j;
    c1_mmj = 9 - c1_b;
    c1_b_a = c1_jm1;
    c1_c = c1_b_a * 10;
    c1_b_b = c1_c + 1;
    c1_jj = c1_b_b;
    c1_c_a = c1_jj + 1;
    c1_jp1j = c1_c_a;
    c1_d_a = c1_mmj;
    c1_b_c = c1_d_a;
    c1_n = c1_b_c + 1;
    c1_ix0 = c1_jj;
    c1_b_n = c1_n;
    c1_b_ix0 = c1_ix0;
    c1_c_n = c1_b_n;
    c1_c_ix0 = c1_b_ix0;
    if (c1_c_n < 1) {
      c1_idxmax = 0;
    } else {
      c1_idxmax = 1;
      if (c1_c_n > 1) {
        c1_ix = c1_c_ix0;
        c1_x = c1_A[_SFD_EML_ARRAY_BOUNDS_CHECK("", (int32_T)_SFD_INTEGER_CHECK(
          "", (real_T)c1_ix), 1, 81, 1, 0) - 1];
        c1_b_x = c1_x;
        c1_c_x = c1_b_x;
        c1_y = muDoubleScalarAbs(c1_c_x);
        c1_d_x = 0.0;
        c1_e_x = c1_d_x;
        c1_b_y = muDoubleScalarAbs(c1_e_x);
        c1_smax = c1_y + c1_b_y;
        c1_d_n = c1_c_n;
        c1_c_b = c1_d_n;
        c1_d_b = c1_c_b;
        if (2 > c1_d_b) {
          c1_overflow = FALSE;
        } else {
          c1_overflow = (c1_d_b > 2147483646);
        }

        c1_b_check_forloop_overflow_error(chartInstance, c1_overflow);
        for (c1_k = 2; c1_k <= c1_d_n; c1_k++) {
          c1_b_k = c1_k;
          c1_e_a = c1_ix + 1;
          c1_ix = c1_e_a;
          c1_f_x = c1_A[_SFD_EML_ARRAY_BOUNDS_CHECK("", (int32_T)
            _SFD_INTEGER_CHECK("", (real_T)c1_ix), 1, 81, 1, 0) - 1];
          c1_g_x = c1_f_x;
          c1_h_x = c1_g_x;
          c1_c_y = muDoubleScalarAbs(c1_h_x);
          c1_i_x = 0.0;
          c1_j_x = c1_i_x;
          c1_d_y = muDoubleScalarAbs(c1_j_x);
          c1_s = c1_c_y + c1_d_y;
          if (c1_s > c1_smax) {
            c1_idxmax = c1_b_k;
            c1_smax = c1_s;
          }
        }
      }
    }

    c1_f_a = c1_idxmax - 1;
    c1_jpiv_offset = c1_f_a;
    c1_g_a = c1_jj;
    c1_e_b = c1_jpiv_offset;
    c1_jpiv = c1_g_a + c1_e_b;
    if (c1_A[_SFD_EML_ARRAY_BOUNDS_CHECK("", (int32_T)_SFD_INTEGER_CHECK("",
          (real_T)c1_jpiv), 1, 81, 1, 0) - 1] != 0.0) {
      if (c1_jpiv_offset != 0) {
        c1_h_a = c1_b_j;
        c1_f_b = c1_jpiv_offset;
        c1_c_c = c1_h_a + c1_f_b;
        c1_ipiv[_SFD_EML_ARRAY_BOUNDS_CHECK("", (int32_T)_SFD_INTEGER_CHECK("",
          (real_T)c1_b_j), 1, 9, 1, 0) - 1] = c1_c_c;
        c1_g_b = c1_jm1 + 1;
        c1_jrow = c1_g_b;
        c1_i_a = c1_jrow;
        c1_h_b = c1_jpiv_offset;
        c1_jprow = c1_i_a + c1_h_b;
        c1_d_ix0 = c1_jrow;
        c1_iy0 = c1_jprow;
        c1_e_ix0 = c1_d_ix0;
        c1_b_iy0 = c1_iy0;
        c1_f_ix0 = c1_e_ix0;
        c1_c_iy0 = c1_b_iy0;
        c1_b_ix = c1_f_ix0;
        c1_iy = c1_c_iy0;
        c1_check_forloop_overflow_error(chartInstance);
        for (c1_c_k = 1; c1_c_k < 10; c1_c_k++) {
          c1_temp = c1_A[_SFD_EML_ARRAY_BOUNDS_CHECK("", (int32_T)
            _SFD_INTEGER_CHECK("", (real_T)c1_b_ix), 1, 81, 1, 0) - 1];
          c1_A[_SFD_EML_ARRAY_BOUNDS_CHECK("", (int32_T)_SFD_INTEGER_CHECK("",
            (real_T)c1_b_ix), 1, 81, 1, 0) - 1] =
            c1_A[_SFD_EML_ARRAY_BOUNDS_CHECK("", (int32_T)_SFD_INTEGER_CHECK("",
            (real_T)c1_iy), 1, 81, 1, 0) - 1];
          c1_A[_SFD_EML_ARRAY_BOUNDS_CHECK("", (int32_T)_SFD_INTEGER_CHECK("",
            (real_T)c1_iy), 1, 81, 1, 0) - 1] = c1_temp;
          c1_j_a = c1_b_ix + 9;
          c1_b_ix = c1_j_a;
          c1_k_a = c1_iy + 9;
          c1_iy = c1_k_a;
        }
      }

      c1_b_jp1j = c1_jp1j;
      c1_l_a = c1_mmj;
      c1_d_c = c1_l_a;
      c1_m_a = c1_jp1j;
      c1_i_b = c1_d_c - 1;
      c1_i85 = c1_m_a + c1_i_b;
      c1_n_a = c1_b_jp1j;
      c1_j_b = c1_i85;
      c1_o_a = c1_n_a;
      c1_k_b = c1_j_b;
      if (c1_o_a > c1_k_b) {
        c1_b_overflow = FALSE;
      } else {
        c1_b_overflow = (c1_k_b > 2147483646);
      }

      c1_b_check_forloop_overflow_error(chartInstance, c1_b_overflow);
      for (c1_i = c1_b_jp1j; c1_i <= c1_i85; c1_i++) {
        c1_b_i = c1_i;
        c1_k_x = c1_A[_SFD_EML_ARRAY_BOUNDS_CHECK("", (int32_T)
          _SFD_INTEGER_CHECK("", (real_T)c1_b_i), 1, 81, 1, 0) - 1];
        c1_e_y = c1_A[_SFD_EML_ARRAY_BOUNDS_CHECK("", (int32_T)
          _SFD_INTEGER_CHECK("", (real_T)c1_jj), 1, 81, 1, 0) - 1];
        c1_z = c1_k_x / c1_e_y;
        c1_A[_SFD_EML_ARRAY_BOUNDS_CHECK("", (int32_T)_SFD_INTEGER_CHECK("",
          (real_T)c1_b_i), 1, 81, 1, 0) - 1] = c1_z;
      }
    } else {
      *c1_info = c1_b_j;
    }

    c1_l_b = c1_b_j;
    c1_e_c = 9 - c1_l_b;
    c1_p_a = c1_jj;
    c1_f_c = c1_p_a;
    c1_q_a = c1_jj;
    c1_g_c = c1_q_a;
    c1_m = c1_mmj;
    c1_e_n = c1_e_c;
    c1_g_ix0 = c1_jp1j;
    c1_d_iy0 = c1_f_c + 9;
    c1_ia0 = c1_g_c + 10;
    c1_d1 = -1.0;
    c1_b_eml_xger(chartInstance, c1_m, c1_e_n, c1_d1, c1_g_ix0, c1_d_iy0, c1_A,
                  c1_ia0);
  }

  if (*c1_info == 0) {
    if (!(c1_A[80] != 0.0)) {
      *c1_info = 9;
    }
  }
}

static void c1_b_eml_xger(SFc1_TMATS_NumericalMethodsInstanceStruct
  *chartInstance, int32_T c1_m, int32_T c1_n, real_T c1_alpha1, int32_T c1_ix0,
  int32_T c1_iy0, real_T c1_A[81], int32_T c1_ia0)
{
  int32_T c1_b_m;
  int32_T c1_b_n;
  real_T c1_b_alpha1;
  int32_T c1_b_ix0;
  int32_T c1_b_iy0;
  int32_T c1_b_ia0;
  int32_T c1_c_m;
  int32_T c1_c_n;
  real_T c1_c_alpha1;
  int32_T c1_c_ix0;
  int32_T c1_c_iy0;
  int32_T c1_c_ia0;
  int32_T c1_d_m;
  int32_T c1_d_n;
  real_T c1_d_alpha1;
  int32_T c1_d_ix0;
  int32_T c1_d_iy0;
  int32_T c1_d_ia0;
  int32_T c1_ixstart;
  int32_T c1_a;
  int32_T c1_jA;
  int32_T c1_jy;
  int32_T c1_e_n;
  int32_T c1_b;
  int32_T c1_b_b;
  boolean_T c1_overflow;
  int32_T c1_j;
  real_T c1_yjy;
  real_T c1_temp;
  int32_T c1_ix;
  int32_T c1_c_b;
  int32_T c1_i86;
  int32_T c1_b_a;
  int32_T c1_d_b;
  int32_T c1_i87;
  int32_T c1_c_a;
  int32_T c1_e_b;
  int32_T c1_d_a;
  int32_T c1_f_b;
  boolean_T c1_b_overflow;
  int32_T c1_ijA;
  int32_T c1_b_ijA;
  int32_T c1_e_a;
  int32_T c1_f_a;
  int32_T c1_g_a;
  c1_b_m = c1_m;
  c1_b_n = c1_n;
  c1_b_alpha1 = c1_alpha1;
  c1_b_ix0 = c1_ix0;
  c1_b_iy0 = c1_iy0;
  c1_b_ia0 = c1_ia0;
  c1_c_m = c1_b_m;
  c1_c_n = c1_b_n;
  c1_c_alpha1 = c1_b_alpha1;
  c1_c_ix0 = c1_b_ix0;
  c1_c_iy0 = c1_b_iy0;
  c1_c_ia0 = c1_b_ia0;
  c1_d_m = c1_c_m;
  c1_d_n = c1_c_n;
  c1_d_alpha1 = c1_c_alpha1;
  c1_d_ix0 = c1_c_ix0;
  c1_d_iy0 = c1_c_iy0;
  c1_d_ia0 = c1_c_ia0;
  if (c1_d_alpha1 == 0.0) {
  } else {
    c1_ixstart = c1_d_ix0;
    c1_a = c1_d_ia0 - 1;
    c1_jA = c1_a;
    c1_jy = c1_d_iy0;
    c1_e_n = c1_d_n;
    c1_b = c1_e_n;
    c1_b_b = c1_b;
    if (1 > c1_b_b) {
      c1_overflow = FALSE;
    } else {
      c1_overflow = (c1_b_b > 2147483646);
    }

    c1_b_check_forloop_overflow_error(chartInstance, c1_overflow);
    for (c1_j = 1; c1_j <= c1_e_n; c1_j++) {
      c1_yjy = c1_A[_SFD_EML_ARRAY_BOUNDS_CHECK("", (int32_T)_SFD_INTEGER_CHECK(
        "", (real_T)c1_jy), 1, 81, 1, 0) - 1];
      if (c1_yjy != 0.0) {
        c1_temp = c1_yjy * c1_d_alpha1;
        c1_ix = c1_ixstart;
        c1_c_b = c1_jA + 1;
        c1_i86 = c1_c_b;
        c1_b_a = c1_d_m;
        c1_d_b = c1_jA;
        c1_i87 = c1_b_a + c1_d_b;
        c1_c_a = c1_i86;
        c1_e_b = c1_i87;
        c1_d_a = c1_c_a;
        c1_f_b = c1_e_b;
        if (c1_d_a > c1_f_b) {
          c1_b_overflow = FALSE;
        } else {
          c1_b_overflow = (c1_f_b > 2147483646);
        }

        c1_b_check_forloop_overflow_error(chartInstance, c1_b_overflow);
        for (c1_ijA = c1_i86; c1_ijA <= c1_i87; c1_ijA++) {
          c1_b_ijA = c1_ijA;
          c1_A[_SFD_EML_ARRAY_BOUNDS_CHECK("", (int32_T)_SFD_INTEGER_CHECK("",
            (real_T)c1_b_ijA), 1, 81, 1, 0) - 1] =
            c1_A[_SFD_EML_ARRAY_BOUNDS_CHECK("", (int32_T)_SFD_INTEGER_CHECK("",
            (real_T)c1_b_ijA), 1, 81, 1, 0) - 1] +
            c1_A[_SFD_EML_ARRAY_BOUNDS_CHECK("", (int32_T)_SFD_INTEGER_CHECK("",
            (real_T)c1_ix), 1, 81, 1, 0) - 1] * c1_temp;
          c1_e_a = c1_ix + 1;
          c1_ix = c1_e_a;
        }
      }

      c1_f_a = c1_jy + 9;
      c1_jy = c1_f_a;
      c1_g_a = c1_jA + 9;
      c1_jA = c1_g_a;
    }
  }
}

static void c1_b_eml_xtrsm(SFc1_TMATS_NumericalMethodsInstanceStruct
  *chartInstance, real_T c1_A[81], real_T c1_B[81])
{
  int32_T c1_m;
  int32_T c1_n;
  real_T c1_alpha1;
  int32_T c1_lda;
  int32_T c1_ldb;
  char_T c1_DIAGA;
  char_T c1_TRANSA;
  char_T c1_UPLO;
  char_T c1_SIDE;
  c1_m = 9;
  c1_n = 9;
  c1_alpha1 = 1.0;
  c1_lda = 9;
  c1_ldb = 9;
  c1_DIAGA = 'N';
  c1_TRANSA = 'N';
  c1_UPLO = 'U';
  c1_SIDE = 'L';
  dtrsm32(&c1_SIDE, &c1_UPLO, &c1_TRANSA, &c1_DIAGA, &c1_m, &c1_n, &c1_alpha1,
          &c1_A[0], &c1_lda, &c1_B[0], &c1_ldb);
}

static void init_dsm_address_info(SFc1_TMATS_NumericalMethodsInstanceStruct
  *chartInstance)
{
}

/* SFunction Glue Code */
void sf_c1_TMATS_NumericalMethods_get_check_sum(mxArray *plhs[])
{
  ((real_T *)mxGetPr((plhs[0])))[0] = (real_T)(1236531207U);
  ((real_T *)mxGetPr((plhs[0])))[1] = (real_T)(1579667108U);
  ((real_T *)mxGetPr((plhs[0])))[2] = (real_T)(127028408U);
  ((real_T *)mxGetPr((plhs[0])))[3] = (real_T)(855481172U);
}

mxArray *sf_c1_TMATS_NumericalMethods_get_autoinheritance_info(void)
{
  const char *autoinheritanceFields[] = { "checksum", "inputs", "parameters",
    "outputs", "locals" };

  mxArray *mxAutoinheritanceInfo = mxCreateStructMatrix(1,1,5,
    autoinheritanceFields);

  {
    mxArray *mxChecksum = mxCreateString("GOdSdqa6Fk84gkC00EyvPF");
    mxSetField(mxAutoinheritanceInfo,0,"checksum",mxChecksum);
  }

  {
    const char *dataFields[] = { "size", "type", "complexity" };

    mxArray *mxData = mxCreateStructMatrix(1,6,3,dataFields);

    {
      mxArray *mxSize = mxCreateDoubleMatrix(1,2,mxREAL);
      double *pr = mxGetPr(mxSize);
      pr[0] = (double)(1);
      pr[1] = (double)(1);
      mxSetField(mxData,0,"size",mxSize);
    }

    {
      const char *typeFields[] = { "base", "fixpt" };

      mxArray *mxType = mxCreateStructMatrix(1,1,2,typeFields);
      mxSetField(mxType,0,"base",mxCreateDoubleScalar(10));
      mxSetField(mxType,0,"fixpt",mxCreateDoubleMatrix(0,0,mxREAL));
      mxSetField(mxData,0,"type",mxType);
    }

    mxSetField(mxData,0,"complexity",mxCreateDoubleScalar(0));

    {
      mxArray *mxSize = mxCreateDoubleMatrix(1,2,mxREAL);
      double *pr = mxGetPr(mxSize);
      pr[0] = (double)(9);
      pr[1] = (double)(1);
      mxSetField(mxData,1,"size",mxSize);
    }

    {
      const char *typeFields[] = { "base", "fixpt" };

      mxArray *mxType = mxCreateStructMatrix(1,1,2,typeFields);
      mxSetField(mxType,0,"base",mxCreateDoubleScalar(10));
      mxSetField(mxType,0,"fixpt",mxCreateDoubleMatrix(0,0,mxREAL));
      mxSetField(mxData,1,"type",mxType);
    }

    mxSetField(mxData,1,"complexity",mxCreateDoubleScalar(0));

    {
      mxArray *mxSize = mxCreateDoubleMatrix(1,2,mxREAL);
      double *pr = mxGetPr(mxSize);
      pr[0] = (double)(9);
      pr[1] = (double)(1);
      mxSetField(mxData,2,"size",mxSize);
    }

    {
      const char *typeFields[] = { "base", "fixpt" };

      mxArray *mxType = mxCreateStructMatrix(1,1,2,typeFields);
      mxSetField(mxType,0,"base",mxCreateDoubleScalar(10));
      mxSetField(mxType,0,"fixpt",mxCreateDoubleMatrix(0,0,mxREAL));
      mxSetField(mxData,2,"type",mxType);
    }

    mxSetField(mxData,2,"complexity",mxCreateDoubleScalar(0));

    {
      mxArray *mxSize = mxCreateDoubleMatrix(1,2,mxREAL);
      double *pr = mxGetPr(mxSize);
      pr[0] = (double)(9);
      pr[1] = (double)(1);
      mxSetField(mxData,3,"size",mxSize);
    }

    {
      const char *typeFields[] = { "base", "fixpt" };

      mxArray *mxType = mxCreateStructMatrix(1,1,2,typeFields);
      mxSetField(mxType,0,"base",mxCreateDoubleScalar(10));
      mxSetField(mxType,0,"fixpt",mxCreateDoubleMatrix(0,0,mxREAL));
      mxSetField(mxData,3,"type",mxType);
    }

    mxSetField(mxData,3,"complexity",mxCreateDoubleScalar(0));

    {
      mxArray *mxSize = mxCreateDoubleMatrix(1,2,mxREAL);
      double *pr = mxGetPr(mxSize);
      pr[0] = (double)(9);
      pr[1] = (double)(9);
      mxSetField(mxData,4,"size",mxSize);
    }

    {
      const char *typeFields[] = { "base", "fixpt" };

      mxArray *mxType = mxCreateStructMatrix(1,1,2,typeFields);
      mxSetField(mxType,0,"base",mxCreateDoubleScalar(10));
      mxSetField(mxType,0,"fixpt",mxCreateDoubleMatrix(0,0,mxREAL));
      mxSetField(mxData,4,"type",mxType);
    }

    mxSetField(mxData,4,"complexity",mxCreateDoubleScalar(0));

    {
      mxArray *mxSize = mxCreateDoubleMatrix(1,2,mxREAL);
      double *pr = mxGetPr(mxSize);
      pr[0] = (double)(9);
      pr[1] = (double)(10);
      mxSetField(mxData,5,"size",mxSize);
    }

    {
      const char *typeFields[] = { "base", "fixpt" };

      mxArray *mxType = mxCreateStructMatrix(1,1,2,typeFields);
      mxSetField(mxType,0,"base",mxCreateDoubleScalar(10));
      mxSetField(mxType,0,"fixpt",mxCreateDoubleMatrix(0,0,mxREAL));
      mxSetField(mxData,5,"type",mxType);
    }

    mxSetField(mxData,5,"complexity",mxCreateDoubleScalar(0));
    mxSetField(mxAutoinheritanceInfo,0,"inputs",mxData);
  }

  {
    mxSetField(mxAutoinheritanceInfo,0,"parameters",mxCreateDoubleMatrix(0,0,
                mxREAL));
  }

  {
    const char *dataFields[] = { "size", "type", "complexity" };

    mxArray *mxData = mxCreateStructMatrix(1,4,3,dataFields);

    {
      mxArray *mxSize = mxCreateDoubleMatrix(1,2,mxREAL);
      double *pr = mxGetPr(mxSize);
      pr[0] = (double)(1);
      pr[1] = (double)(9);
      mxSetField(mxData,0,"size",mxSize);
    }

    {
      const char *typeFields[] = { "base", "fixpt" };

      mxArray *mxType = mxCreateStructMatrix(1,1,2,typeFields);
      mxSetField(mxType,0,"base",mxCreateDoubleScalar(10));
      mxSetField(mxType,0,"fixpt",mxCreateDoubleMatrix(0,0,mxREAL));
      mxSetField(mxData,0,"type",mxType);
    }

    mxSetField(mxData,0,"complexity",mxCreateDoubleScalar(0));

    {
      mxArray *mxSize = mxCreateDoubleMatrix(1,2,mxREAL);
      double *pr = mxGetPr(mxSize);
      pr[0] = (double)(9);
      pr[1] = (double)(9);
      mxSetField(mxData,1,"size",mxSize);
    }

    {
      const char *typeFields[] = { "base", "fixpt" };

      mxArray *mxType = mxCreateStructMatrix(1,1,2,typeFields);
      mxSetField(mxType,0,"base",mxCreateDoubleScalar(10));
      mxSetField(mxType,0,"fixpt",mxCreateDoubleMatrix(0,0,mxREAL));
      mxSetField(mxData,1,"type",mxType);
    }

    mxSetField(mxData,1,"complexity",mxCreateDoubleScalar(0));

    {
      mxArray *mxSize = mxCreateDoubleMatrix(1,2,mxREAL);
      double *pr = mxGetPr(mxSize);
      pr[0] = (double)(9);
      pr[1] = (double)(9);
      mxSetField(mxData,2,"size",mxSize);
    }

    {
      const char *typeFields[] = { "base", "fixpt" };

      mxArray *mxType = mxCreateStructMatrix(1,1,2,typeFields);
      mxSetField(mxType,0,"base",mxCreateDoubleScalar(10));
      mxSetField(mxType,0,"fixpt",mxCreateDoubleMatrix(0,0,mxREAL));
      mxSetField(mxData,2,"type",mxType);
    }

    mxSetField(mxData,2,"complexity",mxCreateDoubleScalar(0));

    {
      mxArray *mxSize = mxCreateDoubleMatrix(1,2,mxREAL);
      double *pr = mxGetPr(mxSize);
      pr[0] = (double)(9);
      pr[1] = (double)(10);
      mxSetField(mxData,3,"size",mxSize);
    }

    {
      const char *typeFields[] = { "base", "fixpt" };

      mxArray *mxType = mxCreateStructMatrix(1,1,2,typeFields);
      mxSetField(mxType,0,"base",mxCreateDoubleScalar(10));
      mxSetField(mxType,0,"fixpt",mxCreateDoubleMatrix(0,0,mxREAL));
      mxSetField(mxData,3,"type",mxType);
    }

    mxSetField(mxData,3,"complexity",mxCreateDoubleScalar(0));
    mxSetField(mxAutoinheritanceInfo,0,"outputs",mxData);
  }

  {
    mxSetField(mxAutoinheritanceInfo,0,"locals",mxCreateDoubleMatrix(0,0,mxREAL));
  }

  return(mxAutoinheritanceInfo);
}

static const mxArray *sf_get_sim_state_info_c1_TMATS_NumericalMethods(void)
{
  const char *infoFields[] = { "chartChecksum", "varInfo" };

  mxArray *mxInfo = mxCreateStructMatrix(1, 1, 2, infoFields);
  const char *infoEncStr[] = {
    "100 S1x5'type','srcId','name','auxInfo'{{M[1],M[9],T\"InvJOut\",},{M[1],M[10],T\"JOut\",},{M[1],M[5],T\"XpOut\",},{M[1],M[13],T\"y_StateOut\",},{M[8],M[0],T\"is_active_c1_TMATS_NumericalMethods\",}}"
  };

  mxArray *mxVarInfo = sf_mex_decode_encoded_mx_struct_array(infoEncStr, 5, 10);
  mxArray *mxChecksum = mxCreateDoubleMatrix(1, 4, mxREAL);
  sf_c1_TMATS_NumericalMethods_get_check_sum(&mxChecksum);
  mxSetField(mxInfo, 0, infoFields[0], mxChecksum);
  mxSetField(mxInfo, 0, infoFields[1], mxVarInfo);
  return mxInfo;
}

static void chart_debug_initialization(SimStruct *S, unsigned int
  fullDebuggerInitialization)
{
  if (!sim_mode_is_rtw_gen(S)) {
    SFc1_TMATS_NumericalMethodsInstanceStruct *chartInstance;
    chartInstance = (SFc1_TMATS_NumericalMethodsInstanceStruct *)
      ((ChartInfoStruct *)(ssGetUserData(S)))->chartInstance;
    if (ssIsFirstInitCond(S) && fullDebuggerInitialization==1) {
      /* do this only if simulation is starting */
      {
        unsigned int chartAlreadyPresent;
        chartAlreadyPresent = sf_debug_initialize_chart
          (_TMATS_NumericalMethodsMachineNumber_,
           1,
           1,
           1,
           10,
           0,
           0,
           0,
           0,
           0,
           &(chartInstance->chartNumber),
           &(chartInstance->instanceNumber),
           ssGetPath(S),
           (void *)S);
        if (chartAlreadyPresent==0) {
          /* this is the first instance */
          init_script_number_translation(_TMATS_NumericalMethodsMachineNumber_,
            chartInstance->chartNumber);
          sf_debug_set_chart_disable_implicit_casting
            (_TMATS_NumericalMethodsMachineNumber_,chartInstance->chartNumber,1);
          sf_debug_set_chart_event_thresholds
            (_TMATS_NumericalMethodsMachineNumber_,
             chartInstance->chartNumber,
             0,
             0,
             0);
          _SFD_SET_DATA_PROPS(0,1,1,0,"IterationIn");
          _SFD_SET_DATA_PROPS(1,2,0,1,"XpOut");
          _SFD_SET_DATA_PROPS(2,1,1,0,"y_xpIn");
          _SFD_SET_DATA_PROPS(3,1,1,0,"PerIn");
          _SFD_SET_DATA_PROPS(4,1,1,0,"XoIn");
          _SFD_SET_DATA_PROPS(5,2,0,1,"InvJOut");
          _SFD_SET_DATA_PROPS(6,2,0,1,"JOut");
          _SFD_SET_DATA_PROPS(7,1,1,0,"JStateIn");
          _SFD_SET_DATA_PROPS(8,1,1,0,"y_StateIn");
          _SFD_SET_DATA_PROPS(9,2,0,1,"y_StateOut");
          _SFD_STATE_INFO(0,0,2);
          _SFD_CH_SUBSTATE_COUNT(0);
          _SFD_CH_SUBSTATE_DECOMP(0);
        }

        _SFD_CV_INIT_CHART(0,0,0,0);

        {
          _SFD_CV_INIT_STATE(0,0,0,0,0,0,NULL,NULL);
        }

        _SFD_CV_INIT_TRANS(0,0,NULL,NULL,0,NULL);

        /* Initialization of MATLAB Function Model Coverage */
        _SFD_CV_INIT_EML(0,1,1,7,0,0,0,0,0,6,3);
        _SFD_CV_INIT_EML_FCN(0,0,"eML_blk_kernel",0,-1,1110);
        _SFD_CV_INIT_EML_IF(0,1,0,410,443,-1,503);
        _SFD_CV_INIT_EML_IF(0,1,1,532,552,630,742);
        _SFD_CV_INIT_EML_IF(0,1,2,561,570,-1,620);
        _SFD_CV_INIT_EML_IF(0,1,3,644,678,-1,734);
        _SFD_CV_INIT_EML_IF(0,1,4,778,811,-1,1103);
        _SFD_CV_INIT_EML_IF(0,1,5,820,840,902,988);
        _SFD_CV_INIT_EML_IF(0,1,6,1005,1029,-1,1095);

        {
          static int condStart[] = { 413, 422 };

          static int condEnd[] = { 418, 443 };

          static int pfixExpr[] = { 0, 1, -3 };

          _SFD_CV_INIT_EML_MCDC(0,1,0,413,443,2,0,&(condStart[0]),&(condEnd[0]),
                                3,&(pfixExpr[0]));
        }

        {
          static int condStart[] = { 647, 656 };

          static int condEnd[] = { 652, 678 };

          static int pfixExpr[] = { 0, 1, -3 };

          _SFD_CV_INIT_EML_MCDC(0,1,1,647,678,2,2,&(condStart[0]),&(condEnd[0]),
                                3,&(pfixExpr[0]));
        }

        {
          static int condStart[] = { 781, 790 };

          static int condEnd[] = { 786, 811 };

          static int pfixExpr[] = { 0, 1, -3 };

          _SFD_CV_INIT_EML_MCDC(0,1,2,781,811,2,4,&(condStart[0]),&(condEnd[0]),
                                3,&(pfixExpr[0]));
        }

        _SFD_TRANS_COV_WTS(0,0,0,1,0);
        if (chartAlreadyPresent==0) {
          _SFD_TRANS_COV_MAPS(0,
                              0,NULL,NULL,
                              0,NULL,NULL,
                              1,NULL,NULL,
                              0,NULL,NULL);
        }

        _SFD_SET_DATA_COMPILED_PROPS(0,SF_DOUBLE,0,NULL,0,0,0,0.0,1.0,0,0,
          (MexFcnForType)c1_e_sf_marshallOut,(MexInFcnForType)NULL);

        {
          unsigned int dimVector[2];
          dimVector[0]= 1;
          dimVector[1]= 9;
          _SFD_SET_DATA_COMPILED_PROPS(1,SF_DOUBLE,2,&(dimVector[0]),0,0,0,0.0,
            1.0,0,0,(MexFcnForType)c1_c_sf_marshallOut,(MexInFcnForType)
            c1_c_sf_marshallIn);
        }

        {
          unsigned int dimVector[1];
          dimVector[0]= 9;
          _SFD_SET_DATA_COMPILED_PROPS(2,SF_DOUBLE,1,&(dimVector[0]),0,0,0,0.0,
            1.0,0,0,(MexFcnForType)c1_d_sf_marshallOut,(MexInFcnForType)NULL);
        }

        {
          unsigned int dimVector[1];
          dimVector[0]= 9;
          _SFD_SET_DATA_COMPILED_PROPS(3,SF_DOUBLE,1,&(dimVector[0]),0,0,0,0.0,
            1.0,0,0,(MexFcnForType)c1_d_sf_marshallOut,(MexInFcnForType)NULL);
        }

        {
          unsigned int dimVector[1];
          dimVector[0]= 9;
          _SFD_SET_DATA_COMPILED_PROPS(4,SF_DOUBLE,1,&(dimVector[0]),0,0,0,0.0,
            1.0,0,0,(MexFcnForType)c1_d_sf_marshallOut,(MexInFcnForType)NULL);
        }

        {
          unsigned int dimVector[2];
          dimVector[0]= 9;
          dimVector[1]= 9;
          _SFD_SET_DATA_COMPILED_PROPS(5,SF_DOUBLE,2,&(dimVector[0]),0,0,0,0.0,
            1.0,0,0,(MexFcnForType)c1_b_sf_marshallOut,(MexInFcnForType)
            c1_b_sf_marshallIn);
        }

        {
          unsigned int dimVector[2];
          dimVector[0]= 9;
          dimVector[1]= 9;
          _SFD_SET_DATA_COMPILED_PROPS(6,SF_DOUBLE,2,&(dimVector[0]),0,0,0,0.0,
            1.0,0,0,(MexFcnForType)c1_b_sf_marshallOut,(MexInFcnForType)
            c1_b_sf_marshallIn);
        }

        {
          unsigned int dimVector[2];
          dimVector[0]= 9;
          dimVector[1]= 9;
          _SFD_SET_DATA_COMPILED_PROPS(7,SF_DOUBLE,2,&(dimVector[0]),0,0,0,0.0,
            1.0,0,0,(MexFcnForType)c1_b_sf_marshallOut,(MexInFcnForType)NULL);
        }

        {
          unsigned int dimVector[2];
          dimVector[0]= 9;
          dimVector[1]= 10;
          _SFD_SET_DATA_COMPILED_PROPS(8,SF_DOUBLE,2,&(dimVector[0]),0,0,0,0.0,
            1.0,0,0,(MexFcnForType)c1_sf_marshallOut,(MexInFcnForType)NULL);
        }

        {
          unsigned int dimVector[2];
          dimVector[0]= 9;
          dimVector[1]= 10;
          _SFD_SET_DATA_COMPILED_PROPS(9,SF_DOUBLE,2,&(dimVector[0]),0,0,0,0.0,
            1.0,0,0,(MexFcnForType)c1_sf_marshallOut,(MexInFcnForType)
            c1_sf_marshallIn);
        }

        {
          real_T *c1_IterationIn;
          real_T (*c1_XpOut)[9];
          real_T (*c1_y_xpIn)[9];
          real_T (*c1_PerIn)[9];
          real_T (*c1_XoIn)[9];
          real_T (*c1_InvJOut)[81];
          real_T (*c1_JOut)[81];
          real_T (*c1_JStateIn)[81];
          real_T (*c1_y_StateIn)[90];
          real_T (*c1_y_StateOut)[90];
          c1_y_StateOut = (real_T (*)[90])ssGetOutputPortSignal(chartInstance->S,
            4);
          c1_y_StateIn = (real_T (*)[90])ssGetInputPortSignal(chartInstance->S,
            5);
          c1_JStateIn = (real_T (*)[81])ssGetInputPortSignal(chartInstance->S, 4);
          c1_JOut = (real_T (*)[81])ssGetOutputPortSignal(chartInstance->S, 3);
          c1_InvJOut = (real_T (*)[81])ssGetOutputPortSignal(chartInstance->S, 2);
          c1_XoIn = (real_T (*)[9])ssGetInputPortSignal(chartInstance->S, 3);
          c1_PerIn = (real_T (*)[9])ssGetInputPortSignal(chartInstance->S, 2);
          c1_y_xpIn = (real_T (*)[9])ssGetInputPortSignal(chartInstance->S, 1);
          c1_XpOut = (real_T (*)[9])ssGetOutputPortSignal(chartInstance->S, 1);
          c1_IterationIn = (real_T *)ssGetInputPortSignal(chartInstance->S, 0);
          _SFD_SET_DATA_VALUE_PTR(0U, c1_IterationIn);
          _SFD_SET_DATA_VALUE_PTR(1U, *c1_XpOut);
          _SFD_SET_DATA_VALUE_PTR(2U, *c1_y_xpIn);
          _SFD_SET_DATA_VALUE_PTR(3U, *c1_PerIn);
          _SFD_SET_DATA_VALUE_PTR(4U, *c1_XoIn);
          _SFD_SET_DATA_VALUE_PTR(5U, *c1_InvJOut);
          _SFD_SET_DATA_VALUE_PTR(6U, *c1_JOut);
          _SFD_SET_DATA_VALUE_PTR(7U, *c1_JStateIn);
          _SFD_SET_DATA_VALUE_PTR(8U, *c1_y_StateIn);
          _SFD_SET_DATA_VALUE_PTR(9U, *c1_y_StateOut);
        }
      }
    } else {
      sf_debug_reset_current_state_configuration
        (_TMATS_NumericalMethodsMachineNumber_,chartInstance->chartNumber,
         chartInstance->instanceNumber);
    }
  }
}

static const char* sf_get_instance_specialization()
{
  return "k8R8UnoAVr5dPvFdSAqZwE";
}

static void sf_opaque_initialize_c1_TMATS_NumericalMethods(void
  *chartInstanceVar)
{
  chart_debug_initialization(((SFc1_TMATS_NumericalMethodsInstanceStruct*)
    chartInstanceVar)->S,0);
  initialize_params_c1_TMATS_NumericalMethods
    ((SFc1_TMATS_NumericalMethodsInstanceStruct*) chartInstanceVar);
  initialize_c1_TMATS_NumericalMethods
    ((SFc1_TMATS_NumericalMethodsInstanceStruct*) chartInstanceVar);
}

static void sf_opaque_enable_c1_TMATS_NumericalMethods(void *chartInstanceVar)
{
  enable_c1_TMATS_NumericalMethods((SFc1_TMATS_NumericalMethodsInstanceStruct*)
    chartInstanceVar);
}

static void sf_opaque_disable_c1_TMATS_NumericalMethods(void *chartInstanceVar)
{
  disable_c1_TMATS_NumericalMethods((SFc1_TMATS_NumericalMethodsInstanceStruct*)
    chartInstanceVar);
}

static void sf_opaque_gateway_c1_TMATS_NumericalMethods(void *chartInstanceVar)
{
  sf_c1_TMATS_NumericalMethods((SFc1_TMATS_NumericalMethodsInstanceStruct*)
    chartInstanceVar);
}

extern const mxArray* sf_internal_get_sim_state_c1_TMATS_NumericalMethods
  (SimStruct* S)
{
  ChartInfoStruct *chartInfo = (ChartInfoStruct*) ssGetUserData(S);
  mxArray *plhs[1] = { NULL };

  mxArray *prhs[4];
  int mxError = 0;
  prhs[0] = mxCreateString("chart_simctx_raw2high");
  prhs[1] = mxCreateDoubleScalar(ssGetSFuncBlockHandle(S));
  prhs[2] = (mxArray*) get_sim_state_c1_TMATS_NumericalMethods
    ((SFc1_TMATS_NumericalMethodsInstanceStruct*)chartInfo->chartInstance);/* raw sim ctx */
  prhs[3] = (mxArray*) sf_get_sim_state_info_c1_TMATS_NumericalMethods();/* state var info */
  mxError = sf_mex_call_matlab(1, plhs, 4, prhs, "sfprivate");
  mxDestroyArray(prhs[0]);
  mxDestroyArray(prhs[1]);
  mxDestroyArray(prhs[2]);
  mxDestroyArray(prhs[3]);
  if (mxError || plhs[0] == NULL) {
    sf_mex_error_message("Stateflow Internal Error: \nError calling 'chart_simctx_raw2high'.\n");
  }

  return plhs[0];
}

extern void sf_internal_set_sim_state_c1_TMATS_NumericalMethods(SimStruct* S,
  const mxArray *st)
{
  ChartInfoStruct *chartInfo = (ChartInfoStruct*) ssGetUserData(S);
  mxArray *plhs[1] = { NULL };

  mxArray *prhs[4];
  int mxError = 0;
  prhs[0] = mxCreateString("chart_simctx_high2raw");
  prhs[1] = mxCreateDoubleScalar(ssGetSFuncBlockHandle(S));
  prhs[2] = mxDuplicateArray(st);      /* high level simctx */
  prhs[3] = (mxArray*) sf_get_sim_state_info_c1_TMATS_NumericalMethods();/* state var info */
  mxError = sf_mex_call_matlab(1, plhs, 4, prhs, "sfprivate");
  mxDestroyArray(prhs[0]);
  mxDestroyArray(prhs[1]);
  mxDestroyArray(prhs[2]);
  mxDestroyArray(prhs[3]);
  if (mxError || plhs[0] == NULL) {
    sf_mex_error_message("Stateflow Internal Error: \nError calling 'chart_simctx_high2raw'.\n");
  }

  set_sim_state_c1_TMATS_NumericalMethods
    ((SFc1_TMATS_NumericalMethodsInstanceStruct*)chartInfo->chartInstance,
     mxDuplicateArray(plhs[0]));
  mxDestroyArray(plhs[0]);
}

static const mxArray* sf_opaque_get_sim_state_c1_TMATS_NumericalMethods
  (SimStruct* S)
{
  return sf_internal_get_sim_state_c1_TMATS_NumericalMethods(S);
}

static void sf_opaque_set_sim_state_c1_TMATS_NumericalMethods(SimStruct* S,
  const mxArray *st)
{
  sf_internal_set_sim_state_c1_TMATS_NumericalMethods(S, st);
}

static void sf_opaque_terminate_c1_TMATS_NumericalMethods(void *chartInstanceVar)
{
  if (chartInstanceVar!=NULL) {
    SimStruct *S = ((SFc1_TMATS_NumericalMethodsInstanceStruct*)
                    chartInstanceVar)->S;
    if (sim_mode_is_rtw_gen(S) || sim_mode_is_external(S)) {
      sf_clear_rtw_identifier(S);
    }

    finalize_c1_TMATS_NumericalMethods
      ((SFc1_TMATS_NumericalMethodsInstanceStruct*) chartInstanceVar);
    free((void *)chartInstanceVar);
    ssSetUserData(S,NULL);
  }

  unload_TMATS_NumericalMethods_optimization_info();
}

static void sf_opaque_init_subchart_simstructs(void *chartInstanceVar)
{
  initSimStructsc1_TMATS_NumericalMethods
    ((SFc1_TMATS_NumericalMethodsInstanceStruct*) chartInstanceVar);
}

extern unsigned int sf_machine_global_initializer_called(void);
static void mdlProcessParameters_c1_TMATS_NumericalMethods(SimStruct *S)
{
  int i;
  for (i=0;i<ssGetNumRunTimeParams(S);i++) {
    if (ssGetSFcnParamTunable(S,i)) {
      ssUpdateDlgParamAsRunTimeParam(S,i);
    }
  }

  if (sf_machine_global_initializer_called()) {
    initialize_params_c1_TMATS_NumericalMethods
      ((SFc1_TMATS_NumericalMethodsInstanceStruct*)(((ChartInfoStruct *)
         ssGetUserData(S))->chartInstance));
  }
}

static void mdlSetWorkWidths_c1_TMATS_NumericalMethods(SimStruct *S)
{
  if (sim_mode_is_rtw_gen(S) || sim_mode_is_external(S)) {
    mxArray *infoStruct = load_TMATS_NumericalMethods_optimization_info();
    int_T chartIsInlinable =
      (int_T)sf_is_chart_inlinable(S,sf_get_instance_specialization(),infoStruct,
      1);
    ssSetStateflowIsInlinable(S,chartIsInlinable);
    ssSetRTWCG(S,sf_rtw_info_uint_prop(S,sf_get_instance_specialization(),
                infoStruct,1,"RTWCG"));
    ssSetEnableFcnIsTrivial(S,1);
    ssSetDisableFcnIsTrivial(S,1);
    ssSetNotMultipleInlinable(S,sf_rtw_info_uint_prop(S,
      sf_get_instance_specialization(),infoStruct,1,
      "gatewayCannotBeInlinedMultipleTimes"));
    if (chartIsInlinable) {
      ssSetInputPortOptimOpts(S, 0, SS_REUSABLE_AND_LOCAL);
      ssSetInputPortOptimOpts(S, 1, SS_REUSABLE_AND_LOCAL);
      ssSetInputPortOptimOpts(S, 2, SS_REUSABLE_AND_LOCAL);
      ssSetInputPortOptimOpts(S, 3, SS_REUSABLE_AND_LOCAL);
      ssSetInputPortOptimOpts(S, 4, SS_REUSABLE_AND_LOCAL);
      ssSetInputPortOptimOpts(S, 5, SS_REUSABLE_AND_LOCAL);
      sf_mark_chart_expressionable_inputs(S,sf_get_instance_specialization(),
        infoStruct,1,6);
      sf_mark_chart_reusable_outputs(S,sf_get_instance_specialization(),
        infoStruct,1,4);
    }

    sf_set_rtw_dwork_info(S,sf_get_instance_specialization(),infoStruct,1);
    ssSetHasSubFunctions(S,!(chartIsInlinable));
  } else {
  }

  ssSetOptions(S,ssGetOptions(S)|SS_OPTION_WORKS_WITH_CODE_REUSE);
  ssSetChecksum0(S,(2212222322U));
  ssSetChecksum1(S,(2616306233U));
  ssSetChecksum2(S,(2081348085U));
  ssSetChecksum3(S,(2419950815U));
  ssSetmdlDerivatives(S, NULL);
  ssSetExplicitFCSSCtrl(S,1);
  ssSupportsMultipleExecInstances(S,1);
}

static void mdlRTW_c1_TMATS_NumericalMethods(SimStruct *S)
{
  if (sim_mode_is_rtw_gen(S)) {
    ssWriteRTWStrParam(S, "StateflowChartType", "Embedded MATLAB");
  }
}

static void mdlStart_c1_TMATS_NumericalMethods(SimStruct *S)
{
  SFc1_TMATS_NumericalMethodsInstanceStruct *chartInstance;
  chartInstance = (SFc1_TMATS_NumericalMethodsInstanceStruct *)malloc(sizeof
    (SFc1_TMATS_NumericalMethodsInstanceStruct));
  memset(chartInstance, 0, sizeof(SFc1_TMATS_NumericalMethodsInstanceStruct));
  if (chartInstance==NULL) {
    sf_mex_error_message("Could not allocate memory for chart instance.");
  }

  chartInstance->chartInfo.chartInstance = chartInstance;
  chartInstance->chartInfo.isEMLChart = 1;
  chartInstance->chartInfo.chartInitialized = 0;
  chartInstance->chartInfo.sFunctionGateway =
    sf_opaque_gateway_c1_TMATS_NumericalMethods;
  chartInstance->chartInfo.initializeChart =
    sf_opaque_initialize_c1_TMATS_NumericalMethods;
  chartInstance->chartInfo.terminateChart =
    sf_opaque_terminate_c1_TMATS_NumericalMethods;
  chartInstance->chartInfo.enableChart =
    sf_opaque_enable_c1_TMATS_NumericalMethods;
  chartInstance->chartInfo.disableChart =
    sf_opaque_disable_c1_TMATS_NumericalMethods;
  chartInstance->chartInfo.getSimState =
    sf_opaque_get_sim_state_c1_TMATS_NumericalMethods;
  chartInstance->chartInfo.setSimState =
    sf_opaque_set_sim_state_c1_TMATS_NumericalMethods;
  chartInstance->chartInfo.getSimStateInfo =
    sf_get_sim_state_info_c1_TMATS_NumericalMethods;
  chartInstance->chartInfo.zeroCrossings = NULL;
  chartInstance->chartInfo.outputs = NULL;
  chartInstance->chartInfo.derivatives = NULL;
  chartInstance->chartInfo.mdlRTW = mdlRTW_c1_TMATS_NumericalMethods;
  chartInstance->chartInfo.mdlStart = mdlStart_c1_TMATS_NumericalMethods;
  chartInstance->chartInfo.mdlSetWorkWidths =
    mdlSetWorkWidths_c1_TMATS_NumericalMethods;
  chartInstance->chartInfo.extModeExec = NULL;
  chartInstance->chartInfo.restoreLastMajorStepConfiguration = NULL;
  chartInstance->chartInfo.restoreBeforeLastMajorStepConfiguration = NULL;
  chartInstance->chartInfo.storeCurrentConfiguration = NULL;
  chartInstance->S = S;
  ssSetUserData(S,(void *)(&(chartInstance->chartInfo)));/* register the chart instance with simstruct */
  init_dsm_address_info(chartInstance);
  if (!sim_mode_is_rtw_gen(S)) {
  }

  sf_opaque_init_subchart_simstructs(chartInstance->chartInfo.chartInstance);
  chart_debug_initialization(S,1);
}

void c1_TMATS_NumericalMethods_method_dispatcher(SimStruct *S, int_T method,
  void *data)
{
  switch (method) {
   case SS_CALL_MDL_START:
    mdlStart_c1_TMATS_NumericalMethods(S);
    break;

   case SS_CALL_MDL_SET_WORK_WIDTHS:
    mdlSetWorkWidths_c1_TMATS_NumericalMethods(S);
    break;

   case SS_CALL_MDL_PROCESS_PARAMETERS:
    mdlProcessParameters_c1_TMATS_NumericalMethods(S);
    break;

   default:
    /* Unhandled method */
    sf_mex_error_message("Stateflow Internal Error:\n"
                         "Error calling c1_TMATS_NumericalMethods_method_dispatcher.\n"
                         "Can't handle method %d.\n", method);
    break;
  }
}
