//
// Generate T-MATS model from NPSS data
// Written by Jeffryes Chapman 
//

function AutoLink_TMATS(TMTS)
% This model uses a T-MATS NPSS to T-MATS block definition structure to
% create a simulink diagram

if ~exist('TMTS','var')
    error('Must execute this function with the proper inputs, AutoLink_TMATS(ModelBuildVariableName)');
end


% find file delimiter
Element.General.FOp = '/';
% determine Model Name
Element.General.ModelName = [TMTS.INFO.ModelName '_TMATS'];
%---------------------------------------------
% Determine Starting flow point of model
%---------------------------------------------
% Get name of InletStart blocks.  These will be the starting location of
% the model.
InletStartName = GetElementName(TMTS,'InletStart');

% Note: T-MATS has no InletStart block, so this will be replaced by Ambient block.
% Get name of Ambient block. This will be the starting block for the model.
AmbientStartName = GetElementName(TMTS,'Ambient');
% set Design flow for InletStart to the Ambient block
if ~isfield(TMTS.(AmbientStartName{1}),'Wdes')
TMTS.(AmbientStartName{1}).Wdes = TMTS.(InletStartName{1}).Wdes;
assignin('base',TMTS.INFO.WorkSpaceName, TMTS);
disp([TMTS.INFO.WorkSpaceName ' updated to include Variable ' AmbientStartName{1} '.Wdes']);
end
%-------------------------------------------------------------------------
% Begin Creation of Linking Matrix
% Format is LinkMtrx{rows,col} where col are element to be placed along
% the x axis and rows are elements that will be displaced in the y axis.
% There will also be an Elment structure created to hold block parameters
%-------------------------------------------------------------------------

