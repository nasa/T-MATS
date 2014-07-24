% function MWS = setup_calcScalars(MWS)
%		T-MATS -- setup_calcScalars.m
% *************************************************************************
% written by Jeffryes Chapman
% NASA Glenn Research Center, Cleveland, OH
% Dec 18th, 2012
%
% This function re-creates the scalars for each component for the JT9D.
% *************************************************************************
Fin = length(FAN_S_Wc);

%------ scalars for Fan -------------
MWS.FAN.s_Wc = FAN_S_Wc(Fin);
MWS.FAN.s_PR = FAN_S_PR(Fin);  
MWS.FAN.s_eff = FAN_S_Eff(Fin);
MWS.FAN.s_Nc = FAN_S_Nc(Fin);

%------ scalars for LPC -------------
MWS.LPC.s_Wc = LPC_S_Wc(Fin);
MWS.LPC.s_PR = LPC_S_PR(Fin);  
MWS.LPC.s_eff = LPC_S_Eff(Fin);
MWS.LPC.s_Nc = LPC_S_Nc(Fin);

%------ scalars for HPC -------------
MWS.HPC.s_Wc = HPC_S_Wc(Fin);
MWS.HPC.s_PR = HPC_S_PR(Fin);  
MWS.HPC.s_eff = HPC_S_Eff(Fin);
MWS.HPC.s_Nc = HPC_S_Nc(Fin);

%--------- scalars for HPT ----------
MWS.HPT.s_Wc = HPT_S_Wc(Fin);
MWS.HPT.s_PR = HPT_S_PR(Fin);  
MWS.HPT.s_eff = HPT_S_Eff(Fin);
MWS.HPT.s_Nc = HPT_S_Nc(Fin);

%--------- scalars for LPT ----------
MWS.LPT.s_Wc = LPT_S_Wc(Fin);
MWS.LPT.s_PR = LPT_S_PR(Fin);  
MWS.LPT.s_eff = LPT_S_Eff(Fin);
MWS.LPT.s_Nc = LPT_S_Nc(Fin);

