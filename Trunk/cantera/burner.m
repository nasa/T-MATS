function inlet(block)

%%
setup(block);

%endfunction

function setup(block)

block.NumDialogPrms = 3;

% Register number of ports
block.NumInputPorts  = 1;
block.NumOutputPorts = 3;

% Setup port properties to be inherited or dynamic
block.SetPreCompInpPortInfoToDynamic;
block.SetPreCompOutPortInfoToDynamic;

% Override input port properties

%incoming flow
block.InputPort(1).Dimensions  = 25;
block.InputPort(1).DatatypeID  = 0;  % double
block.InputPort(1).Complexity  = 'Real';
block.InputPort(1).DirectFeedthrough = true;
block.InputPort(1).SamplingMode = 'Sample';

%outgoing flow
block.OutputPort(1).Dimensions  = 25;
block.OutputPort(1).DatatypeID  = 0; % double
block.OutputPort(1).Complexity  = 'Real';
block.OutputPort(1).SamplingMode = 'Sample';

%Fuel flow
block.OutputPort(2).Dimensions  = 1;
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
block.SampleTimes = [0 0];

block.SimStateCompliance = 'DefaultSimState';
block.RegBlockMethod('Outputs', @Outputs);     % Required
function Outputs(block)

flowindicies;

% load the incoming flow data into FI
FI =  block.InputPort(1).Data;

% copy FI into FO to use as a starting point for the combustion
FO = copyFlow( block.InputPort(1).Data );

% grab the input values 
FAR = block.DialogPrm(1).Data;
hFuel = block.DialogPrm(2).Data;
dP= block.DialogPrm(3).Data;

%apply the pressure drop
FO(Pt) = FI(Pt) *(1-dP);

% create an array to describe the fuel
fuel(W) = FI(W)*FAR;
%set the fuel composition to 100%
fuel(7)= 1.;
fuel(ht) = hFuel;
fuel(25) = 0;

%add the fuel to the flow
FO = add( FO, fuel );


% load FO into the output port
block.OutputPort(1).Data = FO;

% set the fuel flow in the fuel output port
block.OutputPort(2).Data = fuel(1);
block.OutputPort(3).Data = [0];


%end Outputs
