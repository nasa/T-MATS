function SortedMK = Link_Setup(varargin)
% Autocreate Linker Tag for a simulink block
% TMATS.Link_Setup() - Create Default Tags
% TMATS.Link_Setup(_,Name,Value,...,Value) - Create Tags with alternate formating
% Name              Definition
% 'BlockName'     -   Specify Block Name, (_,'BlockName','Str')
% 'DisableMakeLinkerTags'   -   Disable Make Linker Tags , (_,'DisableMakeLinkerTags')
% 'BldIn'         -   Set Number of input Bleeds to block, if applicable, (_,'BldIn',Num)
% 'FbldOut'       -   Set Number of Fractional Bleeds from block, if applicable, (_,'FbldOut',Num)
% 'ConBldNum'    -   Set Number of Constant Bleeds from block if applicable, (_,'CustBldNum',Num)
% 'MKSortTypeEn'  -   Enable Make Linker Tag Sorting , (_,'MKSortTypeEn')
% 'MKSortTypeDis' -   Disable Make Linker Tag Sorting (this is default) , (_,'MKSortTypeDis')
% 'SystemName'    -   Set Name of model file or system file, (_,'SystemName','Str')


%		T-MATS -- Link_Setup.m
% *************************************************************************
% written by Jeffryes Chapman
% NASA Glenn Research Center, Cleveland, OH
% April 19th, 2013
%
% Wire_Setup.m sets up blocks in TMATS to be quickly connected by adding
% goto tags, From blocks, and Terminators. Wire_setup will not change
% any interal block perameters and any block created by this script can be
% deleted as needed. This script also adds Link blocks that are not
% connected to anything.  These Link blocks are for convinience. This
% portion of the code can be enabled or disabled with the bit
% MakeLinkerTags [ true - enable,  false - Disable].
% *************************************************************************

SystemName = [];
ShortBlockName = [];

%Default settings
MakeLinkerTags = true;
BldInNum = 0;
FbldOutNum = 0;
CbldNum = 0;
MKSortTypeEn = false;
Output.Input = [];
Output.Output = [];
Output.Ind = [];
Output.Dep = [];
Output.Shaft = [];
Output.ShaftN = [];
Output.BleedOut = [];
Output.BleedIn = [];
SortedMK = [];

% Alternate configuration settings (only usable when calling function)
if ~isempty(varargin)
    for i = 1:nargin
        switch varargin{i}
            case 'BlockName'
                if nargin > i
                    ShortBlockName = varargin{i+1};
                end
            case 'DisableMakeLinkerTags'
                MakeLinkerTags = false;
            case 'BldIn'
                if nargin > i
                    BldInNum = varargin{i+1};
                end
            case 'FbldOut'
                if nargin > i
                    FbldOutNum = varargin{i+1};
                end
            case 'ConBldNum'
                if nargin > i
                    CbldNum = varargin{i+1};
                end
            case 'MKSortTypeEn'
                MKSortTypeEn = true;
            case 'MKSortTypeDis'
                MKSortTypeEn = false;
            case 'SystemName'
                if nargin > i
                    SystemName = varargin{i+1};
                end
            otherwise
        end
    end
end

if isempty(SystemName)
    SystemName = gcs;
end
if isempty(ShortBlockName)
    BlockName = gcb;
else
    BlockName = [SystemName '/' ShortBlockName];
end



% get_param(BlockName,'ObjectParameters')

