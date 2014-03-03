function [MWS,Max_Iter] = TVec_Ambient(MWS)
%		TMATS -- setup_Ambient.m
% *************************************************************************
% written by Jeffryes Chapman 
% NASA Glenn Research Center, Cleveland, OH
% Dec 18th, 2012
%
% This function creates the properties of the Ambient component.
% *************************************************************************

MWS.TV.VarNames = {'Amb' 'W' 'Alt' 'dTamb' 'MN'};

WIn = [102 104];
AltIn = 1000;
dTambIn = 1;
MNIn = 0.5;


a = 1;
for i = 1:length(WIn)
    for ii = 1:length(AltIn)
        for iii = 1:length(dTambIn)
            for iiii = 1:length(MNIn)
                        MWS.TV.TV(1,a) = WIn(i);
                        MWS.TV.TV(2,a) = AltIn(ii);
                        MWS.TV.TV(3,a) = dTambIn(iii);
                        MWS.TV.TV(4,a) = MNIn(iiii);
                        a = a + 1;
            end
        end
    end
                    
end

 


 
 