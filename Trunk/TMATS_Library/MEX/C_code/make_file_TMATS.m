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

cd ..
delete '*.mex*'
cd C_code

mex Ambient_TMATS.c interp1Ac_TMATS.c -outdir ..
mex Burner_TMATS.c t2hc_TMATS.c h2tc_TMATS.c -outdir ..
mex Compressor_TMATS.c h2tc_TMATS.c pt2sc_TMATS.c sp2tc_TMATS.c t2hc_TMATS.c interp1Ac_TMATS.c interp2Ac_TMATS.c -outdir ..
mex Enth2Temp_TMATS.c h2tc_TMATS.c -outdir ..
mex Mixer_TMATS.c h2tc_TMATS.c PcalcStat_TMATS.c t2hc_TMATS.c interp1Ac_TMATS.c sp2tc_TMATS.c pt2sc_TMATS.c -outdir ..
mex Nozzle_TMATS.c PcalcStat_TMATS.c pt2sc_TMATS.c sp2tc_TMATS.c t2hc_TMATS.c interp1Ac_TMATS.c interp2Ac_TMATS.c -outdir ..
mex PT2S_TMATS.c pt2sc_TMATS.c -outdir ..
mex SP2T_TMATS.c sp2tc_TMATS.c -outdir ..
mex StaticCalc_TMATS.c PcalcStat_TMATS.c pt2sc_TMATS.c sp2tc_TMATS.c t2hc_TMATS.c interp1Ac_TMATS.c interp2Ac_TMATS.c -outdir ..
mex Temp2Enth_TMATS.c t2hc_TMATS.c -outdir ..
mex Turbine_TMATS.c h2tc_TMATS.c pt2sc_TMATS.c sp2tc_TMATS.c t2hc_TMATS.c interp2Ac_TMATS.c -outdir ..
mex Turbine_NPSS_TMATS.c h2tc_TMATS.c pt2sc_TMATS.c sp2tc_TMATS.c t2hc_TMATS.c interp2Ac_TMATS.c -outdir ..
mex Turbine_PSI_TMATS.c h2tc_TMATS.c pt2sc_TMATS.c sp2tc_TMATS.c t2hc_TMATS.c interp2Ac_TMATS.c -outdir ..
mex Valve_TMATS.c interp1Ac_TMATS.c -outdir ..
mex ValvePHY_TMATS.c interp2Ac_TMATS.c interp1Ac_TMATS.c calc_Pstatic_TMATS.c calc_WvsMN_TMATS.c calc_PsvsMN_TMATS.c -outdir ..
