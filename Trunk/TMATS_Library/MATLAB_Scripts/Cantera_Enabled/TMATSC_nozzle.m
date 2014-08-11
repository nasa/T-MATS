function TMATSC_nozzle(block)

%%
setup(block);

%endfunction

function setup(block)

block.NumDialogPrms = 4;

% Register number of ports
block.NumInputPorts  = 2;
block.NumOutputPorts = 3;

% Setup port properties to be inherited or dynamic
block.SetPreCompInpPortInfoToDynamic;
block.SetPreCompOutPortInfoToDynamic;

% Override input port properties

%  Input flow conditions
block.InputPort(1).Dimensions  = 25;
block.InputPort(1).DatatypeID  = 0;  % double
block.InputPort(1).Complexity  = 'Real';
block.InputPort(1).DirectFeedthrough = true;
block.InputPort(1).SamplingMode = 'Sample';

block.InputPort(2).Dimensions  = 1;
block.InputPort(2).DatatypeID  = 0;  % double
block.InputPort(2).Complexity  = 'Real';
block.InputPort(2).DirectFeedthrough = true;
block.InputPort(2).SamplingMode = 'Sample';

% Override output port properties
block.OutputPort(1).Dimensions  = 1;
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

% load in static ppressure from static pressure port
PsExh = block.InputPort(2).Data;

% load in input values
Wdes = block.DialogPrm(1).Data;
Cfg = block.DialogPrm(2).Data;
Ath_in = block.DialogPrm(3).Data;
IDes = block.DialogPrm(4).Data;

% grab the input flow conditions
FI = block.InputPort(1).Data;

% grab the name of the element for use in storing values
path = TMATSC_stripchar( gcb() );

% set the throat conditions based on MN
FTh = TMATSC_set_MNPs( FI, PsExh );

if FTh(MNin) > 1.0
   % static pressur results in MN > 1  Nozzle is choked
   % set MN to 1
   FTh = TMATSC_set_MN1( FTh );
   if IDes < .5
      % design point
      % determine the area to pass the flow
      Ath = FTh(W) / ( FTh(rhos) * FTh(Vflow) )*144.;
      % store area for use in off-design
      TMATSC_setV( 'Ath', path, Ath );
      FTh(A) = Ath;
      % error is actual flow versus design flow
      err = ( FI(W) - Wdes );
   elseif IDes < 1.5
      % off-design
      % load the area in from workspace
      Ath = TMATSC_getV( 'Ath', path );
      FTh(A)=Ath;
      % determine the weight flow this area
      Wnoz = FTh(A) * FTh(rhos)* FTh(Vflow)/144.;
      % error is difference between this weight flow and incoming flow
      err = FI(W) - Wnoz;
   else 
      % off-design
      % use the input throat area
      Ath = Ath_in;
      FTh(A)=Ath;
      % determine the weight flow this area
      Wnoz = FTh(A) * FTh(rhos)* FTh(Vflow)/144.;
      % error is difference between this weight flow and incoming flow
      err = FI(W) - Wnoz;
   end
       
else
   % not choked
   if IDes < .5
      % calculate area based on flow conditions and weight flow
      Ath = FTh(W) / ( FTh(rhos) * FTh(Vflow) )*144.;
      % store area for use in off-design      
      TMATSC_setV( 'Ath', path, Ath );
      FTh(A)=Ath;
      % error is actual flow versus design flow      
      err = ( FTh(W) - Wdes );
   elseif IDes < 1.5
      % off-design
      % load the area in from workspace     
      Ath = TMATSC_getV( 'Ath', path );
      FTh(A)=Ath;
      % determine the weight flow this area     
      Wnoz = FTh(A) * FTh(rhos)* FTh(Vflow)/144.;
      % error is difference between this weight flow and incoming flow
      err = FI(W) - Wnoz;  
   else
      % off-design
      % load the area in from workspace     
      Ath = Ath_in;
      FTh(A)=Ath;
      % determine the weight flow this area     
      Wnoz = FTh(A) * FTh(rhos)* FTh(Vflow)/144.;
      % error is difference between this weight flow and incoming flow
      err = FI(W) - Wnoz;  
   end
end
            
Fg   = (( FTh(W) / 32.174 ) * FTh(Vflow) +  ( FTh(Ps) - PsExh ) * FTh(A))*Cfg;


block.OutputPort(1).Data = err;
block.OutputPort(2).Data = Fg;
block.OutputPort(3).Data = [Ath];
%end Outputs
