function MWS = TVec_Turbine(MWS)
%		TMATS -- setup_Turbine.m
% *************************************************************************
% written by Jeffryes Chapman
% NASA Glenn Research Center, Cleveland, OH
% Dec 18th, 2012
%
% This function creates the properties of the Turbine component.
% *************************************************************************


MWS.TV.VarNames = {'Turb' 'W' 'FARc' 'Tt' 'Pt' 'PR' 'Nmech' 'Wcool' 'Ttcool'...
                    'Ptcool' 'FARccool'};

WIn = [102 104];
FARcIn = 0.03;
TtIn = 2449.02;
PtIn = 99;
PR = 2;
Nmech = 10000;
WIn_cool = [10 11];
FARcIn_cool = 0.0;
TtIn_cool = 600;
PtIn_cool = 100;

a = 1;
for i1 = 1:length(WIn)
    for i2 = 1:length(FARcIn)
        for i3 = 1:length(TtIn)
            for i4 = 1:length(PtIn)
                for i5 = 1:length(PR)
                    for i6 = 1:length(Nmech)
                        for i7 = 1:length(WIn_cool)
                            for i8 = 1:length(FARcIn_cool)
                                for i9 = 1:length(TtIn_cool)
                                    for i10 = 1:length(PtIn_cool)
                                        MWS.TV.TV(1,a) = WIn(i1);
                                        MWS.TV.TV(2,a) = FARcIn(i2);
                                        MWS.TV.TV(3,a) = TtIn(i3);
                                        MWS.TV.TV(4,a) = PtIn(i4);
                                        MWS.TV.TV(5,a) = PR(i5);
                                        MWS.TV.TV(6,a) = Nmech(i6);
                                        MWS.TV.TV(7,a) = WIn_cool(i7);
                                        MWS.TV.TV(8,a) = FARcIn_cool(i8);
                                        MWS.TV.TV(9,a) = TtIn_cool(i9);
                                        MWS.TV.TV(10,a) = PtIn_cool(i10);
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
    
end




