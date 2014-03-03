function TMATSC_compressor(block)

%%
setup(block);

%endfunction

function setup(block)

block.NumDialogPrms = 16;

% Register number of ports
block.NumInputPorts  = 3;
block.NumOutputPorts = 6;

% Setup port properties to be inherited or dynamic
block.SetPreCompInpPortInfoToDynamic;
block.SetPreCompOutPortInfoToDynamic;

% Override input port properties

% incoming flow
block.InputPort(1).Dimensions  = 25;
block.InputPort(1).DatatypeID  = 0;  % double
block.InputPort(1).Complexity  = 'Real';
block.InputPort(1).DirectFeedthrough = true;
block.InputPort(1).SamplingMode = 'Sample';

% Override input port properties
block.InputPort(2).Dimensions  = 1;
block.InputPort(2).DatatypeID  = 0;  % double
block.InputPort(2).Complexity  = 'Real';
block.InputPort(2).DirectFeedthrough = true;
block.InputPort(2).SamplingMode = 'Sample';

% Override input port properties
block.InputPort(3).Dimensions  = 1;
block.InputPort(3).DatatypeID  = 0;  % double
block.InputPort(3).Complexity  = 'Real';
block.InputPort(3).DirectFeedthrough = true;
block.InputPort(3).SamplingMode = 'Sample';

% Override output port properties

% outgoing primary flow
block.OutputPort(1).Dimensions  = 25;
block.OutputPort(1).DatatypeID  = 0; % double
block.OutputPort(1).Complexity  = 'Real';
block.OutputPort(1).SamplingMode = 'Sample';

% outgoing bleed flow
block.OutputPort(2).Dimensions  = 25;
block.OutputPort(2).DatatypeID  = 0; % double
block.OutputPort(2).Complexity  = 'Real';
block.OutputPort(2).SamplingMode = 'Sample';

% outgoing bleed flow
block.OutputPort(3).Dimensions  = 25;
block.OutputPort(3).DatatypeID  = 0; % double
block.OutputPort(3).Complexity  = 'Real';
block.OutputPort(3).SamplingMode = 'Sample';

% outgoing bleed flow
block.OutputPort(4).Dimensions  = 1;
block.OutputPort(4).DatatypeID  = 0; % double
block.OutputPort(4).Complexity  = 'Real';
block.OutputPort(4).SamplingMode = 'Sample';

block.OutputPort(5).Dimensions  = 1;
block.OutputPort(5).DatatypeID  = 0; % double
block.OutputPort(5).Complexity  = 'Real';
block.OutputPort(5).SamplingMode = 'Sample';

block.OutputPort(6).Dimensions  = 4;
block.OutputPort(6).DatatypeID  = 0; % double
block.OutputPort(6).Complexity  = 'Real';
block.OutputPort(6).SamplingMode = 'Sample';
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

C_TSTD = 518.67;
C_PSTD = 14.696;

TMATSC_flowindicies;

%Grab the values from the input
Nmech = block.InputPort(2).Data;
Rline = block.InputPort(3).Data;
MapFile = block.DialogPrm(1).Data;
NcDes = block.DialogPrm(2).Data;
effDes = block.DialogPrm(3).Data;
PRdes = block.DialogPrm(4).Data;
RlineDes = block.DialogPrm(5).Data;
Wfrac1 = block.DialogPrm(6).Data;
hfrac1 = block.DialogPrm(7).Data;
Pfrac1 = block.DialogPrm(8).Data;
Wfrac2 = block.DialogPrm(9).Data;
hfrac2 = block.DialogPrm(10).Data;
Pfrac2 = block.DialogPrm(11).Data;
s_eff_in = block.DialogPrm(12).Data;
s_PR_in = block.DialogPrm(13).Data;
s_Wc_in = block.DialogPrm(14).Data;
s_C_Nc_in =  block.DialogPrm(15).Data;
IDes =  block.DialogPrm(16).Data;

% copy the incoming conditions to FI
FI = block.InputPort(1).Data;

% calcalate the corrected speed and corrected flow
NcIn = Nmech/ sqrt( FI(Tt)/ C_TSTD );
WcIn = FI(W)*sqrt(FI(Tt)/C_TSTD)/(FI(Pt)/C_PSTD);

