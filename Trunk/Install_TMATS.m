function Install_TMATS()
% This subroutine installs T-MATS

% TMATSver and date must remain in the listed form for the ver command to work
% properly.
TMATSver = '1.2.1';
TMATSdate = 'Mar-16-2018';

error = 0;
TMATSInstallmsg = 'Install T-MATS matlab toolbox? Note: Installation will add MATLAB paths and generate MEX files.';
POp = filesep;

switch questdlg(TMATSInstallmsg, 'T-MATS Library','Temporary Install', 'Install', 'Cancel', 'Cancel');
    case 'Temporary Install'
        InstallType = 'Install';
        PermInstall = 0;
    case 'Install'
        InstallType = 'Install';
        PermInstall = 1;
    case 'Cancel'
        InstallType = 'Cancel';
        PermInstall = 0;
end
% check if T-MATS_Library is in the path
switch InstallType
    case 'Install',
        p = path;                               % current path
        [CurrDir,temp1,temp2] = fileparts(mfilename('fullpath')); % Base install directory
        % define new paths
        Pth{1} = strcat(pwd,POp,'TMATS_Library');
        Pth{2} = strcat(pwd,POp,'TMATS_Library',POp,'MEX');
        Pth{3} = strcat(pwd,POp,'TMATS_Library',POp,'TMATS_Support');
        Pth{4} = strcat(pwd,POp,'TMATS_Tools');
        Pth{5} = strcat(pwd,POp,'TMATS_Library',POp,'MATLAB_Scripts');
        Pth{6} = strcat(pwd,POp,'TMATS_Library',POp,'MATLAB_Scripts',POp,'Cantera_Enabled');
        
        perm = zeros( 1 , length(Pth));         % allocate memory for perm
        for i = 1: length(Pth)
            
            perm(i) = isempty(strfind(pathdef,strcat( Pth{i} , ';' )));  % determine if path is already defined
            
            if perm(i)                               % for each path if it is not defined,  define it
                path(pathdef);
                addpath(Pth{i});
                if PermInstall == 1;
                    SP = savepath;
                    if SP==0
                        disp(sprintf(' %s has been saved to the permanent Path structure.',Pth{i}));
                    else
                        error = 1;
                        disp(sprintf('Error: %s has not been added to the permanent Path structure. To use TMATS blocks Install_TMATS.m will need to be run each time MATLAB is opened.',Pth{i}));
                    end
                else
                    disp(sprintf(' %s has been added to the Path structure.',Pth{i}));
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
            
            cd(strcat('TMATS_Library',POp,'MEX'));
            disp('Compiling T-MATS, please wait...');
            make_file_TMATS;
            
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
        if error ==0 && PermInstall == 1
            cd( 'TMATS_Library')
            disp('Building Contents.m file');
            fid = fopen('Contents.m','w');
            fprintf(fid,'%% T-MATS.\n');
            fprintf(fid,['%% Version',' ',TMATSver,' ',TMATSdate,'\n%%\n%% Files\n']);
            fprintf(fid,'%%   Install_TMATS   - This subroutine installs T-MATS\n');
            fprintf(fid,'%%   Uninstall_TMATS - This subroutine uninstalls T-MATS\n');
            fclose(fid);
            eval(['cd ' CurrDir]);
        end
        
                
	disp('Refreshing Simulink Browser...');
	LB = LibraryBrowser.LibraryBrowser2;
        LB.refresh;
        
        disp('T-MATS Simulink library installation complete.');
        disp('For T-MATS Tools installation instructions, please see the users guide.');

    case 'Cancel',
        disp('T-MATS installation aborted.');
end