temp.a = get_param(BlockName,'Name');
temp.block = get_param(BlockName,'ReferenceBlock');
temp.pos = get_param(BlockName,'Position');
switch temp.block
    case('Lib_Turbo_Ambient_TMATS/Ambient')
        AddInPortFromTag(SystemName, temp.a, temp.pos, 1/8, 1, strcat(temp.a,'_W'),'darkGreen');   % W Input From tag
        AddInPortFromTag(SystemName, temp.a, temp.pos, 3/8, 2, strcat(temp.a,'_Alt'),'black');     % Alt Input From tag
        AddInPortFromTag(SystemName, temp.a, temp.pos, 5/8, 3, strcat(temp.a,'_dT'),'black');      % dT Input From tag
        AddInPortFromTag(SystemName, temp.a, temp.pos, 7/8, 4, strcat(temp.a,'_MN'),'black');      % MN Input From tag
        
        AddOutportGoTo(SystemName, temp.a, temp.pos, 3/6, 2, 'Pamb','black');   %  Ambient pressure Output GoTo tag
        AddOutportTerminator(SystemName, temp.a, temp.pos, 5/6, 3);             %  A_Data Output terminator block
        
        if MakeLinkerTags
            Output.Ind =   [Output.Ind {AddMakeLinkerInportGoTo(SystemName, temp.a, temp.pos, 1/8,1,strcat(temp.a,'_W'),'darkGreen')}];
            Output.Input = [Output.Input {AddMakeLinkerInportGoTo(SystemName, temp.a, temp.pos, 3/8,2,strcat(temp.a,'_Alt'),'black')}];
            Output.Input = [Output.Input {AddMakeLinkerInportGoTo(SystemName, temp.a, temp.pos, 5/8,3,strcat(temp.a,'_dT'),'black')}];
            Output.Input = [Output.Input {AddMakeLinkerInportGoTo(SystemName, temp.a, temp.pos, 7/8,4,strcat(temp.a,'_MN'),'black')}];
        end
    case('Lib_Turbo_Bleed_TMATS/Bleed')
        if  BldInNum > 0
            AddInPortFromTag(SystemName, temp.a, temp.pos, 1/4, 1, strcat(temp.a,'_bldIn'),'blue');
        else
            AddOutportConstant(SystemName, temp.a, temp.pos, 1/4, 1)
        end
        
        if FbldOutNum > 0                                                                                             %  Bleed Output
            AddOutportGoTo(SystemName, temp.a, temp.pos, 1/8, 1, strcat(temp.a,'_bldOut'),'blue');
        else
            AddOutportTerminator(SystemName, temp.a, temp.pos, 1/5, 1);
        end
        AddOutportTerminator(SystemName, temp.a, temp.pos, 4/5, 3);                                                        % B_Data terminator block
        
        if MakeLinkerTags
            if BldInNum > 0
                Output.BleedIn = [Output.BleedIn {AddMakeLinkerInportGoTo(SystemName, temp.a, temp.pos, 1/4, 1, strcat(temp.a,'_bldIn'),'blue')}]; % PRin Input From tag
            end
            if FbldOutNum > 0
                Output.BleedOut = [Output.BleedOut {AddMakeLinkerOutportFromTag(SystemName, temp.a, temp.pos, 1/5,1,strcat(temp.a,'_bldOut'),'blue')}];
            end
  
        end
        
    case('Lib_Turbo_Burner_TMATS/Burner')
        AddInPortFromTag(SystemName, temp.a, temp.pos, 1/4, 1, strcat(temp.a,'_Wf'),'orange');      % FuelFlow Input From tag
        
        AddOutportTerminator(SystemName, temp.a, temp.pos, 3/4, 2);                                 % B_Data terminator block
        
        if MakeLinkerTags
            Output.Input = [Output.Input {AddMakeLinkerInportGoTo(SystemName, temp.a, temp.pos, 1/4,1,strcat(temp.a,'_Wf'),'orange')}];
        end
        
        
    case('Lib_Turbo_Compressor_TMATS/Compressor')
        AddInPortFromTag(SystemName, temp.a, temp.pos, 3/6, 2, strcat(temp.a,'_Rline'),'darkGreen');               % Rline Input From tag
        
        if CbldNum > 0                                                                                             %  Constant Bleed Output
            AddOutportGoTo(SystemName, temp.a, temp.pos, 1/12, 1, strcat(temp.a,'_Cbld'),'blue');
        else
            AddOutportTerminator(SystemName, temp.a, temp.pos, 1/12, 1);
        end
        if FbldOutNum > 0                                                                                             %  Fractional Bleed Output
            AddOutportGoTo(SystemName, temp.a, temp.pos, 3/12, 2, strcat(temp.a,'_Fbld'),'blue');
        else
            AddOutportTerminator(SystemName, temp.a, temp.pos, 3/12, 2);
        end
        AddOutportGoTo(SystemName, temp.a, temp.pos, 7/12, 4, strcat(temp.a,'_NErr'),'green');                     %  NErr Output GoTo tag
        AddOutportGoTo(SystemName, temp.a, temp.pos, 9/12, 5, strcat(temp.a,'_TrqOut'),'[1.000000, 0.333333, 0.498039]'); % Torque Output GoTo tag
        AddOutportTerminator(SystemName, temp.a, temp.pos, 11/12, 6);                                              %  C_Data Output terminator block
        
        if MakeLinkerTags
            Output.Ind = [Output.Ind {AddMakeLinkerInportGoTo(SystemName, temp.a, temp.pos, 3/6,2,strcat(temp.a,'_Rline'),'darkGreen')}];
            
            if CbldNum > 0
                Output.BleedOut = [Output.BleedOut {AddMakeLinkerOutportFromTag(SystemName, temp.a, temp.pos, 1/12,1,strcat(temp.a,'_Cbld'),'blue')}];
            end
            if FbldOutNum > 0
                Output.BleedOut = [Output.BleedOut {AddMakeLinkerOutportFromTag(SystemName, temp.a, temp.pos, 3/12,2,strcat(temp.a,'_Fbld'),'blue')}];
            end
            Output.Dep = [Output.Dep {AddMakeLinkerOutportFromTag(SystemName, temp.a, temp.pos, 7/12,4,strcat(temp.a,'_NErr'),'green')}];
            Output.Shaft = [Output.Shaft {AddMakeLinkerOutportFromTag(SystemName, temp.a, temp.pos, 9/12,5,strcat(temp.a,'_TrqOut'),'[1.000000, 0.333333, 0.498039]')}];
        end
    
    case('Lib_Turbo_FlowPathChar_TMATS/FlowPathChar')
        AddInPortFromTag(SystemName, temp.a, temp.pos, 1/8, 1,strcat(temp.a,'_W'),'darkGreen');     % W Input From tag
        
        if MakeLinkerTags
            Output.Ind = [Output.Ind {AddMakeLinkerInportGoTo(SystemName, temp.a, temp.pos, 1/8,1,strcat(temp.a,'_W'),'darkGreen')}];
        end
        
    case('Lib_Turbo_Inlet_TMATS/Inlet')
        AddInPortFromTag(SystemName, temp.a, temp.pos, 3/4, 2, 'Pamb','black');                 % Pamb Input From tag
        
    case('Lib_Turbo_Mixer_TMATS/Mixer')
        AddOutportGoTo(SystemName, temp.a, temp.pos, 3/6, 2, strcat(temp.a,'_NErr'),'green');   %  NErr Output GoTo tag
        AddOutportTerminator(SystemName, temp.a, temp.pos, 5/6, 3);                             % M_Data terminator block
        
        if MakeLinkerTags
            Output.Dep = [Output.Dep {AddMakeLinkerOutportFromTag(SystemName, temp.a, temp.pos, 3/6,2,strcat(temp.a,'_NErr'),'green')}];
        end
           
    case('Lib_Turbo_Nozzle_TMATS/Nozzle')
        AddInPortFromTag(SystemName, temp.a, temp.pos, 3/4, 2, 'Pamb','black');                 % Pamb Input From tag
        
        AddOutportTerminator(SystemName, temp.a, temp.pos, 1/8, 1);                             % WOut terminator block
        AddOutportTerminator(SystemName, temp.a, temp.pos, 3/8, 2);                             % Fg terminator block
        AddOutportGoTo(SystemName, temp.a, temp.pos, 5/8, 3, strcat(temp.a,'_NErr'),'green');   %  NErr Output GoTo tag
        AddOutportTerminator(SystemName, temp.a, temp.pos, 7/8, 4);                             % N_Data terminator block
        
        if MakeLinkerTags
            Output.Dep = [Output.Dep {AddMakeLinkerOutportFromTag(SystemName, temp.a, temp.pos, 5/8,3,strcat(temp.a,'_NErr'),'green')}];
        end
        
    case('Lib_Turbo_Shaft_TMATS/Shaft')
        AddInPortFromTag(SystemName, temp.a, temp.pos, 3/4, 2,strcat(temp.a,'_Nmech'),'red');     % Nmech Input From tag
        
        AddOutportGoTo(SystemName, temp.a, temp.pos, 1/4, 1, strcat(temp.a,'_NmechOut'),'red');      %  Nmech Output GoTo tag
        AddOutportGoTo(SystemName, temp.a, temp.pos, 3/4, 2, strcat(temp.a,'_Ndot'),'magenta');   %  Ndot Output GoTo tag
        
        if MakeLinkerTags
            Output.Input = [Output.Input {AddMakeLinkerInportGoTo(SystemName, temp.a, temp.pos, 3/4,2,strcat(temp.a,'_Nmech'),'red')}];
            
            Output.ShaftN = [Output.ShaftN {AddMakeLinkerOutportFromTag(SystemName, temp.a, temp.pos, 1/4,1,strcat(temp.a,'_NmechOut'),'red')}];
            Output.Output = [Output.Output {AddMakeLinkerOutportFromTag(SystemName, temp.a, temp.pos, 3/4,2,strcat(temp.a,'_Ndot'),'magenta')}];
        end
        
    case('Lib_Turbo_Splitter_TMATS/Splitter')
        AddInPortFromTag(SystemName, temp.a, temp.pos, 3/4, 2,strcat(temp.a,'_BPR'),'darkGreen');     % BPR Input From tag
        
        if MakeLinkerTags
            Output.Ind = [Output.Ind {AddMakeLinkerInportGoTo(SystemName, temp.a, temp.pos, 3/4,2,strcat(temp.a,'_BPR'),'darkGreen')}];
        end
        
    case{'Lib_Turbo_TurbineNPSS_TMATS/Turbine_NPSS', 'Lib_Turbo_Turbine_TMATS/Turbine'}
        if BldInNum > 0
            AddInPortFromTag(SystemName, temp.a, temp.pos, 1/8, 1, strcat(temp.a,'_bldIn'),'blue');                        % Bleed Input From tag
        else
            AddOutportConstant(SystemName, temp.a, temp.pos, 1/8, 1)
        end
        
        AddInPortFromTag(SystemName, temp.a, temp.pos, 5/8, 3, strcat(temp.a,'_PRin'),'darkGreen');                        % PRin Input From tag
        
        AddOutportGoTo(SystemName, temp.a, temp.pos, 3/8, 2, strcat(temp.a,'_NErr'),'green');                              % TrqOut GoTo tag
        AddOutportGoTo(SystemName, temp.a, temp.pos, 5/8, 3, strcat(temp.a,'_TrqOut'),'[1.000000, 0.333333, 0.498039]');   % TrqOut GoTo tag
        AddOutportTerminator(SystemName, temp.a, temp.pos, 7/8, 4);                                                        % T_Data terminator block
        
        if MakeLinkerTags
            if BldInNum > 0
                Output.BleedIn = [Output.BleedIn {AddMakeLinkerInportGoTo(SystemName, temp.a, temp.pos, 1/8, 1, strcat(temp.a,'_bldIn'),'blue')}]; % PRin Input From tag
            end
            Output.Ind = [Output.Ind {AddMakeLinkerInportGoTo(SystemName, temp.a, temp.pos, 5/8,3,strcat(temp.a,'_PRin'),'darkGreen')}];
            
            Output.Dep = [Output.Dep {AddMakeLinkerOutportFromTag(SystemName, temp.a, temp.pos, 3/8,2,strcat(temp.a,'_NErr'),'green')}];
            Output.Shaft = [Output.Shaft {AddMakeLinkerOutportFromTag(SystemName, temp.a, temp.pos, 5/8,3,strcat(temp.a,'_TrqOut'),'[1.000000, 0.333333, 0.498039]')}];
        end
            
    case('Lib_Turbo_Valve_TMATS/Valve')
        AddInPortFromTag(SystemName, temp.a, temp.pos, 3/6, 2,strcat(temp.a,'_ValvePos'),'orange');     % ValvePos Input From tag
        
        AddOutportTerminator(SystemName, temp.a, temp.pos, 5/6, 3);     % V_Data terminator block
        
        if MakeLinkerTags
            Output.Input = [Output.Input {AddMakeLinkerInportGoTo(SystemName, temp.a, temp.pos, 3/6,2,strcat(temp.a,'_ValvePos'),'orange')}];
        end
        
    otherwise
        temp.BT = get_param(BlockName,'BlockType');
        switch lower(temp.BT)
            case('from')
                try   % From add Out1
                    temp.pcx = temp.pos(2)+(temp.pos(4)-temp.pos(2))*1/2;
                    temp.pcy = temp.pos(3);
                    temp.pc = [(temp.pcy + 30) (temp.pcx - 6) (temp.pcy + 60) (temp.pcx + 6)];
                    temp.b = get_param(BlockName,'GoToTag');
                    temp.bname = strcat( SystemName,'/',temp.b);
                    add_block('simulink/Sinks/Out1',temp.bname,'MakeNameUnique','off','ShowName','on','Position',temp.pc, 'Foregroundcolor',get_param(BlockName,'ForegroundColor'))
                    add_line(SystemName,strcat(temp.a , '/1'), strcat(temp.b,'/1'),'autorouting','on')
                catch ME
                    disp('Block Linking naming conflict. Can not complete task.')
                end
            case('goto')
                try   % Goto add In1
                    temp.pcx = temp.pos(2)+(temp.pos(4)-temp.pos(2))*1/2;
                    temp.pcy = temp.pos(1);
                    temp.pc = [(temp.pcy - 60) (temp.pcx - 6) (temp.pcy - 30) (temp.pcx + 6)];
                    temp.b = get_param(BlockName,'GoToTag');
                    temp.bname = strcat( SystemName,'/',temp.b);
                    add_block('simulink/Sources/In1',temp.bname,'MakeNameUnique','off','ShowName','on','Position',temp.pc, 'Foregroundcolor',get_param(BlockName,'ForegroundColor'))
                    add_line(SystemName,strcat(temp.b , '/1'), strcat(temp.a,'/1'),'autorouting','on')
                catch ME
                    disp('Block Linking naming conflict. Can not complete task.')
                end
            case('inport')
                try   % Inport add GoTo
                    temp.pcx = temp.pos(2)+(temp.pos(4)-temp.pos(2))*1/2;
                    temp.pcy = temp.pos(3);
                    temp.pc = [(temp.pcy + 30) (temp.pcx - 6) (temp.pcy + 190) (temp.pcx + 6)];
                    temp.b = get_param(BlockName,'Name');
                    temp.bname = strcat( SystemName,'/',temp.b,'1');
                    add_block('simulink/Signal Routing/Goto',temp.bname,'MakeNameUnique','off','ShowName','off','GotoTag', temp.b, 'Position',temp.pc, 'Foregroundcolor',get_param(BlockName,'ForegroundColor'))
                    add_line(SystemName,strcat(temp.a,'/1'),strcat(temp.b,'1/1'),'autorouting','on')
                catch ME
                    disp('Block Linking naming conflict. Can not complete task.')
                end
            case('outport')
                try   % Inport add From
                    temp.pcx = temp.pos(2)+(temp.pos(4)-temp.pos(2))*1/2;
                    temp.pcy = temp.pos(3);
                    temp.pc = [(temp.pcy - 205) (temp.pcx - 6) (temp.pcy - 50) (temp.pcx + 6)];
                    temp.b = get_param(BlockName,'Name');
                    temp.bname = strcat( SystemName,'/',temp.b,'1');
                    add_block('simulink/Signal Routing/From',temp.bname,'MakeNameUnique','off','ShowName','off','GotoTag', temp.b, 'Position',temp.pc, 'Foregroundcolor',get_param(BlockName,'ForegroundColor'))
                    add_line(SystemName,strcat(temp.b,'1/1'),strcat(temp.a,'/1'),'autorouting','on')
                catch ME
                    disp('Block Linking naming conflict. Can not complete task.')
                end
            case('subsystem')
                temp.Nports = get_param(BlockName,'Ports');
                temp.Nin = temp.Nports(1);
                temp.Nout = temp.Nports(2);
                temp.BLKS = get_param(BlockName,'Blocks');
                if temp.Nin > 0
                    for i = 1:temp.Nin % Create an Input From block for each Subsystem Input
                        try   % Generic Subsystem Input From
                            temp.pcx = temp.pos(2)+(temp.pos(4)-temp.pos(2))* (2*i-1)/(temp.Nin*2);
                            temp.pcy = temp.pos(1);
                            temp.pc = [(temp.pcy - 175) (temp.pcx - 6) (temp.pcy - 15) (temp.pcx + 6)];
                            temp.b = temp.BLKS(i);
                            temp.bname = strcat( SystemName,'/',temp.a,temp.b{1});
                            add_block('simulink/Signal Routing/From',temp.bname,'MakeNameUnique','off','ShowName','off','GotoTag', temp.b{1} , 'Position',temp.pc)
                            temp.i = num2str(i);
                            add_line(SystemName,strcat(temp.a,temp.b{1}, '/1'), strcat(temp.a,'/',temp.i),'autorouting','on')
                        catch
                        end
                    end
                end
                if temp.Nout > 0
                    for i = 1:temp.Nout % Create an Output Goto block for each Subsystem Output
                        try   % Generic Subsystem Output Goto
                            temp.pcx = temp.pos(2)+(temp.pos(4)-temp.pos(2))* (2*i-1)/(temp.Nout*2);
                            temp.pcy = temp.pos(3);
                            temp.pc = [(temp.pcy + 15) (temp.pcx - 6) (temp.pcy + 175) (temp.pcx + 6)];
                            temp.blength = length(temp.BLKS);
                            temp.b = temp.BLKS(temp.blength - temp.Nout + i);
                            temp.bname = strcat( SystemName,'/',temp.a,temp.b{1});
                            add_block('simulink/Signal Routing/Goto',temp.bname,'MakeNameUnique','off','ShowName','off','GotoTag', temp.b{1} , 'Position',temp.pc)
                            temp.i = num2str(i);
                            add_line(SystemName,strcat(temp.a,'/',temp.i) , strcat(temp.a,temp.b{1}, '/1'),'autorouting','on')
                        catch
                        end
                    end
                end
            otherwise
        end
