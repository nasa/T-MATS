#ifndef __SF_DEBUG_MACROS_H__
#define __SF_DEBUG_MACROS_H__

extern unsigned int _TMATS_NumericalMethodsMachineNumber_;
#define _SFD_SET_DATA_VALUE_PTR(v1,v2)\
	sf_debug_set_instance_data_value_ptr(_TMATS_NumericalMethodsMachineNumber_,CHARTINSTANCE_CHARTNUMBER,CHARTINSTANCE_INSTANCENUMBER,v1,(void *)(v2),NULL);
#define _SFD_UNSET_DATA_VALUE_PTR(v1)\
	sf_debug_unset_instance_data_value_ptr(_TMATS_NumericalMethodsMachineNumber_,CHARTINSTANCE_CHARTNUMBER,CHARTINSTANCE_INSTANCENUMBER,v1);
#define _SFD_SET_DATA_VALUE_PTR_VAR_DIM(v1,v2,v3)\
	sf_debug_set_instance_data_value_ptr(_TMATS_NumericalMethodsMachineNumber_,CHARTINSTANCE_CHARTNUMBER,CHARTINSTANCE_INSTANCENUMBER,v1,(void *)(v2),(void *)(v3));
#define _SFD_DATA_RANGE_CHECK_MIN_MAX(dVal,dNum,dMin,dMax)\
                      sf_debug_data_range_error_wrapper_min_max(_TMATS_NumericalMethodsMachineNumber_,\
CHARTINSTANCE_CHARTNUMBER,\
CHARTINSTANCE_INSTANCENUMBER,\
                                             dNum,(double)(dVal),(double)dMin,(double)dMax)
#define _SFD_DATA_RANGE_CHECK_MIN(dVal,dNum,dMin)\
                      sf_debug_data_range_error_wrapper_min(_TMATS_NumericalMethodsMachineNumber_,\
CHARTINSTANCE_CHARTNUMBER,\
CHARTINSTANCE_INSTANCENUMBER,\
                                             dNum,(double)(dVal),(double)dMin)
#define _SFD_DATA_RANGE_CHECK_MAX(dVal,dNum,dMax)\
                      sf_debug_data_range_error_wrapper_max(_TMATS_NumericalMethodsMachineNumber_,\
CHARTINSTANCE_CHARTNUMBER,\
CHARTINSTANCE_INSTANCENUMBER,\
                                             dNum,(double)(dVal),(double)dMax)
#define _SFD_DATA_RANGE_CHECK(dVal,dNum)\
                      sf_debug_data_range_wrapper(_TMATS_NumericalMethodsMachineNumber_,\
CHARTINSTANCE_CHARTNUMBER,\
CHARTINSTANCE_INSTANCENUMBER,\
                                             dNum,(double)(dVal))
#define _SFD_DATA_READ_BEFORE_WRITE_CHECK(dNum,dVal)\
                      sf_debug_read_before_write_check(_TMATS_NumericalMethodsMachineNumber_,\
CHARTINSTANCE_CHARTNUMBER,\
CHARTINSTANCE_INSTANCENUMBER,\
                                             (unsigned int)(dNum),(bool)dVal)
#define _SFD_ARRAY_BOUNDS_CHECK(v1,v2,v3,v4,v5,v6) \
                      sf_debug_data_array_bounds_error_check(_TMATS_NumericalMethodsMachineNumber_,\
CHARTINSTANCE_CHARTNUMBER,\
CHARTINSTANCE_INSTANCENUMBER,\
                                             (v1),(int)(v2),(int)(v3),(int)(v4),(int)(v5),(int)(v6))
#define _SFD_RUNTIME_SIZE_MISMATCH_CHECK(v1,v2,v3,v4,v5) \
                      sf_debug_data_runtime_size_mismatch_error_check(_TMATS_NumericalMethodsMachineNumber_,\
CHARTINSTANCE_CHARTNUMBER,\
CHARTINSTANCE_INSTANCENUMBER,\
                                             (v1),(v2),(unsigned int)(v3),(int)(v4),(int)(v5))
