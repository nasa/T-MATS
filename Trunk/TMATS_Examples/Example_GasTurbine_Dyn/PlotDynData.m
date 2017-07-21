TMATS.TDplot('GasTurbine_Dyn_Template');

% ---------------------------------------------------------------
% plot remaining itemts that do not appear with the TDplot command


fontname = 'Helvetica';
set(0,'defaultaxesfontname',fontname);
set(0,'defaulttextfontname',fontname);

% save figure settings
Temp.set_old = get(0,'DefaultFigureWindowStyle');
% set figure to place new figures into the same window
set(0,'DefaultFigureWindowStyle','docked');

% plot stability margins
Temp.f = figure;
set(Temp.f,'name','StbMrgn','numbertitle','off');
plot(C_Data.SMavail.Time,C_Data.SMavail.Data )
ylabel('Stability Margin, %')
xlabel('Time, s')

% plot additional engine items,  thrust and control inputs
Temp.f = figure;
set(Temp.f,'name','Cntrl,Thrst','numbertitle','off');
subplot(3,1,1)
plot(Fnet.Time,Fnet.Data)
ylabel('Fnet, lbf')
subplot(3,1,2)
plot(Wf.Time,Wf.Data)
ylabel('Wf, pps')
subplot(3,1,3)
plot(SFC.Time,SFC.Data)
ylabel('SFC, (lbm/hr)/lbf' )
xlabel('Time, s')

%return settings
set(0,'DefaultFigureWindowStyle',Temp.set_old);

clear Temp