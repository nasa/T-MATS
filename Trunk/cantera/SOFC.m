

function sofc(block)

%%
setup(block);

%endfunction

function setup(block)

block.NumDialogPrms = 22;

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

block.InputPort(2).Dimensions  = 25;
block.InputPort(2).DatatypeID  = 0;  % double
block.InputPort(2).Complexity  = 'Real';
block.InputPort(2).DirectFeedthrough = true;
block.InputPort(2).SamplingMode = 'Sample';

% Override output port properties
block.OutputPort(1).Dimensions  = 25;
block.OutputPort(1).DatatypeID  = 0; % double
block.OutputPort(1).Complexity  = 'Real';
block.OutputPort(1).SamplingMode = 'Sample';

block.OutputPort(2).Dimensions  = 25;
block.OutputPort(2).DatatypeID  = 0; % double
block.OutputPort(2).Complexity  = 'Real';
block.OutputPort(2).SamplingMode = 'Sample';

block.OutputPort(3).Dimensions  = 25;
block.OutputPort(3).DatatypeID  = 0; % double
block.OutputPort(3).Complexity  = 'Real';
block.OutputPort(3).SamplingMode = 'Sample';
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

flowindicies;

AirStoich=block.DialogPrm(1).Data;
Alpha =block.DialogPrm(2).Data;
cellActiveArea =block.DialogPrm(3).Data;
effIntercon = block.DialogPrm(4).Data;
EidealStd= block.DialogPrm(5).Data; 
h_aOut = block.DialogPrm(6).Data;
iDens = block.DialogPrm(7).Data;
iDensExchange= block.DialogPrm(8).Data;
iDensInternal = block.DialogPrm(9).Data;
iDensLimit= block.DialogPrm(10).Data;
LHVfuel = block.DialogPrm(11).Data;
numCells= block.DialogPrm(12).Data;
pAnode= block.DialogPrm(13).Data;
pCathode = block.DialogPrm(14).Data;
pctH2_lost= block.DialogPrm(15).Data;
pctH2recyc_a = block.DialogPrm(16).Data;
pctH2util = block.DialogPrm(17).Data;
pctLossIntercon= block.DialogPrm(18).Data;
pctO2_lost = block.DialogPrm(19).Data;
resist= block.DialogPrm(20).Data;
Temp = block.DialogPrm(21).Data;
Z = block.DialogPrm(22).Data;


% -switchMode "CELL" designs the number of cells required by the fuel cell stack to meet
%   a specified power and efficiency, and outputs this number of cells for use in the
%   OFFDESIGN mode.
% -switchMode "PWR" designs the SOFC stack based on input number of cells and outputs
%   power based on this design and a specified efficiency.
% -switchMode "EFF" designs the SOFC stack based on input number of cells and outputs
%   efficiency based on this design and a specified power.


Fl_I1= block.InputPort(1).Data;
Fl_I2=block.InputPort(2).Data;
Fl_O2=block.OutputPort(1).Data;
Fl_O2=block.OutputPort(2).Data;

	TempK = (((Temp - 460.0) - 32.0) * 5/9) + 273.15;
		
%**********************
% Anode Properties
%**********************
%Calculates equilibrium properties after setting gas T = cell T to determine 1st energy transfer
    h_aIn = Fl_I1(ht) * Fl_I1(W); %btu/sec
	Fl_Anode1=set_TP( Fl_I1, Temp,pAnode);

    hA1 = Fl_Anode1(ht) * Fl_Anode1(W);  %btu/sec
    %1st heat transfer needed:
    qA1 = hA1 - h_aIn;  % btu/sec (positive is heat into anode inlet gas from cell)
		
%Gets total mass flow rate
	w_Anode1 = Fl_Anode1(W)* 453.;  % g/sec
	
%Composition in mass fractions AFTER FIRST EQUILIBRIUM
	xH2_Anode1 = getMassFraction( Fl_Anode1, 'H2' )
	xH2O_Anode1 = getMassFraction( Fl_Anode1, 'H2O' );

%Composition as mass flow (g/sec)
	wH2_Anode1 = xH2_Anode1 * w_Anode1; 
	wH2O_Anode1 = xH2O_Anode1 * w_Anode1; 
	
%Composition as molar flow rate (mol/sec)	
	M_H2_Anode1 = wH2_Anode1 / 2.0159
	M_H2O_Anode1 = wH2O_Anode1 / 18.01528;