#define _SFD_EML_ARRAY_BOUNDS_CHECK(v1,v2,v3,v4,v5,v6) \
                      sf_debug_eml_data_array_bounds_error_check(_TMATS_NumericalMethodsMachineNumber_,\
CHARTINSTANCE_CHARTNUMBER,\
CHARTINSTANCE_INSTANCENUMBER,\
                                             (v1),(int)(v2),(int)(v3),(int)(v4),(int)(v5),(int)(v6))
#define _SFD_INTEGER_CHECK(v1,v2) \
                      sf_debug_integer_check(_TMATS_NumericalMethodsMachineNumber_,\
CHARTINSTANCE_CHARTNUMBER,\
CHARTINSTANCE_INSTANCENUMBER,\
                                             (v1),(double)(v2))
#define _SFD_NOT_NAN_CHECK(v1,v2) \
                      sf_debug_not_nan_check(_TMATS_NumericalMethodsMachineNumber_,\
CHARTINSTANCE_CHARTNUMBER,\
CHARTINSTANCE_INSTANCENUMBER,\
                                             (v1),(double)(v2))
#define _SFD_NON_NEGATIVE_CHECK(v1,v2) \
                      sf_debug_non_negative_check(_TMATS_NumericalMethodsMachineNumber_,\
CHARTINSTANCE_CHARTNUMBER,\
CHARTINSTANCE_INSTANCENUMBER,\
                                             (v1),(double)(v2))
#define _SFD_CAST_TO_UINT8(v1) \
                      sf_debug_cast_to_uint8_T(_TMATS_NumericalMethodsMachineNumber_,\
CHARTINSTANCE_CHARTNUMBER,\
CHARTINSTANCE_INSTANCENUMBER,\
                                             (v1),0,0)
#define _SFD_CAST_TO_UINT16(v1) \
                      sf_debug_cast_to_uint16_T(_TMATS_NumericalMethodsMachineNumber_,\
CHARTINSTANCE_CHARTNUMBER,\
CHARTINSTANCE_INSTANCENUMBER,\
                                             (v1),0,0)
#define _SFD_CAST_TO_UINT32(v1) \
                      sf_debug_cast_to_uint32_T(_TMATS_NumericalMethodsMachineNumber_,\
CHARTINSTANCE_CHARTNUMBER,\
CHARTINSTANCE_INSTANCENUMBER,\
                                             (v1),0,0)
#define _SFD_CAST_TO_INT8(v1) \
                      sf_debug_cast_to_int8_T(_TMATS_NumericalMethodsMachineNumber_,\
CHARTINSTANCE_CHARTNUMBER,\
CHARTINSTANCE_INSTANCENUMBER,\
                                             (v1),0,0)
#define _SFD_CAST_TO_INT16(v1) \
                      sf_debug_cast_to_int16_T(_TMATS_NumericalMethodsMachineNumber_,\
CHARTINSTANCE_CHARTNUMBER,\
CHARTINSTANCE_INSTANCENUMBER,\
                                             (v1),0,0)
#define _SFD_CAST_TO_INT32(v1) \
                      sf_debug_cast_to_int32_T(_TMATS_NumericalMethodsMachineNumber_,\
CHARTINSTANCE_CHARTNUMBER,\
CHARTINSTANCE_INSTANCENUMBER,\
                                             (v1),0,0)
#define _SFD_CAST_TO_SINGLE(v1) \
                      sf_debug_cast_to_real32_T(_TMATS_NumericalMethodsMachineNumber_,\
CHARTINSTANCE_CHARTNUMBER,\
CHARTINSTANCE_INSTANCENUMBER,\
                                             (v1),0,0)
