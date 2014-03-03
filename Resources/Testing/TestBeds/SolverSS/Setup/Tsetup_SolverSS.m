function [MWS] = Tsetup_SolverSS(MWS)
%		TMATS -- setup_SolverSS.m
% *************************************************************************
% written by Jeffryes Chapman
% NASA Glenn Research Center, Cleveland, OH
% Dec 18th, 2012
%
% This function creates the properties of the Steady State Solver component.
% *************************************************************************


MWS.SolverSS.Per = [0.05; 0.05];

MWS.SolverSS.IC = [1; 1];


				
