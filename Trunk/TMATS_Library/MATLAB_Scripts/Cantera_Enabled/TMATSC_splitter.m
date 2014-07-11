function TMATSC_splitter(block)

%%
setup(block);

%endfunction

function setup(block)


% Register number of ports
block.NumDialogPrms = 0;
block.NumInputPorts  = 2;
block.NumOutputPorts = 3;

% Setup port properties to be inherited or dynamic
block.SetPreCompInpPortInfoToDynamic;
block.SetPreCompOutPortInfoToDynamic;

% Override input port properties

% primary flow
block.InputPort(1).Dimensions  = 25;
block.InputPort(1).DatatypeID  = 0;  % double
block.InputPort(1).Complexity  = 'Real';
block.InputPort(1).SamplingMode = 'Sample';

block.InputPort(2).Dimensions  = 1;
block.InputPort(2).DatatypeID  = 0;  % double
block.InputPort(2).Complexity  = 'Real';
block.InputPort(2).SamplingMode = 'Sample';



% Override output port properties

% primary outlet flow
block.OutputPort(1).Dimensions  = 25;
block.OutputPort(1).DatatypeID  = 0; % double
block.OutputPort(1).Complexity  = 'Real';
block.OutputPort(1).SamplingMode = 'Sample';

% secondary outlet flow
block.OutputPort(2).Dimensions  = 25;
block.OutputPort(2).DatatypeID  = 0; % double
block.OutputPort(2).Complexity  = 'Real';
block.OutputPort(2).SamplingMode = 'Sample';

block.OutputPort(3).Dimensions  = 1;
block.OutputPort(3).DatatypeID  = 0; % double
block.OutputPort(3).Complexity  = 'Real';
block.OutputPort(3).SamplingMode = 'Sample';


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

global fs;

% load the input data in
BPR = block.InputPort(2).Data;

TMATSC_flowindicies

% grab the input flow
FI = block.InputPort(1).Data;

% set the output flows
FS = TMATSC_flowcopy( FI );
FP = TMATSC_flowcopy( FI );
FS( W ) = FI( W )*BPR/ (1. + BPR );
FP( W ) = FI( W )*1/( 1. + BPR );

% load the output conditions
block.OutputPort(1).Data = FS;
block.OutputPort(2).Data = FP;
block.OutputPort(3).Data = [0];

%end Outputs