% determine the name of this element
path = stripchar_TMATS( gcb() );

% if we are at design point calculated the design speed scalar
% and store the value in the workspace
if IDes < 0.5
    s_C_Nc = NcDes / NcIn;
    path = stripchar_TMATS( gcb() );
    TMATSC_setV( 's_C_Nc', path, s_C_Nc );
elseif IDes < 1.5
    % get the speed scalar from the workspace
    s_C_Nc = TMATSC_getV( 's_C_Nc', path );
else
    s_C_Nc = s_C_Nc_in;
end

% calculate NcMap from the incoming design speed and scalar
NcMap = NcIn * s_C_Nc;

% read the map based on Rline and NcMap
[effMap, PRmap, WcMap, stallMargin ] = TMATSC_RlineMap( MapFile, NcMap, Rline);

% if we are at design point calculate the scalars for eff, PW
% and wc
% if not, load these values from the workspace
if IDes < .5
    s_eff = effDes / effMap;
    s_PR = ( PRdes - 1 )/( PRmap - 1 );
    s_Wc = WcIn/ WcMap;
    TMATSC_setV( 's_eff', path, s_eff );
    TMATSC_setV( 's_Wc', path, s_Wc );
    TMATSC_setV( 's_PR', path, s_PR );
    
elseif IDes < 1.5
    % get the maps scalars from the workspace
    s_eff= TMATSC_getV( 's_eff', path );
    s_Wc= TMATSC_getV( 's_Wc', path );
    s_PR= TMATSC_getV( 's_PR', path );

else 
    % use the input values
    s_eff = s_eff_in;
    s_Wc = s_Wc_in;
    s_PR = s_PR_in;   
end
    
% calculate eff, Wc and PR
eff = s_eff*effMap;
Wc = s_Wc*WcMap;
PR = s_PR*( PRmap - 1 )+1;

% error calculation depends if we are in design mode
% in design drive Rline to RlineDes
% in off desing for the map corrected flow to match
% incoming corrected flow
if IDes < .5
    err = Rline-RlineDes;
else
    err = ( Wc - WcIn )/Wc; 
end
    
% calculate exit pressure
PtOut = FI(Pt)*PR;


% determine the ideal exit conditions from 
FOideal = TMATSC_set_SP( FI, FI(s),PtOut ); 

% determine the actual enthalpy
htOut = FI(ht) + ( FOideal(ht) - FI(ht) )/eff;
 
% set the exit conditions to knowm enthalpy and pressure
FO = TMATSC_set_hP( FI, htOut, PtOut );

% set the condtions for bleed 1
Wbleed1 = Wfrac1*FI(W);
htBleed1 = FI(ht)+1.*hfrac1*( FO(ht) - FI(ht) );
PtBleed1 = FI(Pt) + Pfrac1*( FO(Pt) - FI(Pt) );
Fbleed1 = TMATSC_set_hP( FI, htBleed1,PtBleed1 );
Fbleed1( W ) = Wbleed1;

% set the conditions for bleed 2
Wbleed2 = Wfrac2*FI(W);
htBleed2 = FI(ht)+1.*hfrac2*( FO(ht) - FI(ht) );
PtBleed2 = FI(Pt) + Pfrac2*( FO(Pt) - FI(Pt) );
Fbleed2 = TMATSC_set_hP( FI, htBleed2,PtBleed2 );
Fbleed2( W ) = Wbleed2;

% subtract the bleed flow from the exit flow
Wout = FI(W) - Fbleed1(W) - Fbleed2(W);
FO( W ) = Wout;

% determine the power
pwr = FI(W) * (  FI(ht) - FO(ht) ) * 1.4148;
pwr = pwr + Fbleed1(W)*(Fbleed1(ht)-FO(ht))*1.4148 + Fbleed2(W)*(Fbleed2(ht)-FO(ht))*1.4148;

% set the output conditions
block.OutputPort(1).Data = FO;
block.OutputPort(2).Data = Fbleed1;
block.OutputPort(3).Data = Fbleed2;

block.OutputPort(4).Data(1) = err;
block.OutputPort(5).Data(1) = pwr;
block.OutputPort(6).Data = [stallMargin,eff,Wc,PR];
%end Outputs
