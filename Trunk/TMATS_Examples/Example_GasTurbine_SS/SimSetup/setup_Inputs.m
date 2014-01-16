function MWS = setup_Inputs(MWS)
%		T-MATS -- setup_Inputs.m
% *************************************************************************
% written by Jeffyres Chapman
% NASA Glenn Research Center, Cleveland, OH
% Dec 19th, 2012
%
% This file contains setup constants for the Plant Solver Template
% defining the dynamic input. To create different test vectors simply over
% right this file with the desired time dependent input.
% *************************************************************************

MWS.in.SimTime = 700 * MWS.Solve.T; % MWS.Solve.T set to 0.015sec


