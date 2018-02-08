function MWS = TVec_t2h(MWS)
%		TMATS -- setup_t2h.m
% *************************************************************************
% written by Jeffryes Chapman 
% NASA Glenn Research Center, Cleveland, OH
% Sept 21, 2017
%
% This function creates the properties of the t2h component.
% *************************************************************************


MWS.TV.VarNames = {'t2h' 't' 'FAR'};

t = [518 3000];
FAR = [0 0.01 0.02];

a = 1;
for i1 = 1:length(t)
    for i2 = 1:length(FAR)
                    MWS.TV.TV(1,a) = t(i1);
                    MWS.TV.TV(2,a) = FAR(i2);
                    a = a + 1;
    end
end




