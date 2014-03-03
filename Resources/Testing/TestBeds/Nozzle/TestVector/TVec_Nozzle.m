function MWS = TVec_Nozzle(MWS)
%		TMATS -- setup_Nozzle.m
% *************************************************************************
% written by Jeffryes Chapman 
% NASA Glenn Research Center, Cleveland, OH
% Dec 18th, 2012
%
% This function creates the properties of the Nozzle component.
% *************************************************************************


MWS.TV.VarNames = {'Noz' 'W' 'FARc' 'Tt' 'Pt' 'Pa' 'Nmech'};

WIn = [102 104];
FARcIn = 0.03;
TtIn = 2449.02;
PtIn = [99];
Pa = 14.696;
Nmech = 10000;

a = 1;
for i = 1:length(WIn)
    for ii = 1:length(FARcIn)
        for iii = 1:length(TtIn)
            for iiii = 1:length(PtIn)
                for iiiii = 1:length(Pa)
                    for iiiiii = 1:length(Nmech)
                        MWS.TV.TV(1,a) = WIn(i);
                        MWS.TV.TV(2,a) = FARcIn(ii);
                        MWS.TV.TV(3,a) = TtIn(iii);
                        MWS.TV.TV(4,a) = PtIn(iiii);
                        MWS.TV.TV(5,a) = Pa(iiiii);
                        MWS.TV.TV(6,a) = Nmech(iiiiii);
                        a = a + 1;
                   end
                end
            end
        end
    end
                    
end
 


 
 