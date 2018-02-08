function MWS = TVec_StaticCalc(MWS)
%		TMATS -- setup_StaticCalc.m
% *************************************************************************
% written by Jeffryes Chapman
% NASA Glenn Research Center, Cleveland, OH
% Sept 21, 2017
%
% This function creates the properties of the StaticCalc component.
% *************************************************************************


MWS.TV.VarNames = {'StaticCalc','W','t','p','FAR'};

W = [100 101];
t = [518 600];
p = [14.7 16];
FAR = [0 0.01 0.02];

a = 1;
for i1 = 1:length(W)
    for i2 = 1:length(t)
        for i3 = 1:length(p)
            for i4 = 1:length(FAR)
            MWS.TV.TV(1,a) = W(i1);
            MWS.TV.TV(2,a) = t(i2);
            MWS.TV.TV(3,a) = p(i3);
            MWS.TV.TV(4,a) = FAR(i4);
            a = a + 1;
            end
        end
    end
end




