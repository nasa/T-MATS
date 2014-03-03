function [MWS] = Tsetup_Valve_PHY(MWS)
%		TMATS -- setup_Valve_PHY.m
% *************************************************************************
% written by Jeffryes Chapman
% NASA Glenn Research Center, Cleveland, OH
% Dec 18th, 2012
%
% This function creates the properties of the Valve PHY component.
% *************************************************************************
				
MWS.ValvePHY.VAA = 20;

MWS.ValvePHY.VAbyp = 10;

MWS.ValvePHY.VAth = 15;

MWS.ValvePHY.FARvec = [0 1];
MWS.ValvePHY.RtArray = [0.068 0.068];

MWS.ValvePHY.Ttvec = [0 1000];
MWS.ValvePHY.GArray = [1.4 1.4];
