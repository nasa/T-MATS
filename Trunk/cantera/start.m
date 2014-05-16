function start(block)

%%
setup(block);

%endfunction

function setup(block)

block.NumDialogPrms = 13;

% Register number of ports
block.NumInputPorts  = 0
block.NumOutputPorts = 1;

% Setup port properties to be inherited or dynamic
block.SetPreCompInpPortInfoToDynamic;
block.SetPreCompOutPortInfoToDynamic;



% Override output port properties
% outgoing flow
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
block.SampleTimes = [0 0];

% Specify the block simStateCompliance. The allowed values are:
%    'UnknownSimState', < The default setting; warn and assume DefaultSimState
%    'DefaultSimState', < Same sim state as a built-in block
%    'HasNoSimState',   < No sim state
%    'CustomSimState',  < Has GetSimState and SetSimState methods
%    'DisallowSimState' < Error out when saving or restoring the model sim state
block.SimStateCompliance = 'DefaultSimState';


block.RegBlockMethod('Outputs', @Outputs);     % Required




function Outputs(block)


% grab the recovery from theinput
W = block.DialogPrm(1).Data;
Tt = block.DialogPrm(2).Data;
Pt = block.DialogPrm(3).Data;

FO(1) = W;
FO(5) = block.DialogPrm(4).Data;
FO(6) = block.DialogPrm(5).Data;
FO(7) = block.DialogPrm(6).Data;
FO(8) = block.DialogPrm(7).Data;
FO(9) = block.DialogPrm(8).Data;
FO(10) = block.DialogPrm(9).Data;
FO(11) = block.DialogPrm(10).Data;
FO(12) = block.DialogPrm(11).Data;
FO(13) = block.DialogPrm(12).Data;
FO(14) = block.DialogPrm(13).Data;
% set the exit conditions to the incoming enthalpy and exit pressure
FO = set_TP( FO, Tt , Pt );

% set the values in the port
block.OutputPort(1).Data=FO;

%end Outputs
