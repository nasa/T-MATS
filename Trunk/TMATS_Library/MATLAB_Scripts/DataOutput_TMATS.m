function DataOutput_TMATS(block)

%%
setup(block);

%endfunction

function setup(block)

block.NumDialogPrms = 3;

% Register number of ports
block.NumInputPorts  = 1;
block.NumOutputPorts = 0;

% Setup port properties to be inherited or dynamic
block.SetPreCompInpPortInfoToDynamic;
block.SetPreCompOutPortInfoToDynamic;

% Override input port properties

% incoming flow
block.InputPort(1).Dimensions  = 1;
block.InputPort(1).DatatypeID  = 0;  % double
block.InputPort(1).Complexity  = 'Real';
block.InputPort(1).DirectFeedthrough = true;
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

function Outputs(block)

%Gather values from the inputs
DataVar = block.InputPort(1).Data;
File_Name = block.DialogPrm(1).Data;
VarName = block.DialogPrm(2).Data;
SVarName = block.DialogPrm(3).Data;

Processed_SurVariable_Name = stripchar_TMATS( SVarName );

eval(strcat(Processed_SurVariable_Name,VarName,'=',num2str(DataVar),';'));

if exist(File_Name, 'file');
    save(File_Name,strcat(Processed_SurVariable_Name,VarName),'-append');
else
    save(File_Name,strcat(Processed_SurVariable_Name,VarName));
end
    
    %end
    