#define _SFD_TRANSITION_CONFLICT(v1,v2) sf_debug_transition_conflict_error(_TMATS_NumericalMethodsMachineNumber_,\
CHARTINSTANCE_CHARTNUMBER,\
CHARTINSTANCE_INSTANCENUMBER,\
v1,v2)
#define _SFD_ANIMATE() sf_debug_animate(_TMATS_NumericalMethodsMachineNumber_,\
CHARTINSTANCE_CHARTNUMBER,\
CHARTINSTANCE_INSTANCENUMBER)
#define _SFD_CHART_CALL(v1,v2,v3,v4) sf_debug_call(_TMATS_NumericalMethodsMachineNumber_,\
CHARTINSTANCE_CHARTNUMBER,\
CHARTINSTANCE_INSTANCENUMBER,\
CHART_OBJECT,v1,v2,v3,v4,\
0,NULL,_sfTime_,1)
#define _SFD_CC_CALL(v2,v3,v4) _SFD_CHART_CALL(CHART_OBJECT,v2,v3,v4)
#define _SFD_CS_CALL(v2,v3,v4) _SFD_CHART_CALL(STATE_OBJECT,v2,v3,v4)
#define _SFD_CT_CALL(v2,v3,v4) _SFD_CHART_CALL(TRANSITION_OBJECT,v2,v3,v4)
#define _SFD_CE_CALL(v2,v3,v4) _SFD_CHART_CALL(EVENT_OBJECT,v2,v3,v4)
#define _SFD_EML_CALL(v1,v2,v3) eml_debug_line_call(_TMATS_NumericalMethodsMachineNumber_,\
CHARTINSTANCE_CHARTNUMBER,\
CHARTINSTANCE_INSTANCENUMBER,\
v1,v2,\
v3,_sfTime_,0)
#define _SFD_SCRIPT_TRANSLATION(v1,v2,v3) sf_debug_set_script_translation(_TMATS_NumericalMethodsMachineNumber_,\
v1,v2,v3)
#define _SFD_SCRIPT_CALL(v1,v2,v3) eml_debug_line_call(_TMATS_NumericalMethodsMachineNumber_,\
CHARTINSTANCE_CHARTNUMBER,\
CHARTINSTANCE_INSTANCENUMBER,\
v1,v2,\
v3,_sfTime_,1)
#define _SFD_CCP_CALL(v3,v4,v5,v6) sf_debug_call(_TMATS_NumericalMethodsMachineNumber_,\
CHARTINSTANCE_CHARTNUMBER,\
CHARTINSTANCE_INSTANCENUMBER,\
CHART_OBJECT,TRANSITION_OBJECT,TRANSITION_GUARD_COVERAGE_TAG,v3,v6,\
v4,NULL,_sfTime_,(unsigned int)(v5))
#define _SFD_STATE_TEMPORAL_THRESHOLD(v1,v2,v4) sf_debug_temporal_threshold(_TMATS_NumericalMethodsMachineNumber_,\
CHARTINSTANCE_CHARTNUMBER,\
CHARTINSTANCE_INSTANCENUMBER,\
(unsigned int)(v1),(v2),STATE_OBJECT,(v4))
#define _SFD_TRANS_TEMPORAL_THRESHOLD(v1,v2,v4) sf_debug_temporal_threshold(_TMATS_NumericalMethodsMachineNumber_,\
CHARTINSTANCE_CHARTNUMBER,\
CHARTINSTANCE_INSTANCENUMBER,\
(unsigned int)(v1),(v2),TRANSITION_OBJECT,(v4))
#define CV_EVAL(v1,v2,v3,v4) cv_eval_point(_TMATS_NumericalMethodsMachineNumber_,\
		  CHARTINSTANCE_CHARTNUMBER,\
		  CHARTINSTANCE_INSTANCENUMBER,\
		  (v1),(v2),(v3),(boolean_T)(v4))
