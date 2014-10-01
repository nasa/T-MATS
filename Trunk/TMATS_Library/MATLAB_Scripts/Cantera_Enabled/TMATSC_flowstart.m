function TMATSC_flowstart(block)

%%
setup(block);

%endfunction

function setup(block)

% Register number of ports
block.NumInputPorts  = 1;
block.NumOutputPorts = 1;
block.NumDialogPrms = 8;

% Override input port properties
block.InputPort(1).Dimensions  = 1;
block.InputPort(1).DatatypeID  = 0;  % double
block.InputPort(1).Complexity  = 'Real';
block.InputPort(1).DirectFeedthrough = true;
block.InputPort(1).SamplingMode = 'Sample';


% Override output port properties
block.OutputPort(1).Dimensions  = 25;
block.OutputPort(1).DatatypeID  = 0; % double
block.OutputPort(1).Complexity  = 'Real';
block.OutputPort(1).SamplingMode = 'Sample';


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

% load the input values
TtOut = block.DialogPrm(1).Data;
PtOut = block.DialogPrm(2).Data;
Wout = block.InputPort(1).Data;

% load in the composition values
compNN(1) = block.DialogPrm(3).Data;
compNN(2) = block.DialogPrm(4).Data;
compNN(3) = block.DialogPrm(5).Data;
compNN(4) = block.DialogPrm(6).Data;
compNN(5) = block.DialogPrm(7).Data;
compNN(6)  = block.DialogPrm(8).Data;

%Create flow object
FO = FlowDef(compNN, Wout, TtOut, PtOut);


FO_vec = FO.FlwVec();
block.OutputPort(1).Data = FO_vec;
%end Outputs
