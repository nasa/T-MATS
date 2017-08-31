function MWS = GasTurbine_SS_setup_everything(varargin)
%		T-MATS -- GasTurbine_SS_setup_MWS.m
% *************************************************************************
% written by Jeffryes Chapman based on work by Ryan May,Jeff Csank, and Dean K. Fredrick
% NASA Glenn Research Center, Cleveland, OH
% Feb 14th, 2012
%
% This file is run as part of setup_everything. 
% It adds all necessary paths and then calls the required variable setup scripts.
% *************************************************************************

 if(nargin == 0)  % if engName argument is not passed into setup_MWS
    MWS.engName = 'engine1';
    inputs = [];
 elseif(nargin == 1)
    inputs = varargin{1};
    MWS.engName = 'engine1';
 elseif(nargin == 2)
    inputs = varargin{1};
    MWS.engName = varargin{2};
 end
%--------- get name of starting directory ----
MWS.top_level = pwd;
POp = filesep;

%--------- add necessary subdirectories to current MATLAB path
eval(['addpath ',MWS.top_level,POp,'SimSetup'])

%------ run various setup files to get tables & constants ---------
MWS = setup_Solve_temp(MWS);	% develops variables for the iterative solver
MWS = setup_Inputs(MWS);    	% sets up input vectors
MWS = setup_HPC(MWS);           % develops variables for HPC
MWS = setup_Shaft(MWS);         % develops variables for Shaft
MWS = setup_Noz(MWS);           % develops variables for Noz
MWS = setup_HPT(MWS);           % develops variables for HPT
MWS = setup_Burner(MWS);        % develops variables for Burner
MWS = setup_Duct(MWS);          % develops variables for Duct
MWS = setup_Inlet(MWS);         % develops variables for Inlet

% if no output is specified, then just assign MWS to the workspace
if(nargout == 0)
    assignin('base','MWS',MWS);
end

% open template
open_system('GasTurbine_SS_Template.slx');

% loading complete
disp('** GasTurbine_SS example ready to execute **')
disp('** See NASA/TM-2014-218410 for more information on model development **')