#define CV_CHART_EVAL(v2,v3,v4) CV_EVAL(CHART_OBJECT,(v2),(v3),(v4))
#define CV_STATE_EVAL(v2,v3,v4) CV_EVAL(STATE_OBJECT,(v2),(v3),(v4))
#define CV_TRANSITION_EVAL(v1,v2) cv_eval_point(_TMATS_NumericalMethodsMachineNumber_,\
		  CHARTINSTANCE_CHARTNUMBER,\
		  CHARTINSTANCE_INSTANCENUMBER,\
		  TRANSITION_OBJECT,(v1),0,((v2)!=0))
#define CV_TESTOBJECTIVE_EVAL(v1,v2,v3,v4)  cv_eval_testobjective(_TMATS_NumericalMethodsMachineNumber_,\
		  CHARTINSTANCE_CHARTNUMBER,\
		  CHARTINSTANCE_INSTANCENUMBER,\
		  (v1),(v2),(v3),(v4))

/* Coverage Macros for MATLAB  */
#define CV_EML_EVAL(v1,v2,v3,v4,v5) cv_eml_eval(_TMATS_NumericalMethodsMachineNumber_,\
		  CHARTINSTANCE_CHARTNUMBER,\
		  CHARTINSTANCE_INSTANCENUMBER,\
		  (v1),(v2),(v3),(v4),(int)(v5))
#define CV_EML_FCN(v2,v3) CV_EML_EVAL(CV_EML_FCN_CHECK,(v2),1,(v3),0)
#define CV_EML_TESTOBJECTIVE(v2,v3,v4,v5) CV_EML_EVAL(CV_EML_TESTOBJECTIVE_CHECK,(v2),(v3),(v4),((v5) != 0))
#define CV_EML_SATURATION(v2,v3,v4,v5) CV_EML_EVAL(CV_EML_SATURATION_CHECK,(v2),(v3),(v4),((v5) != 0))
#define CV_EML_IF(v2,v3,v4,v5) CV_EML_EVAL(CV_EML_IF_CHECK,(v2),(v3),(v4),((v5) != 0))
#define CV_EML_FOR(v2,v3,v4,v5) CV_EML_EVAL(CV_EML_FOR_CHECK,(v2),(v3),(v4),(v5))
#define CV_EML_WHILE(v2,v3,v4,v5) CV_EML_EVAL(CV_EML_WHILE_CHECK,(v2),(v3),(v4),((v5) != 0))
#define CV_EML_SWITCH(v2,v3,v4,v5) CV_EML_EVAL(CV_EML_SWITCH_CHECK,(v2),(v3),(v4),(v5))
#define CV_EML_COND(v2,v3,v4,v5) CV_EML_EVAL(CV_EML_COND_CHECK,(v2),(v3),(v4),((v5) != 0))
#define CV_EML_MCDC(v2,v3,v4,v5) CV_EML_EVAL(CV_EML_MCDC_CHECK,(v2),(v3),(v4),(v5))
#define CV_SCRIPT_EVAL(v1,v2,v3,v4) cv_script_eval(_TMATS_NumericalMethodsMachineNumber_,\
		  CHARTINSTANCE_CHARTNUMBER,\
		  CHARTINSTANCE_INSTANCENUMBER,\
		  (v1),(v2),(v3),(int)(v4))