end

if MKSortTypeEn
    SortedMK = Output;
end


function AddOutportTerminator(SystemName, ShortBlockName, BlockPos, Xfrac, BlockOutportNumber)
try   % add terminator block
    pcx = BlockPos(2)+(BlockPos(4)-BlockPos(2))*Xfrac;
    pcy = BlockPos(3);
    pc = [(pcy + 15) (pcx - 5) (pcy + 25) (pcx + 5)];
    BNstr = num2str(BlockOutportNumber);
    FullBName = strcat( SystemName,'/',ShortBlockName,'_term',BNstr);
    add_block('simulink/Commonly Used Blocks/Terminator',FullBName,'MakeNameUnique','off','ShowName','off','Position',pc)
    add_line(SystemName,[ShortBlockName '/' BNstr],strcat(ShortBlockName,'_term',BNstr,'/1'),'autorouting','on')
catch ME
    disp(ME);
end

function AddOutportConstant(SystemName, ShortBlockName, BlockPos, Xfrac, BlockOutportNumber)
try   % add terminator block
    pcx = BlockPos(2)+(BlockPos(4)-BlockPos(2))*Xfrac;
    pcy = BlockPos(1);
    pc = [(pcy - 115) (pcx - 10) (pcy - 15) (pcx + 10)];
    BNstr = num2str(BlockOutportNumber);
    FullBName = strcat( SystemName,'/',ShortBlockName,'_Const',BNstr);
    add_block('simulink/Commonly Used Blocks/Constant',FullBName,'MakeNameUnique','off','ShowName','off','Position',pc, 'Value','[0 0 0 0 0]')
    add_line(SystemName,strcat(ShortBlockName,'_Const',BNstr,'/1'),[ShortBlockName '/' BNstr],'autorouting','on')
