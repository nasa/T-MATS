function MWS = TVec_Valve(MWS)
%		TMATS -- setup_Valve.m
% *************************************************************************
% written by Jeffryes Chapman  based on work by Dean K. Frederick, J. Hillmon, and Ryan May
% NASA Glenn Research Center, Cleveland, OH
% Dec 18th, 2012
%
% This function creates the properties of the Valve component.
% *************************************************************************


MWS.TV.VarNames = {'Valve' 'W' 'FARc' 'Tt' 'Pt' 'Pos' 'Wbyp' 'Ttbyp'...
    'Ptbyp' 'FARcbyp'};

WIn = [102 104];
FARcIn = 0.03;
TtIn = 2449.02;
PtIn = 99;
Pos = 2;
WIn_byp = [10 11];
FARcIn_byp = 0.0;
TtIn_byp = 600;
PtIn_byp = 100;

a = 1;
for i1 = 1:length(WIn)
    for i2 = 1:length(FARcIn)
        for i3 = 1:length(TtIn)
            for i4 = 1:length(PtIn)
                for i5 = 1:length(Pos)
                    for i6 = 1:length(WIn_byp)
                        for i7 = 1:length(FARcIn_byp)
                            for i8 = 1:length(TtIn_byp)
                                for i9 = 1:length(PtIn_byp)
                                    MWS.TV.TV(1,a) = WIn(i1);
                                    MWS.TV.TV(2,a) = FARcIn(i2);
                                    MWS.TV.TV(3,a) = TtIn(i3);
                                    MWS.TV.TV(4,a) = PtIn(i4);
                                    MWS.TV.TV(5,a) = Pos(i5);
                                    MWS.TV.TV(6,a) = WIn_byp(i6);
                                    MWS.TV.TV(7,a) = FARcIn_byp(i7);
                                    MWS.TV.TV(8,a) = TtIn_byp(i8);
                                    MWS.TV.TV(9,a) = PtIn_byp(i9);
                                    a = a + 1;
                                end
                            end
                        end
                    end
                end
            end
        end
    end
    
end