#define CV_SCRIPT_FCN(v2,v3) CV_SCRIPT_EVAL(CV_SCRIPT_FCN_CHECK,(v2),(v3),0)
#define CV_SCRIPT_TESTOBJECTIVE(v2,v3,v4) CV_SCRIPT_EVAL(CV_SCRIPT_TESTOBJECTIVE_CHECK,(v2),(v3),((v4) != 0))
#define CV_SCRIPT_SATURATION(v2,v3,v4) CV_SCRIPT_EVAL(CV_SCRIPT_SATURATION_CHECK,(v2),(v3),((v4) != 0))
#define CV_SCRIPT_IF(v2,v3,v4) CV_SCRIPT_EVAL(CV_SCRIPT_IF_CHECK,(v2),(v3),((v4) != 0))
#define CV_SCRIPT_FOR(v2,v3,v4) CV_SCRIPT_EVAL(CV_SCRIPT_FOR_CHECK,(v2),(v3),(v4))
#define CV_SCRIPT_WHILE(v2,v3,v4) CV_SCRIPT_EVAL(CV_SCRIPT_WHILE_CHECK,(v2),(v3),((v4) != 0))
#define CV_SCRIPT_SWITCH(v2,v3,v4) CV_SCRIPT_EVAL(CV_SCRIPT_SWITCH_CHECK,(v2),(v3),(v4))
#define CV_SCRIPT_COND(v2,v3,v4) CV_SCRIPT_EVAL(CV_SCRIPT_COND_CHECK,(v2),(v3),((v4) != 0))
#define CV_SCRIPT_MCDC(v2,v3,v4) CV_SCRIPT_EVAL(CV_SCRIPT_MCDC_CHECK,(v2),(v3),(v4))

#define _SFD_CV_INIT_EML(v1,v2,v3,v4,v5,v6,v7,v8,v9,v10,v11) cv_eml_init_script(\
       _TMATS_NumericalMethodsMachineNumber_,\
		  CHARTINSTANCE_CHARTNUMBER,\
		  CHARTINSTANCE_INSTANCENUMBER,\
		  (v1),(v2),(v3),(v4),(v5),(v6),(v7),(v8),(v9),(v10),(v10))

#define _SFD_CV_INIT_EML_FCN(v1,v2,v3,v4,v5,v6) cv_eml_init_fcn(\
       _TMATS_NumericalMethodsMachineNumber_,\
		  CHARTINSTANCE_CHARTNUMBER,\
		  CHARTINSTANCE_INSTANCENUMBER,\
		  (v1),(v2),(v3),(v4),(v5),(v6))

#define _SFD_CV_INIT_EML_TESTOBJECTIVE(v1,v2,v3,v4,v5,v6,v7) cv_eml_init_testobjective(\
       _TMATS_NumericalMethodsMachineNumber_,\
       CHARTINSTANCE_CHARTNUMBER,\
       CHARTINSTANCE_INSTANCENUMBER,\
       (v1),(v2),(v3),(v4),(v5),(v6),(v7))

#define _SFD_CV_INIT_EML_SATURATION(v1,v2,v3,v4,v5,v6) cv_eml_init_saturation(\
       _TMATS_NumericalMethodsMachineNumber_,\
       CHARTINSTANCE_CHARTNUMBER,\
       CHARTINSTANCE_INSTANCENUMBER,\
       (v1),(v2),(v3),(v4),(v5),(v6))

#define _SFD_CV_INIT_EML_IF(v1,v2,v3,v4,v5,v6,v7) cv_eml_init_if(\
       _TMATS_NumericalMethodsMachineNumber_,\
		  CHARTINSTANCE_CHARTNUMBER,\
		  CHARTINSTANCE_INSTANCENUMBER,\
		  (v1),(v2),(v3),(v4),(v5),(v6),(v7))

#define _SFD_CV_INIT_EML_FOR(v1,v2,v3,v4,v5,v6) cv_eml_init_for(\
       _TMATS_NumericalMethodsMachineNumber_,\
		  CHARTINSTANCE_CHARTNUMBER,\
		  CHARTINSTANCE_INSTANCENUMBER,\
		  (v1),(v2),(v3),(v4),(v5),(v6))

#define _SFD_CV_INIT_EML_WHILE(v1,v2,v3,v4,v5,v6) cv_eml_init_while(\
       _TMATS_NumericalMethodsMachineNumber_,\
		  CHARTINSTANCE_CHARTNUMBER,\
		  CHARTINSTANCE_INSTANCENUMBER,\
		  (v1),(v2),(v3),(v4),(v5),(v6))

