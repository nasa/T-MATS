function MWS = TVec_ControllerPI(MWS)
%		TMATS -- setup_ControllerPI.m
% *************************************************************************
% written by Jeffryes Chapman 
% NASA Glenn Research Center, Cleveland, OH
% Dec 18th, 2012
%
% This function creates the properties of the PI Controller component.
% *************************************************************************


MWS.TV.VarNames = {'ControllerPI' 'Inp'};

% step at second 3 of a 10 second run. Initial value defined in the setup file.
Inp = 10;           %Final value of step


a = 1;
for i1 = 1:length(Inp)
                    MWS.TV.TV(1,a) = Inp(i1);
                    a = a + 1;

end




