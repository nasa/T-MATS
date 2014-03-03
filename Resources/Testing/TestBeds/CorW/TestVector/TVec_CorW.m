function MWS = TVec_CorW(MWS)
%		TMATS -- setup_CorW.m
% *************************************************************************
% written by Jeffryes Chapman 
% NASA Glenn Research Center, Cleveland, OH
% Dec 18th, 2012
%
% This function creates the properties of the corrected gas path flow component.
% *************************************************************************


MWS.TV.VarNames = {'CorW' 'W' 'Tt' 'Pt'};

WIn = [3 4];
Tt = 500;
Pt = 40;


a = 1;
for i1 = 1:length(WIn)
    for i2 = 1:length(Tt)
        for i3 = 1:length(Pt)
                    MWS.TV.TV(1,a) = WIn(i1);
                    MWS.TV.TV(2,a) = Tt(i2);
                    MWS.TV.TV(3,a) = Pt(i3);
                    a = a + 1;
        end
    end
end