%Calculates composition after electrochemistry...	
	M_H2_Anode2 = (1.0-pctH2util)*M_H2_Anode1;
	M_H2O_Anode2 = M_H2O_Anode1 + pctH2util*M_H2_Anode1;
	%error check: make sure H2 does not increase due to reaction
	wH2_lost = (M_H2_Anode1 - M_H2_Anode2 )* 2.0159 * 0.002205 % lb/sec
	
	if wH2_lost < 0.0
		fprintf( 'ERROR - wH2_lost is negative, = %g\n', wH2_lost );
	end
	%error check: make sure not more H2 was used than was available
	pctH2_lost = wH2_lost / (wH2_Anode1*0.002205);
	if  pctH2_lost > 1.0 
		fprintf( 'ERROR - pctH2_lost is > 100%, = %g\n', pctH2_lost  );
	end
	%error check: make sure H2O does not decrease due to reaction
	wH2O_gain = (M_H2O_Anode2 - M_H2O_Anode1)*18.0152 * 0.002205 % lb/sec
	if wH2O_gain < 0.0 
		fprintf( 'ERROR - wH2O_gain is negative, = %g\n', wH2O_gain  );
	end

%...and sets it	
    Fl_Anode2 = Fl_Anode1;	
    Fl_tempH2 = Fl_Anode1;
    Fl_tempH2(6) = 1;
    Fl_tempH2(4) = 0;
    Fl_tempH2(8) = 0;
    Fl_tempH2(7) = 0;
    Fl_tempH2(5) = 0;
    Fl_tempH2(9) = 0;
    Fl_tempH2(10) = 0;

    Fl_tempH2(W) =-wH2_lost;
	Fl_tempH2 = set_TP( Fl_tempH2, Fl_Anode1(Tt), Fl_Anode1(Pt) );
	
    Fl_tempH2O = Fl_Anode1;
    Fl_tempH2O(7) = 1;
    Fl_tempH2O(4) = 0;
    Fl_tempH2O(5) = 0;
    Fl_tempH2O(6) = 0;
    Fl_tempH2O(8) = 0;
    Fl_tempH2O(9) = 0;
    Fl_tempH2O(10) = 0;
	Fl_tempH2O(W) = wH2O_gain;
	Fl_tempH2O = set_TP( Fl_tempH2O, Fl_Anode1(Tt), Fl_Anode1(Pt) );
    hA2 = Fl_Anode2(ht)* Fl_Anode2(W)+Fl_tempH2O(ht)*Fl_tempH2O(W) + Fl_tempH2(ht)*Fl_tempH2(W);  %btu/sec
	
	Fl_O1=add( Fl_Anode2, Fl_tempH2O ); 
	Fl_O1=add( Fl_O1, Fl_tempH2 );
	
%Calculates equilibrium properties after setting gas T = cell T to determine 2nd energy transfer

	Fl_O1=set_TP( Fl_O1, Temp, pAnode );
    h_aOut = Fl_O1(ht) * Fl_O1(W);  %btu/sec
%2nd heat transfer needed
    qA2 = hA2 - h_aOut;  % btu/sec (positive is heat into air from cell)

%Find Partial Pressures for Nernst equation    
    %Composition of Anode exit gas in mass fractions
	xH2_aOut =  getMassFraction( Fl_O1, 'H2' );
	xH2O_aOut = getMassFraction( Fl_O1, 'H2O' );
	xCO2_aOut =  getMassFraction( Fl_O1, 'CO2' );
	xCO_aOut =  getMassFraction( Fl_O1, 'CO' );
	xCH4_aOut = getMassFraction( Fl_O1, 'CH4' );
	xN2_aOut =  getMassFraction( Fl_O1, 'N2' );
	xO2_aOut =  getMassFraction( Fl_O1, 'O2' );	
	xTrace_aOut = 1.0 - (xH2_aOut + xH2O_aOut + xCO2_aOut + xCO_aOut + xCH4_aOut + xN2_aOut + xO2_aOut);
	%error check: make sure constituents are what is expected
	if xTrace_aOut > 0.01 
		fprintf( 'WARNING - Trace gasses out of Anode exceeds 1%, = %g', xTrace_aOut);
	end

    %gets total mass flow rate
	w_aOut = Fl_O1(W) * 453.;  %units g/sec

	%composition as mass flow (g/sec)	
	wH2_aOut = xH2_aOut * w_aOut; 
	wH2O_aOut = xH2O_aOut * w_aOut; 
	wCO2_aOut = xCO2_aOut * w_aOut; 
	wCO_aOut = xCO_aOut * w_aOut; 
	wCH4_aOut = xCH4_aOut * w_aOut; 
	wN2_aOut = xN2_aOut * w_aOut; 
	wO2_aOut = xO2_aOut * w_aOut;
	wTrace_aOut = xTrace_aOut * w_aOut; 
	
	%composition as molar flow rate (mol/sec)	
	M_H2_aOut = wH2_aOut / 2.015;
	M_H2O_aOut = wH2O_aOut / 18;
	M_CO2_aOut = wCO2_aOut / 44.01;
	M_CO_aOut = wCO_aOut / 28.01;
	M_CH4_aOut = wCH4_aOut / 16.04;
	M_N2_aOut = wN2_aOut / 14.0067;
	M_O2_aOut = wO2_aOut / 32;
	M_Trace_aOut = wTrace_aOut / 16.04; %treats Trace gasses as having MW = CH4

	%calculates total of molar flows, for use in denominator of pp equation
	M_Total_aOut = (M_H2_aOut + M_H2O_aOut + M_CO2_aOut + M_CO_aOut + M_CH4_aOut + M_N2_aOut + M_O2_aOut + M_Trace_aOut);
	%calculates partial pressure for use in Nernst
	ppH2_aOut = M_H2_aOut / (M_Total_aOut) * (pAnode*0.06895);
	ppH2O_aOut = M_H2O_aOut / (M_Total_aOut) * (pAnode*0.06895);


