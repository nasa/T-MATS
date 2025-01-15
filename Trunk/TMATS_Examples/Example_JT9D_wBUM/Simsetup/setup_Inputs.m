function MWS = setup_Inputs(MWS)
%		T-MATS -- setup_Inputs.m
% *************************************************************************
% written by Jeffryes Chapman
% NASA Glenn Research Center, Cleveland, OH
% July 8th, 2014
%
% This function creates the properties of the Duct components for the JT9D.
% *************************************************************************

MWS.Input.W = 674.22;
MWS.Input.Fan_Rline = 2.3706;
MWS.Input.BPR = 5.0408;
MWS.Input.LPC_Rline = 1.7664;
MWS.Input.HPC_Rline = 2.0424;
MWS.Input.HPT_PR = 2.670;
MWS.Input.LPT_PR = 4.886;
MWS.Input.LP_Shaft = 3667.6;
MWS.Input.HP_Shaft = 7390.0 ;

MWS.Input.Tt = 448.46;
MWS.Input.Pt = 5.528;
MWS.Input.Pamb = 3.626;
MWS.Input.FARin = 0.01871;

% Set IC
MWS.Input.NRIC = [MWS.Input.W MWS.Input.Fan_Rline MWS.Input.BPR MWS.Input.LPC_Rline...
    MWS.Input.HPC_Rline MWS.Input.HPT_PR MWS.Input.LPT_PR MWS.Input.LP_Shaft MWS.Input.HP_Shaft];

MWS.Input.NRICDyn = [MWS.Input.W MWS.Input.Fan_Rline MWS.Input.BPR MWS.Input.LPC_Rline...
    MWS.Input.HPC_Rline MWS.Input.HPT_PR MWS.Input.LPT_PR];

% IDesign value
MWS.iDes = 2;

% Shaft Inertia
MWS.Shaft.Iner_lp = 100;
MWS.Shaft.Iner_hp = 20;