function MWS = TVec_Shaft(MWS)
%		TMATS -- setup_Shaft.m
% *************************************************************************
% written by Jeffryes Chapman 
% NASA Glenn Research Center, Cleveland, OH
% Dec 18th, 2012
%
% This function creates the properties of the Shaft component.
% *************************************************************************


MWS.TV.VarNames = {'Shaft' 'Trq1' 'Trq2' 'Trq3' 'Nmech' 'Inertia'};

Trq1 = [2000 1800];
Trq2 = -1000;
Trq3 = -1000;
Nmech = 10000;
Inertia = 0.3183;


a = 1;
for i1 = 1:length(Trq1)
    for i2 = 1:length(Trq2)
        for i3 = 1:length(Trq3)
            for i4 = 1:length(Nmech)
                for i5 = 1:length(Inertia)
                    MWS.TV.TV(1,a) = Trq1(i1);
                    MWS.TV.TV(2,a) = Trq2(i2);
                    MWS.TV.TV(3,a) = Trq3(i3);
                    MWS.TV.TV(4,a) = Nmech(i4);
                    MWS.TV.TV(5,a) = Inertia(i5);
                    a = a + 1;
                end
            end
        end
    end
end