catch ME
    disp(ME);
end

function AddOutportGoTo(SystemName, ShortBlockName, BlockPos, Xfrac, BlockOutNumber, FillName,color)
try   % add GoTo tag
    pcx = BlockPos(2)+(BlockPos(4)-BlockPos(2))*Xfrac;
    pcy = BlockPos(3);
    pc = [(pcy + 15) (pcx - 6) (pcy + 175) (pcx + 6)];
    BNstr = num2str(BlockOutNumber);
    FullBName = strcat( SystemName,'/',ShortBlockName,'_GoTo',BNstr);
    add_block('simulink/Signal Routing/Goto',FullBName,'MakeNameUnique','off','ShowName','off','GotoTag', FillName, 'Position',pc, 'Foregroundcolor',color)
    add_line(SystemName,[ShortBlockName '/' BNstr],strcat(ShortBlockName,'_GoTo',BNstr,'/1'),'autorouting','on')
catch ME
    disp(ME);
end

function AddInPortFromTag(SystemName, ShortBlockName, BlockPos, Xfrac, BlockInNumber, FillName,color)
try   % add From tag
    pcx = BlockPos(2)+(BlockPos(4)-BlockPos(2))*Xfrac;
    pcy = BlockPos(1);
    pc = [(pcy - 175) (pcx - 6) (pcy - 15) (pcx + 6)];
    BNstr = num2str(BlockInNumber);
    FullBName = strcat( SystemName,'/',ShortBlockName,'_From',BNstr);
    add_block('simulink/Signal Routing/From',FullBName,'MakeNameUnique','off','ShowName','off','GotoTag', FillName, 'Position',pc, 'Foregroundcolor',color)
    add_line(SystemName,strcat(ShortBlockName,'_From',BNstr,'/1'), strcat(ShortBlockName,'/',BNstr),'autorouting','on')
