function MWS = JT9D_SS_Cantera_setup_everything(varargin)
%		T-MATS -- JT9D_SS_Cantera_setup_MWS.m
% *************************************************************************
% written by Jeffryes Chapman
% NASA Glenn Research Center, Cleveland, OH
% Feb 14th, 2012
%
% This file is run as part of setup_everything.
% It adds all necessary paths and then calls the required variable setup scripts.
% *************************************************************************

%Create Cantera flow structure
global fs;
fs = importPhase('gri30.xml');
assignin('base','fs',fs);


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
eval(['addpath ',MWS.top_level,POp,'Maps'])

% if no output is specified, then just assign MWS to the workspace
if(nargout == 0)
    assignin('base','MWS',MWS);
end

% open template
open_system('JT9D_SS_Cantera_Template.slx');

% loading complete
disp('** GasTurbine_SS example ready to execute **')



