function CreateMaps_NPSS_Rline
% This function calls a map generation algorithm that can be used to
% generate maps formated for T-MATS.

%specify input file
Files{1} = 'LPT.map'; %Fan
% Files{2} = 'FanHubBooster.map'; %Booster
% Files{3} = 'ncp03.map'; %HPC
% Files{4} = 'ncp04.map'; %HPT
% Files{5} = 'ncp05.map'; %LPT

%specify output variable prefix
Fnames{1} = 'MWS.LPT.';
% Fnames{2} = 'MWS.LPCcore.';
% Fnames{3} = 'MWS.HPC.';
% Fnames{4} = 'MWS.HPT.';
% Fnames{5} = 'MWS.LPT.';

%specify component type (used in R_line_NPSS2Mtlb to determine correct
%names
Type{1} = 'turbine';
% Type{2} = 'compressor';
% Type{3} = 'compressor';
% Type{4} = 'Turbine';
% Type{5} = 'Turbine';

%specify output file name
F1{1} = 'setup_LPT.m';
% F1{2} = 'setup_LPC.m';
% F1{3} = 'setup_HPC.m';
% F1{4} = 'setup_HPT.m';
% F1{5} = 'setup_LPT.m';

for n = 1:length(Files)
    R_line_NPSS2Mtlb(Files{n}, F1{n}, Fnames{n},Type{n})
end
end

function R_line_NPSS2Mtlb(Infile, Outputfile, Name,Type)
% This function gnerates a T-MATS formated map from a map in a typical NPSS
% format.  In almost all cases the format below will need to be updated per
% the form of the current map.

% Currently This file is setup for two formats ("Type"), turbine and compressor.
% Additional formats may be created per users discression.

FileId = fopen(Infile);

i = 1;
line = {};
Read = 0;

Rflag = 0;
Ncflag = 0;

NcIter = 1;
WcIter = 1;


switch lower(Type)
    % determine names of varibles to be brought out variable 1 is the name
    % in the file and variable 2 is the name it will be in the output
    case 'compressor'
        % Determiners
        DetNm{1,1} = ' alpha ';
        DetNm{1,2} = 'alphaMap';
        % Vector Names
        VecNm{1,1} = 'NcMap';
        VecNm{1,2} = 'NcVec';
        VecNm{2,1} = 'betaMap';
        VecNm{2,2} = 'RVec';
        % Array Names
        ArrNm{1,1} = 'WcMap';
        ArrNm{1,2} = 'WcArray';
        ArrNm{2,1} = 'EfMap';
        ArrNm{2,2} = 'EffArray';
        ArrNm{3,1} = 'PRmap';
        ArrNm{3,2} = 'PRArray';
    case 'turbine'
        % Determiners
        DetNm{1,1} = 'parmGeomDes';
        DetNm{1,2} = 'PgeomMap';
        % Vector Names
        VecNm{1,1} = 'NcMap';
        VecNm{1,2} = 'NcVec';
        VecNm{2,1} = 'PRmap';
        VecNm{2,2} = 'PRVec';
        % Array Names
        ArrNm{1,1} = 'effMap';
        ArrNm{1,2} = 'effArray';
        ArrNm{2,1} = 'WcMap';
        ArrNm{2,2} = 'WcArray';
end

NumOfArrays = size(ArrNm,1);
Aflag = zeros(NumOfArrays,1);
AIter = ones(NumOfArrays,1);

while (~feof(FileId))
    %setup flags
    ignore_text = 1;
    
    % Get Data,  read lines and determine if the line should be used for map
    % generation
    templine(i) = {fgetl(FileId)};
    linei = num2str(cell2mat(templine(i)));
    
    
    if sum(strfind(linei,'//')) > 0; % discard comments
        ignore_text = 1;
    elseif sum(strfind(linei,DetNm{1,1})) > 0; % execute alpha = command
        eval(linei);
        ignore_text = 0;
    elseif sum(strfind(linei,DetNm{1,2}))> 0 && sum(strfind(linei,'='))> 0 && sum(strfind(linei,'{'))> 0; % AlphaMap = number
        linei = strrep(linei, '{', ';');
        eval(linei);
        Geometry = eval(strcat(DetNm{1,2},'==',DetNm{1,1}));
        if Geometry
            Read = 1;
        else
            Read = 0;
            NcIter = 1;
            Ncflag = 1;
        end
    end
    
    % Create Maps , augment lines to be used in map creation into a form
    % that fits T-MATS format.
    if (Read == 1 && (sum(strfind(linei,'interp'))+ sum(strfind(linei,'extrap'))) == 0)
        if sum(strfind(linei,VecNm{1,1}))> 0 && sum(strfind(linei,'='))> 0; % NcorrMap Entry
            linei = strrep(linei, '{', ';');
            eval(linei);
            NcVec(NcIter) = eval(VecNm{1,1});
            NcIter = NcIter + 1;
        elseif ((sum(strfind(linei,VecNm{2,1}))> 0 && sum(strfind(linei,'='))> 0 && sum(strfind(linei,'*'))== 0) || Rflag == 1)&& Rflag < 2; % RlineMap Entry
            ignore_text = 0;
            Rflag = 1;
            linei = strrep(linei, '{', '[');
            linei = strrep(linei, ',', ' ');
            linei = strrep(linei, VecNm{2,1}, strcat(Name,VecNm{2,2},'(',num2str(WcIter),',:)'));
            
            if sum(strfind(linei,'}'))> 0
                Rflag = 2;
                linei = strrep(linei, '}', '];');
            else
                linei = strcat(linei,'...');
            end
        end
        for n = 1: NumOfArrays
            if (sum(strfind(linei,ArrNm{n,1}))> 0 && sum(strfind(linei,'='))> 0 && sum(strfind(linei,'*'))== 0) || Aflag(n) == 1;
                ignore_text = 0;
                Aflag(n) = 1;
                linei = strrep(linei, '{', '[');
                linei = strrep(linei, ',', ' ');
                linei = strrep(linei, ArrNm{n,1}, strcat(Name,ArrNm{n,2},'(',num2str(AIter(n)),',:)'));
                
                if sum(strfind(linei,'}'))> 0
                    Aflag(n) = 2;
                    AIter(n) = AIter(n)+1;
                    linei = strrep(linei, '}', '];');
                else
                    linei = strcat(linei,'...');
                end
            end
        end
    end
    
    if ignore_text == 0
        line(i) = cellstr(strcat('\r ',linei));
        i = i+1;
    end
end
if Ncflag > 0
    NcStng = strcat({'\r '},Name, VecNm{1,2},' = [');
    for ii = 1:length(NcVec)
        NcStng = strcat(NcStng,{' '},num2str(NcVec(ii)));
    end
    line(i) = cellstr(strcat(NcStng,'];'));
end

fclose(FileId);

FileId = fopen(Outputfile, 'w'); 
for n = 1:length(line)
    fprintf(FileId, cell2mat(line(n)));
end
fclose(FileId);
end
