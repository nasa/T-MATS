function [MWS] = Tsetup_Actuator_FO(MWS)
%		TMATS -- setup_Actuator_FO.m
% *************************************************************************
% written by Jeffryes Chapman
% NASA Glenn Research Center, Cleveland, OH
% Dec 18th, 2012
%
% This function creates the properties of the 1st order Actuator component.
% *************************************************************************

MWS.Actuator_FO.Gain = 1;
MWS.Actuator_FO.Tau = 0.01;


%Step inputs
MWS.Actuator_FO.Step_Time = 3;
MWS.Actuator_FO.Step_IV = 0;

				