#define _SFD_CV_INIT_EML_MCDC(v1,v2,v3,v4,v5,v6,v7,v8,v9,v10,v11) cv_eml_init_mcdc(\
       _TMATS_NumericalMethodsMachineNumber_,\
		  CHARTINSTANCE_CHARTNUMBER,\
		  CHARTINSTANCE_INSTANCENUMBER,\
		  (v1),(v2),(v3),(v4),(v5),(v6),(v7),(v8),(v9),(v10),(v11))

#define _SFD_CV_INIT_EML_SWITCH(v1,v2,v3,v4,v5,v6,v7,v8,v9) cv_eml_init_switch(\
       _TMATS_NumericalMethodsMachineNumber_,\
		  CHARTINSTANCE_CHARTNUMBER,\
		  CHARTINSTANCE_INSTANCENUMBER,\
		  (v1),(v2),(v3),(v4),(v5),(v6),(v7),(v8),(v9))

#define _SFD_CV_INIT_SCRIPT(v1,v2,v3,v4,v5,v6,v7,v8,v9,v10) cv_script_init_script(\
       _TMATS_NumericalMethodsMachineNumber_,\
		  CHARTINSTANCE_CHARTNUMBER,\
		  CHARTINSTANCE_INSTANCENUMBER,\
		  (v1),(v2),(v3),(v4),(v5),(v6),(v7),(v8),(v9),(v10))

#define _SFD_CV_INIT_SCRIPT_FCN(v1,v2,v3,v4,v5,v6) cv_script_init_fcn(\
       _TMATS_NumericalMethodsMachineNumber_,\
		  CHARTINSTANCE_CHARTNUMBER,\
		  CHARTINSTANCE_INSTANCENUMBER,\
		  (v1),(v2),(v3),(v4),(v5),(v6))

#define _SFD_CV_INIT_SCRIPT_TESTOBJECTIVE(v1,v2,v3,v4,v5,v6) cv_script_init_testobjective(\
       _TMATS_NumericalMethodsMachineNumber_,\
		  CHARTINSTANCE_CHARTNUMBER,\
		  CHARTINSTANCE_INSTANCENUMBER,\
		  (v1),(v2),(v3),(v4),(v5),(v6))

#define _SFD_CV_INIT_SCRIPT_SATURATION(v1,v2,v3,v4,v5,v6) cv_script_init_saturation(\
       _TMATS_NumericalMethodsMachineNumber_,\
       CHARTINSTANCE_CHARTNUMBER,\
       CHARTINSTANCE_INSTANCENUMBER,\
       (v1),(v2),(v3),(v4),(v5),(v6))

#define _SFD_CV_INIT_SCRIPT_IF(v1,v2,v3,v4,v5,v6) cv_script_init_if(\
       _TMATS_NumericalMethodsMachineNumber_,\
		  CHARTINSTANCE_CHARTNUMBER,\
		  CHARTINSTANCE_INSTANCENUMBER,\
		  (v1),(v2),(v3),(v4),(v5),(v6))

#define _SFD_CV_INIT_SCRIPT_FOR(v1,v2,v3,v4,v5) cv_script_init_for(\
       _TMATS_NumericalMethodsMachineNumber_,\
		  CHARTINSTANCE_CHARTNUMBER,\
		  CHARTINSTANCE_INSTANCENUMBER,\
		  (v1),(v2),(v3),(v4),(v5))

#define _SFD_CV_INIT_SCRIPT_WHILE(v1,v2,v3,v4,v5) cv_script_init_while(\
       _TMATS_NumericalMethodsMachineNumber_,\
		  CHARTINSTANCE_CHARTNUMBER,\
		  CHARTINSTANCE_INSTANCENUMBER,\
		  (v1),(v2),(v3),(v4),(v5))

#define _SFD_CV_INIT_SCRIPT_MCDC(v1,v2,v3,v4,v5,v6,v7,v8,v9,v10) cv_script_init_mcdc(\
       _TMATS_NumericalMethodsMachineNumber_,\
		  CHARTINSTANCE_CHARTNUMBER,\
		  CHARTINSTANCE_INSTANCENUMBER,\
		  (v1),(v2),(v3),(v4),(v5),(v6),(v7),(v8),(v9),(v10))

