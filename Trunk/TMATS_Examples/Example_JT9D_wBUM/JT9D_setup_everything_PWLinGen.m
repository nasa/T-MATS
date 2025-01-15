function MWS = JT9D_setup_everything_PWLinGen(varargin)
%		T-MATS -- JT9D_setup_everything.m
% *************************************************************************
% written by Jeffryes Chapman based on work by Ryan May,Jeff Csank, and Dean K. Fredrick
% NASA Glenn Research Center, Cleveland, OH
% Feb 14th, 2014
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
cd Simsetup

%------ run various setup files to get tables & constants ---------
MWS = setup_Fan(MWS);               % develops variables for FAN
MWS = setup_LPC(MWS);               % sets up input vectors
MWS = setup_HPC(MWS);               % develops variables for HPC
MWS = setup_HPT(MWS);               % develops variables for HPT
MWS = setup_LPT(MWS);               % develops variables for LPT
MWS = setup_Noz(MWS);               % develops variables for core Noz
MWS = setup_Noz_byp(MWS);           % develops variables for bypass Noz
MWS = setup_Burner(MWS);            % develops variables for Burner
MWS = setup_Duct(MWS);              % develops variables for Duct
MWS = setup_Inlet(MWS);             % develops variables for Inlet
MWS = setup_Inputs(MWS);

cd ..
% if no output is specified, then just assign MWS to the workspace
if(nargout == 0)
    assignin('base','MWS',MWS);
end

% open template
open_system('JT9D_Model_PWLin.mdl');

% loading complete
disp('** JT9D_Lin example ready to execute **')
disp('** This JT9D-like model was created using publically available data only **')
disp('** The model is meant for educational purposes only, as such the developers make no specific claims of model accuracy. **')
disp('** See NASA/TM-2014-218409 for more information on model development **')



