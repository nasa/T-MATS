function TMATS_comparison(varargin)
%RESULT = CMAPSS2_COMPARISON
%  Compares two simulation output files created with CMAPSS2_AUTOTEST (a
%  baseline and a new simulation).  The two results files must have been
%  created with the same autotest test script.  If the tests are
%  equivalent, then the results for each output variable in both the
%  steady-state and transient tests will be compared to the baseline.  Any
%  deviation greater than allowed will be flagged.
%    DEVIATION = (new - baseline) / baseline
%  The function returns a structure, RESULT with the following fields:
%    RESULT.PASS: 1 if no deviations, 0 if any deviations
%    RESULT.NUMSSDEVIATIONS: number of deviations in state-space tests
%    RESULT.NUMTRANSDEVIATIONS: number of deviations in transient tests

%**************************************************************************
% LOAD THE USER PREFERENCE FILE                                           *
%**************************************************************************
prefs.outputPath = strcat(pwd,'\Output_Data');
comp.outputPath = prefs.outputPath;


%**************************************************************************
% GET INFORMATION FROM USER IF NECESSARY                                  *
%**************************************************************************
if(isfield(comp,'baselineResults')~=1)  %determine which output file to load as baseline
    [comp.baselineResultsFile,path] = uigetfile_echeck('*.mat','Select Baseline Output File:',prefs.outputPath);
    comp.baselineResults = fullfile(path,comp.baselineResultsFile);
end
if(isfield(comp,'newtestResults')~=1)  %determine which output file to compare baseline to
    [comp.newtestResultsFile,path] = uigetfile_echeck('*.mat','Select Output File for Comparison:',prefs.outputPath);
    comp.newtestResults = fullfile(path,comp.newtestResultsFile);
end
if(isfield(comp,'outputPath')~=1)  %determine where to save the output file
    comp.outputPath = uigetdir_echeck(prefs.outputPath,'Where to save the output file:');
end

%**************************************************************************
% LOAD THE RESULTS FILES                                                  *
%**************************************************************************
try     %load output file #1 - baseline test results
    output1 = load(comp.baselineResults);
catch ME
    error('Unable to load baseline file from: %s\n%s',comp.baselineResults,ME.message);
end


try     %load output file #2 - new test results
    output2 = load(comp.newtestResults);
catch ME
    error('Unable to load new comparison file from: %s\n%s',comp.newtestResults,ME.message);
end

%ensure that they used the same testscript
% assert(isequal(output1.test, output2.test)==1,'The two output files were not executed with the same testscript');
a = 1;
TST = [{'Test Name'}; {'Test Error %'}; {'Total Test Number'}];
for n = 1: length(output1.Out)
    clear CMP
    
    if size(output1.Out{n}) ~= size(output2.Out{n})
        disp('The two output files are not the same sizes');
    else
        CMP.MtxEr = abs(100*(output1.Out{n} - output2.Out{n})./ max(abs(output1.Out{n}),max(abs(output2.Out{n}),0.000001)));
    end

%initialize array
CMP.SumSizeMx = sum(size(CMP.MtxEr) > 1);
if CMP.SumSizeMx == 1
    CMP.MxEr_tst_3x = CMP.MtxEr;
elseif CMP.SumSizeMx == 2
    CMP.MxEr_tst_3x = max(CMP.MtxEr);
elseif CMP.SumSizeMx > 2
CMP.MxEr_tst_3x = max(max(CMP.MtxEr));
else
    disp('Error in calculating Max Matrix Errors')
    CMP.MxEr_tst_3x = 0;
end

CMP.MtxErRows = length(CMP.MxEr_tst_3x(1,1,:));
for i = 1: CMP.MtxErRows
    %reduce MxEr_tst to a ax1 from a 1x1xa
    MxEr_tst(a) = max(CMP.MxEr_tst_3x(1,1,i));
    
    TST{1,a+1} = [output1.TV{n}.VarNames{1},'_' ,num2str(i)];
    TST{2,a+1} = num2str(MxEr_tst(a));
    TST{3,a+1} = num2str(a);
    a = a+1;
end

end

assignin('base','TST',TST);

plot(MxEr_tst);

