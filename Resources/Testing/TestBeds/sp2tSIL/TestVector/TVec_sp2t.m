function MWS = TVec_sp2t(MWS)
%		TMATS -- setup_sp2t.m
% *************************************************************************
% written by Jeffryes Chapman
% NASA Glenn Research Center, Cleveland, OH
% Sept 21, 2017
%
% This function creates the properties of the sp2t component.
% *************************************************************************


MWS.TV.VarNames = {'sp2t','s','p','FAR'};

s = [-0.173 -0.16];
p = [518 600];
FAR = [0 0.01 0.02];

a = 1;
for i1 = 1:length(s)
    for i2 = 1:length(p)
        for i3 = 1:length(FAR)
            MWS.TV.TV(1,a) = s(i1);
            MWS.TV.TV(2,a) = p(i2);
            MWS.TV.TV(3,a) = FAR(i3);
            a = a + 1;
        end
    end
end