%**********************
% Cathode Properties
%**********************
%Insert Josh's calculations for humid air - see pemfc.int

%calculates properties after setting gas T = cell T to determine 1st energy transfer
    h_cIn = Fl_I2(ht) * Fl_I2(W); %btu/sec

    Fl_Cathode1=set_TP( Fl_I2, Temp, pCathode );
    hC1 = Fl_Cathode1(ht) * Fl_Cathode1(W);  %btu/sec
    %1st heat transfer needed:
    qC1 = hC1 - h_cIn;  % btu/sec (positive is heat into air from cell)

%Gets total mass flow rate
	w_Cathode1 = Fl_Cathode1(W) * 453.;  % g/sec

%Composition in mass fractions AFTER FIRST EQUILIBRIUM
  
	xO2_Cathode1 = getMassFraction( Fl_Cathode1, 'O2' )
   
%Composition as mass flow (g/sec)
	wO2_Cathode1 = xO2_Cathode1 * w_Cathode1
	
%Composition as molar flow rate (mol/sec)	
	M_O2_Cathode1 = wO2_Cathode1 / 32.

%Calculates composition after electrochemistry...	
	M_O2_Cathode2 = M_O2_Cathode1 - ((M_H2_Anode1 / 2.0) * pctH2util);
	wO2_lost = (M_O2_Cathode1 - M_O2_Cathode2)*32. * 0.002205 % lb/sec
	%error check: make sure O2 does not increase in cell
	if wO2_lost < 0.0 
		fprintf(  'ERROR - wO2_lost is negative, = %g\n', wO2_lost );
	end
	%error check: make sure not more O2 was used than was available
	pctO2_lost = wO2_lost / (wO2_Cathode1*0.002205);
	if  pctO2_lost > 1.0 
		fprintf( 'ERROR - NOT ENOUGH AIR - pctO2_lost is > 100%, = %g\n', pctO2_lost*100.0');
	end
	
%...and sets it	
    Fl_Cathode2 = Fl_Cathode1;
	Fl_tempO2(8) = 1;
    Fl_tempO2(5) = 0;
    Fl_tempO2(6) = 0;
    Fl_tempO2(7) = 0;
    Fl_tempO2(5) = 0;
    Fl_tempO2(9) = 0;
    Fl_tempO2(10) = 0;
	Fl_tempO2(W) = -wO2_lost;
	Fl_tempO2= set_TP( Fl_tempO2, Fl_Cathode2(Tt), Fl_Cathode2(Pt) );
    hC2 = Fl_Cathode2(ht)* Fl_Cathode2(W)+Fl_tempO2(ht)*Fl_tempO2(W);
	
	Fl_O2=add( Fl_Cathode2, Fl_tempO2) ; 

	
    Fl_O2=set_TP( Fl_O2, Temp, pCathode );
    h_cOut = Fl_O2(ht) * Fl_O2(W);  % btu/sec
%2nd heat transfer needed
    qC2 = hC2 - h_cOut;  % btu/sec (positive is heat into air from cell)

%Find Partial Pressures for Nernst equation    
    %Composition of Cathode exit gas in mass fractions
	xN2_cOut = getMassFraction( Fl_O2, 'N2' );
	xO2_cOut = getMassFraction( Fl_O2, 'O2' );
	%error check: make sure no other constituents are in "Air" besides O2 and N2
	xTotal_cOut = xN2_cOut + xO2_cOut;
	if xTotal_cOut < 0.9800 || xTotal_cOut > 1.0100
		fprintf( 'ERROR - Mass fraction out of cathode does not equal 1.0 (+/- 0.01), =%g ', xTotal_cOut);
	end
	
    %gets total mass flow rate
	w_cOut = Fl_O2(W) * 453.;  %units g/sec

	%composition as mass flow (g/sec)	
	wN2_cOut = xN2_cOut * w_cOut; 
	wO2_cOut = xO2_cOut * w_cOut;
	
	%composition as molar flow rate (mol/sec)	
	M_N2_cOut = wN2_cOut / 28.013;
	M_O2_cOut = wO2_cOut / 32.;

	%calculates partial pressure for use in Nernst
	ppO2_cOut = M_O2_cOut / (M_O2_cOut + M_N2_cOut) * (pCathode*0.06895);
    
%*********************************    
% Voltage, Losses, and Power
%*********************************

	cellVoltIdeal = EidealStd + (8.314*TempK/(2.0*96485.0))*log(ppH2_aOut*(ppO2_cOut^0.5)/ppH2O_aOut);
	voltLossOhmic = resist * (iDens+iDensInternal);
	voltLossActiv = (8.314*TempK/(2.0*Alpha*96485.0))*log((iDens+iDensInternal) / iDensExchange);
	voltLossConc = -1.0*(8.314*TempK/(Z*96485.0))*log(1.0-((iDens+iDensInternal) / iDensLimit));
	cellVoltOper = cellVoltIdeal - voltLossOhmic - voltLossActiv - voltLossConc;
  	pwrDens = cellVoltOper * iDens / 1000.0;
  	
  	effIntercon = 1.0 - ( (pctLossIntercon / (1.0 - pctLossIntercon))*(voltLossOhmic / cellVoltIdeal) );
 	
%	pElec = (2.0*cellVoltOper*F)*(M_H2_Anode1 - M_H2_Anode2) / 1000;
	
	
%**************************************
% Design - Off-Design Calculations
%**************************************


	  totActiveArea = cellActiveArea * numCells;
	  pElec = effIntercon * (totActiveArea*pwrDens / 1000.0);


	
%********************	
% Flows for Solver
%********************

%Hydrogen - used in solver
	M_H2use = pElec*1000.0 / (2.0*cellVoltOper*96485.0); % mol/sec
	M_H2required = M_H2use / pctH2util;  %used in solver
	wH2use = M_H2use*2.015;  % g/sec
	
	%eventually can put this part in external H2 separator element:	
	M_H2extra = M_H2use*(1.0 - pctH2recyc_a)*((1.0 / pctH2util) - 1.0);% mol/sec
	M_H2feed = M_H2use + M_H2extra;
	wH2feed = M_H2feed*2.015;  % g/sec  - used in solver
%	vH2feed = wH2feed / rhoH2 * 60;  % l/min

%Cathode oxygen flow determined by H2 flow and stoichiometry
	M_O2required = (M_H2_Anode1 / 2) * AirStoich;  %used in solver


%*******************************	
% Efficiency and Heat
%*******************************
    totActiveArea = cellActiveArea * numCells;
	pElec = effIntercon * (totActiveArea*pwrDens / 1000.0)
    cellActiveArea
    numCells
	eff = pElec / (LHVfuel*(wH2_Anode1/1000.0));
	qGen = pElec*((cellVoltIdeal / cellVoltOper) - 1.0);

%calculates net heat gain or loss in cell
%positive: the cell gains heat from flows; negative: it loses heat to flows
%can use this in solver to find Temp where qNet = 0 (for design mode)
	qNet = qGen - 1.05435*((h_aOut - h_aIn) - (h_cOut - h_cIn));  %kW
	
	if iDens + iDensInternal >= iDensLimit 
		printf( 'ERROR - Current Density exceeds upper limit, = ' + tostr( iDens) +'\n' );
	end
	if iDens + iDensInternal <= iDensExchange 
		printf( 'ERROR - Current Density below lower limit, = ' + tostr( iDens)+'\n' );
    end
  

    
    
    block.OutputPort(1).Data = Fl_O1;
    block.OutputPort(2).Data = Fl_O2;
    
    
    block.OutputPort(3).Data(1) = pElec;
    block.OutputPort(3).Data(2) = cellVoltIdeal;
    block.OutputPort(3).Data(3) = cellVoltOper;
    block.OutputPort(3).Data(4) = effIntercon;
    block.OutputPort(3).Data(5) = qGen;
    block.OutputPort(3).Data(6) = qNet;
    block.OutputPort(3).Data(7) = pwrDens;
    block.OutputPort(3).Data(8) = wH2use;