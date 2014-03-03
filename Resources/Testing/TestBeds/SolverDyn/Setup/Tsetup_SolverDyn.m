function [MWS] = Tsetup_SolverDyn(MWS)
%		TMATS -- setup_SolverDyn.m
% *************************************************************************
% written by Jeffryes Chapman
% NASA Glenn Research Center, Cleveland, OH
% Dec 18th, 2012
%
% This function creates the properties of the Dynamic Solver component.
% *************************************************************************


MWS.SolverDyn.Per = [0.01; 0.05];

MWS.SolverDyn.IC = [2; 0.5];


				
