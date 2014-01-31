function TMATS_Param_Step(block)

%%
setup(block);


function setup(block)

block.RegBlockMethod('SetInputPortDimensionsMode',  @SetInputDimsMode);

% Register number of ports
block.NumDialogPrms = 5;
block.NumInputPorts  = 1;
block.NumOutputPorts = 0;

% Setup port properties to be inherited or dynamic
block.SetPreCompInpPortInfoToDynamic;

% Override input port properties

% primary flow
block.InputPort(1).Dimensions  = 1;
block.InputPort(1).DatatypeID  = 0;  % double
block.InputPort(1).Complexity  = 'Real';
block.InputPort(1).SamplingMode = 'Sample';


% Register sample times
%  [0 offset]            : Continuous sample time
%  [positive_num offset] : Discrete sample time
%
%  [-1, 0]               : Inherited sample time
%  [-2, 0]               : Variable sample time
block.SampleTimes = [0 0];

% Specify the block simStateCompliance. The allowed values are:
%    'UnknownSimState', < The default setting; warn and assume DefaultSimState
%    'DefaultSimState', < Same sim state as a built-in block
%    'HasNoSimState',   < No sim state
%    'CustomSimState',  < Has GetSimState and SetSimState methods
%    'DisallowSimState' < Error out when saving or restoring the model sim state
block.SimStateCompliance = 'DefaultSimState';
block.RegBlockMethod('Outputs', @Outputs);     % Required

function SetInputDimsMode(block, port, dm)
% Set dimension mode
block.InputPort(port).DimensionsMode = dm;
block.OutputPort(port).DimensionsMode = dm;

function Outputs(block)
Time = block.InputPort(1).Data;
BlockName = block.DialogPrm(1).Data;
ParameterName = block.DialogPrm(2).Data;
InitialValue = block.DialogPrm(3).Data;
FinalValue = block.DialogPrm(4).Data;
StepTime = block.DialogPrm(5).Data;

if Time < StepTime
%     set_param(BlockName, ParameterName, InitialValue);
else
    set_param(BlockName, ParameterName, FinalValue);
end
