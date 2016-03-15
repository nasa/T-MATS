function SetIDesFun_TMATS(FileName, IDesVal)
%Find all libraries in the model file
try
    LibD = libinfo(FileName);
catch
    disp(['File name ' '" ' FileName ' "' ' is not valid'])
end

%Determine number of libraries
LenLib = size(LibD,1);
if LenLib > 0
    % Search libraries for blcoks with the IDesign_M parameter
    for i = 1:LenLib
        try
           set_param(LibD(i).Block, 'IDesign_M', num2str(IDesVal));
           disp(['IDesign_M in block ',LibD(i).Block,' has been updated to ', num2str(IDesVal)]) 
        catch
        end
        
        
        
    end
end

