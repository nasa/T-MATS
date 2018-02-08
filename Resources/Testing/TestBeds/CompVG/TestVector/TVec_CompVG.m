function MWS = TVec_CompVG(MWS)
%		TMATS -- setup_CompVG.m
% *************************************************************************
% written by Jeffryes Chapman
% NASA Glenn Research Center, Cleveland, OH
% Sept 21, 2012
%
% This function creates the properties of the variable Compressor component.
% *************************************************************************


MWS.TV.VarNames = {'CompVG' 'W' 'FARc' 'Tt' 'Pt' 'Rline' 'Alpha' 'Nmech'};

WIn = [102 104];
FARcIn = 0.02;
TtIn = 2449.02;
PtIn = 99;
Rline = [1.99, 2.01];
Alpha = [1.5, 2.1];
Nmech = 10001;


a = 1;
for i1 = 1:length(WIn)
    for i2 = 1:length(FARcIn)
        for i3 = 1:length(TtIn)
            for i4 = 1:length(PtIn)
                for i5 = 1:length(Rline)
                    for i6 = 1:length(Alpha)
                        for i7 = 1:length(Nmech)
                            MWS.TV.TV(1,a) = WIn(i1);
                            MWS.TV.TV(2,a) = FARcIn(i2);
                            MWS.TV.TV(3,a) = TtIn(i3);
                            MWS.TV.TV(4,a) = PtIn(i4);
                            MWS.TV.TV(5,a) = Rline(i5);
                            MWS.TV.TV(6,a) = Alpha(i6);
                            MWS.TV.TV(7,a) = Nmech(i7);
                            a = a + 1;
                        end
                    end
                end
            end
        end
    end
end




