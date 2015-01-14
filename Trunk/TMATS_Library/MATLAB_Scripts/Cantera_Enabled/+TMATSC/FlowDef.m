classdef FlowDef
    %T-MATS_Cantera flow class definition
    properties
        W       %Flow [pps]
        s       %Entropy
        Tt      %Total Temperature [degR]
        Pt      %Total Pressure [psia]
        ht      %Total Enthaply
        rhot    %Total density
        gamt    %Total heat constant
        Ts      %Static Temperature [degR]
        Ps      %Static Pressure [psia]
        hs      %Static Enthalpy
        rhos    %Static density
        gams    %Static heat constant
        Vflow   %Flow Velocity
        MN      %Mach Number
        A       %Area [in2]
        FtU     %Future Use
        CompVal_TMATS     %Composition Values (6x1), T-MATS format [frac]
        CompVal_Can       %Composition Values Cantera format
    end
    properties (Constant)
        Name = TMATSC.canteraload('Name');          %Species Names
        Species = TMATSC.canteraload('Species');    %Species Content values
    end
    methods
        %Constructor
        function obj = FlowDef(varargin)
            %Flow constructor. 4 acceptable input types: 0 input, empty flow object, 1 input, flow vector transform (25x1 flow vector), 4 input, flow value calculation of totals (CompVal(6x1 vector), W, Tt, Pt), 5 input, complete flow value calculation (CompVal(6x1 vector), W, Ts, Ps, MN) 
            if nargin == 0
                SW = 'None';
            elseif nargin == 1 % Build by VectorFlow
            	VectorFlow = cell2mat(varargin(1));
                SW = 'VF';
            elseif nargin == 4 % Build from Total Values
                compNN = cell2mat(varargin(1));
                W = cell2mat(varargin(2));
                Tt = cell2mat(varargin(3));
                Pt = cell2mat(varargin(4));
                SW = 'TV';
            elseif nargin == 5 % Build from Static Values
                compNN = cell2mat(varargin(1));
                W = cell2mat(varargin(2));
                Ts = cell2mat(varargin(3));
                Ps = cell2mat(varargin(4));
                MN = cell2mat(varargin(5));
                SW = 'SV';
            else
                SW = 'None';
                disp('FlowDef input incorrect. Acceptable inputs include: (), (25x1 flow vector), (compNN, W, Tt, Pt), or (compNN, W, Ts, Ps, MN).')
            end
            switch(SW)
                case('VF')
                    % Define object by flow vector
                    obj.W =                 VectorFlow(1);
                    obj.s =                 VectorFlow(2);
                    obj.Tt =                VectorFlow(3);
                    obj.Pt =                VectorFlow(4);
                    obj.ht =                VectorFlow(5);
                    obj.rhot =              VectorFlow(6);
                    obj.gamt =              VectorFlow(7);
                    obj.Ts =                VectorFlow(8);
                    obj.Ps =                VectorFlow(9);
                    obj.hs =                VectorFlow(10);
                    obj.rhos =              VectorFlow(11);
                    obj.gams =              VectorFlow(12);
                    obj.Vflow =             VectorFlow(13);
                    obj.MN =                VectorFlow(14);
                    obj.A =                 VectorFlow(15);
                    obj.CompVal_TMATS =    [VectorFlow(16)...
                        VectorFlow(17)...
                        VectorFlow(18)...
                        VectorFlow(19)...
                        VectorFlow(20)...
                        VectorFlow(21)];
                    obj.FtU =              [VectorFlow(22)...
                        VectorFlow(23)...
                        VectorFlow(24)...
                        VectorFlow(25)];
                    obj = obj.CanSet();
                case('TV')
                    % Define flow object by total pressure and temperature
                    obj.CompVal_TMATS = compNN;
                    obj = obj.CanSet();          % set Cantera composition format
                    obj.W = W;
                    obj = obj.set_TP(Tt, Pt);
                case('SV')
                    % Define flow object with static variables
                    obj = obj.CanSet(compNN);          % set Cantera composition format
                    obj.W = W;
                    if ( MN > 0 )
                        % Speed != 0
                        obj = obj.set_TsPsMN(Ts, Ps, MN);
                    else
                        % Speed = 0, Total = Static
                        obj = obj.set_TP(Ts, Ps);
                        obj = obj.StaticSet(obj.Tt, obj.Pt, obj.ht, obj.rhot, obj.gams);
                        obj = obj.SpeedSet(0,0,0);
                    end
                case('None')
                    % empty flow object
                    obj.W = 0;
                    obj.CompVal_TMATS = [0 0 0 0 0 0];
                    obj = obj.TotalSet(0, 0, 0, 0, 0, 0);
                    obj = obj.SClr();
                    obj = obj.CanSet();
            end
            fclose('all');
        end
        function obj = CanSet(obj, varargin)
            %Set Composition Values in Cantera format (CompVal_Can) based on CompVal_TMATS(6x1)
            %write out a mass composition array for cantera
            if nargin > 1 && nargin < 3
                % set CompVal_TMATS vector to input
                obj.CompVal_TMATS = cell2mat(varargin(1));
            end
            obj.CompVal_Can = '';
            if any(obj.CompVal_TMATS)
                compname = {'', '', '', '', '', '', '', '', '', '', '', ''};
                fract = [0 0 0 0 0 0 0 0 0 0 0 0];
                numcurrent = 0;
                for cName = 1:6
                    for cSpecies = 1:6
                        if obj.Species{cName,cSpecies}*obj.CompVal_TMATS(cName) > 0.00001
                            numcurrent = numcurrent+1;
                            fract(numcurrent)=obj.Species{cName,cSpecies}*obj.CompVal_TMATS(cName);
                            
                            compname{numcurrent} = obj.Name{cName,cSpecies};
                            obj.CompVal_Can = sprintf( '%s %s:%f', obj.CompVal_Can, obj.Name{cName,cSpecies},obj.Species{cName,cSpecies}*obj.CompVal_TMATS(cName) );
                        end
                    end
                end
            end
            fclose('all');
        end
        %Cantera interface functions
        function obj = flowadd(obj1, obj2)
            % Add flow1 and flow2 together, conserving enthalpy and mass.
            
            obj = TMATSC.FlowDef();
            
            % add wieght flows
            obj.W = obj1.W + obj2.W;
            
            %Assume final flow is at primary flow pressure
            obj.Pt = obj1.Pt;
            
            %average composition values
            obj.ht = (obj1.W * obj1.ht + obj2.W * obj2.ht)/ obj.W;
            obj.CompVal_TMATS(1) = (obj1.W * obj1.CompVal_TMATS(1) + obj2.W * obj2.CompVal_TMATS(1))/ obj.W;
            obj.CompVal_TMATS(2) = (obj1.W * obj1.CompVal_TMATS(2) + obj2.W * obj2.CompVal_TMATS(2))/ obj.W;
            obj.CompVal_TMATS(3) = (obj1.W * obj1.CompVal_TMATS(3) + obj2.W * obj2.CompVal_TMATS(3))/ obj.W;
            obj.CompVal_TMATS(4) = (obj1.W * obj1.CompVal_TMATS(4) + obj2.W * obj2.CompVal_TMATS(4))/ obj.W;
            obj.CompVal_TMATS(5) = (obj1.W * obj1.CompVal_TMATS(5) + obj2.W * obj2.CompVal_TMATS(5))/ obj.W;
            obj.CompVal_TMATS(6) = (obj1.W * obj1.CompVal_TMATS(6) + obj2.W * obj2.CompVal_TMATS(6))/ obj.W;
            
            %Write new mass composition array for cantera
            obj = obj.CanSet();
            
            %Set flow
            global fs;
            set(fs,'Y', obj.CompVal_Can);
            
            %iterate on temperature until you get the desired enthalpy
            Ttlast = -999;
            count = 0;
            Ttg = (obj1.W * obj1.Tt + obj2.W * obj2.Tt)/ obj.W;
            htOut = obj.ht;
            Ptg = obj.Pt;
            
            last = Ttlast;
            lasterr=1;
            
            while  abs (lasterr)>.000000001 && count < 50
                set(fs,'Y',obj.CompVal_Can);
                set(fs, 'T', Ttg*5./9.,'P',Ptg*6894.75729 );
                equilibrate(fs, 'TP');
                htg = enthalpy_mass( fs )*.0004302099943161011;
                root = htg-htOut;
                sec_out = TMATSC.FlowDef.iterSecant( root, Ttg, last, lasterr, .1 );
                next = sec_out(1);
                last = sec_out(2);
                lasterr = sec_out(3);
                Ttg = next;
            end
            
            %set output
            sOut = entropy_mass( fs )*0.000238845896627;
            rhotOut = density( fs )*.0624;
            gamtOut = cp_mass( fs )/ cv_mass( fs );
            
            obj = obj.TotalSet(sOut, Ttg, Ptg, htOut, rhotOut, gamtOut);
            fclose( 'all' );
        end
        function obj = flowcopy(obj)
            % Copy an input flow to an output flow
        end
        function frac = getMassFraction(obj, compound)
            %Return mass fraction of compound in object flow.
            global fs;
            set( fs, 'Y',obj.CompVal_Can);
            
            % iterate on temperature until it matches the input enthalpy
            Ttlast = -999;
            lasterr=1;
            count = 0;
            TtOut = obj.Tt;
            PtOut = obj.Pt;
            h = obj.ht;
            while  abs (lasterr)>.000000001 && count < 50
                set(fs,'Y',obj.CompVal_Can);
                set(fs, 'T', TtOut*5./9.,'P',PtOut*6894.75729 );
                equilibrate(fs, 'TP');
                count= count+1;
                htg = enthalpy_mass( fs )*.0004302099943161011;
                root = htg-h;
                sec_out = TMATSC.FlowDef.iterSecant( root, TtOut, Ttlast, lasterr, .1 );
                TtOut = sec_out(1);
                Ttlast = sec_out(2);
                lasterr = sec_out(3);
            end
            
            frac = massFraction( fs, compound );
            fclose( 'all' );
        end
        function obj = set_hP(obj,ht,Pt)
            %Set flow condition based on ht and Pt
            
            obj.Pt = Pt;
            obj.ht = ht;
            
            global fs;
            set(fs,'Y',obj.CompVal_Can);
            %iterate on temperature until you get the desired enthalpy
            
            Ttg = obj.Tt;
            htOut = obj.ht;
            Ptg = obj.Pt;
            
            Ttlast = -999;
            count = 0;
            
            last = Ttlast;
            lasterr=1;
            
            while  abs (lasterr)>.000000001 && count < 50
                set(fs,'Y',obj.CompVal_Can);
                set(fs, 'T', Ttg*5./9.,'P',Ptg*6894.75729 );
                equilibrate(fs, 'TP');
                htg = enthalpy_mass( fs )*.0004302099943161011;
                root = htg-htOut;
                sec_out = TMATSC.FlowDef.iterSecant( root, Ttg, last, lasterr, .1 );
                next = sec_out(1);
                last = sec_out(2);
                lasterr = sec_out(3);
                Ttg = next;
            end
            
            sOut = entropy_mass( fs )*0.000238845896627;
            rhotOut = density( fs )*.0624;
            gamtOut = cp_mass( fs )/ cv_mass( fs );
            
            obj = obj.TotalSet(sOut, Ttg, Ptg, htg, rhotOut, gamtOut);
            
            %reset static values because they may not be accurate
            obj = obj.SClr();
            fclose( 'all' );
        end
        function obj = set_MN1(obj)
            %Set the static conditions to sonic flow based on flow coditions
            global fs;
            set(fs,'Y',obj.CompVal_Can);
            
            Pslast = -999;
            count = 0;
            
            last = Pslast;
            lasterr=1;
            
            Psg = obj.Pt/(1+(obj.gamt -1)/2)^(obj.gamt/(obj.gamt-1));
            
            while  abs (lasterr)>.000000001 && count < 50
                
                % set the conditions based on entropy and input pressure
                set( fs, 'S',obj.s/0.000238845896627, 'P', Psg*6894.75729,'Y',obj.CompVal_Can);
                equilibrate( fs, 'SP' );
                
                % set static conditions
                
                hsg = enthalpy_mass( fs )*0.0004302099943161011;
                MMW = meanMolecularWeight(fs );
                gamsg = cp_mass( fs )/ cv_mass( fs );
                Tsg = temperature( fs )*9./5.;
                Vson = sqrt(gamsg * Tsg * 5./9. * 8314.4621 / MMW )*3.28084;
                Vflowg =   sqrt(  778.169 * 32.1740 * 2. * ( obj.ht - hsg ));
                MNing = Vflowg/Vson ;
                root = 1 - MNing;
                sec_out = TMATSC.FlowDef.iterSecant( root, Psg, last, lasterr, .1 );
                next = sec_out(1);
                last = sec_out(2);
                lasterr = sec_out(3);
                Psg = next;
            end
            rhosOut = density( fs )*.0624;
            AOut = obj.W/ (Vflowg * rhosOut/144);
            
            obj = obj.StaticSet(Tsg, Psg, hsg, rhosOut, gamsg);
            obj = obj.SpeedSet(MNing, AOut, Vflowg);
            fclose( 'all' );
        end
        function obj = set_Ps(obj, PsExh)
            %Set the static conditions based on flow and static pressure
            global fs
            set( fs, 'Y',obj.CompVal_Can);
            
            if ( PsExh >  obj.Pt )
                PsExh = obj.Pt;
            end
            
            % set the conditions based on entropy and input pressure
            set( fs, 'S',obj.s/0.000238845896627, 'P', PsExh*6894.75729);
            equilibrate( fs, 'SP' );
            
            % set static conditions
            gamsOut = cp_mass( fs )/ cv_mass( fs );
            TsOut= temperature( fs )*9./5.;
            hsOut = enthalpy_mass( fs )*0.0004302099943161011;
            rhosOut = density( fs )*.0624;
            PsOut = PsExh;
            MMW = meanMolecularWeight(fs );
            Vson = sqrt(gamsOut * TsOut*5./9.*8314.4621/ MMW )*3.28084;
            if obj.ht < hsOut
                hsOut = obj.ht;
            end
            VflowOut = sqrt(778.169 * 32.1740 * 2. * (obj.ht - hsOut));
            MNOut = VflowOut/ Vson;
            AOut = obj.W/ (VflowOut * rhosOut/144);
            
            obj = obj.StaticSet(TsOut, PsOut, hsOut, rhosOut, gamsOut);
            obj = obj.SpeedSet(MNOut, AOut, VflowOut);
            fclose('all');
        end
        function obj = set_SP(obj,S, Pt)
            %Set the total conditions based on flow and input entropy and total pressure
            global fs
            set( fs, 'Y',obj.CompVal_Can);
            set(fs, 'S', S/0.000238845896627,'P',Pt*6894.75729 );
            equilibrate(fs, 'SP');
            TtOut = temperature(fs)*9./5.;
            htOut = enthalpy_mass(fs)*.0004302099943161011;
            rhotOut = density( fs )*.0624;
            gamtOut = cp_mass( fs )/ cv_mass( fs );
            sOut = entropy_mass( fs )*0.000238845896627;
            
            %set output
            obj = obj.TotalSet(sOut, TtOut, Pt, htOut, rhotOut, gamtOut);
            % Clear static variable b/c they may not be valid in the new flow
            obj = obj.SClr();
            
            fclose('all');
        end
        function obj = set_TP(obj,Tt,Pt)
            %Set the total conditions based on flow, total temperatuer, and total pressure
            global fs
            set( fs, 'Y',obj.CompVal_Can);
            
            % set the conditions
            set(fs, 'T', Tt*5./9.,'P',Pt*6894.75729 );
            equilibrate(fs, 'TP');
            
            htOut = enthalpy_mass( fs )*.0004302099943161011;
            sOut = entropy_mass( fs )*0.000238845896627;
            rhotOut = density( fs )*.0624;
            gamtOut =  cp_mass( fs )/ cv_mass( fs );
            
            % set output
            obj = obj.TotalSet(sOut, Tt, Pt, htOut, rhotOut, gamtOut);
            % Clear static variable b/c they may not be valid in the new flow
            obj = obj.SClr();
            fclose('all');
        end
        function obj = set_TsPsMN(obj,Ts,Ps,MN)
            %Set the conditions based on flow, static temperature, static pressure, and Mach number
            global fs;
            set( fs, 'Y',obj.CompVal_Can);
            
            % set the static state based on temperature and pressure
            set( fs, 'T',  Ts*5./9.,'P',Ps*6894.75729 );
            equilibrate( fs, 'TP' );
            
            % determine static values
            hsOut = enthalpy_mass( fs )*.0004302099943161011;
            rhosOut = density( fs )*.0624;
            MMW = meanMolecularWeight(fs );
            gamsOut =  cp_mass( fs )/ cv_mass( fs );
            sOut = entropy_mass( fs ) *0.000238845896627;
            
            % calculate total enthalpy
            Vson = sqrt( gamsOut*Ts*1544/MMW*32.174);
            VflowOut = MN*Vson;
            htOut = hsOut + (VflowOut)^2/(  778.169 * 32.1740 * 2 );
            
            % iterate on total pressure until the entropy of the total state
            % matches the static state
            Ptg = Ps*( 1+( gamsOut - 1 ) /2.* MN^2 )^( gamsOut /( gamsOut -1 ));
            last = -9999;
            lasterr=1;
            count = 0;
            while  abs (lasterr)>.0000001 && count < 50
                set( fs, 'H', htOut/0.0004302099943161011, 'P',Ptg*6894.75729 );
                equilibrate( fs, 'HP' );
                root = sOut - entropy_mass( fs )*0.000238845896627;
                count = count + 1;
                sec_out = TMATSC.FlowDef.iterSecant( root, Ptg, last, lasterr, .1 );
                next = sec_out(1);
                last = sec_out(2);
                lasterr = sec_out(3);
                Ptg = next;
            end
            
            % load the values into the output
            TtOut = temperature( fs )*9./5.;
            rhotOut = density( fs )*.0624;
            gamtOut =  cp_mass( fs )/ cv_mass( fs );
            AOut = obj.W/ (VflowOut * rhosOut/144);
            
            %set total outputs
            obj = obj.TotalSet(sOut, TtOut, Ptg, htOut, rhotOut, gamtOut);
            %set static outputs
            obj = obj.StaticSet(Ts, Ps, hsOut, rhosOut, gamsOut);
            obj = obj.SpeedSet(MN, AOut, VflowOut);
            
            fclose('all');
        end
        %object set functions
        function obj = TotalSet(obj, s, Tt, Pt, ht, rhot, gamt)
            % Manually set total values in the flow object
            obj.s = s;
            obj.Tt = Tt;
            obj.Pt = Pt;
            obj.ht = ht;
            obj.rhot = rhot;
            obj.gamt = gamt;
        end
        function obj = SpeedSet(obj, MN, A, Vflow)
            % Manually set static values in the flow object
            obj.MN = MN;
            obj.A = A;
            obj.Vflow = Vflow;
        end
        function obj = StaticSet(obj, Ts, Ps, hs, rhos, gams)
            %Manually set static values in the flow object
            obj.Ts = Ts;
            obj.Ps = Ps;
            obj.hs = hs;
            obj.rhos = rhos;
            obj.gams = gams;
        end
        function obj = SClr(obj)
            %Clear static and speed values in the flow object
            Rst = 0;
            obj = obj.StaticSet(Rst, Rst, Rst, Rst, Rst);
            obj = obj.SpeedSet(Rst, Rst, Rst);
        end
        function V = FlwVec(obj)
            %Create flow vector for Simulink transport
            V(25) = 0;
            V(1) = obj.W;
            V(2) = obj.s;
            V(3) = obj.Tt;
            V(4) = obj.Pt;
            V(5) = obj.ht;
            V(6) = obj.rhot;
            V(7) = obj.gamt;
            V(8) = obj.Ts;
            V(9) = obj.Ps;
            V(10) = obj.hs;
            V(11) = obj.rhos;
            V(12) = obj.gams;
            V(13) = obj.Vflow;
            V(14) = obj.MN;
            V(15) = obj.A;
            V(16) = obj.CompVal_TMATS(1);
            V(17) = obj.CompVal_TMATS(2);
            V(18) = obj.CompVal_TMATS(3);
            V(19) = obj.CompVal_TMATS(4);
            V(20) = obj.CompVal_TMATS(5);
            V(21) = obj.CompVal_TMATS(6);
        end
    end
    methods (Static, Access = private)
        function output = iterSecant(err, current, lastin, lasterrin, max)
            % simple secant solver
            % determine if this is first pass
            if lastin == -999;
                % first pass, change .05 for initial perturbation if not on soluttion
                if abs(err)>.000000001
                    next = current*1.05;
                else
                    % first pass worked, dont change value
                    next = current;
                end
            else
                % not first pass, determine the next guess value
                next = current - err * ( current - lastin )/(err - lasterrin );
                % check to see if we violate the maximum value
                if abs(( next - current )/current) > max
                    next = current + .1*current*sign(next -current );
                end
            end
            %return these values for use on next pass as previous results
            last = current;
            lasterr = err;
            output = [next,last,lasterr];
        end
    end
end