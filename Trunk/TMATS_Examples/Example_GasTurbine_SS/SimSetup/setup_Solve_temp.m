function MWS = setup_Solve_temp(MWS)
%		T-MATS -- setup_Solve_temp.m
% *************************************************************************
% written by Jeffyres Chapman
% NASA Glenn Research Center, Cleveland, OH
% Dec 19th, 2012
%
% This file contains the setup constants for the Plant Solver Template
% *************************************************************************


MWS.Solve.C_Lim = 1e-5;
MWS.Solve.J_Calc_Per = [0.01 0.05 0.05 0.01; -0.01 -0.05 -0.05 -0.01];
MWS.Solve.NR_IC = [100 2 3 10000]';
MWS.Solve.NR_dx = 5;
MWS.Solve.NR_Jac_Re = 20;
MWS.Solve.Max_Iter = 200;
MWS.Solve.T = 0.015;