% Start a row for the InletStartName (assumes there is only one
LinkMtrx{1,1} = AmbientStartName{1};
Element = PopElementGen(Element, AmbientStartName{1}, TMTS);
LinkMtrx{1,2} = GetOutputElementName(TMTS,strcat(InletStartName{1},'.Fl_O'));
Element = PopElementGen(Element, LinkMtrx{1,2}, TMTS);

% begin populating linking matrix
ActiveLinkMtxGen = true;
n = 2;
i = 1;
while ActiveLinkMtxGen == true;
    % Find next block based on where output of current block is linked to
    if strcmp(Element.(LinkMtrx{i,n}).Type, 'Splitter')
        % check if block is a splitter
        % Perform 2 block lookups, one for each splitter output
        StationOutputBlockName = GetOutputElementName(TMTS,strcat(LinkMtrx{i,n},'.Fl_O1'));
        Element = PopElementGen(Element, StationOutputBlockName, TMTS);
        StationOutputBlockName2 = GetOutputElementName(TMTS,strcat(LinkMtrx{i,n},'.Fl_O2'));
        % Second splitter output will populate LinkerMatrix next Row
        LinkMtrx{i+1,1} = LinkMtrx{i,n};
        Element = PopElementGen(Element, LinkMtrx{i+1,1}, TMTS);
        LinkMtrx{i+1,2} = StationOutputBlockName2;
        Element = PopElementGen(Element, StationOutputBlockName2, TMTS);
        Element.(LinkMtrx{i,n}).Outport1 = StationOutputBlockName2;
        
    elseif strcmp(Element.(LinkMtrx{i,n}).Type, 'Mixer')
        % If new block is a mixer check if this is the first input
        idx = find(strcmp(TMTS.INFO.STATIONS(1,:),strcat(LinkMtrx(i,n-1),'.Fl_O')));
        % Determine name of output block at station
        LongName = TMTS.INFO.STATIONS{3,idx};
        % remove name information from the flow direction information
        Temp = strsplit(LongName,'.');
        InputNumber = Temp{2};
        
        if strcmp(InputNumber,'Fl_I1')
            % if this is the first input continue adding blocks
            StationOutputBlockName = GetOutputElementName(TMTS,strcat(LinkMtrx{i,n},'.Fl_O'));
            Element = PopElementGen(Element, StationOutputBlockName, TMTS);
        else
            % if this is not the first input stop adding blocks
            StationOutputBlockName = [];
            % Add input name
            Element.(LinkMtrx{i,n}).Inport1 = LinkMtrx{i,n-1};
        end
    else
        % Perform standard block lookup
        StationOutputBlockName = GetOutputElementName(TMTS,strcat(LinkMtrx{i,n},'.Fl_O'));
        StationOutputBlockType = GetElementType(TMTS,StationOutputBlockName);
    end
    
    
    if strcmp(Element.(LinkMtrx{i,n}).Type, 'Mixer')
        
    end
    
    %Populate Linker Matrix with block unless it is the end of the flow
    %line.
    if isempty(StationOutputBlockName) || strcmp(StationOutputBlockType, 'FlowEnd')
        i = i + 1;
        n = 2;
    else
        n = n + 1;
        LinkMtrx{i,n} = StationOutputBlockName;
        Element = PopElementGen(Element, StationOutputBlockName, TMTS);
    end
    
    if i > size(LinkMtrx,1)
        ActiveLinkMtxGen = false;
    end
end

%Linking matrix generation completed

% Create BleedLinking Matrix
BleedLinker = {'OutputElementName'; 'OutputFromName'; 'RawOutputName'; 'InputElementName'; 'InputGoToName' ; 'RawInputName'; 'FillName'};
BleedOutLen = length(TMTS.INFO.BLEED_OUT);
if BleedOutLen > 1
    for n = 1: BleedOutLen
        % Get Outport element name and set from tag name.
        FullOutputPortName = TMTS.INFO.BLEED_OUT{n};
        SplitOutputPortName = strsplit(FullOutputPortName,'.');
        OutputElementName = SplitOutputPortName{1};
        FromName = [SplitOutputPortName{1} '_' SplitOutputPortName{2}];
        % Get Inport element name.
        idx = find(strcmp(TMTS.INFO.STATIONS(1,:),FullOutputPortName));
        % Set From tag fill name
        Fill = TMTS.INFO.STATIONS{2,idx};
        
        % Set goto tag name
        % Determine name of block recieving bleed flow (input block)
        LongName = TMTS.INFO.STATIONS{3,idx};
        Temp = strsplit(LongName,'.');
        InputElementName = Temp{1};
        InputGoToName = [Temp{1} '_' Temp{2}];
        BleedLinker =[BleedLinker, {OutputElementName; FromName; FullOutputPortName; InputElementName; InputGoToName; LongName; Fill}];
    end
end

disp('completed creation of Linker Matrix ')

%------------------------------------------------------
% Begin creation of simulink diagram
%------------------------------------------------------
% Create new system
if exist(Element.General.ModelName,'file') ==0
    new_system(Element.General.ModelName)  % Create model
end
open_system(Element.General.ModelName) % Open model

%-----------------------------------------------
% start model creation
%-----------------------------------------------
% Set nexus point
LocationAdjust = [0 0 0 0];
MKSortVecs = [];

%-----------------------------------------------
% Place Flow Blocks
%-----------------------------------------------

for n = 1: size(LinkMtrx,1)
    % Set Location of first elment in row.
    if n == 1
        % If this is the first block it can't have been placed before
        FirstBlockPlacedAlready = false;
    else
        % Look through all blocks currently placed for the current block
        % This typically occurs only when the flow is split.
        n_old = 1:n-1;
        [idi,idj] = find(strcmp(LinkMtrx(n_old,:),LinkMtrx(n,1)));
        FirstBlockPlacedAlready = ~isempty(idi);
        if FirstBlockPlacedAlready
            % make the set location based on the first occurance
            LocationAdjust = Element.(LinkMtrx{n,1}).Location + [500 -500 500 -500];
            % if block has been placed already. Make inport to block and
            % use outport number equal to: 1 minus the min of the previous occurances.
            %search for last Outport number
            Found = false;
            iter = 1;
            while Found==false && iter < 100
                s = int2str(iter);
                OutportName = ['Outport' s];
                % search Element for Output number ~ iter
                if isfield(Element.(LinkMtrx{n,i}), OutportName)
                    Found = true;
                else
                    iter = iter +1;
                end
            end
            % outport number is 1 - min current output number
            OutportName = ['Outport' num2str(max(iter -1,1))];
            if ~isempty(LinkMtrx{n,i+1})
                Element.(LinkMtrx{n,i}).(OutportName) =  LinkMtrx{n,i+1};
            end
        else
            % If block has not been placed yet start it at front of model
            Element.(LinkMtrx{n,1}).Location = [0 -500 0 -500];
            LocationAdjust = [0 0 0 0];
        end
    end
    
    % Determine and place each element in a Linker Matrix row.
    for i = 1: size(LinkMtrx,2)
        % Determine if block has already been placed not including the
        % first block in a row.
        % Will typically only occur when merging.
        CurrentBlockPlacedAlready = false;
        if (i > 1 && n > 1)
        n_old = 1:n-1;
        [idi,idj] = find(strcmp(LinkMtrx(n_old,:),LinkMtrx(n,i)));
        CurrentBlockPlacedAlready = ~isempty(idi);
        end
        if isempty(LinkMtrx{n,i})
            % Do nothing
            UpdatedLocation = LocationAdjust;
        elseif (i == 1 && FirstBlockPlacedAlready) || CurrentBlockPlacedAlready
            % this is the first block and it has already been placed...
            % do nothing
            UpdatedLocation = LocationAdjust;
        else
            
            if length(LinkMtrx(n,:)) > i
                NextBlock = LinkMtrx{n,i+1};
            else
                NextBlock = [];
            end
            if i > 1
                LastBlock = LinkMtrx{n,i-1};
            else
                LastBlock = [];
            end
            % Populate Element with block specific port information and
            % location
            [Element, UpdatedLocation] = LocateBlock(Element, LinkMtrx{n,i}, LocationAdjust, NextBlock, LastBlock);
            % place block
            placeBlock(Element, LinkMtrx{n,i});
            % run GoTo tag linker
            MKSorted = runLinker(Element, LinkMtrx{n,i});
            % Set maker tag groups, to be used to sort tags later
            MKSortVecs = UpdateMKSortVector(MKSortVecs, MKSorted);
        end
        
        LocationAdjust = UpdatedLocation;
    end
    i = 1;
end
disp('completed placing flow blocks ')

%------------------------------------------------
% Place Shaft Blocks
%---------------------------------------------

NumberOfShafts = numel(TMTS.INFO.SHAFT);
% Starting shaft position
ShaftPos = [0 600 0 600];

for shaftnum = 1:NumberOfShafts
    % Get name of this shaft
    ShftName = TMTS.INFO.SHAFT{shaftnum}{1};
    % populate parameters for this shaft element
    Element = PopElementGen(Element, ShftName, TMTS);
    % place shaft block
    [Element, UpdatedLocation] = LocateBlock(Element, ShftName, ShaftPos, 'None', 'None');
    placeBlock(Element, ShftName);
    % perform linker on shaft block
    MKSorted = runLinker(Element, ShftName);
    % update MKsort to contain shaft tags
    MKSortVecs = UpdateMKSortVector(MKSortVecs, MKSorted);
    
    % Define Mux to combine torques
    NumberOfTorquesp1 = numel(TMTS.INFO.SHAFT{shaftnum});
    MuxName = [ShftName '_Mux'];
    % place a Mux
    MuxPosition = placeMux(Element.General.ModelName,Element.General.FOp, NumberOfTorquesp1, ShftName, MuxName, ShaftPos);
    
    BasePosition = MuxPosition;
    
    % place from tags for each torque
    for torqI = 2:NumberOfTorquesp1
        % Split Name
        TorqueName = TMTS.INFO.SHAFT{shaftnum}{torqI};
        SplitTorqueName = strsplit(TorqueName,'.');
        FromTagName = [SplitTorqueName{1} '_' SplitTorqueName{2}];
        idx = find(strcmp(TMTS.INFO.STATIONS(3,:),TorqueName));
         FillName    = TMTS.INFO.STATIONS{2,idx};
        % set port number and Location Fraction
        PortNumber = torqI-1;
        LocFrac = PortNumber/NumberOfTorquesp1;
        % place torque from tag
        placeFromTag(Element.General.ModelName,Element.General.FOp, LocFrac, BasePosition, FromTagName, FillName, MuxName, PortNumber, '[1.000000, 0.333333, 0.498039]')
    end
    
    % Place HPX constant on last mux input
    ConstantName = [ShftName '_HPX'];
    BFill = [TMTS.INFO.WorkSpaceName '.' ShftName '.HPX'];
    placeConstant(Element.General.ModelName, Element.General.FOp, BasePosition, BFill, ConstantName, MuxName, NumberOfTorquesp1)
    % Update location for next Shaft
    ShaftPos = UpdatedLocation + [500 0 500 0];
end

disp('completed placing shaft blocks')
%-----------------------------------------------------------------------
% Move Maker tags to be grouped by sort type and make connections to block
% links
%------------------------------------------------------------------------
Vecfields = {'None'};
if ~isempty(MKSorted)
    Vecfields = fieldnames(MKSortVecs);
end
xstp = 800;
ystp = 800;
MKpos.Input     = [0 ystp 0 ystp];
MKpos.Output    = [1*xstp ystp 1*xstp ystp];
MKpos.Ind       = [2*xstp ystp 2*xstp ystp];
MKpos.Dep       = [3*xstp ystp 3*xstp ystp];
MKpos.Shaft     = [4*xstp ystp 4*xstp ystp];
MKpos.ShaftN    = [5*xstp ystp 5*xstp ystp];
MKpos.BleedOut  = [6*xstp ystp 6*xstp ystp];
MKpos.BleedIn   = [7*xstp ystp 7*xstp ystp];
TMTS.INFO.Inputs = [];
TMTS.INFO.Inputs.IndependentICs = [];
TMTS.INFO.Inputs.IndependentNames = {};

for fid = 1:numel(Vecfields)
    % create Label for Make Linker catagory
    add_block('built-in/Note',[Element.General.ModelName Element.General.FOp Vecfields{fid}],'Position',MKpos.(Vecfields{fid}),'FontSize', 36);
    for elms = 1:numel(MKSortVecs.(Vecfields{fid}))
        % For each maker tag in a catagory, move to new labeled location
        % and, where appropriate, add station links, constant inputs or terminators.
        
        % update position
        MKpos.(Vecfields{fid}) = MKpos.(Vecfields{fid}) + [0 50 0 50];
        % change position of MK tag
        CurPos = get_param(MKSortVecs.(Vecfields{fid}){elms},'Position');
        MKsize = [(CurPos(3)-CurPos(1)),(CurPos(4) - CurPos(2))];
        newPosition = MKpos.(Vecfields{fid}) + [0 0 MKsize(1) MKsize(2)];
        set_param(MKSortVecs.(Vecfields{fid}){elms},'Position', newPosition);
        
        if strcmp(Vecfields{fid},'Input')
            % if the MK tag is a Input tag add constants
            MKFillSplt = strsplit(get_param(MKSortVecs.Input{elms},'GotoTag'),{'_'});
            MKShortName = get_param(MKSortVecs.Input{elms},'Name');
            if length(MKFillSplt) > 2
                BlockDest = strjoin(MKFillSplt(1:end-1),'_');
            else
                BlockDest = MKFillSplt{1};
            end
            BlockInput = MKFillSplt{end};
            CnstName = ['Cnst_' BlockDest '_' BlockInput];
            CnstValue = [TMTS.INFO.WorkSpaceName '.' BlockDest '.' BlockInput];
            
            TMTS.INFO.Inputs.(CnstName) = TMTS.(BlockDest).(BlockInput);
            
            placeConstant(Element.General.ModelName, Element.General.FOp, newPosition, CnstValue, CnstName, MKShortName , 1);
            
        elseif strcmp(Vecfields{fid},'Ind')
            % if the MK tag is a Independent tag add constants
            MKFillSplt = strsplit(get_param(MKSortVecs.Ind{elms},'GotoTag'),{'_'});
            MKShortName = get_param(MKSortVecs.Ind{elms},'Name');
            if length(MKFillSplt) > 2
                BlockDest = strjoin(MKFillSplt(1:end-1),'_');
            else
                BlockDest = MKFillSplt{1};
            end
            TMTSIndName = MKFillSplt{end};
            BlockInput = Tag2Const(TMTS,BlockDest,TMTSIndName);
            CnstName = ['Cnst_' BlockDest '_' BlockInput];
            CnstValue = [TMTS.INFO.WorkSpaceName '.' BlockDest '.' BlockInput];
            
            TMTS.INFO.Inputs.IndependentICs = [TMTS.INFO.Inputs.IndependentICs TMTS.(BlockDest).(BlockInput)];
            TMTS.INFO.Inputs.IndependentNames = [TMTS.INFO.Inputs.IndependentNames [BlockDest '.' BlockInput]];
            
            placeConstant(Element.General.ModelName, Element.General.FOp, newPosition, CnstValue, CnstName, MKShortName , 1);
            
            
        elseif strcmp(Vecfields{fid},'BleedOut')
            % if the MK tag is a bleed out tag add Links to Bleed Stations
            %Get name of MakerLink origin
            MKNameSplt = strsplit(get_param(MKSortVecs.BleedOut{elms},'GotoTag'),{'_'});
            if length(MKNameSplt) > 2
                BlockOrigin = strjoin(MKNameSplt(1:end-1),'_');
            else 
                BlockOrigin = MKNameSplt{1};
            end
            % pull number of bleeds
            BleedRowNums = find(strcmp(BleedLinker(1,:),BlockOrigin));
            NumberofBleeds = length(BleedRowNums);
            isBleedBlock = strcmp(Element.(BlockOrigin).Type, 'Bleed');
            % set default starting position
            BasePosition = newPosition;
            BaseName = get_param(MKSortVecs.(Vecfields{fid}){elms},'Name');
            if  NumberofBleeds > 1
                % place a DeMux
                % Get Name for Demux block
                MuxName = [BlockOrigin '_Demux'];
                % set baseline position used when placing Tags
                BaseName = MuxName;
                DemuxOutputsStr = strcat('[ ',num2str(5*ones(1,NumberofBleeds)), ' ]');
                BasePosition = placeDemux(Element.General.ModelName, Element.General.FOp, NumberofBleeds, get_param(MKSortVecs.(Vecfields{fid}){elms},'Name'), MuxName, BasePosition, DemuxOutputsStr);
                % update Overall position vector to reflect a large Demux
                MKpos.(Vecfields{fid}) = MKpos.(Vecfields{fid})+ (BasePosition(4)- BasePosition(2))*[0 1 0 1];
            end
            % place GoTo blocks for each Bleed within this Port
            for bnum = 1:NumberofBleeds
                DestBlock = BleedLinker{4,BleedRowNums(bnum)};
                GoToTagFillName = BleedLinker{7,BleedRowNums(bnum)};
                GoToTagName= BleedLinker{2,BleedRowNums(bnum)};
                
                LocFrac = bnum/NumberofBleeds;
                
                if isfield(Element, DestBlock);
                    % place bleed station tag
                    placeGoToTag(Element.General.ModelName,Element.General.FOp, LocFrac, BasePosition, GoToTagName, GoToTagFillName, BaseName, bnum, 'blue');
                else
                    % if Destination block is not in the Element matrix
                    % place a terminator
                    placeTerminator(Element.General.ModelName,Element.General.FOp, LocFrac, BasePosition, GoToTagName, BaseName, bnum);
                end
            end
            
        elseif strcmp(Vecfields{fid},'BleedIn')
            % if the MK tag is a bleed in tag add Links to Bleed Stations
            %Get name of MakerLink origin
            MKNameSplt = strsplit(get_param(MKSortVecs.BleedIn{elms},'GotoTag'),{'_'});
            if length(MKNameSplt) > 2
                BlockOrigin = strjoin(MKNameSplt(1:end-1),'_');
            else 
                BlockOrigin = MKNameSplt{1};
            end
            BleedRowNums = find(strcmp(BleedLinker(4,:),BlockOrigin));
            NumberofBleeds = length(BleedRowNums);
            isBleedBlock = strcmp(Element.(BlockOrigin).Type, 'Bleed');
            
            BasePosition = newPosition;
            BaseName = get_param(MKSortVecs.(Vecfields{fid}){elms},'Name');
            if  NumberofBleeds > 1
                MuxName = [BlockOrigin '_Mux'];
                BasePosition = BasePosition + 50*[ 0 1 0 1];
                % place a Mux
                BasePosition = placeMux(Element.General.ModelName, Element.General.FOp, NumberofBleeds, BaseName, MuxName, BasePosition);
                % update Overall position vector to reflect a large Mux
                MKpos.(Vecfields{fid}) = MKpos.(Vecfields{fid}) + (BasePosition(4)- BasePosition(2))*[0 1 0 1];
                BaseName = MuxName;
            end
            % place GoTo blocks for each Bleed within this Port
            for bnum = 1:NumberofBleeds
                % Get block name and Fill string
                BFFillName = BleedLinker{7,BleedRowNums(bnum)};
                BFName= BleedLinker{5,BleedRowNums(bnum)};

                LocFrac = bnum/NumberofBleeds;
                % place Bleed Station Tag
                placeFromTag(Element.General.ModelName,Element.General.FOp, LocFrac, BasePosition, BFName, BFFillName, BaseName, bnum, 'blue')
            end
            
        elseif strcmp(Vecfields{fid},'Shaft')
            % if the MK tag is a shaft torque, link to appropriate shaft torque
            % station
            
            % Get name of MakerLink origin
            BasePosition = newPosition;
            BaseName = get_param(MKSortVecs.(Vecfields{fid}){elms},'Name');
            
            % Get block name and Fill string
            MKNameSplt = strsplit(MKSortVecs.Shaft{elms},{'/','_'});
            BlockOrigin = MKNameSplt{numel(MKNameSplt)-1};
            
            idx = find(strcmp(TMTS.INFO.STATIONS(1,:),[BlockOrigin '.Sh_O']));
            BFFillName = TMTS.INFO.STATIONS{2,idx};
            BFName  = [BlockOrigin '_' BFFillName];
            
            placeGoToTag(Element.General.ModelName,Element.General.FOp, 1, BasePosition, BFName, BFFillName, BaseName, 1, '[1.000000, 0.333333, 0.498039]');
            
        elseif strcmp(Vecfields{fid},'ShaftN')
            % for shaft speed value, create shaft Nmech link
            
            % Get name of MakerLink origin
            BasePosition = newPosition;
            BaseName = get_param(MKSortVecs.(Vecfields{fid}){elms},'Name');
            
            % Get block name and Fill string
            MKNameSplt = strsplit(BaseName,{'_'});
            if length(MKNameSplt) > 2
                BlockOrigin = strjoin(MKNameSplt(1:end-1),'_');
            else 
                BlockOrigin = MKNameSplt{1};
            end
            
            FillName = [BlockOrigin '_NmechElem'];
            BFName  = FillName;
            %place Station shaft speed tag
            placeGoToTag(Element.General.ModelName,Element.General.FOp, 1, BasePosition, BFName, FillName, BaseName, 1, 'red');
            
            % Get names of components connected to shaft
            ShaftFound = false;
            shaftIter = 1;
            while ShaftFound == false && shaftIter < NumberOfShafts
                if strcmp(BlockOrigin, TMTS.INFO.SHAFT{shaftIter}{1})
                    ShaftFound = true;
                else
                    shaftIter = shaftIter + 1;
                end
            end
                        
            % for each element connected to the shaft add a from tag
            NumberOfElementsOnShaft = numel(TMTS.INFO.SHAFT{shaftIter})-1;
            for elemShaft = 2: NumberOfElementsOnShaft+1
                % get name of component on shaft
                ShaftLinkingName = TMTS.INFO.SHAFT{shaftIter}{elemShaft};
                idx = find(strcmp(TMTS.INFO.STATIONS(3,:),ShaftLinkingName));
                ElementStationName = TMTS.INFO.STATIONS{1,idx};
                ElementStationSplitName = strsplit(ElementStationName,{'.'});
                ElementName = ElementStationSplitName{1};
                TagName = [ElementName '_Nmech'];
                % TagFill name will be the same as FillName defined
                % above.
                
                BasePosition = Element.(ElementName).Location;
                portNumber = Element.(ElementName).ShaftInPort;
                LocFrac =  1+ portNumber/4.25;
                % Create From tag to connect to block.  Note: Element should
                % contain a ShaftInPort field created when the function
                % LocateBlock was run.
                placeFromTag(Element.General.ModelName,Element.General.FOp, LocFrac, BasePosition, TagName, FillName, ElementName, portNumber, 'red')
            end
        end
        
    end
end
disp('completed moving maker tags ')

%----------------------------------------------------
% Tag creation Checking
%----------------------------------------------------
% Get blocks within system
BlkD = find_system(Element.General.ModelName);

% Check If all bleed tags were created and linked.

% Check for BleedOut tags
for i = 2: size(BleedLinker,2)
    BleedOutFound = find(strcmp([Element.General.ModelName Element.General.FOp BleedLinker{2,i}], BlkD));
    if isempty(BleedOutFound)
        disp(strcat('Warning: Bleed Output "', BleedLinker(2,i) ,'" not created.'))
    end
end
% Check for BleedIn tags
for i = 2: size(BleedLinker,2)
    SplitBlockName = strsplit(BleedLinker{4,i},'_');
    ShortName = SplitBlockName{1};
    idx = find(strcmp(ShortName, TMTS.INFO.ELEMENT(1,:)));
    BlockType = TMTS.INFO.ELEMENT(2,idx);
    if ~strcmp(BlockType, 'FlowEnd')
        BleedInFound = find(strcmp([Element.General.ModelName Element.General.FOp BleedLinker{4,i}], BlkD));
        if isempty(BleedInFound)
            disp(strcat('Warning: Bleed Input "', BleedLinker(4,i) ,'" not created.'))
        end
    end
end

% Check If all shaft torque tags were created and linked
for i = 1:numel(TMTS.INFO.SHAFT)
    for n = 2: numel(TMTS.INFO.SHAFT{i})
        TorqueTagNameSplit = strsplit(TMTS.INFO.SHAFT{i}{n},'.');
        TorqueTagName = [TorqueTagNameSplit{1} '_' TorqueTagNameSplit{2}];
        TorqueFound = find(strcmp([Element.General.ModelName Element.General.FOp TorqueTagName], BlkD));
        if isempty(TorqueFound)
            disp(strcat('Warning: Torque "', TorqueTagName ,'" not created.'))
        end
    end
end
%------------------------------------
% Populate Blocks
%------------------------------------
TFields = fields(TMTS);
for n = 1: numel(TFields)
    if strcmp(TFields{n},'INFO') 
        %do nothing
    else
        TSFields = fields(TMTS.(TFields{n}));
        for i = 1: numel(TSFields)
            idMask = strfind(TSFields{i},'_M');
            if ~isempty(idMask)
                % parameter is a mask parameter
                FullBlockName = [Element.General.ModelName Element.General.FOp TFields{n}];
                if isempty(TMTS.(TFields{n}).(TSFields{i}))
                    VariableName = '0';
                else
                    VariableName = [TMTS.INFO.WorkSpaceName '.' TFields{n} '.' TSFields{i}];
                end
                % try to set parameter variable name.
                try
                    set_param(FullBlockName,TSFields{i}, VariableName);
                catch
                    try
                        % if it didn't work try to set to parameter value
                        set_param(FullBlockName,TSFields{i}, TMTS.(TFields{n}).(TSFields{i}));
                    catch
                        disp(['Warning did not set variable ' TFields{n} '.' TSFields{i} ' in ' FullBlockName]);
                    end
                end
                
            end
        end
    end
end

%------------------------------------
% Wire Flow Blocks
%-------------------------------------
for n = 1: size(LinkMtrx,1)
    for i = 1: size(LinkMtrx,2)-1
        try
            OutBlock = LinkMtrx{n,i};
            InBlock = LinkMtrx{n,i+1};
            if ~isempty(InBlock) && ~isempty(OutBlock)
                [OutPortName, OutPortNumber] = GetOutPortStationName(Element, OutBlock,InBlock);
                [InPortName, InPortNumber] = GetInPortStationName(Element, InBlock,OutBlock);
                if isempty(OutPortName) || isempty(InPortName)
                elseif str2double(OutPortNumber) >0 && str2double(InPortNumber) > 0
                    add_line(Element.General.ModelName,strcat(OutBlock,'/',OutPortNumber), strcat(InBlock,'/',InPortNumber),'autorouting','on');
                end
            end
        catch ME
            disp(strcat('Warning: Wiring from "',OutBlock,'/',OutPortNumber,'" to "',InBlock,'/',InPortNumber,'" not created.'));
        end
    end
end
disp('completed Wiring Flow Blocks ')
assignin('base',TMTS.INFO.WorkSpaceName, TMTS);
disp([TMTS.INFO.WorkSpaceName ' updated to include Input Variables. It will need to be saved to keep these updates for future uses.']);
disp('Model Generation complete ')

disp(['This model is meant to be a first step in the conversion process.'...
'Many NPSS parameter and block performance assumptions were made.'...
'These assumptions may not be appropriate for all applications.'...
' Additionally, all custom blocks were assumed to be simple pass throughs. These blocks will need to be handled manually.'])

%-------------------------------------------------------------------------
%   Model Generation Completed
%   Begin defintiion of sub-functions.
%-------------------------------------------------------------------------


function ElementNames = GetElementName(MWS,Type)
try
    idx = find(strcmp(MWS.INFO.ELEMENT(2,:),Type));
    % Determine name at place
    ElementNames = MWS.INFO.ELEMENT(1,idx);
catch
    ElementNames = [];
end

function ElementType = GetElementType(MWS,ElementName)
try
    idx = find(strcmp(MWS.INFO.ELEMENT(1,:),ElementName));
    % Determine type of element
    ElementType = MWS.INFO.ELEMENT{2,idx};
catch
    ElementType = [];
end

function StationOutputBlockName = GetOutputElementName(MWS,StationInputBlockOutput)
try
    idx = find(strcmp(MWS.INFO.STATIONS(1,:),StationInputBlockOutput));
    % Determine name of output block at station
    LongName = MWS.INFO.STATIONS{3,idx};
    % remove Fl input information from name
    Temp = strsplit(LongName,'.');
    StationOutputBlockName = Temp{1};
catch
    StationOutputBlockName = [];
end

function [Element, UpdatedLocation] = LocateBlock(Element, BlkName, CurrentLocation, NextBlock, LastBlock)
% place new block in updated location and set input output number for
% wiring

% get file delimiter
FOp = Element.General.FOp;


% Gather block specific information
Element.(BlkName).BlockTypeGood = true;
Element.(BlkName).Xblock = 0;
Element.(BlkName).Yblock = 0;
Element.(BlkName).SimulinkBlock = false;
Element.(BlkName).LinkSetupEnable = true;
BlockFile = 'TMATS_Turbo';
load_system(BlockFile);


switch Element.(BlkName).Type   %NPSS Type
    case 'Ambient'
        Element.(BlkName).TMATS_Type = 'Ambient';
        CurPos = get_param([BlockFile FOp Element.(BlkName).TMATS_Type],'Position');
        Element.(BlkName).Xblock = CurPos(3) -CurPos(1);
        Element.(BlkName).Yblock = CurPos(4) - CurPos(2);
        Element.(BlkName).Outport1 = NextBlock;
        
    case 'Bleed'
        Element.(BlkName).TMATS_Type = 'Bleed';
        CurPos = get_param([BlockFile FOp Element.(BlkName).TMATS_Type],'Position');
        Element.(BlkName).Xblock = CurPos(3) -CurPos(1);
        Element.(BlkName).Yblock = CurPos(4) - CurPos(2);
        Element.(BlkName).Inport2 = LastBlock;
        Element.(BlkName).Outport2 = NextBlock;
        Element.(BlkName).BleedOutPort = 1;
        Element.(BlkName).BleedInPort = 1;
        
        
    case 'Burner'
        Element.(BlkName).TMATS_Type = 'Burner';
        CurPos = get_param([BlockFile FOp Element.(BlkName).TMATS_Type],'Position');
        Element.(BlkName).Xblock = CurPos(3) -CurPos(1);
        Element.(BlkName).Yblock = CurPos(4) - CurPos(2);
        Element.(BlkName).Inport2 = LastBlock;
        Element.(BlkName).Outport1 = NextBlock;
        
    case 'Compressor'
        Element.(BlkName).TMATS_Type = 'Compressor';
        CurPos = get_param([BlockFile FOp Element.(BlkName).TMATS_Type],'Position');
        Element.(BlkName).Xblock = CurPos(3) - CurPos(1);
        Element.(BlkName).Yblock = CurPos(4) - CurPos(2);
        Element.(BlkName).Inport1 = LastBlock;
        Element.(BlkName).Outport3 = NextBlock;
        Element.(BlkName).BleedOutPort = 2;
        Element.(BlkName).PortNmech = 3;
        Element.(BlkName).PortTrq = 5;
        Element.(BlkName).ShaftInPort = 3;
        
    case 'Duct'
        Element.(BlkName).TMATS_Type = 'Duct';
        CurPos = get_param([BlockFile FOp Element.(BlkName).TMATS_Type],'Position');
        Element.(BlkName).Xblock = CurPos(3) - CurPos(1);
        Element.(BlkName).Yblock = CurPos(4) - CurPos(2);
        Element.(BlkName).Inport1 = LastBlock;
        Element.(BlkName).Outport1 = NextBlock;
        Element.(BlkName).LinkSetupEnable = false;
        
    case 'Inlet'
        Element.(BlkName).TMATS_Type = 'Inlet';
        CurPos = get_param([BlockFile FOp Element.(BlkName).TMATS_Type],'Position');
        Element.(BlkName).Xblock = CurPos(3) - CurPos(1);
        Element.(BlkName).Yblock = CurPos(4) - CurPos(2);
        Element.(BlkName).Inport1 = LastBlock;
        Element.(BlkName).Outport1 = NextBlock;
        
    case 'Nozzle'
        Element.(BlkName).TMATS_Type = 'Nozzle';
        CurPos = get_param([BlockFile FOp Element.(BlkName).TMATS_Type],'Position');
        Element.(BlkName).Xblock = CurPos(3) - CurPos(1);
        Element.(BlkName).Yblock = CurPos(4) - CurPos(2);
        Element.(BlkName).Inport1 = LastBlock;
        
    case 'Mixer'
        Element.(BlkName).TMATS_Type = 'Mixer';
        CurPos = get_param([BlockFile FOp Element.(BlkName).TMATS_Type],'Position');
        Element.(BlkName).Xblock = CurPos(3) - CurPos(1);
        Element.(BlkName).Yblock = CurPos(4) - CurPos(2);
        Element.(BlkName).Inport2 = LastBlock;
        Element.(BlkName).Outport1 = NextBlock; 
        
    case 'Shaft'
        Element.(BlkName).TMATS_Type = 'Shaft';
        CurPos = get_param([BlockFile FOp Element.(BlkName).TMATS_Type],'Position');
        Element.(BlkName).Xblock = CurPos(3) - CurPos(1);
        Element.(BlkName).Yblock = CurPos(4) - CurPos(2);
        Element.(BlkName).Inport1 = LastBlock;
        Element.(BlkName).Outport2 = NextBlock;
        
    case 'Splitter'
        Element.(BlkName).TMATS_Type = 'Splitter';
        CurPos = get_param([BlockFile FOp Element.(BlkName).TMATS_Type],'Position');
        Element.(BlkName).Xblock = CurPos(3) - CurPos(1);
        Element.(BlkName).Yblock = CurPos(4) - CurPos(2);
        Element.(BlkName).Inport1 = LastBlock;
        Element.(BlkName).Outport2 = NextBlock;
        
        
    case 'Turbine'
        Element.(BlkName).TMATS_Type = 'Turbine_NPSS';
        CurPos = get_param([BlockFile FOp Element.(BlkName).TMATS_Type],'Position');
        Element.(BlkName).Xblock = CurPos(3) - CurPos(1);
        Element.(BlkName).Yblock = CurPos(4) - CurPos(2);
        Element.(BlkName).Inport2 = LastBlock;
        Element.(BlkName).Outport1 = NextBlock;
        Element.(BlkName).ShaftInPort = 4;
        
    otherwise
        % Use generic subsystem
        close_system(BlockFile);
        BlockFile = 'Simulink/Commonly Used Blocks';
        Element.(BlkName).TMATS_Type = 'Subsystem';
        Element.(BlkName).Xblock = 100;
        Element.(BlkName).Yblock = 42;
        Element.(BlkName).Inport1 = LastBlock;
        Element.(BlkName).Outport1 = NextBlock;
        %disp('could not find block')
        Element.(BlkName).BlockTypeGood = true;
        Element.(BlkName).SimulinkBlock = true;
        Element.(BlkName).LinkSetupEnable = false;
end

if ~Element.(BlkName).SimulinkBlock
    close_system(BlockFile);
end
Element.(BlkName).LibName = BlockFile;
BlockLocation = [CurrentLocation(1), CurrentLocation(2), (CurrentLocation(1)+Element.(BlkName).Xblock), (CurrentLocation(2)+Element.(BlkName).Yblock)];
Element.(BlkName).Location = BlockLocation;
UpdatedLocation = [BlockLocation(3) CurrentLocation(2) BlockLocation(3) CurrentLocation(4)] + [500 0 500 0];



function placeBlock(Element, BlkName)

FOp = Element.General.FOp;
ModelName = Element.(BlkName).FullName;
BlockLocation = Element.(BlkName).Location + [0 0 Element.(BlkName).Xblock Element.(BlkName).Yblock];

if Element.(BlkName).BlockTypeGood
    if exist(Element.(BlkName).LibName,'file') == 4 || Element.(BlkName).SimulinkBlock == true
        load_system(Element.(BlkName).LibName);
        try
            %place block
            add_block([Element.(BlkName).LibName FOp Element.(BlkName).TMATS_Type],ModelName,'MakeNameUnique','off','Position',BlockLocation);
            disp([BlkName ' created'])
        catch ME
            disp(strcat('Warning: Block "',ModelName,'" has not been created'))
            Element.(BlkName).LinkSetupEnable = false;
        end
        close_system(Element.(BlkName).LibName);
    end
end

function MuxPosition = placeMux(SystemName,FOp, NumInputs, RefBlock, MuxName, RefPos)
MuxSize     = [0, -(NumInputs)*30, 5, 0];
MuxPosition = [(RefPos(1)-50) RefPos(2) (RefPos(1)-50) RefPos(2)] + MuxSize;
FullMuxName = [SystemName FOp MuxName];
try
    add_block('Simulink/Commonly Used Blocks/Mux',FullMuxName ,'MakeNameUnique','off', 'Position', MuxPosition,'Inputs',num2str(NumInputs));
    add_line(SystemName, strcat(MuxName,'/1'),strcat(RefBlock,'/1'),'autorouting','on');
catch ME
    disp(strcat('Warning: Block or wiring for "',MuxName,'" not created.'));
end

function DemuxPosition = placeDemux(SystemName,FOp, NumInputs, RefBlock, MuxName, RefPos, OutputsStr)
MuxSize     = [0, 0, 5, (NumInputs)*30];
DemuxPosition = [(RefPos(3)+50) RefPos(2) (RefPos(3)+50) RefPos(2)] + MuxSize;
FullMuxName = [SystemName FOp MuxName];
try
    add_block('Simulink/Commonly Used Blocks/Demux',FullMuxName ,'MakeNameUnique','off', 'Position', DemuxPosition,'Outputs',OutputsStr);
    add_line(SystemName, strcat(RefBlock,'/1'), strcat(MuxName,'/1'),'autorouting','on');
catch ME
    disp(strcat('Warning: Block or wiring for "',MuxName,'" not created.'));
end

function placeFromTag(SystemName, FOp, LocFrac, BasePosition, FromTagName, FillName, DestName, PortNumber, color)
pcy = BasePosition(2)+(BasePosition(4)-BasePosition(2))*LocFrac;
pcx = BasePosition(1);
BPos = [(pcx - 175) (pcy-20) (pcx-15) (pcy-8)];
try
    add_block('simulink/Signal Routing/From',strcat(SystemName,FOp, FromTagName), 'MakeNameUnique','off','ShowName','off','GotoTag', FillName, 'Position',BPos, 'Foregroundcolor',color);
    %wire Tag
    add_line(SystemName,strcat(FromTagName,'/1'),strcat(DestName,'/',num2str(PortNumber)),'autorouting','on');
catch ME
    disp(strcat('Warning: Block or wiring for "',FromTagName,'" not created.'))
end

function placeGoToTag(SystemName,FOp, LocFrac, BasePosition, GoToTagName, GoToTagFillName, OriginName, PortNumber, color)
% Use Base position to determine tag position
pcy = BasePosition(2)+(BasePosition(4)-BasePosition(2))*LocFrac;
pcx = BasePosition(3);
% Set block position
BPos = [(pcx + 15) (pcy-6) (pcx+175) (pcy+6)];
FullTagName = [SystemName FOp GoToTagName];
try
    add_block('simulink/Signal Routing/Goto',FullTagName, 'MakeNameUnique','off','ShowName','off','GotoTag', GoToTagFillName, 'Position',BPos, 'Foregroundcolor', color);
    %wire Tag
    add_line(SystemName, strcat(OriginName,'/',num2str(PortNumber)),strcat(GoToTagName,'/1'),'autorouting','on');
catch ME
    disp(strcat('Warning: Block or wiring for "',GoToTagName,'" not created.'));
end

function placeConstant(SystemName, FOp, BasePosition, BFill, ConstantName, DestName, InportNumber)
pcy = BasePosition(2)+(BasePosition(4)-BasePosition(2));
pcx = BasePosition(1);
BPos = [(pcx - 175) (pcy-20) (pcx-15) (pcy-8)];
try
    add_block('simulink/Commonly Used Blocks/Constant',strcat(SystemName,FOp, ConstantName), 'MakeNameUnique','off','ShowName','off','Value', BFill, 'Position',BPos, 'Foregroundcolor','black');
    %wire Tag
    add_line(SystemName,strcat(ConstantName,'/1'),strcat(DestName,'/',num2str(InportNumber)),'autorouting','on');
catch ME
    disp(strcat('Warning: Block or wiring for "',ConstantName,'" not created.'))
end

function placeTerminator(SystemName, FOp, LocFrac, BasePosition, TermName, BaseName, bnum)
% Use Base position to determine tag position
pcy = BasePosition(2)+(BasePosition(4)-BasePosition(2))*LocFrac;
pcx = BasePosition(3);
% Set block position
BPos = [(pcx + 15) (pcy-6) (pcx+27) (pcy+6)];
FullTagName = [SystemName FOp TermName];
try
    add_block('simulink/Commonly Used Blocks/Terminator',FullTagName, 'MakeNameUnique','off','ShowName','off', 'Position',BPos, 'Foregroundcolor', 'black');
    %wire Tag
    add_line(SystemName, strcat(BaseName,'/',num2str(bnum)),strcat(TermName,'/1'),'autorouting','on');
catch ME
    disp(strcat('Warning: Block or wiring for "',TermName,'" not created.'));
end


function MKSorted = runLinker(Element, BlkName)
% run T-MATS linker tool
MKSorted = [];
if Element.(BlkName).LinkSetupEnable
    MKSorted = TMATS.Link_Setup('SystemName',Element.General.ModelName,'BlockName', BlkName,'BldIn',Element.(BlkName).BldIn,'FbldOut',Element.(BlkName).BldOut,'MKSortTypeEn');
end

function BlockLocation = GetBlockLocation(BlockName,LinkMtrx,LocationMtrx)
% find block within LinkMtrx (there should be only one occurance)
try
    [idi,idj] = find(strcmp(LinkMtrx,BlockName));
    % Lookup Location based on LinkMtrx position
    BlockLocation = LocationMtrx{idi,idj};
catch
    BlockLocation = [0 0 0 0];
end

function Element = PopElementGen(Element, Name, MWS)
% Populate Element structure with General variables
% This creates the Element fields Type, FullName, and FbldNum
if isempty(Name)
    Element = Element;
else
    Element.(Name).Type = GetElementType(MWS,Name);
    Element.(Name).FullName = [Element.General.ModelName Element.General.FOp Name];
    % Determine number of bleeds coming from the bleed port.
    BldIn = 0;
    BldOut = 0;
    if isfield(MWS,Name)
        if isfield(MWS.(Name),'BleedInNames')
            BldIn = length(MWS.(Name).BleedInNames);
        end
        if isfield(MWS.(Name),'BleedOutNames')
            BldOut = length(MWS.(Name).BleedOutNames);
        end
    end
    Element.(Name).BldIn = BldIn;
    Element.(Name).BldOut = BldOut;
end

function [OutPortName,OutPortNumber] = GetOutPortStationName(Element, OutBlock,InBlock)
% get name of outport that connect to described block
nameFound = false;
iteration = 0;
OutPortName = [];
OutPortNumber = [];
while nameFound == false && iteration < 50
    iteration = iteration + 1;
    portnumber = num2str(iteration);
    TestName = ['Outport' portnumber];
    if isfield(Element.(OutBlock),TestName)
        if strcmp(Element.(OutBlock).(TestName), InBlock)
            OutPortName = TestName;
            nameFound = true;
            OutPortNumber = portnumber;
        end
    end
    
end

function [InPortName,InPortNumber] = GetInPortStationName(Element, InBlock,OutBlock)
% get name of inport that connect to described block
nameFound = false;
iteration = 0;
InPortName = [];
InPortNumber = [];
while nameFound == false && iteration < 50
    iteration = iteration + 1;
    portnumber = num2str(iteration);
    TestName = ['Inport' portnumber];
    if isfield(Element.(InBlock),TestName)
        if strcmp(Element.(InBlock).(TestName), OutBlock)
            InPortName = TestName;
            nameFound = true;
            InPortNumber = portnumber;
        end
    end
    
end

function MKSortVecs = UpdateMKSortVector(MKSortVecs,MKSorted)
if ~isempty(MKSorted)
    fields = fieldnames(MKSorted);
    % Gather Sorted Maker block names for later placing
    for fn = 1:numel(fields)
        if ~isempty(MKSorted.(fields{fn}))
            if ~isfield(MKSortVecs, fields{fn})
                % initialize field
                MKSortVecs.(fields{fn}) = [];
            end
            MKSortVecs.(fields{fn}) =[MKSortVecs.(fields{fn}) MKSorted.(fields{fn})];
            
        end
    end
end

function NPSSVariableName = Tag2Const(Element, BlockName ,TMATSVariableName)
VN = [];
if isfield(Element.(BlockName) ,'type')
    BlockType = Element.(BlockName).type;
    
    switch BlockType
        case 'Compressor'
            VN = 'RlineDes_M';
        case {'Turbine' 'Turbine_NPSS'}
            VN = 'PRdes';
        case 'Splitter'
            VN = 'BPR';
        case 'Ambient'
            VN = 'Wdes';
        otherwise
    end
end
NPSSVariableName = VN;


