%       T-MATS -- TMATSC_setTsPsMN.m
% *************************************************************************
% written by Jeffryes Chapman
% NASA Glenn Research Center, Cleveland, OH
% Jan 14, 2015
%
% Set all conditions based on flow, static temperature, static pressure, and Mach number
% *************************************************************************/
function TMATSC_setTsPsMN(block)

%%
setup(block);

%endfunction

function setup(block)

block.NumDialogPrms = 0;

% Register number of ports
block.NumInputPorts  = 4;
block.NumOutputPorts = 2;

% Setup port properties to be inherited or dynamic
block.SetPreCompInpPortInfoToDynamic;
block.SetPreCompOutPortInfoToDynamic;

% Override input port properties

%incoming flow 1
block.InputPort(1).Dimensions  = 25;
block.InputPort(1).DatatypeID  = 0;  % double
block.InputPort(1).Complexity  = 'Real';
block.InputPort(1).DirectFeedthrough = true;
block.InputPort(1).SamplingMode = 'Sample';

%incoming Ts
block.InputPort(2).Dimensions  = 1;
block.InputPort(2).DatatypeID  = 0;  % double
block.InputPort(2).Complexity  = 'Real';
block.InputPort(2).DirectFeedthrough = true;
block.InputPort(2).SamplingMode = 'Sample';

%incoming Ps
block.InputPort(3).Dimensions  = 1;
block.InputPort(3).DatatypeID  = 0;  % double
block.InputPort(3).Complexity  = 'Real';
block.InputPort(3).DirectFeedthrough = true;
block.InputPort(3).SamplingMode = 'Sample';

%incoming MN
block.InputPort(4).Dimensions  = 1;
block.InputPort(4).DatatypeID  = 0;  % double
block.InputPort(4).Complexity  = 'Real';
block.InputPort(4).DirectFeedthrough = true;
block.InputPort(4).SamplingMode = 'Sample';


% Override output port properties
% outgoing flow
block.OutputPort(1).Dimensions  = 25;
block.OutputPort(1).DatatypeID  = 0; % double
block.OutputPort(1).Complexity  = 'Real';
block.OutputPort(1).SamplingMode = 'Sample';

block.OutputPort(2).Dimensions  = 1;
block.OutputPort(2).DatatypeID  = 0; % double
block.OutputPort(2).Complexity  = 'Real';
block.OutputPort(2).SamplingMode = 'Sample';

% Register sample times
%  [0 offset]            : Continuous sample time
%  [positive_num offset] : Discrete sample time
%
%  [-1, 0]               : Inherited sample time
%  [-2, 0]               : Variable sample time
block.SampleTimes = [-1 0];

% Specify the block simStateCompliance. The allowed values are:
%    'UnknownSimState', < The default setting; warn and assume DefaultSimState
%    'DefaultSimState', < Same sim state as a built-in block
%    'HasNoSimState',   < No sim state
%    'CustomSimState',  < Has GetSimState and SetSimState methods
%    'DisallowSimState' < Error out when saving or restoring the model sim state
block.SimStateCompliance = 'DefaultSimState';


block.RegBlockMethod('Outputs', @Outputs);     % Required

function Outputs(block)
import TMATSC.*

% grab the data from the input port
FI = FlowDef(block.InputPort(1).Data);
TsIn = block.InputPort(2).Data;
PsIn = block.InputPort(3).Data;
MNIn = block.InputPort(4).Data;

% add secondary flow to primary flow
FO = FI.set_TsPsMN(TsIn, PsIn, MNIn);

FO_vec = FO.FlwVec();

% set the exit condition in the exit port
block.OutputPort(1).Data = FO_vec;
% set Data output for troubleshooting
block.OutputPort(2).Data = 0;

%end Outputs