#define _SFD_CV_INIT_SCRIPT_SWITCH(v1,v2,v3,v4,v5,v6,v7,v8) cv_script_init_switch(\
       _TMATS_NumericalMethodsMachineNumber_,\
		  CHARTINSTANCE_CHARTNUMBER,\
		  CHARTINSTANCE_INSTANCENUMBER,\
		  (v1),(v2),(v3),(v4),(v5),(v6),(v7),(v8))


#define _SFD_SET_DATA_PROPS(dataNumber,dataScope,isInputData,isOutputData,dataName)\
 sf_debug_set_chart_data_props(_TMATS_NumericalMethodsMachineNumber_,CHARTINSTANCE_CHARTNUMBER,\
	(dataNumber),(dataScope),(isInputData),(isOutputData),(dataName))
#define _SFD_SET_DATA_COMPILED_PROPS(dataNumber,dataType,numDims,dimArray,isFixedPoint,isSigned,wordLength,bias,slope,exponent,complexity,mexOutFcn, mexInFcn)\
 sf_debug_set_chart_data_compiled_props(_TMATS_NumericalMethodsMachineNumber_,CHARTINSTANCE_CHARTNUMBER,CHARTINSTANCE_INSTANCENUMBER,\
	(dataNumber),(dataType),(numDims),(dimArray),(isFixedPoint),(isSigned),(wordLength),(bias),(slope),(exponent),(complexity),(mexOutFcn),(mexInFcn))
#define _SFD_STATE_INFO(v1,v2,v3)\
	sf_debug_set_chart_state_info(_TMATS_NumericalMethodsMachineNumber_,CHARTINSTANCE_CHARTNUMBER,(v1),(v2),(v3))
#define _SFD_CH_SUBSTATE_INDEX(v1,v2)\
	sf_debug_set_chart_substate_index(_TMATS_NumericalMethodsMachineNumber_,CHARTINSTANCE_CHARTNUMBER,(v1),(v2))
#define _SFD_ST_SUBSTATE_INDEX(v1,v2,v3)\
   sf_debug_set_chart_state_substate_index(_TMATS_NumericalMethodsMachineNumber_,CHARTINSTANCE_CHARTNUMBER,(v1),(v2),(v3))
#define _SFD_ST_SUBSTATE_COUNT(v1,v2)\
	sf_debug_set_chart_state_substate_count(_TMATS_NumericalMethodsMachineNumber_,CHARTINSTANCE_CHARTNUMBER,(v1),(v2))
#define _SFD_STATE_COV_WTS(v1,v2,v3,v4)\
	sf_debug_set_instance_state_coverage_weights(_TMATS_NumericalMethodsMachineNumber_,CHARTINSTANCE_CHARTNUMBER,CHARTINSTANCE_INSTANCENUMBER,(v1),(v2),(v3),(v4))
#define _SFD_STATE_COV_MAPS(v1,v2,v3,v4,v5,v6,v7,v8,v9,v10) \
 sf_debug_set_chart_state_coverage_maps(_TMATS_NumericalMethodsMachineNumber_,CHARTINSTANCE_CHARTNUMBER,\
   (v1),(v2),(v3),(v4),(v5),(v6),(v7),(v8),(v9),(v10))
#define _SFD_TRANS_COV_WTS(v1,v2,v3,v4,v5) \
	sf_debug_set_instance_transition_coverage_weights(_TMATS_NumericalMethodsMachineNumber_,CHARTINSTANCE_CHARTNUMBER,CHARTINSTANCE_INSTANCENUMBER,\
   (v1),(v2),(v3),(v4),(v5))
