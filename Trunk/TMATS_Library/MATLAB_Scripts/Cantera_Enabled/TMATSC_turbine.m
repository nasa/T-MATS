function TMATSC_turbine(block)

%%
setup(block);

%endfunction

function setup(block)

block.NumDialogPrms = 12;

% Register number of ports
block.NumInputPorts  = 5;
block.NumOutputPorts = 4;

% Setup port properties to be inherited or dynamic
block.SetPreCompInpPortInfoToDynamic;
block.SetPreCompOutPortInfoToDynamic;

% Override input port properties
block.InputPort(1).Dimensions  = 25;
block.InputPort(1).DatatypeID  = 0;  % double
block.InputPort(1).Complexity  = 'Real';
block.InputPort(1).DirectFeedthrough = true;
block.InputPort(1).SamplingMode = 'Sample';

% Override input port properties
block.InputPort(2).Dimensions  = 25;
block.InputPort(2).DatatypeID  = 0;  % double
block.InputPort(2).Complexity  = 'Real';
block.InputPort(2).DirectFeedthrough = true;
block.InputPort(2).SamplingMode = 'Sample';

% Override input port properties
block.InputPort(3).Dimensions  = 25;
block.InputPort(3).DatatypeID  = 0;  % double
block.InputPort(3).Complexity  = 'Real';
block.InputPort(3).DirectFeedthrough = true;
block.InputPort(3).SamplingMode = 'Sample';


% Override output port properties
block.InputPort(4).Dimensions  = 1;
block.InputPort(4).DatatypeID  = 0; % double
block.InputPort(4).Complexity  = 'Real';
block.InputPort(4).SamplingMode = 'Sample';
  
% Override output port properties
block.InputPort(5).Dimensions  = 1;
block.InputPort(5).DatatypeID  = 0; % double
block.InputPort(5).Complexity  = 'Real';
block.InputPort(5).SamplingMode = 'Sample';


% Override output port properties
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


block.OutputPort(4).Dimensions  = 2;
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

block.RegBlockMethod('Outputs', @Outputs);     % Required

function Outputs(block)


C_TSTD = 518.67;
C_PSTD = 14.696;


TMATSC_flowindicies 

% load input data
MapFile = block.DialogPrm(1).Data;
NcDes = block.DialogPrm(2).Data;
effDes = block.DialogPrm(3).Data;
PRmapDes = block.DialogPrm(4).Data;
Ndes = block.DialogPrm(5).Data;
BSW1 = block.DialogPrm(6).Data;
BSW2 = block.DialogPrm(7).Data;
s_eff_in =  block.DialogPrm(8).Data;
s_Wc_in =  block.DialogPrm(9).Data;
s_C_Nc_in =  block.DialogPrm(10).Data;
s_PR_in =  block.DialogPrm(11).Data;
IDes =  block.DialogPrm(12).Data;

% load the input data
Nmech = block.InputPort(4).Data;
PR = block.InputPort(5).Data;

% load the input flow
FI = block.InputPort(1).Data;

% load in the bleed flows
b1 = block.InputPort(2).Data;
b2 = block.InputPort(3).Data;
F41 = FI;

% determine the F41 flowstation by adding the bleed flows 
% in that come in the front of the turbine

if BSW1 > .5 
  F41 = TMATSC_flowadd( F41, b1 );
end

if BSW2 > .5
  F41 = TMATSC_flowadd( F41, b2 );
end

% determine the incoming entropy
sin = F41( s );

% determine the correct conditons
NcIn = Nmech/ sqrt( F41(Tt) );
WcIn = F41(W)*sqrt( F41(Tt) )/( F41( Pt) );

% create workspace values for design value
path = TMATSC_stripchar( gcb() );
tempNc = sprintf( '%s_sNc', path);  
tempeff = sprintf( '%s_seff', path);  
tempPR = sprintf( '%s_sPR', path);  
tempWc = sprintf( '%s_sWc', path);  

% if design point then calculate and store the values
if IDes < 0.5
    path = TMATSC_stripchar( gcb() );
    s_C_Nc = NcDes / NcIn;
    TMATSC_setV( 's_C_Nc', path, s_C_Nc );
    s_PR = ( PRmapDes - 1 )/( PR - 1 );
    TMATSC_setV( 's_PR', path, s_PR );    
elseif IDes < 1.5 
    s_C_Nc = TMATSC_getV( 's_C_Nc', path );
    s_PR = TMATSC_getV( 's_PR', path );
else
    s_C_Nc = s_C_Nc_in;
    s_PR = s_PR_in;
end


% calculate map values
NcMap = NcIn * s_C_Nc;
PRmap = s_PR*( PR - 1 ) + 1;

% read the map values
[effMap, WcMap ] = TMATSC_PRmapFile( MapFile, NcMap, PRmap );

% if off design then calculate design values
if IDes < .5
    s_eff = effDes / effMap; 
    s_PR = ( PRmapDes - 1 )/( PRmap - 1 );
    s_Wc = WcIn/ WcMap;
    TMATSC_setV( 's_eff', path, s_eff );
    TMATSC_setV( 's_Wc', path, s_Wc );
elseif IDes < 1.5
    % get the design values from the worksapce
    s_eff= TMATSC_getV( 's_eff', path );
    s_Wc= TMATSC_getV( 's_Wc', path );
else 
    s_eff = s_eff_in;
    s_Wc = s_Wc_in;
end

    
% scale the map values
eff = s_eff*effMap;
Wc = s_Wc*WcMap;

% calculate the error 
% if design then force the speed to match design speed
% if off design force flow to matc
if IDes < .5
    err = Ndes - Nmech;
else
    err = ( Wc - WcIn )/Wc; 
end
    
PtOut = F41(Pt)/PR;

% determine the ideal exit conditions
FoutIdeal = TMATSC_set_SP( F41, sin, PtOut );

% determine the actual exit conditions by adjusting the enthalpy
% for the efficiency
htOut = F41( ht ) + ( FoutIdeal( ht ) - F41(ht) )*eff;
F48 = TMATSC_set_hP( F41, htOut, PtOut );

% calculate the power
pwr = F41(W) * (  F41(ht) - F48(ht) ) * 1.4148;

% add in the bleeds that are after the turbine
F5 = F48;
if BSW1 < .5 
  F5= TMATSC_flowadd( F5, b1);
end

if BSW2 < .5
  F5 = TMATSC_flowadd( F5, b2);
end

% set the output values
block.OutputPort(1).Data= F5;
block.OutputPort(2).Data(1) = err;
block.OutputPort(3).Data(1) = pwr;
block.OutputPort(4).Data = [eff, WcMap];
%end Outputs
