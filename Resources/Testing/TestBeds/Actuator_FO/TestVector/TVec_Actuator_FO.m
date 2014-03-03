function MWS = TVec_Actuator_FO(MWS)
%		TMATS -- setup_Actuator_FO.m
% *************************************************************************
% written by Jeffryes Chapman 
% NASA Glenn Research Center, Cleveland, OH
% Dec 18th, 2012
%
% This function creates the properties of the 1st order Actuator component.
% *************************************************************************


MWS.TV.VarNames = {'Actuator_FO' 'Inp' 'max' 'min' 'DeadBand' 'RateLim'};

% step at second 3 of a 10 second run. Initial value defined in the setup file.
Inp = 10;           %Final value of step
max = 9.9;
min = 0.1;
DeadBand = 0.1;
RateLim = 2;


a = 1;
for i1 = 1:length(Inp)
    for i2 = 1:length(max)
        for i3 = 1:length(min)
            for i4 = 1:length(DeadBand)
                for i5 = 1:length(RateLim)
                    MWS.TV.TV(1,a) = Inp(i1);
                    MWS.TV.TV(2,a) = max(i2);
                    MWS.TV.TV(3,a) = min(i3);
                    MWS.TV.TV(4,a) = DeadBand(i4);
                    MWS.TV.TV(5,a) = RateLim(i5);
                    a = a + 1;
                end
            end
        end
    end
end




