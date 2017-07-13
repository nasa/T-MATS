function SSplot(filename, StationVarVec, varargin)
% T-MATS steady state data plotter.
% TMATS.SSplot(filename, StationVarVec) - Search for station data and performance
% maps, then plot them.
% TMATS.SSplot(_,Name,Value,...,Value) - plot steady state T-MATS data with alternate formating
% Name              Definition
% 'TSR'          -   Don't plot T-s data, (_,'SR')
% 'SR'          -   Don't plot station data, (_,'SR')
% 'NR'          -   Don't plot speed figures, (_,'NR')
% 'CompR'       -   Don't plot compressor map figures, (_,'CompR')
% 'TurbR'       -   Don't plot turbine map figures, (_,'TurbR')
% 'MappntsR'    -   Don't plot data points on map figures, (_,'MappntsR')

%set preferences to dock the output plots
set_old = get(0,'DefaultFigureWindowStyle');
set(0,'DefaultFigureWindowStyle','docked');

% By defualt plot everything
PlotTS = 1;
PlotS = 1;
PlotN = 1;
PlotCompMap = 1;
PlotTurbMap = 1;
PlotMapPnts = 1;


% review input options
n = 1;
while n <= length(varargin)
    switch varargin{n}
        case 'TSR'
            % remove station figure
            PlotTS = 0;
        case 'SR'
            % remove station figure
            PlotS = 0;
        case 'NR'
            % remove speed figure
            PlotN = 0;
        case 'CompR'
            % remove compressor map figures
            PlotCompMap = 0;
            
        case 'TurbR'
            %remove turbine map figures
            PlotTurbMap = 0;
            
        case 'MappntsR'
            % Remove data points on map figures
            PlotMapPnts = 0;
            
        otherwise
            disp(['TDplot is skipping unknown input:' varargin(n)])
    end
    n = n+1;
end




% find all models being referenced in the model
[ref, blocks] = find_mdlrefs(filename);

%initialize find numbers
CompNum = 0;
TurbNum = 0;
ShftSpdNum = 0;
LibD = {};
gb = {};

% perform a search for each T-MATS data block within each model
for n = 1:length(ref)
    load_system(ref(n));
    %Get library names of all blocks within simulink diagram
    LibD = [libinfo(ref(n));LibD];
    % get block names of all ToWorkspace blocks
    gb = [find_system(ref(n),'BlockType','ToWorkspace');gb];
end

%search diagram for specific libraries that coincide with T-MATS
%turbines or compressors (anything that needs a map)
for i = 1:length(LibD)
    LibraryName = LibD(i).Library;
    switch LibraryName
        case 'Lib_Turbo_Compressor_TMATS'
            if PlotCompMap
                CompNum = CompNum +1;
                CompBlocks{CompNum} = LibD(i).Block;
            end
        case 'Lib_Turbo_TurbineNPSS_TMATS'
            if PlotTurbMap
                TurbNum = TurbNum +1;
                TurbBlocks{TurbNum} = LibD(i).Block;
            end
        case 'Lib_Turbo_Turbine_TMATS'
            if PlotTurbMap
                TurbNum = TurbNum +1;
                TurbBlocks{TurbNum} = LibD(i).Block;
            end
    end
end % End search through libraries


% initialize variables
StnNum = 0;
% Search workspace for specified station variables
for i = 1: length(StationVarVec)
    Var = evalin('base',StationVarVec{i});
    
    % determine if data is in the expected format
    IsStruct = isstruct(Var);
    
    if IsStruct
        % Determine criteria for a Flow ToWorkspace Node
        TtE = sum(strcmp(fieldnames(Var), 'Tt'));
        PtE = sum(strcmp(fieldnames(Var), 'Pt'));
        FARE = sum(strcmp(fieldnames(Var), 'FAR'));
    else
        TtE = 0;
        PtE = 0;
        
    end
    
    % If flow, temperature, and pressure exist in the variable, Get and
    % Plot Flow variables
    if TtE && PtE && PlotS && FARE
        StnNum = StnNum +1;
        % Get the Station data
        Pt(StnNum) = Var.Pt.Data(end);
        Tt(StnNum) = Var.Tt.Data(end);
        FAR(StnNum) = Var.FAR.Data(end);
        Nm(StnNum) = {StationVarVec{i}};
    end
end

% plot Station Data
if StnNum > 0
    %create new figure
    f = figure;
    set(f,'name','Station','numbertitle','off');
    
    %plot the data in a tab
    subplot(2,1,1)
    plot(Tt,'-o')
    ylabel('Temperatures [R]')
    ax = gca;
    set(ax,'xtick',[]);
    subplot(2,1,2)
    plot(Pt,'-o')
    ylabel('Pressures [psia]')
    ax = gca;
    set(ax,'xtick',1:length(Nm));
    set(ax,'xticklabel',Nm);
    ax.XTickLabelRotation=45;
end

% plot Station Data
if StnNum > 0 && PlotTS == 1
    %create new figure
    f = figure;
    set(f,'name','T-s diagram','numbertitle','off');
    
    %Generate entropy values
    for i = 1:length(Tt)
        s(i) = TMATS.pt2s(Pt(i),Tt(i), FAR(i));
    end
    %plot the data in a tab
    plot(s,Tt,'-bo')
    hold on
    plot([s(1),s(end)],[Tt(1),Tt(end)],'b')
    ylabel('Temperatures [R]');
    xlabel('Enthalpy [BTU/(lbm*R)]');
    
end



