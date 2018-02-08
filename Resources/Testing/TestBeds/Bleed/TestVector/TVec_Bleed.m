function MWS = TVec_Bleed(MWS)
%		TMATS -- setup_Bleed.m
% *************************************************************************
% written by Jeffryes Chapman
% NASA Glenn Research Center, Cleveland, OH
% Sept 20th, 2017
%
% This function creates the properties of the Bleed component.
% *************************************************************************


MWS.TV.VarNames = {'Bleed' 'W1' 'Tt1' 'Pt1' 'FAR1' 'W2' 'Tt2' 'Pt2' 'FAR2'};

W1 = 1.0;
Tt1 = 732.86;
Pt1 = 40.417;
FAR1 = 0.001;
W2 = 135.59;
Tt2 = 1814.58;
Pt2 = 41.589;
FAR2 = 0.02331;


a = 1;
for i1 = 1:length(W1)
    for i2 = 1:length(Tt1)
        for i3 = 1:length(Pt1)
            for i4 = 1:length(FAR1)
                for i5 = 1:length(W2)
                    for i6 = 1:length(Tt2)
                        for i7 = 1:length(Pt2)
                            for i8 = 1:length(FAR2)
                                MWS.TV.TV(1,a) = W1(i1);
                                MWS.TV.TV(2,a) = Tt1(i2);
                                MWS.TV.TV(3,a) = Pt1(i3);
                                MWS.TV.TV(4,a) = FAR1(i4);
                                MWS.TV.TV(5,a) = W2(i5);
                                MWS.TV.TV(6,a) = Tt2(i6);
                                MWS.TV.TV(7,a) = Pt2(i7);
                                MWS.TV.TV(8,a) = FAR2(i8);
                                a = a + 1;
                            end
                        end
                    end
                end
            end
        end
    end
end





