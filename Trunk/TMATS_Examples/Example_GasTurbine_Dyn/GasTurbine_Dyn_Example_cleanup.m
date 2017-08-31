function GasTurbine_Dyn_Example_cleanup(MWS)
%		T-MATS -- GasTurbine_Example_cleanup.m
% *************************************************************************
% written by Jeffryes Chapman based on work by Ryan May
% NASA Glenn Research Center, Cleveland, OH
% Feb 14th, 2012
%
% This file is optionally called after T-MATS has been run.  It serves to
% cleanup the list of user paths so that there are no remenants of the
% simulation left in MATLAB.  The simulation is closed without saving
% changes.
% *************************************************************************

%=============================================
% Remove the various directories from the path
%=============================================
try
    if(~exist('MWS','var'))
        top_level = evalin('base','MWS.top_level');
    else
        top_level = MWS.top_level;
    end
    
    pathOperator = filesep;
    
    % remove all of the directories that were added to the path
    Pth1 = strcat(top_level,pathOperator,'SimSetup');

    perm1 = ~isempty(strfind(path,Pth1));  % determine if path is already defined
    if perm1
    eval(['rmpath ',Pth1])
    end
    % end
catch
    disp('Unable to remove T-MATS Dynamic Gas Turbine example directories from the path.')
end
