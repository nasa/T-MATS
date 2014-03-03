function MWS = TVec_Mod_Source(MWS)
%		TMATS -- setup_Mod_Source.m
% *************************************************************************
% written by Jeffryes Chapman
% NASA Glenn Research Center, Cleveland, OH
% Dec 18th, 2012
%
% This function creates the properties of the Mod_Source component.
% *************************************************************************


MWS.TV.VarNames = {'Mod_Source' 'T' 'Inp'};

T = 1;
Inp = 1;

a = 1;
for i1 = 1:length(T)
    for i2 = 1:length(Inp)
        MWS.TV.TV(1,a) = T(i1);
        MWS.TV.TV(2,a) = Inp(i2);
        a = a + 1;
    end
end




