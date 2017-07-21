function TDplot(filename, varargin)
% T-MATS Time dependent data plotter.
% TMATS.TDplot(file name) - Search diagram for flow data and performance
% maps and plot them.
% TMATS.TDplot(_,Name,Value,...,Value) - plot time dependent T-MATS data with alternate formating
% Name              Definition
% 'WR'          -   Don't plot flow station figures, (_,'WR')
% 'NR'          -   Don't plot speed figures, (_,'NR')
% 'CompR'       -   Don't plot compressor map figures, (_,'CompR')
% 'TurbR'       -   Don't plot turbine map figures, (_,'TurbR')
% 'MappntsR'    -   Don't plot data points on map figures, (_,'MappntsR')

%set preferences to dock the output plots
set_old = get(0,'DefaultFigureWindowStyle');
set(0,'DefaultFigureWindowStyle','docked');

% By defualt plot everything
PlotW = 1;
PlotN = 1;
PlotCompMap = 1;
PlotTurbMap = 1;
PlotMapPnts = 1;


% review input options
n = 1;
while n <= length(varargin)
    switch varargin{n}
        case 'WR'
            % remove flow station figures
            PlotW = 0;
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

% Determine which ToWorkspace blocks hold flow data and plot them
for i = 1:length(gb)
    %get variable from workspace
    Vnm = get_param(gb{i},'VariableName');
    Var = evalin('base',Vnm);
    
    % determine if data is in the expected format
    IsStruct = isstruct(Var);
    
    if IsStruct
        % Determine criteria for a Flow ToWorkspace Node
        WE = sum(strcmp(fieldnames(Var), 'W'));
        TtE = sum(strcmp(fieldnames(Var), 'Tt'));
        PtE = sum(strcmp(fieldnames(Var), 'Pt'));
        
        % Determine criteria for map node with shaft speed
        BlkNmE = sum(strcmp(fieldnames(Var), 'BlkNm'));
        NmechE = sum(strcmp(fieldnames(Var), 'Nmech'));
    else
        WE = 0;
        TtE = 0;
        PtE = 0;
        BlkNmE = 0;
        NmechE = 0;
    end
    
    % If flow, temperature, and pressure exist in the variable, Get and
    % Plot Flow variables
    if WE && TtE && PtE && PlotW
        
        % Get the Flow
        W = Var.W.Data;
        Pt = Var.Pt.Data;
        Tt = Var.Tt.Data;
        Time = Var.W.Time;
        
        %create new figure
        f = figure;
        set(f,'name',Vnm,'numbertitle','off');
        
        %plot the data in a tab
        subplot(3,1,1)
        plot(Time,W)
        ylabel('W, pps')
        title([Vnm,' Data'])
        subplot(3,1,2)
        plot(Time,Pt)
        ylabel('Pt, psia')
        subplot(3,1,3)
        plot(Time,Tt)
        ylabel(sprintf('Tt, %cR',char(176)))
        xlabel('Time, s');
    end
    
    
    % If Nmech and Block name exist in the variable, Get Speeds
    if NmechE && BlkNmE && PlotN
        % Get the shaft speed data
        ShftSpdNum = ShftSpdNum + 1;
        itmp = ShftSpdNum;
        N(itmp,:) = Var.Nmech.Data;
        Ntime(itmp,:) = Var.Nmech.Time;
        NBlkNm(itmp) = {char(Var.BlkNm.Data(end,:))};
    end
end % end search through To Workspace variables


% Plot Speeds
if ShftSpdNum > 0
    % initialize plotted speed vector, a speed should only be plotted
    % once
    Nplt = zeros(size(N,2),1)';
    n = 0;
    
    % sort shaft speeds to eliminate repeats
    for i = 1:ShftSpdNum
        PlotSpeed = 1;
        for ii = 1:size(Nplt,1)
            if N(i,:) == Nplt(ii,:)
                PlotSpeed = 0;
            end
        end
        if PlotSpeed
            n = n + 1;
            Nplt(n,:) = N(i,:);
            Tplt(n,:) = Ntime(i,:);
            Ntitle{n} = NBlkNm{i};
        end
    end
    
    if n > 0
        f = figure;
        set(f,'name','Speeds','numbertitle','off');
        % plot remainder of shaft speeds
        tempsize = size(Nplt,1);
        for i = 1:tempsize
            subplot(tempsize,1,i)
            plot(Tplt(i,:), Nplt(i,:));
            ylabel('Speed, rpm');
            title(Ntitle{i});
        end
        xlabel('Time, s');
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
                plot(CTWc, CTPR,'o');
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
                plot(CTWc, CTPR,'o');
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



