
function  [MWS, ATOutput] = TMATS_Auto_Test(MWS, Name)
%		TMATS -- TMATS_Auto_Test.m
% *************************************************************************
% written by Jeffryes Chapman 
% NASA Glenn Research Center, Cleveland, OH
% Dec 18th, 2012
% *************************************************************************

clear ATOutput;

MWS.top_level = pwd;
MWS.Auto.NameFile = Name;
MWS.Auto.BedName = strcat(Name, '_TestBed.slx');
MWS.Auto.SetupName = strcat('Tsetup_', Name);
MWS.Auto.TVName = strcat('TVec_', Name);
MWS.Auto.OuputFile = strcat('TB_', Name, '_Output');

cd(strcat(MWS.top_level , '\TestBeds\', Name))

% Setup paths
MWS.Testname = MWS.Auto.NameFile;
MWS.Test_top_level = pwd;
PathSetup(MWS);
assignin('base','MWS',MWS);




%Setup Cosntant Variables
MWS.TV.TV = 0;
assignin('base','MWS',MWS);
[MWS] =  evalin('base', strcat(MWS.Auto.SetupName,'(MWS)'));
assignin('base','MWS',MWS);

%Setup Test Vector
[MWS] =  evalin('base', strcat(MWS.Auto.TVName,'(MWS)'));
assignin('base','MWS',MWS);

a = 1;
LengthTV = length(MWS.TV.TV(1,:));
for i = 1:LengthTV
    for ii = 1:length(MWS.TV.VarNames)-1
        MWS.Auto.VarName = strcat('MWS.', MWS.TV.VarNames{1},'.' ,MWS.TV.VarNames{ii+1});
        eval([MWS.Auto.VarName ' = ' num2str(MWS.TV.TV(ii,i)) ';']) ;
    end
    assignin('base','MWS',MWS);
    
    %Run Simulation
    TB_Output = sim(MWS.Auto.BedName,'SrcWorkspace','current');
%     TB_Output = sim(MWS.Auto.BedName,'SrcWorkspace','current','SimulationMode','rapid');
%     TB_Output = sim(MWS.Auto.BedName,'SrcWorkspace','current');
    %Gather Outputs
    %                                         if(nargout == 0)
    %                                             assignin('base','TB_Output',TB_Output);
    %                                         end
    %                         Temp.Output(a) = evalin('base','TB_Output;');
    SimData = TB_Output.get('TB_Output');
    % stored data will take the form of an nxmxa matrix with: rows
    % n = time, colums m = output data, and  dimention a = additional runs
    % output data names may be viewd by looking at the .slx file.
    ATOutput(:,:,a) = SimData(:,:);
    clear TB_Output
    a = a+1;
    
end
% Set this variable to 1 if you wish to take data for each component
ComponentTakeData = 0;

if (ComponentTakeData == 1)
cd Output_Data
Out{1} = ATOutput;
TV{1} = MWS.TV;
Temp.name = strcat(MWS.Auto.NameFile ,'_',datestr(now,30), '.mat');
save(Temp.name,'Out', 'TV')
cd(MWS.Test_top_level)
end

%Set MWS to Workspace
assignin('base','MWS',MWS);

%remove paths
PathRemove(MWS);
cd(MWS.top_level)


function PathSetup(MWS)
eval(['addpath ',MWS.Test_top_level,'/Setup'])
eval(['addpath ',MWS.Test_top_level,'/TestVector'])

function PathRemove(MWS)

if(ispc == 1)   % if using Windows OS then the path is marked by \
    pathOperator = '\';
else            % if using Mac or Unix then path is marked by /
    pathOperator = '/';
end
eval(['rmpath ',MWS(1).Test_top_level,pathOperator,'Setup'])
eval(['rmpath ',MWS(1).Test_top_level,pathOperator,'TestVector'])


