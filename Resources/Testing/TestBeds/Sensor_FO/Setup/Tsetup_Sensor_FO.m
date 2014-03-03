function [MWS] = Tsetup_Sensor_FO(MWS)
%		TMATS -- setup_Sensor_FO.m
% *************************************************************************
% written by Jeffryes Chapman
% NASA Glenn Research Center, Cleveland, OH
% Dec 18th, 2012
%
% This function creates the properties of the 1st order sensor component.
% *************************************************************************

MWS.Sensor_FO.Gain = 1;
MWS.Sensor_FO.Tau = 0.01;

MWS.Sensor_FO.Noise_SampleTime = 0.001;

%Step inputs
MWS.Sensor_FO.Step_Time = 3;
MWS.Sensor_FO.Step_IV = 0;

				
