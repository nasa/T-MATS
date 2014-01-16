function MWS = setup_Burner(MWS)
%		T-MATS -- setup_Burner.m
% *************************************************************************
% written by Jeffyres Chapman
% NASA Glenn Research Center, Cleveland, OH
% Dec 19th, 2012
%
% This function creates the properties of the Burner Module 
% *************************************************************************

% Lower Heating Value
MWS.Burner.LHV = 18400 ; 
% normalized delta P across the burner.
MWS.Burner.dP = 0.05 ;