% Iterate through ToWorkspace blocks to find Speeds.
for i = 1:length(gb)
    % get variable
    Vnm = get_param(gb{i},'VariableName');
    Var = evalin('base',Vnm);
    
    % determine if data is in the expected format
    IsStruct = isstruct(Var);
    
    if IsStruct
        % Determine criteria for map node with shaft speed
        BlkNmE = sum(strcmp(fieldnames(Var), 'BlkNm'));
        NmechE = sum(strcmp(fieldnames(Var), 'Nmech'));
    else
        BlkNmE = 0;
        NmechE = 0;
    end
    
    % If Nmech and Block name exist in the variable, Get Speeds
    if NmechE && BlkNmE && PlotN
        % Get the shaft speed data
        ShftSpdNum = ShftSpdNum + 1;
        itmp = ShftSpdNum;
        N(itmp) = Var.Nmech.Data(end);
        SimpleBlkNm = get_param(char(Var.BlkNm.Data(end,:)), 'name');
        NBlkNm(itmp) = {SimpleBlkNm};
    end
end % end search through To Workspace variables


% Plot Speeds
if ShftSpdNum > 0
    % initialize plotted speed vector, a speed should only be plotted
    % once
    Nplt = 0;
    n = 0;
    
    % sort shaft speeds to eliminate repeats
    for i = 1:ShftSpdNum
        PlotSpeed = 1;
        for ii = 1:length(Nplt)
            if N(i) == Nplt(ii)
                PlotSpeed = 0;
            end
        end
        if PlotSpeed
            n = n + 1;
            Nplt(n) = N(i);
            NpltNm(n) = NBlkNm(i);
        end
    end
    
    if n > 0
        f = figure;
        set(f,'name','Speeds','numbertitle','off');
        % plot remainder of shaft speeds
        bar(Nplt);
        ylabel('Speed [rpm]');
        ax = gca;
        set(ax,'xtick',1:length(NpltNm));
        set(ax,'xticklabel',NpltNm);
        ax.XTickLabelRotation=45;
        axis([0.5,length(Nplt)+0.5,0,max(N)*1.1])
        for ii = 1: length(Nplt)
            text(ii,Nplt(ii)*1.05,[num2str(Nplt(ii)), ' [rpm]'],'HorizontalAlignment','left','fontsize',13)
        end
    end
end


%Create comporessor maps
if(CompNum > 0)
    for i = 1:CompNum
        %Get Block name
        BN = CompBlocks{i};
        %Get Parameters for plotting the map
        Nc = evalin('base',get_param(BN,'Y_C_Map_NcVec_M'));
        Wc = evalin('base',get_param(BN,'T_C_Map_WcArray_M'));
        PR = evalin('base',get_param(BN,'T_C_Map_PRArray_M'));
        Eff = evalin('base',get_param(BN,'T_C_Map_EffArray_M'));
        CompNm = get_param(BN,'Name');
        %create new figure
        f = figure;
        set(f,'name',CompNm,'numbertitle','off');
        % plot map
        TMATS.PlotCMap(Nc,Wc,PR,Eff,'mapname',['Compressor Map', ' : ' ,BN]);
        
        if PlotMapPnts
            % get Map to Workspace information
            [CTWc,CTPR,DF] = getMapTWD(gb,BN);
            % If map workspace info was found plot it
            if DF == 1
                hold on
                plot(CTWc(end), CTPR(end),'bo','MarkerSize',7,'LineWidth',2);
                hold off
            end
        end
    end
end

%Create turbine maps
if(TurbNum > 0)
    for i = 1:TurbNum
        %Get Block name
        BN = TurbBlocks{i};
        %Get Parameters for plotting the map
        Nc = evalin('base',get_param(BN,'Y_T_Map_NcVec_M'));
        Wc = evalin('base',get_param(BN,'T_T_Map_WcArray_M'));
        PR = evalin('base',get_param(BN,'X_T_Map_PRVec_M'));
        Eff = evalin('base',get_param(BN,'T_T_Map_EffArray_M'));
        TurbNm = get_param(BN,'Name');
        %create new figure
        f = figure;
        set(f,'name',TurbNm,'numbertitle','off');
        % plot map
        TMATS.PlotTMap(Nc,PR,Wc,Eff,'mapname',['Turbine Map',' : ',BN]);
        
        if PlotMapPnts
            % get Map to Workspace information
            [CTWc,CTPR,DF] = getMapTWD(gb,BN);
            % If map workspace info was found plot it
            if DF == 1
                hold on
                plot(CTWc(end), CTPR(end),'bo','MarkerSize',7,'LineWidth',2);
                hold off
            end
        end
    end
end
try
    bdclose('all')
catch
end

%return settings
set(0,'DefaultFigureWindowStyle',set_old);


function [CTWc, CTPR, datafound] = getMapTWD(gb, FeedBlock)
% search ToWorkspace blocks for flow or map data outputs
datafound = 0;
CTWc = 0;
CTPR = 0;
for i = 1:length(gb)
    %get variable from workspace
    Var = evalin('base',get_param(gb{i},'VariableName'));
    
    % determine if data is in the expected format
    IsStruct = isstruct(Var);
    
    if IsStruct
        % Determine if data set is a MapToWorkspace Node
        WcMapE = sum(strcmp(fieldnames(Var), 'WcMap'));
        PRMapE = sum(strcmp(fieldnames(Var), 'PRMap'));
        BlkNmE = sum(strcmp(fieldnames(Var), 'BlkNm'));
    else
        WcMapE = 0;
        PRMapE = 0;
        BlkNmE = 0;
    end
    
    if WcMapE && PRMapE && BlkNmE
        % Determine if the data was produced by the correct block
        CTInpBlk = char(Var.BlkNm.Data);
        if strcmp(FeedBlock, CTInpBlk(end,:))
            CTWc = Var.WcMap.Data;
            CTPR = Var.PRMap.Data;
            datafound = 1;
        end
    end
end



