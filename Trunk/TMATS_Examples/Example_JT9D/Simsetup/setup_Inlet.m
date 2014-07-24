function MWS = setup_Inlet(MWS)
%		T-MATS -- setup_Inlet.m
% *************************************************************************
% written by Jeffryes Chapman
% NASA Glenn Research Center, Cleveland, OH
% Dec 18th, 2012
%
% This function creates the properties of the Inlet components for the JT9D.
% *************************************************************************
            
  
 MWS.Inlet.eRamBase = 0.9920;
 MWS.Inlet.eRamVec = [.99 1 1.007 1.028 1.1 2];
 MWS.Inlet.eRamtbl = [1 1 1.001 1.002 1.003 1.003];
 
