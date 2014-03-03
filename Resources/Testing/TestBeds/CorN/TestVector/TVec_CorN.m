function MWS = TVec_CorN(MWS)
%		TMATS -- setup_CorN.m
% *************************************************************************
% written by Jeffryes Chapman 
% NASA Glenn Research Center, Cleveland, OH
% Dec 18th, 2012
%
% This function creates the properties of the corrected speed component.
% *************************************************************************


MWS.TV.VarNames = {'CorN' 'N' 'Tt'};

N = [10000 10000];
Tt = 500;


a = 1;
for i1 = 1:length(N)
    for i2 = 1:length(Tt)
                    MWS.TV.TV(1,a) = N(i1);
                    MWS.TV.TV(2,a) = Tt(i2);
                    a = a + 1;
    end
end