#define 	_SFD_TRANS_COV_MAPS(v1,v2,v3,v4,v5,v6,v7,v8,v9,v10,v11,v12,v13) \
	sf_debug_set_chart_transition_coverage_maps(_TMATS_NumericalMethodsMachineNumber_,CHARTINSTANCE_CHARTNUMBER,\
      (v1),\
		(v2),(v3),(v4),\
		(v5),(v6),(v7),\
		(v8),(v9),(v10),\
		(v11),(v12),(v13))

#define _SFD_DATA_CHANGE_EVENT_COUNT(v1,v2) \
	sf_debug_set_number_of_data_with_change_event_for_chart(_TMATS_NumericalMethodsMachineNumber_,CHARTINSTANCE_CHARTNUMBER,\
	(v1),(v2))
#define _SFD_STATE_ENTRY_EVENT_COUNT(v1,v2) \
	sf_debug_set_number_of_states_with_entry_event_for_chart(_TMATS_NumericalMethodsMachineNumber_,CHARTINSTANCE_CHARTNUMBER,\
	(v1),(v2))
#define _SFD_STATE_EXIT_EVENT_COUNT(v1,v2) \
	sf_debug_set_number_of_states_with_exit_event_for_chart(_TMATS_NumericalMethodsMachineNumber_,CHARTINSTANCE_CHARTNUMBER,\
	(v1),(v2))
#define _SFD_EVENT_SCOPE(v1,v2)\
	sf_debug_set_chart_event_scope(_TMATS_NumericalMethodsMachineNumber_,\
	CHARTINSTANCE_CHARTNUMBER,(v1),(v2))

#define _SFD_CH_SUBSTATE_COUNT(v1) \
	sf_debug_set_chart_substate_count(_TMATS_NumericalMethodsMachineNumber_,CHARTINSTANCE_CHARTNUMBER,(v1))
#define _SFD_CH_SUBSTATE_DECOMP(v1) \
	sf_debug_set_chart_decomposition(_TMATS_NumericalMethodsMachineNumber_,CHARTINSTANCE_CHARTNUMBER,(v1))

#define _SFD_CV_INIT_CHART(v1,v2,v3,v4)\
 sf_debug_cv_init_chart(_TMATS_NumericalMethodsMachineNumber_,CHARTINSTANCE_CHARTNUMBER,\
	CHARTINSTANCE_INSTANCENUMBER,(v1),(v2),(v3),(v4))

#define _SFD_CV_INIT_STATE(v1,v2,v3,v4,v5,v6,v7,v8)\
	sf_debug_cv_init_state(_TMATS_NumericalMethodsMachineNumber_,CHARTINSTANCE_CHARTNUMBER,\
	CHARTINSTANCE_INSTANCENUMBER,(v1),(v2),(v3),(v4),(v5),(v6),(v7),(v8))

#define _SFD_CV_INIT_TRANSITION_TESTOBJECTIVE(v1,v2,v3,v4)\
     sf_debug_cv_init_testobjectives(_TMATS_NumericalMethodsMachineNumber_,\
	  CHARTINSTANCE_CHARTNUMBER,\
	  CHARTINSTANCE_INSTANCENUMBER,\
	  TRANSITION_OBJECT,(v1),(v2),(v3),(v4))

#define _SFD_CV_INIT_STATE_TESTOBJECTIVE(v1,v2,v3,v4)\
     sf_debug_cv_init_testobjectives(_TMATS_NumericalMethodsMachineNumber_,\
	  CHARTINSTANCE_CHARTNUMBER,\
	  CHARTINSTANCE_INSTANCENUMBER,\
	  STATE_OBJECT, (v1),(v2),(v3),(v4))

#define _SFD_CV_INIT_TRANS(v1,v2,v3,v4,v5,v6)\
     sf_debug_cv_init_trans(_TMATS_NumericalMethodsMachineNumber_,\
	  CHARTINSTANCE_CHARTNUMBER,\
	  CHARTINSTANCE_INSTANCENUMBER,\
	  (v1),(v2),(v3),(v4),(v5),(v6))
#endif