catch ME
    disp(ME)
end

function FullBName = AddMakeLinkerInportGoTo(SystemName, ShortBlockName, BlockPos, Xfrac, BlockInNumber, FillName,color)
try   % add GoTo maker tag
    pcx = BlockPos(2)+(BlockPos(4)-BlockPos(2))*Xfrac;
    pcy = BlockPos(1);
    pc = [(pcy - 360) (pcx - 6) (pcy -200) (pcx + 6)];
    BNstr = num2str(BlockInNumber);
    FullBName = strcat( SystemName,'/',ShortBlockName,'_GoToMK',BNstr);
    add_block('simulink/Signal Routing/Goto',FullBName,'MakeNameUnique','off','ShowName','off','GotoTag', FillName, 'Position',pc, 'Foregroundcolor',color)
catch ME
    disp(ME)
end

function FullBName = AddMakeLinkerOutportFromTag(SystemName, ShortBlockName, BlockPos, Xfrac, BlockOutportNumber, FillName,color)
try   % add From maker tag
    pcx = BlockPos(2)+(BlockPos(4)-BlockPos(2))*Xfrac;
    pcy = BlockPos(3);
    pc = [(pcy + 200) (pcx - 6) (pcy + 360) (pcx + 6)];
    BNstr = num2str(BlockOutportNumber);
    FullBName = strcat( SystemName,'/',ShortBlockName,'_FromMK',BNstr);
    add_block('simulink/Signal Routing/From',FullBName,'MakeNameUnique','off','ShowName','off','GotoTag', FillName, 'Position',pc, 'Foregroundcolor',color)
catch ME
    disp(ME)
end

