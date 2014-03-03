function MWS = TVec_SolverDyn(MWS)
%		TMATS -- setup_SolverDyn.m
% *************************************************************************
% written by Jeffryes Chapman 
% NASA Glenn Research Center, Cleveland, OH
% Dec 18th, 2012
%
% This function creates the properties of the Dynamic Solver component.
% *************************************************************************


MWS.TV.VarNames = {'SolverDyn' 'dX' 'Jatm' 'Cond_Lim'};

dX = 5;
Jatm = [20 4];
Cond_Lim = [0.1 0.001];

a = 1;
for i1 = 1:length(dX)
    for i2 = 1:length(Jatm)
        for i3 = 1:length(Cond_Lim)
                    MWS.TV.TV(1,a) = dX(i1);
                    MWS.TV.TV(2,a) = Jatm(i2);
                    MWS.TV.TV(3,a) = Cond_Lim(i3);
                    a = a + 1;
        end
    end
end




