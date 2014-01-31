function Install_TMATS()

error = 0;
TMATSInstallmsg = 'Install T-MATS matlab toolbox? Note: Installation will add MATLAB paths and generate MEX files.';
POp = filesep;

switch questdlg(TMATSInstallmsg, 'T-MATS Library', 'Yes', 'No', 'No');
    
    % check if T-MATS_Library is in the path
    
    case 'Yes',
        p = path;                               % current path
        CurrDir = pwd;                          % current directory
        
        Pth{1} = strcat(pwd,POp,'TMATS_Library');    % define new path
        Pth{2} = strcat(pwd,POp,'TMATS_Library',POp,'MEX');    % define new path
        Pth{3} = strcat(pwd,POp,'TMATS_Library',POp,'TMATS_Support');    % define new path
        Pth{4} = strcat(pwd,POp,'TMATS_Tools');    % define new path
        Pth{5} = strcat(pwd,POp,'TMATS_Library',POp,'MATLAB_Scripts');    % define new path
        
        perm = zeros( 1 , length(Pth));         % allocate memory for perm
        for i = 1: length(Pth)
            
            perm(i) = isempty(strfind(pathdef,strcat( Pth{i} , ';' )));  % determine if path is already defined
            
            if perm(i)                               % for each path if it is not defined,  define it
                path(pathdef);
                addpath(Pth{i});
                SP = savepath;
                if SP==0
                    disp(sprintf(' %s has been saved to the Path structure.',Pth{i}));
                else
                    error = 1;
                    disp(sprintf('Error: %s has not been added to the Path structure. To use TMATS blocks Install_TMATS.m will need to be run each time MATLAB is opened.',Pth{i}));
                end
            else
                disp (sprintf('%s is already defined in the path structure',Pth{i}));
            end
        end
        
        
        % return to current path.
        path(p);
        for i = 1:length(Pth)
            addpath(Pth{i});
        end
        
        try
            % component mex make file
            
            cd(strcat('TMATS_Library',POp,'MEX',POp,'C_code'));
            disp('Compiling T-MATS, please wait...');
            make_file_TMATS;
            eval(['cd ' CurrDir]);
            
            % return to the root directory
            eval(['cd ' CurrDir]);
            disp('All mex files generated');
        catch ME
            % return to the root directory
            eval(['cd ' CurrDir]);
            error = 1;
            % rethrow exception to generate error on screen
            throw(ME);
        end
        if error ==0;
            disp('T-MATS installation complete.');
        end
    case 'No',
        disp('T-MATS installation aborted.');
end

