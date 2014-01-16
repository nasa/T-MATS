function compressor(block)

%%
setup(block);

%endfunction

function setup(block)

block.SimStateCompliance = 'DefaultSimState';
block.NumInputPorts  = 1;
block.NumOutputPorts = 1;


block.RegBlockMethod('Outputs', @Outputs);     % Required
block.InputPort(1).Dimensions  = 25;
block.InputPort(1).DatatypeID  = 0;  % double
block.InputPort(1).Complexity  = 'Real';
block.InputPort(1).DirectFeedthrough = true;
block.InputPort(1).SamplingMode = 'Sample';
block.OutputPort(1).Dimensions  = 1;
block.OutputPort(1).DatatypeID  = 0; % double
block.OutputPort(1).Complexity  = 'Real';
block.OutputPort(1).SamplingMode = 'Sample';


function Outputs(block)

global comp1;
global comp2;
global comp3;
global IDes;
global fs;
global Species;
global Name;

W = 1;
Tt = 2;
Pt = 3;
ht = 4;
entropy = 11;
rhot = 12;
Ts = 13;
Ps = 14;
hs = 15;
rhos = 16;
Vflow = 17;
MNin = 18;
area = 19;
gamt = 20;
gams = 21;

C_TSTD = 518.67;
C_PSTD = 14.696;

FO(5) = 1;
FO(6) = 0;
FO(7) = 0;
FO(8) = 0;
FO(9) = 0;
FO(10)  = 0;
FO(25) = 0;

T = 518;
P = 15;

% Test set_TP
FO = set_TP( FO, T, P );
fprintf( 'set TP\n' );
fprintf( '%g\n', FO(ht) );
fprintf( '%g\n', FO(Tt) );
fprintf( '%g\n', FO(Pt) );
fprintf( '%g\n', FO(rhot) );
fprintf( '%g\n', FO(gamt) );
fprintf( '%g\n', FO(entropy) );

h = FO(ht);
s = FO(entropy);

% Test set_hP
FO = set_TP( FO, 1000, 40 );
FO = set_hP( FO, h, P );
fprintf( 'set hP\n' );
fprintf( '%g\n', FO(ht) );
fprintf( '%g\n', FO(Tt) );
fprintf( '%g\n', FO(Pt) );
fprintf( '%g\n', FO(rhot) );
fprintf( '%g\n', FO(gamt) );
fprintf( '%g\n', FO(entropy) );

% Test set_SP
FO = set_TP( FO, 1000, 40 );
FO = set_SP( FO, s, P );
fprintf( 'set sP\n' );
fprintf( '%g\n', FO(ht) );
fprintf( '%g\n', FO(Tt) );
fprintf( '%g\n', FO(Pt) );
fprintf( '%g\n', FO(rhot) );
fprintf( '%g\n', FO(gamt) );
fprintf( '%g\n', FO(entropy) );

% test delh
FO = set_TP( FO, 1000, 40 );
fprintf( 'test delh\n' );
fprintf( '%g\n', FO(ht) - h );

FO = set_TP( FO, 1000, 40 );
fprintf( 'test dels\n' );
fprintf( '%g\n', FO(entropy) - s  );


%end Outputs
