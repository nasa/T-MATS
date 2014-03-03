function [MWS] = Tsetup_ControllerPI(MWS)
%		TMATS -- setup_ControllerPI.m
% *************************************************************************
% written by Jeffryes Chapman
% NASA Glenn Research Center, Cleveland, OH
% Dec 18th, 2012
%
% This function creates the properties of the PI Controller component.
% *************************************************************************

MWS.ControllerPI.Gain = 1;
MWS.ControllerPI.Tau = 0.01;
MWS.ControllerPI.IC = 0;


%Step inputs
MWS.ControllerPI.Step_Time = 3;
MWS.ControllerPI.Step_IV = 0;

				
