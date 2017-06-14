function Uninstall_TMATS()
% This subroutine uninstalls T-MATS
error = 0;
TMATSRemovemsg = 'Remove T-MATS matlab toolbox? Note: Un-Installation will remove MATLAB paths.';
POp = filesep;

switch questdlg(TMATSRemovemsg, 'T-MATS Library', 'Yes', 'No', 'No')
    
    % check if T-MATS_Library is in the path
    
    case 'Yes'
        p = path;                               % current path
        [CurrDir,temp1,temp2] = fileparts(mfilename('fullpath')); % Base install directory
        % define new paths
        Pth{1} = strcat(pwd,POp,'TMATS_Library ');
        Pth{2} = strcat(pwd,POp,'TMATS_Library',POp,'MEX');
        Pth{3} = strcat(pwd,POp,'TMATS_Library',POp,'TMATS_Support');
        Pth{4} = strcat(pwd,POp,'TMATS_Tools');
        Pth{5} = strcat(pwd,POp,'TMATS_Library',POp,'MATLAB_Scripts');
        Pth{6} = strcat(pwd,POp,'TMATS_Library',POp,'MATLAB_Scripts',POp,'Cantera_Enabled');
        
        perm = zeros( 1 , length(Pth));  % allocate memory for perm
        
        for i = 1:length(Pth)
            
            perm(i) = ~isempty(strfind(pathdef,strcat(Pth{i},';')));  % determine if path is already defined
            
            
            if perm(i)                               % for each path if defined,  remove it from pathdef
                path(pathdef);
                rmpath(Pth{i});
                SP = savepath;
                if SP==0
                    disp(sprintf(' %s has been removed from the Path structure.',Pth{i}));
                else
                    error = 1;
                    disp(sprintf('Error: %s Has not been removed from the Path structure',Pth{i}));
                end
            else
                disp (sprintf('%s does not exist in the path structure',Pth{i}));
            end
            
        end
        
        
        path(p)
        for i = 1:length(Pth)
            perm(i) = ~isempty(strfind(path,Pth{i}));  % determine if path is already defined
            
            if perm(i)
                eval(['rmpath ',Pth{i}])
            end
        end
        
        if error ==0
            FileExist = exist(strcat(pwd,POp,'TMATS_Library ',POp,'Contents.m'), 'file');
            if FileExist == 2
                cd( 'TMATS_Library')
                disp('Removing Contents.m');
                fid = fopen('Contents.m');
                fclose(fid);
                delete('Contents.m');
                eval(['cd ' CurrDir]);
            end
            
            disp('T-MATS path removal complete.');
            disp('Note: changes made to the startup.m file will need to be removed manually.. see User Manual for details');
        end
    case 'No'
        disp('T-MATS removal aborted.');
end