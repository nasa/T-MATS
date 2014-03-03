function [MWS] = Tsetup_Valve(MWS)
%		TMATS -- setup_Valve.m
% *************************************************************************
% written by Jeffryes Chapman
% NASA Glenn Research Center, Cleveland, OH
% Dec 18th, 2012
%
% This function creates the properties of the Valve component.
% *************************************************************************


MWS.Valve.VFO = 0.9;

MWS.Valve.VDZ = 0.1;
				
MWS.Valve.VAA = 20;

MWS.Valve.PRvec = [1 10];

MWS.Valve.VWArray = [0 0.99];
