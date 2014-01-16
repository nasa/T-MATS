function Uninstall_TMATS()

error = 0;
TMATSRemovemsg = 'Remove T-MATS matlab toolbox? Note: Un-Installation will remove MATLAB paths.';
POp = filesep;

switch questdlg(TMATSRemovemsg, 'T-MATS Library', 'Yes', 'No', 'No');
    
    % check if T-MATS_Library is in the path
    
    case 'Yes',
        p = path;                               % current path
        CurrDir = pwd;                          % current directory
        Pth{1} = strcat(pwd,POp,'TMATS_Library ');    % define new path
        Pth{2} = strcat(pwd,POp,'TMATS_Library',POp,'MEX');    % define new path
        Pth{3} = strcat(pwd,POp,'TMATS_Library',POp,'TMATS_Support');    % define new path
        Pth{4} = strcat(pwd,POp,'TMATS_Tools');    % define new path
        
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
        
        if error ==0;
            disp('T-MATS path removal complete.');
            disp('Note: changes made to the startup.m file will need to be removed manually.. see User Manual for details');
        end
    case 'No',
        disp('T-MATS removal aborted.');
end