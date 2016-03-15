function PlotCMap(NcVec, WcArray, PRArray, EffArray, varargin)
% Plot Compressor Map
% TMATS.PlotCMap(NcVec, WcArray, PRArray, EffArray) - plot default compressor map
% TMATS.PlotCMap(_,Name,Value,...,Value) - plot compressor map with alternate formating
% Name              Definition
% 'linesize'    -   Define linesize (Default=1), (_,'linesize',Num)
% 'defsurge'    -   Define surge line, (_,'defsurge',Wc_SM,PR_SM)
% 'efflines'    -   Define number of efficiency lines (Default=10), (_,'efflines',Num)
% 'slformat'    -   Define speed line marker value format (Default='%2.2f'), (_,'slformat',Str)
% 'offset'      -   Define speed line marker location offset (Default=0.02), (_,'offset',Num)
% 'scalar'      -   Define compressor map values updated with scalars, (_,'scalar', s_Wc, s_PR, s_Eff)
% 'mapname'     -   Define the plot title (Default='Compressor Map'), (_,'mapname',Str)
% 'plotlim'     -   Define the plot sizing limit by fraction of total size (Default=0.1), (_,'plotlim',Num)
% 'NcR'         -   Remove Nc lines from plot , (_,'NcR')
% 'RlineR'      -   Remove Rlines from plot , (_,'RlineR')
% 'EffR'        -   Remove Efficiency from plot , (_,'EffR')

%% TMATS.PlotCMaps.m
% Jeffyres Chapman based on work by Jeffrey Csank and George Thomas
% NASA Glenn Research Center
% Jan 12, 2016
%
% This file plots compressor maps
%
%--------------------------------------------------------------------------

%rline/speed line size
linesize=0.4;

%set speed line text offset
offset = 0.02;

% set speed line text format
SLformat = '%2.2f';

%surge line size
surgesize=2.5;

%manage extra matlab plotting space
plim = 0.1;

%number of efficiency lines
efflines=10;

%define stall line
defSline = 0;

%scalar definition
scalar = 0;

% Default plots
PlotRline = 1;
PlotNc = 1;
PlotEff = 1;

%map name
MapName = ['Compressor' ' Map'];

n = 1;


while n <= length(varargin)
    switch varargin{n}
        case 'RlineR'
            % Remove Rline from plot
            PlotRline = 0;
        case 'NcR'
            % Remove Nc lines from plot
            PlotNc = 0;
        case 'EffR'
            % Remove Efficiency from plot
            PlotEff = 0;
            
        case 'linesize'
            % define linesize
            n = n+1;
            linesize = varargin{n};
            
        case 'defsurge'
            defSline = 1;
            % define surge line
            n = n+1;
            SM_Wc       = varargin{n};
            n = n+1;
            PRstallMap  = varargin{n};
            
        case 'efflines'
            % define number of efficiency lines
            n = n+1;
            efflines = varargin{n};
            
        case 'slformat'
            % define speed line txt format
            n = n+1;
            SLformat= varargin{n};
            
        case 'offset'
            % define speed offset fraction
            n = n+1;
            offset= varargin{n};
            
        case 'scalar'
            % 9 inputs - plot compressor with scalars and no stall line
            scalar = 1;
            n = n+1;
            s_Wc        = varargin{n};
            n = n+1;
            s_PR        = varargin{n};
            n = n+1;
            s_Eff       = varargin{n};
            
        case 'mapname'
            % define speed offset fraction
            n = n+1;
            MapName = varargin{n};
            
        case 'plotlim'
            % define speed offset fraction
            n = n+1;
            plim = varargin{n};
            
        otherwise
            disp(['PlotCMap is skipping unknown input:' varargin(n)])
    end
    n = n+1;
end


if scalar == 1
    WcArray =  WcArray*s_Wc;
    EffArray = EffArray*s_Eff;
    PRArray = (PRArray-1)*s_PR + 1;
    if defSline == 1
        SM_Wc = SM_Wc * s_Wc;
        PRstallMap = (PRstallMap - 1)*s_PR + 1;
    end
end

set(gca,'fontsize',12);
hold off;
% create Rline
if PlotRline ==1
    plot(WcArray,PRArray,'c-','Linewidth',linesize);
    hold on;
end
%create Speed lines
if PlotNc ==1
    plot(WcArray',PRArray','b-','Linewidth',linesize);
    hold on;
end
%plot efficiency contour
if PlotEff ==1
    contour(WcArray,PRArray,EffArray,efflines,'ShowText','on','Linewidth',linesize*2,'Linecolor','m','LineStyle','--');
    hold on;
end
% if stall line if defined plot stall line
if (defSline == 1)
    plot(SM_Wc,PRstallMap,'r-','Linewidth',surgesize);
end


% develop speed marker offset
Wcmax = max(max(WcArray));
PRmax = max(max(PRArray));
Wcmin = min(min(WcArray));
PRmin = min(min(PRArray));

if PlotNc ==1
Wcadj = (Wcmax - Wcmin) * offset;
PRadj = (PRmax - PRmin) * offset;
    % create speed markers
    for j=1:length(WcArray(:,1))
        xmax=WcArray(j,1)- Wcadj;
        ymax=PRArray(j,1)+ PRadj;
        strmin = num2str(NcVec(j),SLformat);
        text(xmax,ymax,strmin,'HorizontalAlignment','left','fontsize',8);
    end
end

%label plot
grid on;
xlabel('Wc, lbm/s');
ylabel('PR');
title(MapName);

Wcadjp = (Wcmax - Wcmin) * plim;
PRadjp = (PRmax - PRmin) * plim;

ylim([PRmin-PRadjp PRmax+PRadjp])
xlim([Wcmin-Wcadjp Wcmax+Wcadjp])

