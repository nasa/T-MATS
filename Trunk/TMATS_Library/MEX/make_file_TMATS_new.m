%		T-MATS -- make_file.m
% *************************************************************************
% written by Jeffryes Chapman based on work by Ryan May and Jeff Csank 
% NASA Glenn Research Center, Cleveland, OH
% Dec 3, 2012
%
% This file compiles all the required C files for the engine components into MEX functions.  
% All output files are stored in the parent directory.
% Generally, this file will be called by make_all.m in the root T-MATS folder.  
% *************************************************************************/

delete '*.mex*'

mex Ambient_TMATS.c pt2sc_TMATS.c sp2tc_TMATS.c t2hc_TMATS.c interp1Ac_TMATS.c interp2Ac_TMATS.c functions_TMATS.c
mex Bleed_TMATS.c t2hc_TMATS.c h2tc_TMATS.c functions_TMATS.c
mex Burner_TMATS.c t2hc_TMATS.c h2tc_TMATS.c functions_TMATS.c
mex Compressor_TMATS.c h2tc_TMATS.c pt2sc_TMATS.c sp2tc_TMATS.c t2hc_TMATS.c interp1Ac_TMATS.c interp2Ac_TMATS.c interp3Ac_TMATS.c functions_TMATS.c
mex DO_TMATS.c 
mex Enth2Temp_TMATS.c h2tc_TMATS.c 
mex H2TM_TMATS.c h2tc_TMATS.c
mex Mixer_TMATS.c h2tc_TMATS.c PcalcStat_TMATS.c t2hc_TMATS.c interp1Ac_TMATS.c interp2Ac_TMATS.c sp2tc_TMATS.c pt2sc_TMATS.c functions_TMATS.c
mex Nozzle_TMATS.c PcalcStat_TMATS.c pt2sc_TMATS.c sp2tc_TMATS.c t2hc_TMATS.c interp1Ac_TMATS.c interp2Ac_TMATS.c functions_TMATS.c
mex PT2S_TMATS.c pt2sc_TMATS.c
mex PT2SM_TMATS.c pt2sc_TMATS.c
mex SP2T_TMATS.c sp2tc_TMATS.c
mex SP2TM_TMATS.c sp2tc_TMATS.c
mex StaticCalc_TMATS.c PcalcStat_TMATS.c pt2sc_TMATS.c sp2tc_TMATS.c t2hc_TMATS.c interp1Ac_TMATS.c interp2Ac_TMATS.c functions_TMATS.c
mex T2HM_TMATS.c t2hc_TMATS.c
mex Temp2Enth_TMATS.c t2hc_TMATS.c
mex Turbine_TMATS.c h2tc_TMATS.c pt2sc_TMATS.c sp2tc_TMATS.c t2hc_TMATS.c interp2Ac_TMATS.c functions_TMATS.c
mex Turbine_PSI_TMATS.c h2tc_TMATS.c pt2sc_TMATS.c sp2tc_TMATS.c t2hc_TMATS.c interp2Ac_TMATS.c functions_TMATS.c
mex Valve_TMATS.c interp1Ac_TMATS.c functions_TMATS.c
mex ValvePHY_TMATS.c interp2Ac_TMATS.c interp1Ac_TMATS.c calc_Pstatic_TMATS.c calc_WvsMN_TMATS.c calc_PsvsMN_TMATS.c functions_TMATS.c
mex Ambient_TMATS_new.c Ambient_TMATS_body.c pt2sc_TMATS.c sp2tc_TMATS.c t2hc_TMATS.c interp1Ac_TMATS.c interp2Ac_TMATS.c functions_TMATS.c
mex Bleed_TMATS_new.c Bleed_TMATS_body.c t2hc_TMATS.c h2tc_TMATS.c functions_TMATS.c
mex Burner_TMATS_new.c Burner_TMATS_body.c t2hc_TMATS.c h2tc_TMATS.c functions_TMATS.c
mex Compressor_TMATS_new.c Compressor_TMATS_body.c h2tc_TMATS.c pt2sc_TMATS.c sp2tc_TMATS.c t2hc_TMATS.c interp1Ac_TMATS.c interp2Ac_TMATS.c interp3Ac_TMATS.c functions_TMATS.c
mex DO_TMATS_new.c
mex Enth2Temp_TMATS_new.c h2tc_TMATS.c
mex Mixer_TMATS_new.c Mixer_TMATS_body.c h2tc_TMATS.c PcalcStat_TMATS.c t2hc_TMATS.c interp1Ac_TMATS.c interp2Ac_TMATS.c sp2tc_TMATS.c pt2sc_TMATS.c functions_TMATS.c
mex Nozzle_TMATS_new.c Nozzle_TMATS_body.c PcalcStat_TMATS.c pt2sc_TMATS.c sp2tc_TMATS.c t2hc_TMATS.c interp1Ac_TMATS.c interp2Ac_TMATS.c functions_TMATS.c
mex PT2S_TMATS_new.c pt2sc_TMATS.c
mex SP2T_TMATS_new.c sp2tc_TMATS.c
mex StaticCalc_TMATS_new.c StaticCalc_TMATS_body.c PcalcStat_TMATS.c pt2sc_TMATS.c sp2tc_TMATS.c t2hc_TMATS.c interp1Ac_TMATS.c interp2Ac_TMATS.c functions_TMATS.c
mex Temp2Enth_TMATS_new.c t2hc_TMATS.c
mex Turbine_TMATS_new.c Turbine_TMATS_body.c h2tc_TMATS.c pt2sc_TMATS.c sp2tc_TMATS.c t2hc_TMATS.c interp2Ac_TMATS.c functions_TMATS.c
mex Turbine_PSI_TMATS_new.c Turbine_PSI_TMATS_body.c h2tc_TMATS.c pt2sc_TMATS.c sp2tc_TMATS.c t2hc_TMATS.c interp2Ac_TMATS.c functions_TMATS.c
mex Valve_TMATS_new.c Valve_TMATS_body.c interp1Ac_TMATS.c functions_TMATS.c
mex ValvePHY_TMATS_new.c ValvePHY_TMATS_body.c interp2Ac_TMATS.c interp1Ac_TMATS.c calc_Pstatic_TMATS.c calc_WvsMN_TMATS.c calc_PsvsMN_TMATS.c functions_TMATS.c