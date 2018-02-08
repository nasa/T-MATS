function MWS = TVec_h2t(MWS)
%		TMATS -- setup_h2t.m
% *************************************************************************
% written by Jeffryes Chapman 
% NASA Glenn Research Center, Cleveland, OH
% Sept 21, 2017
%
% This function creates the properties of the h2t component.
% *************************************************************************


MWS.TV.VarNames = {'h2t' 'h' 'FAR'};

h = [155 160];
FAR = [0 0.01 0.02];

a = 1;
for i1 = 1:length(h)
    for i2 = 1:length(FAR)
                    MWS.TV.TV(1,a) = h(i1);
                    MWS.TV.TV(2,a) = FAR(i2);
                    a = a + 1;
    end
end




