function MWS = TVec_VCond(MWS)
%		TMATS -- setup_VCond.m
% *************************************************************************
% written by Jeffryes Chapman 
% NASA Glenn Research Center, Cleveland, OH
% Dec 18th, 2012
%
% This function creates the properties of the Vector Condition component.
% *************************************************************************


MWS.TV.VarNames = {'VCond' 'Inp1' 'Inp2' 'CondLim'};

Inp1 = [1 0.009];
Inp2 = 0.001;
CondLim = 0.01;



a = 1;
for i1 = 1:length(Inp1)
    for i2 = 1:length(Inp2)
        for i3 = 1:length(CondLim)
                    MWS.TV.TV(1,a) = Inp1(i1);
                    MWS.TV.TV(2,a) = Inp2(i2);
                    MWS.TV.TV(3,a) = CondLim(i3);
                    a = a + 1;
        end
    end
end




