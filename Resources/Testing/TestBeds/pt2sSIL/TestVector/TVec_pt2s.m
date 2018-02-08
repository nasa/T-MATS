function MWS = TVec_pt2s(MWS)
%		TMATS -- setup_pt2s.m
% *************************************************************************
% written by Jeffryes Chapman
% NASA Glenn Research Center, Cleveland, OH
% Sept 21, 2017
%
% This function creates the properties of the pt2s component.
% *************************************************************************


MWS.TV.VarNames = {'pt2s','p','t','FAR'};

p = [15 16];
t = [518 600];
FAR = [0 0.01 0.02];

a = 1;
for i1 = 1:length(p)
    for i2 = 1:length(t)
        for i3 = 1:length(FAR)
            MWS.TV.TV(1,a) = p(i1);
            MWS.TV.TV(2,a) = t(i2);
            MWS.TV.TV(3,a) = FAR(i3);
            a = a + 1;
        end
    end
end




