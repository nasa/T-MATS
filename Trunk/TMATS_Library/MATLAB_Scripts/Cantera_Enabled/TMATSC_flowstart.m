function TMATSC_flowstart(block)

%%
setup(block);

%endfunction

function setup(block)

% Register number of ports
block.NumInputPorts  = 1;
block.NumOutputPorts = 1;
block.NumDialogPrms = 0;

% Override input port properties
block.InputPort(1).Dimensions  = 9;
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
TtOut = block.InputPort(1).Data(2);
PtOut = block.InputPort(1).Data(3);
Wout = block.InputPort(1).Data(1);

% load in the composition values
compNN(1) = block.InputPort(1).Data(4);
compNN(2) = block.InputPort(1).Data(5);
compNN(3) = block.InputPort(1).Data(6);
compNN(4) = block.InputPort(1).Data(7);
compNN(5) = block.InputPort(1).Data(8);
compNN(6)  = block.InputPort(1).Data(9);

%Create flow object
FO = FlowDef(compNN, Wout, TtOut, PtOut);


FO_vec = FO.FlwVec();
block.OutputPort(1).Data = FO_vec;
%end Outputs
