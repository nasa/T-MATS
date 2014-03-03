%		T-MATS -- TMATS_Run_Example.m
% *************************************************************************
% Example_setup_everything.m
% *************************************************************************
% written by Jeffryes Chapman
% NASA Glenn Research Center, Cleveland, OH
% Feb 14th, 2012
%
% This file calls the setup_everything file from the specified example then opens the template model.
% *************************************************************************

% Select which Example setup should be run
TMATS.POp = filesep;
TMATS.P = pwd;
InpMsg{1} = 'Select the enumeration of the T-MATS Example to be setup:';
InpMsg{2} = '1) NewtonRaphson_Equation_Solver';
InpMsg{3} = '2) Steady State GasTurbine';
InpMsg{4} = '3) Dynamic GasTurbine';
InpMsg{5} = '5) Steady State JT9D, Cantera';
InpMsg{6} = '6) Cancel Setup';
InpMsgFinal = '';

for i = 1: length(InpMsg)
    InpMsgFinal = strcat(InpMsgFinal,InpMsg{i},'\n');
end
TMATS.S = input(InpMsgFinal,'s');

switch TMATS.S
    case '1'
        disp(strcat('Loading Simulation from: ', TMATS.P ,TMATS.POp,'Example_NewtonRaphson_Equation_Solver'))
        cd Example_NewtonRaphson_Equation_Solver
        
        % open template
        open_system('NewtonRaphson_Equation_Solver.slx');
        
        % loading complete
        disp('** NewtonRaphson_Equation_Solver example ready to execute **')
    case '2'
        disp(strcat('Loading Simulation from: ', TMATS.P ,TMATS.POp,'Example_GasTurbine_SS'))
        cd Example_GasTurbine_SS
        %------ Performs example setup ---------
        GasTurbine_SS_setup_everything;
    case '3'
        disp(strcat('Loading Simulation from: ', TMATS.P ,TMATS.POp,'Example_GasTurbine_Dyn'))
        cd Example_GasTurbine_Dyn
        %------ Performs example setup ---------
        GasTurbine_Dyn_setup_everything;
        
    case '4'
        disp(strcat('Loading Simulation from: ', TMATS.P ,TMATS.POp,'Example_JT9D_SS_Cantera'))
        cd Example_JT9D_SS_Cantera
        %------ Performs example setup ---------
        JT9D_SS_Cantera_setup_everything;
        
    case '5'
        disp('Example Setup Canceled');
    otherwise,
        disp('Selection Invalid')
end
clear TMATS;

cd ..