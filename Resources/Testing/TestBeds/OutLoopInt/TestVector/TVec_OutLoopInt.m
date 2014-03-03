function MWS = TVec_OutLoopInt(MWS)
%		TMATS -- setup_OutLoopInt.m
% *************************************************************************
% written by Jeffryes Chapman 
% NASA Glenn Research Center, Cleveland, OH
% Dec 18th, 2012
%
% This function creates the properties of the Outer Loop Integrator component.
% *************************************************************************


MWS.TV.VarNames = {'OutLoopInt' 'Inp1' 'Inp2'};

Inp1 = 2;
Inp2 = -1;
a = 1;
for i1 = 1:length(Inp1)
    for i2 = 1:length(Inp2)
                    MWS.TV.TV(1,a) = Inp1(i1);
                    MWS.TV.TV(2,a) = Inp2(i2);
                    a = a + 1;
    end
end




