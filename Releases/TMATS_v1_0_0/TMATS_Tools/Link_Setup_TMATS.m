function Link_Setup_TMATS(callbackInfo)

%		T-MATS -- Link_Setup_TMATS.m
% *************************************************************************
% written by Jeffryes Chapman
% NASA Glenn Research Center, Cleveland, OH
% April 19th, 2013
%
% Wire_Setup.m sets up blocks in TMATS to be quickly connected by adding
% goto tags, From blocks, and Terminators. Wire_setup will not change
% any interal block perameters and any block created by this script can be
% deleted as needed. This script also adds Link blocks that are not
% connected to anything.  These Link blocks are for convinience. This
% portion of the code can be enabled or disabled with the bit
% MakeLinkerTags [ 1 - enable,  0 - Disable].
% *************************************************************************

% get_param(gcb,'ObjectParameters')
MakeLinkerTags = 1;
temp.a = get_param(gcb,'Name');
temp.block = get_param(gcb,'ReferenceBlock');
temp.pos = get_param(gcb,'Position');
switch temp.block
    case('Lib_Turbo_Ambient_TMATS/Ambient')
        try   % Ambient add A_Data terminator block
            temp.pcx = temp.pos(2)+(temp.pos(4)-temp.pos(2))*5/6;
            temp.pcy = temp.pos(3);
            temp.pc = [(temp.pcy + 15) (temp.pcx - 5) (temp.pcy + 25) (temp.pcx + 5)];
            temp.bname = strcat( gcs,'/',temp.a,'_term1');
            add_block('simulink/Commonly Used Blocks/Terminator',temp.bname,'MakeNameUnique','off','ShowName','off','Position',temp.pc)
            add_line(gcs,strcat(temp.a,'/3'),strcat(temp.a,'_term1/1'),'autorouting','on')
        catch
        end
        try   % Ambient add Pamb GoTo tag
            temp.pcx = temp.pos(2)+(temp.pos(4)-temp.pos(2))*3/6;
            temp.pcy = temp.pos(3);
            temp.pc = [(temp.pcy + 15) (temp.pcx - 6) (temp.pcy + 175) (temp.pcx + 6)];
            temp.b = strcat('Pamb');
            temp.bname = strcat( gcs,'/','Pamb1');
            add_block('simulink/Signal Routing/Goto',temp.bname,'MakeNameUnique','off','ShowName','off','GotoTag', temp.b, 'Position',temp.pc)
            add_line(gcs,strcat(temp.a,'/2'),strcat('Pamb1/1'),'autorouting','on')
        catch
        end
        try   % Ambient add W From tag
            temp.pcx = temp.pos(2)+(temp.pos(4)-temp.pos(2))*1/8;
            temp.pcy = temp.pos(1);
            temp.pc = [(temp.pcy - 175) (temp.pcx - 6) (temp.pcy - 15) (temp.pcx + 6)];
            temp.b = strcat('W');
            temp.bname = strcat( gcs,'/','W1');
            add_block('simulink/Signal Routing/From',temp.bname,'MakeNameUnique','off','ShowName','off','GotoTag', temp.b, 'Position',temp.pc, 'Foregroundcolor','darkGreen')
            add_line(gcs,strcat('W1/1'), strcat(temp.a,'/1'),'autorouting','on')
        catch
        end
        try   % Ambient add Alt From tag
            temp.pcx = temp.pos(2)+(temp.pos(4)-temp.pos(2))*3/8;
            temp.pcy = temp.pos(1);
            temp.pc = [(temp.pcy - 175) (temp.pcx - 6) (temp.pcy - 15) (temp.pcx + 6)];
            temp.b = strcat('Altitude');
            temp.bname = strcat( gcs,'/',temp.a,'_Alt1');
            add_block('simulink/Signal Routing/From',temp.bname,'MakeNameUnique','off','ShowName','off','GotoTag', temp.b, 'Position',temp.pc)
            add_line(gcs,strcat(temp.a,'_Alt1/1'), strcat(temp.a,'/2'),'autorouting','on')
        catch
        end
        try   % Ambient add dTamb From tag
            temp.pcx = temp.pos(2)+(temp.pos(4)-temp.pos(2))*5/8;
            temp.pcy = temp.pos(1);
            temp.pc = [(temp.pcy - 175) (temp.pcx - 6) (temp.pcy - 15) (temp.pcx + 6)];
            temp.b = strcat('DeltaTamb');
            temp.bname = strcat( gcs,'/',temp.a,'_dTamb1');
            add_block('simulink/Signal Routing/From',temp.bname,'MakeNameUnique','off','ShowName','off','GotoTag', temp.b, 'Position',temp.pc)
            add_line(gcs,strcat(temp.a,'_dTamb1/1'), strcat(temp.a,'/3'),'autorouting','on')
        catch
        end
        try   % Ambient add MN From tag
            temp.pcx = temp.pos(2)+(temp.pos(4)-temp.pos(2))*7/8;
            temp.pcy = temp.pos(1);
            temp.pc = [(temp.pcy - 175) (temp.pcx - 6) (temp.pcy - 15) (temp.pcx + 6)];
            temp.b = strcat('MachNumb');
            temp.bname = strcat( gcs,'/',temp.a,'_MN1');
            add_block('simulink/Signal Routing/From',temp.bname,'MakeNameUnique','off','ShowName','off','GotoTag', temp.b, 'Position',temp.pc)
            add_line(gcs,strcat(temp.a,'_MN1/1'), strcat(temp.a,'/4'),'autorouting','on')
        catch
        end
        if (MakeLinkerTags > 0.5)
            try   % Ambient add W Goto tag to link to
                temp.pcx = temp.pos(2)+(temp.pos(4)-temp.pos(2))*1/8;
                temp.pcy = temp.pos(1);
                temp.pc = [(temp.pcy - 360) (temp.pcx - 6) (temp.pcy - 200) (temp.pcx + 6)];
                temp.b = strcat('W');
                temp.bname = strcat( gcs,'/','WI');
                add_block('simulink/Signal Routing/Goto',temp.bname,'MakeNameUnique','off','ShowName','off','GotoTag', temp.b, 'Position',temp.pc, 'Foregroundcolor','darkGreen')
            catch
            end
            try   % Ambient add Alt Goto tag to link to
                temp.pcx = temp.pos(2)+(temp.pos(4)-temp.pos(2))*3/8;
                temp.pcy = temp.pos(1);
                temp.pc = [(temp.pcy - 360) (temp.pcx - 6) (temp.pcy - 200) (temp.pcx + 6)];
                temp.b = strcat('Altitude');
                temp.bname = strcat( gcs,'/',temp.a,'_AltE');
                add_block('simulink/Signal Routing/Goto',temp.bname,'MakeNameUnique','off','ShowName','off','GotoTag', temp.b, 'Position',temp.pc)
            catch
            end
            try   % Ambient add dTamb Goto tag to link to
                temp.pcx = temp.pos(2)+(temp.pos(4)-temp.pos(2))*5/8;
                temp.pcy = temp.pos(1);
                temp.pc = [(temp.pcy - 360) (temp.pcx - 6) (temp.pcy - 200) (temp.pcx + 6)];
                temp.b = strcat('DeltaTamb');
                temp.bname = strcat( gcs,'/',temp.a,'_dTambE');
                add_block('simulink/Signal Routing/Goto',temp.bname,'MakeNameUnique','off','ShowName','off','GotoTag', temp.b, 'Position',temp.pc)
            catch
            end
            try   % Ambient add MN Goto tag to link to
                temp.pcx = temp.pos(2)+(temp.pos(4)-temp.pos(2))*7/8;
                temp.pcy = temp.pos(1);
                temp.pc = [(temp.pcy - 360) (temp.pcx - 6) (temp.pcy - 200) (temp.pcx + 6)];
                temp.b = strcat('MachNumb');
                temp.bname = strcat( gcs,'/',temp.a,'_MNE');
                add_block('simulink/Signal Routing/Goto',temp.bname,'MakeNameUnique','off','ShowName','off','GotoTag', temp.b, 'Position',temp.pc)
            catch
            end
        end
    case('Lib_Turbo_Burner_TMATS/Burner')
        try   % Burner add B_Data terminator block
            temp.pcx = temp.pos(2)+(temp.pos(4)-temp.pos(2))*3/4;
            temp.pcy = temp.pos(3);
            temp.pc = [(temp.pcy + 15) (temp.pcx - 5) (temp.pcy + 25) (temp.pcx + 5)];
            temp.bname = strcat( gcs,'/',temp.a,'_term1');
            add_block('simulink/Commonly Used Blocks/Terminator',temp.bname,'MakeNameUnique','off','ShowName','off','Position',temp.pc)
            add_line(gcs,strcat(temp.a,'/2'),strcat(temp.a,'_term1/1'),'autorouting','on')
        catch
        end
        try   % Burner add FuelFlow From tag
            temp.pcx = temp.pos(2)+(temp.pos(4)-temp.pos(2))*1/4;
            temp.pcy = temp.pos(1);
            temp.pc = [(temp.pcy - 175) (temp.pcx - 6) (temp.pcy - 15) (temp.pcx + 6)];
            temp.b = strcat( temp.a , '_Wf');
            temp.bname = strcat( gcs,'/',temp.a,'_Wf1');
            add_block('simulink/Signal Routing/From',temp.bname,'MakeNameUnique','off','ShowName','off','GotoTag', temp.b, 'Position',temp.pc, 'Foregroundcolor','orange')
            add_line(gcs,strcat(temp.a,'_Wf1/1'), strcat(temp.a,'/1'),'autorouting','on')
        catch
        end
        if (MakeLinkerTags > 0.5)
            try   % Burner add Wf Goto tag to link to
                temp.pcx = temp.pos(2)+(temp.pos(4)-temp.pos(2))*1/4;
                temp.pcy = temp.pos(1);
                temp.pc = [(temp.pcy - 360) (temp.pcx - 6) (temp.pcy - 200) (temp.pcx + 6)];
                temp.b = strcat( temp.a , '_Wf');
                temp.bname = strcat( gcs,'/',temp.a,'_WfC');
                add_block('simulink/Signal Routing/Goto',temp.bname,'MakeNameUnique','off','ShowName','off','GotoTag', temp.b, 'Position',temp.pc, 'Foregroundcolor','orange')
            catch
            end
        end
        
    case('Lib_Turbo_Turbine_TMATS/Turbine')
        try   % Turbine add T_Data terminator block
            temp.pcx = temp.pos(2)+(temp.pos(4)-temp.pos(2))*7/8;
            temp.pcy = temp.pos(3);
            temp.pc = [(temp.pcy + 15) (temp.pcx - 5) (temp.pcy + 25) (temp.pcx + 5)];
            temp.bname = strcat( gcs,'/',temp.a,'_term1');
            add_block('simulink/Commonly Used Blocks/Terminator',temp.bname,'MakeNameUnique','off','ShowName','off','Position',temp.pc)
            add_line(gcs,strcat(temp.a,'/4'),strcat(temp.a,'_term1/1'),'autorouting','on')
        catch
        end
        try   % Turbine add TrqOut GoTo tag
            temp.pcx = temp.pos(2)+(temp.pos(4)-temp.pos(2))*5/8;
            temp.pcy = temp.pos(3);
            temp.pc = [(temp.pcy + 15) (temp.pcx - 6) (temp.pcy + 175) (temp.pcx + 6)];
            temp.b = strcat( temp.a , '_TrqOut');
            temp.bname = strcat( gcs,'/',temp.a,'_TrqOut1');
            add_block('simulink/Signal Routing/Goto',temp.bname,'MakeNameUnique','off','ShowName','off','GotoTag', temp.b, 'Position',temp.pc, 'Foregroundcolor','[1.000000, 0.333333, 0.498039]')
            add_line(gcs,strcat(temp.a,'/3'),strcat(temp.a,'_TrqOut1/1'),'autorouting','on')
        catch
        end
        try   % Turbine add NErr GoTo tag
            temp.pcx = temp.pos(2)+(temp.pos(4)-temp.pos(2))*3/8;
            temp.pcy = temp.pos(3);
            temp.pc = [(temp.pcy + 15) (temp.pcx - 6) (temp.pcy + 175) (temp.pcx + 6)];
            temp.b = strcat( temp.a , '_NErr');
            temp.bname = strcat( gcs,'/',temp.a,'_NErr1');
            add_block('simulink/Signal Routing/Goto',temp.bname,'MakeNameUnique','off','ShowName','off','GotoTag', temp.b, 'Position',temp.pc, 'Foregroundcolor','green')
            add_line(gcs,strcat(temp.a,'/2'),strcat(temp.a,'_NErr1/1'),'autorouting','on')
        catch
        end
        try   % Turbine add PRin From tag
            temp.pcx = temp.pos(2)+(temp.pos(4)-temp.pos(2))*5/8;
            temp.pcy = temp.pos(1);
            temp.pc = [(temp.pcy - 175) (temp.pcx - 6) (temp.pcy - 15) (temp.pcx + 6)];
            temp.b = strcat( temp.a , '_PRin');
            temp.bname = strcat( gcs,'/',temp.a,'_PRin1');
            add_block('simulink/Signal Routing/From',temp.bname,'MakeNameUnique','off','ShowName','off','GotoTag', temp.b, 'Position',temp.pc, 'Foregroundcolor','darkGreen')
            add_line(gcs,strcat(temp.a,'_PRin1/1'), strcat(temp.a,'/3'),'autorouting','on')
        catch
        end
        
        if (MakeLinkerTags > 0.5) % if make linker tags is enabled
            try   % Turbine add TrqOut From tag to link to
                temp.pcx = temp.pos(2)+(temp.pos(4)-temp.pos(2))*5/8;
                temp.pcy = temp.pos(3);
                temp.pc = [(temp.pcy + 200) (temp.pcx - 6) (temp.pcy + 360) (temp.pcx + 6)];
                temp.b = strcat( temp.a , '_TrqOut');
                temp.bname = strcat( gcs,'/',temp.a,'_TrqOutT');
                add_block('simulink/Signal Routing/From',temp.bname,'MakeNameUnique','off','ShowName','off','GotoTag', temp.b, 'Position',temp.pc, 'Foregroundcolor','[1.000000, 0.333333, 0.498039]')
            catch
            end
            try   % Turbine add NErr From tag to link to
                temp.pcx = temp.pos(2)+(temp.pos(4)-temp.pos(2))*3/8;
                temp.pcy = temp.pos(3);
                temp.pc = [(temp.pcy + 200) (temp.pcx - 6) (temp.pcy + 360) (temp.pcx + 6)];
                temp.b = strcat( temp.a , '_NErr');
                temp.bname = strcat( gcs,'/',temp.a,'_NErrD');
                add_block('simulink/Signal Routing/From',temp.bname,'MakeNameUnique','off','ShowName','off','GotoTag', temp.b, 'Position',temp.pc, 'Foregroundcolor','green')
            catch
            end
            try   % Turbine add PRin GoTo tag to link from
                temp.pcx = temp.pos(2)+(temp.pos(4)-temp.pos(2))*5/8;
                temp.pcy = temp.pos(1);
                temp.pc = [(temp.pcy - 360) (temp.pcx - 6) (temp.pcy - 200) (temp.pcx + 6)];
                temp.b = strcat( temp.a , '_PRin');
                temp.bname = strcat( gcs,'/',temp.a,'_PRinI');
                add_block('simulink/Signal Routing/Goto',temp.bname,'MakeNameUnique','off','ShowName','off','GotoTag', temp.b, 'Position',temp.pc, 'Foregroundcolor','darkGreen')
            catch
            end
        end
        
    case('Lib_Turbo_Compressor_TMATS/Compressor')
        try   % Compressor add C_Data terminator block
            temp.pcx = temp.pos(2)+(temp.pos(4)-temp.pos(2))*11/12;
            temp.pcy = temp.pos(3);
            temp.pc = [(temp.pcy + 15) (temp.pcx - 5) (temp.pcy + 25) (temp.pcx + 5)];
            temp.bname = strcat( gcs,'/',temp.a,'_term1');
            add_block('simulink/Commonly Used Blocks/Terminator',temp.bname,'MakeNameUnique','off','ShowName','off','Position',temp.pc)
            add_line(gcs,strcat(temp.a,'/6'),strcat(temp.a,'_term1/1'),'autorouting','on')
        catch
        end
        try   % Compressor add TrqOut GoTo tag
            temp.pcx = temp.pos(2)+(temp.pos(4)-temp.pos(2))*9/12;
            temp.pcy = temp.pos(3);
            temp.pc = [(temp.pcy + 15) (temp.pcx - 6) (temp.pcy + 175) (temp.pcx + 6)];
            temp.b = strcat( temp.a , '_TrqOut');
            temp.bname = strcat( gcs,'/',temp.a,'_TrqOut1');
            add_block('simulink/Signal Routing/Goto',temp.bname,'MakeNameUnique','off','ShowName','off','GotoTag', temp.b, 'Position',temp.pc, 'Foregroundcolor','[1.000000, 0.333333, 0.498039]')
            add_line(gcs,strcat(temp.a,'/5'),strcat(temp.a,'_TrqOut1/1'),'autorouting','on')
        catch
        end
        try   % Compressor add NErr GoTo tag
            temp.pcx = temp.pos(2)+(temp.pos(4)-temp.pos(2))*7/12;
            temp.pcy = temp.pos(3);
            temp.pc = [(temp.pcy + 15) (temp.pcx - 6) (temp.pcy + 175) (temp.pcx + 6)];
            temp.b = strcat( temp.a , '_NErr');
            temp.bname = strcat( gcs,'/',temp.a,'_NErr1');
            add_block('simulink/Signal Routing/Goto',temp.bname,'MakeNameUnique','off','ShowName','off','GotoTag', temp.b, 'Position',temp.pc, 'Foregroundcolor','green')
            add_line(gcs,strcat(temp.a,'/4'),strcat(temp.a,'_NErr1/1'),'autorouting','on')
        catch
        end
        try   % Compressor add Fractional Bleed GoTo tag
            temp.pcx = temp.pos(2)+(temp.pos(4)-temp.pos(2))*3/12;
            temp.pcy = temp.pos(3);
            temp.pc = [(temp.pcy + 15) (temp.pcx - 6) (temp.pcy + 175) (temp.pcx + 6)];
            temp.b = strcat( temp.a , '_Fbld');
            temp.bname = strcat( gcs,'/',temp.a,'_Fbld1');
            add_block('simulink/Signal Routing/Goto',temp.bname,'MakeNameUnique','off','ShowName','off','GotoTag', temp.b, 'Position',temp.pc, 'Foregroundcolor','blue')
            add_line(gcs,strcat(temp.a,'/2'),strcat(temp.a,'_Fbld1/1'),'autorouting','on')
        catch
        end
        try   % Compressor add Fractional Bleed GoTo tag
            temp.pcx = temp.pos(2)+(temp.pos(4)-temp.pos(2))*3/12;
            temp.pcy = temp.pos(3);
            temp.pc = [(temp.pcy + 15) (temp.pcx - 6) (temp.pcy + 175) (temp.pcx + 6)];
            temp.b = strcat( temp.a , '_Fbld');
            temp.bname = strcat( gcs,'/',temp.a,'_Fbld1');
            add_block('simulink/Signal Routing/Goto',temp.bname,'MakeNameUnique','off','ShowName','off','GotoTag', temp.b, 'Position',temp.pc, 'Foregroundcolor','blue')
            add_line(gcs,strcat(temp.a,'/2'),strcat(temp.a,'_Fbld1/1'),'autorouting','on')
        catch
        end
        try   % Compressor add CustBlds CharOut terminator block
            temp.pcx = temp.pos(2)+(temp.pos(4)-temp.pos(2))*1/12;
            temp.pcy = temp.pos(3);
            temp.pc = [(temp.pcy + 15) (temp.pcx - 5) (temp.pcy + 25) (temp.pcx + 5)];
            temp.bname = strcat( gcs,'/',temp.a,'_term2');
            add_block('simulink/Commonly Used Blocks/Terminator',temp.bname,'MakeNameUnique','off','ShowName','off','Position',temp.pc)
            add_line(gcs,strcat(temp.a,'/1'),strcat(temp.a,'_term2/1'),'autorouting','on')
        catch
        end
        try   % Compressor add Rline From tag
            temp.pcx = temp.pos(2)+(temp.pos(4)-temp.pos(2))*3/6;
            temp.pcy = temp.pos(1);
            temp.pc = [(temp.pcy - 175) (temp.pcx - 6) (temp.pcy - 15) (temp.pcx + 6)];
            temp.b = strcat( temp.a , '_Rline');
            temp.bname = strcat( gcs,'/',temp.a,'_Rline1');
            add_block('simulink/Signal Routing/From',temp.bname,'MakeNameUnique','off','ShowName','off','GotoTag', temp.b, 'Position',temp.pc, 'Foregroundcolor','darkGreen')
            add_line(gcs,strcat(temp.a,'_Rline1/1'), strcat(temp.a,'/2'),'autorouting','on')
        catch
        end
        
        if (MakeLinkerTags > 0.5)
            try   % Compressor add TrqOut From tag to link to
                temp.pcx = temp.pos(2)+(temp.pos(4)-temp.pos(2))*9/12;
                temp.pcy = temp.pos(3);
                temp.pc = [(temp.pcy + 200) (temp.pcx - 6) (temp.pcy + 360) (temp.pcx + 6)];
                temp.b = strcat( temp.a , '_TrqOut');
                temp.bname = strcat( gcs,'/',temp.a,'_TrqOutT');
                add_block('simulink/Signal Routing/From',temp.bname,'MakeNameUnique','off','ShowName','off','GotoTag', temp.b, 'Position',temp.pc, 'Foregroundcolor','[1.000000, 0.333333, 0.498039]')
            catch
            end
            try   % Compressor add NErr From tag to link to
                temp.pcx = temp.pos(2)+(temp.pos(4)-temp.pos(2))*7/12;
                temp.pcy = temp.pos(3);
                temp.pc = [(temp.pcy + 200) (temp.pcx - 6) (temp.pcy + 360) (temp.pcx + 6)];
                temp.b = strcat( temp.a , '_NErr');
                temp.bname = strcat( gcs,'/',temp.a,'_NErrD');
                add_block('simulink/Signal Routing/From',temp.bname,'MakeNameUnique','off','ShowName','off','GotoTag', temp.b, 'Position',temp.pc, 'Foregroundcolor','green')
            catch
            end
            try   % Compressor add Fractional Bleed From tag to link to
                temp.pcx = temp.pos(2)+(temp.pos(4)-temp.pos(2))*3/12;
                temp.pcy = temp.pos(3);
                temp.pc = [(temp.pcy + 200) (temp.pcx - 6) (temp.pcy + 360) (temp.pcx + 6)];
                temp.b = strcat( temp.a , '_Fbld');
                temp.bname = strcat( gcs,'/',temp.a,'_FbldF');
                add_block('simulink/Signal Routing/From',temp.bname,'MakeNameUnique','off','ShowName','off','GotoTag', temp.b, 'Position',temp.pc, 'Foregroundcolor','blue')
            catch
            end
            try   % Compressor add Rline GoTo tag to link from
                temp.pcx = temp.pos(2)+(temp.pos(4)-temp.pos(2))*3/6;
                temp.pcy = temp.pos(1);
                temp.pc = [(temp.pcy - 360) (temp.pcx - 6) (temp.pcy - 200) (temp.pcx + 6)];
                temp.b = strcat( temp.a , '_Rline');
                temp.bname = strcat( gcs,'/',temp.a,'_RlineI');
                add_block('simulink/Signal Routing/Goto',temp.bname,'MakeNameUnique','off','ShowName','off','GotoTag', temp.b, 'Position',temp.pc, 'Foregroundcolor','darkGreen')
            catch
            end
        end
        
    case('Lib_Turbo_Nozzle_TMATS/Nozzle')
        try   % Nozzle add N_Data terminator block
            temp.pcx = temp.pos(2)+(temp.pos(4)-temp.pos(2))*7/8;
            temp.pcy = temp.pos(3);
            temp.pc = [(temp.pcy + 15) (temp.pcx - 5) (temp.pcy + 25) (temp.pcx + 5)];
            temp.bname = strcat( gcs,'/',temp.a,'_term1');
            add_block('simulink/Commonly Used Blocks/Terminator',temp.bname,'MakeNameUnique','off','ShowName','off','Position',temp.pc)
            add_line(gcs,strcat(temp.a,'/4'),strcat(temp.a,'_term1/1'),'autorouting','on')
        catch
        end
        try   % Nozzle add NErr GoTo tag
            temp.pcx = temp.pos(2)+(temp.pos(4)-temp.pos(2))*5/8;
            temp.pcy = temp.pos(3);
            temp.pc = [(temp.pcy + 15) (temp.pcx - 6) (temp.pcy + 175) (temp.pcx + 6)];
            temp.b = strcat( temp.a , '_NErr');
            temp.bname = strcat( gcs,'/',temp.a,'_NErr1');
            add_block('simulink/Signal Routing/Goto',temp.bname,'MakeNameUnique','off','ShowName','off','GotoTag', temp.b, 'Position',temp.pc, 'Foregroundcolor','green')
            add_line(gcs,strcat(temp.a,'/3'),strcat(temp.a,'_NErr1/1'),'autorouting','on')
        catch
        end
        try   % Nozzle add Fg terminator block
            temp.pcx = temp.pos(2)+(temp.pos(4)-temp.pos(2))*3/8;
            temp.pcy = temp.pos(3);
            temp.pc = [(temp.pcy + 15) (temp.pcx - 5) (temp.pcy + 25) (temp.pcx + 5)];
            temp.bname = strcat( gcs,'/',temp.a,'_term2');
            add_block('simulink/Commonly Used Blocks/Terminator',temp.bname,'MakeNameUnique','off','ShowName','off','Position',temp.pc)
            add_line(gcs,strcat(temp.a,'/2'),strcat(temp.a,'_term2/1'),'autorouting','on')
        catch
        end
        try   % Nozzle add WOut terminator block
            temp.pcx = temp.pos(2)+(temp.pos(4)-temp.pos(2))*1/8;
            temp.pcy = temp.pos(3);
            temp.pc = [(temp.pcy + 15) (temp.pcx - 5) (temp.pcy + 25) (temp.pcx + 5)];
            temp.bname = strcat( gcs,'/',temp.a,'_term3');
            add_block('simulink/Commonly Used Blocks/Terminator',temp.bname,'MakeNameUnique','off','ShowName','off','Position',temp.pc)
            add_line(gcs,strcat(temp.a,'/1'),strcat(temp.a,'_term3/1'),'autorouting','on')
        catch
        end
        try   % Nozzle add Pamb From tag
            temp.pcx = temp.pos(2)+(temp.pos(4)-temp.pos(2))*3/4;
            temp.pcy = temp.pos(1);
            temp.pc = [(temp.pcy - 175) (temp.pcx - 6) (temp.pcy - 15) (temp.pcx + 6)];
            temp.b = strcat('Pamb');
            temp.bname = strcat( gcs,'/',temp.a,'_Pamb1');
            add_block('simulink/Signal Routing/From',temp.bname,'MakeNameUnique','off','ShowName','off','GotoTag', temp.b, 'Position',temp.pc)
            add_line(gcs,strcat(temp.a,'_Pamb1/1'), strcat(temp.a,'/2'),'autorouting','on')
        catch
        end
        
        if (MakeLinkerTags > 0.5)
            try   % Nozzle add NErr From tag to link to
                temp.pcx = temp.pos(2)+(temp.pos(4)-temp.pos(2))*5/8;
                temp.pcy = temp.pos(3);
                temp.pc = [(temp.pcy + 200) (temp.pcx - 6) (temp.pcy + 360) (temp.pcx + 6)];
                temp.b = strcat( temp.a , '_NErr');
                temp.bname = strcat( gcs,'/',temp.a,'_NErrD');
                add_block('simulink/Signal Routing/From',temp.bname,'MakeNameUnique','off','ShowName','off','GotoTag', temp.b, 'Position',temp.pc, 'Foregroundcolor','green')
            catch
            end
        end
        
    case('Lib_Turbo_Shaft_TMATS/Shaft')
        try   % Shaft add Ndot GoTo tag
            temp.pcx = temp.pos(2)+(temp.pos(4)-temp.pos(2))*3/4;
            temp.pcy = temp.pos(3);
            temp.pc = [(temp.pcy + 15) (temp.pcx - 6) (temp.pcy + 175) (temp.pcx + 6)];
            temp.b = strcat( temp.a , '_Ndot');
            temp.bname = strcat( gcs,'/',temp.a,'_Ndot1');
            add_block('simulink/Signal Routing/Goto',temp.bname,'MakeNameUnique','off','ShowName','off','GotoTag', temp.b, 'Position',temp.pc, 'Foregroundcolor','magenta')
            add_line(gcs,strcat(temp.a,'/2'),strcat(temp.a,'_Ndot1/1'),'autorouting','on')
        catch
        end
        try   % Shaft add Nmech GoTo tag
            temp.pcx = temp.pos(2)+(temp.pos(4)-temp.pos(2))*1/4;
            temp.pcy = temp.pos(3);
            temp.pc = [(temp.pcy + 15) (temp.pcx - 6) (temp.pcy + 175) (temp.pcx + 6)];
            temp.b = strcat(temp.a , '_Nmech');
            temp.bname = strcat( gcs,'/',temp.a,'_Nmech1');
            add_block('simulink/Signal Routing/Goto',temp.bname,'MakeNameUnique','off','ShowName','off','GotoTag', temp.b, 'Position',temp.pc, 'Foregroundcolor','red')
            add_line(gcs,strcat(temp.a,'/1'),strcat(temp.a,'_Nmech1/1'),'autorouting','on')
        catch
        end
        try   % Shaft add NmechIn From tag
            temp.pcx = temp.pos(2)+(temp.pos(4)-temp.pos(2))*3/4;
            temp.pcy = temp.pos(1);
            temp.pc = [(temp.pcy - 175) (temp.pcx - 6) (temp.pcy - 15) (temp.pcx + 6)];
            temp.b = strcat( temp.a , '_NmechIn');
            temp.bname = strcat( gcs,'/',temp.a,'_NmechIn1');
            add_block('simulink/Signal Routing/From',temp.bname,'MakeNameUnique','off','ShowName','off','GotoTag', temp.b, 'Position',temp.pc, 'Foregroundcolor','red')
            add_line(gcs,strcat(temp.a,'_NmechIn1/1'), strcat(temp.a,'/2'),'autorouting','on')
        catch
        end
        if (MakeLinkerTags > 0.5)
            try   % Shaft add Ndot From tag to link to
                temp.pcx = temp.pos(2)+(temp.pos(4)-temp.pos(2))*3/4;
                temp.pcy = temp.pos(3);
                temp.pc = [(temp.pcy + 200) (temp.pcx - 6) (temp.pcy + 360) (temp.pcx + 6)];
                temp.b = strcat( temp.a , '_Ndot');
                temp.bname = strcat( gcs,'/',temp.a,'_NdotD');
                add_block('simulink/Signal Routing/From',temp.bname,'MakeNameUnique','off','ShowName','off','GotoTag', temp.b, 'Position',temp.pc, 'Foregroundcolor','magenta')
            catch
            end
            try   % Shaft add Nmech From tag to link to
                temp.pcx = temp.pos(2)+(temp.pos(4)-temp.pos(2))*1/4;
                temp.pcy = temp.pos(3);
                temp.pc = [(temp.pcy + 200) (temp.pcx - 6) (temp.pcy + 360) (temp.pcx + 6)];
                temp.b = strcat( temp.a , '_Nmech');
                temp.bname = strcat( gcs,'/',temp.a,'_NmechI');
                add_block('simulink/Signal Routing/From',temp.bname,'MakeNameUnique','off','ShowName','off','GotoTag', temp.b, 'Position',temp.pc, 'Foregroundcolor','red')
            catch
            end
            try   % Shaft add NmechIn GoTo tag to link from
                temp.pcx = temp.pos(2)+(temp.pos(4)-temp.pos(2))*3/4;
                temp.pcy = temp.pos(1);
                temp.pc = [(temp.pcy - 360) (temp.pcx - 6) (temp.pcy - 200) (temp.pcx + 6)];
                temp.b = strcat( temp.a , '_NmechIn');
                temp.bname = strcat( gcs,'/',temp.a,'_NmechInI');
                add_block('simulink/Signal Routing/Goto',temp.bname,'MakeNameUnique','off','ShowName','off','GotoTag', temp.b, 'Position',temp.pc, 'Foregroundcolor','Red')
            catch
            end
        end
        
    case('Lib_Turbo_Valve_TMATS/Valve')
        try   % Valve add V_Data terminator block
            temp.pcx = temp.pos(2)+(temp.pos(4)-temp.pos(2))*5/6;
            temp.pcy = temp.pos(3);
            temp.pc = [(temp.pcy + 15) (temp.pcx - 5) (temp.pcy + 25) (temp.pcx + 5)];
            temp.bname = strcat( gcs,'/',temp.a,'_term1');
            add_block('simulink/Commonly Used Blocks/Terminator',temp.bname,'MakeNameUnique','off','ShowName','off','Position',temp.pc)
            add_line(gcs,strcat(temp.a,'/3'),strcat(temp.a,'_term1/1'),'autorouting','on')
        catch
        end
        try   % Valve add ValvePos From tag
            temp.pcx = temp.pos(2)+(temp.pos(4)-temp.pos(2))*3/6;
            temp.pcy = temp.pos(1);
            temp.pc = [(temp.pcy - 175) (temp.pcx - 6) (temp.pcy - 15) (temp.pcx + 6)];
            temp.b = strcat( temp.a , '_ValvePos');
            temp.bname = strcat( gcs,'/',temp.a,'_ValvePos1');
            add_block('simulink/Signal Routing/From',temp.bname,'MakeNameUnique','off','ShowName','off','GotoTag', temp.b, 'Position',temp.pc, 'Foregroundcolor','orange')
            add_line(gcs,strcat(temp.a,'_ValvePos1/1'), strcat(temp.a,'/2'),'autorouting','on')
        catch
        end
        if (MakeLinkerTags > 0.5)
            try   % Valve add ValvePos Goto tag to link to
                temp.pcx = temp.pos(2)+(temp.pos(4)-temp.pos(2))*3/6;
                temp.pcy = temp.pos(1);
                temp.pc = [(temp.pcy - 360) (temp.pcx - 6) (temp.pcy - 200) (temp.pcx + 6)];
                temp.b = strcat( temp.a , '_ValvePos');
                temp.bname = strcat( gcs,'/',temp.a,'_ValvePosC');
                add_block('simulink/Signal Routing/Goto',temp.bname,'MakeNameUnique','off','ShowName','off','GotoTag', temp.b, 'Position',temp.pc, 'Foregroundcolor','orange')
            catch
            end
        end
    case('Lib_Turbo_FlowPathChar_TMATS/Flowpathchar')
        try   % FPC add W From tag
            temp.pcx = temp.pos(2)+(temp.pos(4)-temp.pos(2))*1/8;
            temp.pcy = temp.pos(1);
            temp.pc = [(temp.pcy - 175) (temp.pcx - 6) (temp.pcy - 15) (temp.pcx + 6)];
            temp.b = strcat('W');
            temp.bname = strcat( gcs,'/','W1');
            add_block('simulink/Signal Routing/From',temp.bname,'MakeNameUnique','off','ShowName','off','GotoTag', temp.b, 'Position',temp.pc, 'Foregroundcolor','darkGreen')
            add_line(gcs,strcat('W1/1'), strcat(temp.a,'/1'),'autorouting','on')
        catch
        end
        if (MakeLinkerTags > 0.5)
            try   % FPC add W Goto tag to link to
                temp.pcx = temp.pos(2)+(temp.pos(4)-temp.pos(2))*1/8;
                temp.pcy = temp.pos(1);
                temp.pc = [(temp.pcy - 360) (temp.pcx - 6) (temp.pcy - 200) (temp.pcx + 6)];
                temp.b = strcat('W');
                temp.bname = strcat( gcs,'/','WI');
                add_block('simulink/Signal Routing/Goto',temp.bname,'MakeNameUnique','off','ShowName','off','GotoTag', temp.b, 'Position',temp.pc, 'Foregroundcolor','darkGreen')
            catch
            end
        end
    case('Lib_Turbo_Splitter_TMATS/Splitter')
        try   % Splitter add BPR From tag
            temp.pcx = temp.pos(2)+(temp.pos(4)-temp.pos(2))*3/4;
            temp.pcy = temp.pos(1);
            temp.pc = [(temp.pcy - 175) (temp.pcx - 6) (temp.pcy - 15) (temp.pcx + 6)];
            temp.b = strcat(temp.a,'_BPR');
            temp.bname = strcat( gcs,'/',temp.a,'_BPR1');
            add_block('simulink/Signal Routing/From',temp.bname,'MakeNameUnique','off','ShowName','off','GotoTag', temp.b, 'Position',temp.pc, 'Foregroundcolor','darkGreen')
            add_line(gcs,strcat(temp.a,'_BPR1/1'), strcat(temp.a,'/2'),'autorouting','on')
        catch
        end
        if (MakeLinkerTags > 0.5)
            try   % Splitter add BPR Goto tag to link to
                temp.pcx = temp.pos(2)+(temp.pos(4)-temp.pos(2))*3/4;
                temp.pcy = temp.pos(1);
                temp.pc = [(temp.pcy - 360) (temp.pcx - 6) (temp.pcy - 200) (temp.pcx + 6)];
                temp.b = strcat(temp.a,'_BPR');
                temp.bname = strcat( gcs,'/',temp.a,'_BPRI');
                add_block('simulink/Signal Routing/Goto',temp.bname,'MakeNameUnique','off','ShowName','off','GotoTag', temp.b, 'Position',temp.pc, 'Foregroundcolor','darkGreen')
            catch
            end
        end
    otherwise
        temp.BT = get_param(gcb,'BlockType');
        switch lower(temp.BT)
            case('from')
                try   % From add Out1
                    temp.pcx = temp.pos(2)+(temp.pos(4)-temp.pos(2))*1/2;
                    temp.pcy = temp.pos(3);
                    temp.pc = [(temp.pcy + 30) (temp.pcx - 6) (temp.pcy + 60) (temp.pcx + 6)];
                    temp.b = get_param(gcb,'GoToTag');
                    temp.bname = strcat( gcs,'/',temp.b);
                    add_block('simulink/Sinks/Out1',temp.bname,'MakeNameUnique','off','ShowName','on','Position',temp.pc, 'Foregroundcolor',get_param(gcb,'ForegroundColor'))
                    add_line(gcs,strcat(temp.a , '/1'), strcat(temp.b,'/1'),'autorouting','on')
                catch ME
                     disp('Block Linking naming conflict. Can not complete task.')
                end
            case('goto')
                try   % Goto add In1
                    temp.pcx = temp.pos(2)+(temp.pos(4)-temp.pos(2))*1/2;
                    temp.pcy = temp.pos(1);
                    temp.pc = [(temp.pcy - 60) (temp.pcx - 6) (temp.pcy - 30) (temp.pcx + 6)];
                    temp.b = get_param(gcb,'GoToTag');
                    temp.bname = strcat( gcs,'/',temp.b);
                    add_block('simulink/Sources/In1',temp.bname,'MakeNameUnique','off','ShowName','on','Position',temp.pc, 'Foregroundcolor',get_param(gcb,'ForegroundColor'))
                    add_line(gcs,strcat(temp.b , '/1'), strcat(temp.a,'/1'),'autorouting','on')
                catch ME
                    disp('Block Linking naming conflict. Can not complete task.')
                end
            case('inport')
                try   % Inport add GoTo
                    temp.pcx = temp.pos(2)+(temp.pos(4)-temp.pos(2))*1/2;
                    temp.pcy = temp.pos(3);
                    temp.pc = [(temp.pcy + 30) (temp.pcx - 6) (temp.pcy + 190) (temp.pcx + 6)];
                    temp.b = get_param(gcb,'Name');
                    temp.bname = strcat( gcs,'/',temp.b,'1');
                    add_block('simulink/Signal Routing/Goto',temp.bname,'MakeNameUnique','off','ShowName','off','GotoTag', temp.b, 'Position',temp.pc, 'Foregroundcolor',get_param(gcb,'ForegroundColor'))          
                    add_line(gcs,strcat(temp.a,'/1'),strcat(temp.b,'1/1'),'autorouting','on')
                catch ME
                    disp('Block Linking naming conflict. Can not complete task.')
                end
            case('outport')
                try   % Inport add From
                    temp.pcx = temp.pos(2)+(temp.pos(4)-temp.pos(2))*1/2;
                    temp.pcy = temp.pos(3);
                    temp.pc = [(temp.pcy - 205) (temp.pcx - 6) (temp.pcy - 50) (temp.pcx + 6)];
                    temp.b = get_param(gcb,'Name');
                    temp.bname = strcat( gcs,'/',temp.b,'1');
                    add_block('simulink/Signal Routing/From',temp.bname,'MakeNameUnique','off','ShowName','off','GotoTag', temp.b, 'Position',temp.pc, 'Foregroundcolor',get_param(gcb,'ForegroundColor'))
                    add_line(gcs,strcat(temp.b,'1/1'),strcat(temp.a,'/1'),'autorouting','on')
                catch ME
                    disp('Block Linking naming conflict. Can not complete task.')
                end
            case('subsystem')
                temp.Nports = get_param(gcb,'Ports');
                temp.Nin = temp.Nports(1);
                temp.Nout = temp.Nports(2);
                temp.BLKS = get_param(gcb,'Blocks');
                if temp.Nin > 0;
                    for i = 1:temp.Nin % Create an Input From block for each Subsystem Input
                        try   % Generic Subsystem Input From
                            temp.pcx = temp.pos(2)+(temp.pos(4)-temp.pos(2))* (2*i-1)/(temp.Nin*2);
                            temp.pcy = temp.pos(1);
                            temp.pc = [(temp.pcy - 175) (temp.pcx - 6) (temp.pcy - 15) (temp.pcx + 6)];
                            temp.b = temp.BLKS(i);
                            temp.bname = strcat( gcs,'/',temp.a,temp.b{1});
                            add_block('simulink/Signal Routing/From',temp.bname,'MakeNameUnique','off','ShowName','off','GotoTag', temp.b{1} , 'Position',temp.pc)
                            temp.i = num2str(i);
                            add_line(gcs,strcat(temp.a,temp.b{1}, '/1'), strcat(temp.a,'/',temp.i),'autorouting','on')
                        catch
                        end
                    end
                end
                if temp.Nout > 0;
                    for i = 1:temp.Nout % Create an Output Goto block for each Subsystem Output
                        try   % Generic Subsystem Output Goto
                            temp.pcx = temp.pos(2)+(temp.pos(4)-temp.pos(2))* (2*i-1)/(temp.Nout*2);
                            temp.pcy = temp.pos(3);
                            temp.pc = [(temp.pcy + 15) (temp.pcx - 6) (temp.pcy + 175) (temp.pcx + 6)];
                            temp.blength = length(temp.BLKS);
                            temp.b = temp.BLKS(temp.blength - temp.Nout + i);
                            temp.bname = strcat( gcs,'/',temp.a,temp.b{1});
                            add_block('simulink/Signal Routing/Goto',temp.bname,'MakeNameUnique','off','ShowName','off','GotoTag', temp.b{1} , 'Position',temp.pc)
                            temp.i = num2str(i);
                            add_line(gcs,strcat(temp.a,'/',temp.i) , strcat(temp.a,temp.b{1}, '/1'),'autorouting','on')
                        catch
                        end
                    end
                end
            otherwise
        end
end