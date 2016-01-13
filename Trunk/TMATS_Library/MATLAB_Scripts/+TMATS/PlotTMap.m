function PlotTMap(NcVec, PRVec, WcArray, EffArray, varargin)
% Plot Turbine Map
% TMATS.PlotTMap(NcVec, PRVec, WcArray, EffArray) - plot default compressor map
% TMATS.PlotTMap(_,Name,Value,...,Value) - plot compressor map with alternate formating
% Name              Definition
% 'linesize'    -   Define linesize (Default=1), (_,'linesize',Num) 
% 'effcolor'    -   Define efficiency color darkness (Default=1), (_,'effcolor',Num)
% 'slformat'    -   Define speed line marker value format (Default='%2.0f'), (_,'slformat',Str)
% 'offset'      -   Define speed line marker location offset (Default=0.02), (_,'offset',Num)
% 'scalar'      -   Define compressor map values updated with scalars, (_,'scalar', s_Wc, s_PR, s_Eff)
% 'mapname'     -   Define the plot title (Default='Turbine Map'), (_,'mapname',Str)
% 'plotlim'     -   Define the plot sizing limit by fraction of total size (Default=0.1), (_,'plotlim',Num)

%% TMATS.PlotTMaps.m
% Jeffyres Chapman based on work by Jeffrey Csank and George Thomas
% NASA Glenn Research Center
% Jan 12, 2016
%
% This file plots turbine maps
%
%--------------------------------------------------------------------------

%rline/speed line size
linesize = 1;

%set speed line text offset
offset = 0.02;

% set speed line text format
SLformat = '%2.0f';

%effdarkness
effdarkenss = 1;

%manage extra matlab plotting space
plim = 0.1;

%scalar definition
scalar = 0;

%map name
MapName = ['Turbine' ' Map'];

n = 1;


while n <= length(varargin)
    switch varargin{n}
        case 'linesize'
            % define linesize
            n = n+1;
            linesize = varargin{n};
             
        case 'effcolor'
            % define Efficiency color value
            n = n+1;
            effdarkenss= varargin{n};
            
        case 'slformat'
            % define Efficiency color value
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
            disp(['PlotTMap is skipping unknown input:' varargin(n)])
    end
    n = n+1;
end

if scalar == 1
    WcArray =  WcArray*s_Wc;
    EffArray = EffArray*s_Eff;
    PRVec = (PRVec-1)*s_PR + 1;
end

set(gca,'fontsize',12);
hold off;
% create speed lines
plot(WcArray,PRVec,'k-','Linewidth',linesize);
hold on;
%shade plot for efficiency
surf(WcArray,ones(length(NcVec),1)*PRVec,EffArray);
alpha(effdarkenss)

% develop speed marker offset
Wcmax = max(max(WcArray));
PRmax = max(PRVec);
Wcmin = min(min(WcArray));
PRmin = min(PRVec);
PRadj = (PRmax - PRmin) * offset;

% create speed markers
for j=1:length(WcArray(:,1))
    xmax=max(WcArray(j,:));
    ymax=max(PRVec) + PRadj;
    strmin = num2str(NcVec(j),SLformat);
    text(xmax,ymax,strmin,'HorizontalAlignment','left','fontsize',8);
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

