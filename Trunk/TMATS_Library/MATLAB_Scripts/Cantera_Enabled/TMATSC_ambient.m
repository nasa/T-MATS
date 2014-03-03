%       T-MATS -- TMATSC_ambient.m
% *************************************************************************
% written by Thomas Lavelle and Jeffryes Chapman
% NASA Glenn Research Center, Cleveland, OH
% Feb 26, 2014
%
% set flow conditions based on Alt, dT, and MN
% *************************************************************************/


function TMATSC_ambient(block)

%%
setup(block);

%endfunction

function setup(block)

% Register number of ports
block.NumInputPorts  = 1;
block.NumOutputPorts = 4;
block.NumDialogPrms = 3;

% Override input port properties
block.InputPort(1).Dimensions  = 1;
block.InputPort(1).DatatypeID  = 0;  % double
block.InputPort(1).Complexity  = 'Real';
block.InputPort(1).DirectFeedthrough = true;
block.InputPort(1).SamplingMode = 'Sample';

% Override output port properties

% outgoing primary flow
block.OutputPort(1).Dimensions  = 25;
block.OutputPort(1).DatatypeID  = 0; % double
block.OutputPort(1).Complexity  = 'Real';
block.OutputPort(1).SamplingMode = 'Sample';

block.OutputPort(2).Dimensions  = 1;
block.OutputPort(2).DatatypeID  = 0; % double
block.OutputPort(2).Complexity  = 'Real';
block.OutputPort(2).SamplingMode = 'Sample';

block.OutputPort(3).Dimensions  = 1;
block.OutputPort(3).DatatypeID  = 0; % double
block.OutputPort(3).Complexity  = 'Real';
block.OutputPort(3).SamplingMode = 'Sample';

block.OutputPort(4).Dimensions  = 1;
block.OutputPort(4).DatatypeID  = 0; % double
block.OutputPort(4).Complexity  = 'Real';
block.OutputPort(4).SamplingMode = 'Sample';

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


%block.RegBlockMethod('InitializeConditions', @InitializeConditions);
block.RegBlockMethod('Outputs', @Outputs);     % Required



function Outputs(block)

global fs;



% grab data from input
Wout = block.InputPort(1).Data;
alt = block.DialogPrm(1).Data;
MN = block.DialogPrm(2).Data;
dTs = block.DialogPrm(3).Data;

compNN = [1,0,0,0,0,0];

TMATSC_flowindicies



REARTH = 6369.0;
GMR = 34.163195;
htab = [ 0.0,  11.0, 20.0, 32.0, 47.0, 51.0, 71.0, 84.852 ];
ttab = [ 288.15, 216.65, 216.65, 228.65, 270.65, 270.65, 214.65, 186.946 ];
ptab = [ 1.0, 2.2336110E-1, 5.4032950E-2, 8.5666784E-3, 1.0945601E-3, 6.6063531E-4, 3.9046834E-5, 3.68501E-6 ];
gtab = [ -6.5, 0.0, 1.0, 2.8, 0, -2.8, -2.0, 0.0 ];

Ts=0;
Ps=0;

h = alt/3280.84*REARTH/(alt/3280.84+REARTH);	

i = 1;
while h > htab(i+1)
   i = i + 1;
end

tbase = ttab(i)*9/5;
tgrad = gtab(i);    
deltah=h-htab(i);
Ts=tbase+tgrad*deltah*9/5;
if (tgrad == 0. )
   Ps = ptab(i)*exp(-GMR*deltah/tbase*9./5.)*14.696;
else
   Ps =ptab(i)*((tbase/Ts)^(GMR/tgrad))*14.696;
end
    
% adjust the temperature for the input delta
Ts = dTs + Ts;


if ( MN > 0 )
   FO = TMATSC_set_TsPsMN( Ts, Ps, MN, compNN );
else
   FO(5) = 1; 
   FO( 25 ) = 0; 
   FO = set_TP( FO, Ts, Ps );
end
FO( W ) = Wout;
Fram = FO(Vflow)*FO(W)/32.174;

FO( W ) = Wout;


block.OutputPort(1).Data = FO;
block.OutputPort(2).Data = Ps;
block.OutputPort(3).Data = Fram;
block.OutputPort(4).Data = [0];



%end Outputs
