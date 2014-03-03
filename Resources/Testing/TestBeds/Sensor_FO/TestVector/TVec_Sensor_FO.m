function MWS = TVec_Sensor_FO(MWS)
%		TMATS -- setup_Sensor_FO.m
% *************************************************************************
% written by Jeffryes Chapman 
% NASA Glenn Research Center, Cleveland, OH
% Dec 18th, 2012
%
% This function creates the properties of the 1st order sensor component.
% *************************************************************************


MWS.TV.VarNames = {'Sensor_FO' 'Inp' 'max' 'min' 'FltG' 'FltD'};

% step at second 3 of a 10 second run. Initial value defined in the setup file.
Inp = 10;           %Final value of step
max = 9.9;
min = 0.1;
FltG = [1 .5];
FltD = [0 1];


a = 1;
for i1 = 1:length(Inp)
    for i2 = 1:length(max)
        for i3 = 1:length(min)
            for i4 = 1:length(FltG)
                for i5 = 1:length(FltD)
                    MWS.TV.TV(1,a) = Inp(i1);
                    MWS.TV.TV(2,a) = max(i2);
                    MWS.TV.TV(3,a) = min(i3);
                    MWS.TV.TV(4,a) = FltG(i4);
                    MWS.TV.TV(5,a) = FltD(i5);
                    a = a + 1;
                end
            end
        end
    end
end




