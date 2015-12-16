function TableGen(type, phasefile,Name1, Name2, Species1, Species2, R1, R2, R3)
% this function generates replacement tables for the T-MATS simulation
% package

cleanup;
%Load Phase definition
g = importPhase(phasefile);

%Normalize Species values
F1 = Species1;
F2 = Species2;
Frac1 = F1./sum(F1);
Frac2 = F2./sum(F2);

%initialize vectors
nmd = nSpecies(g);
YStream1 = zeros(nmd,1);
YStream2 = zeros(nmd,1);

%Define Stream1 vector
for i=1:length(Name1)
    Ynum = speciesIndex(g,Name1{i});
    YStream1(Ynum) = Frac1(i);
end
%Define Stream2 vector
for i=1:length(Name2)
    Ynum = speciesIndex(g,Name2{i});
    YStream2(Ynum) = Frac2(i);
end

%% PT2S
%define Range
TempA = R1; % in degR
Temp = TempA*(5/9); % convert to degK
LT = length(Temp);
PressA = R2; % in psia
Press = PressA* 6894.75729; % convert to Pa
LP = length(Press);
Frc = R3; %in S1/S2
LF = length(Frc);

% define table size
S = zeros(LP,LT,LF);
H = zeros(LP,LT,LF);

p = 0;
disp('calculating pt2sc table');
for i1= 1: LF
    %Combine the two streams as described by Fraction (Stream2/Stream1)
    Yreact = YStream1*(1-Frc(i1)) + YStream2*Frc(i1);
    
    for i2 = 1: LT
        for i3 = 1: LP
            %Define reactant composition
            set(g,'Y', Yreact);
            
            try
                %calc values
                set(g,'T', Temp(i2),'P',Press(i3));
                equilibrate(g, 'TP');
                S(i3,i2,i1) = entropy_mass(g);
                H(i3,i2,i1) = enthalpy_mass(g);
                
            catch ME
                disp(sprintf('Warning: S at Pres: %f,Temp: %f,and Frac: %f, Failed to calculate',PressA(i3),TempA(i2),Frc(i1)))
                S(i3,i2,i1) = 0;
                H(i3,i2,i1) = -9999*10^6;
            end
        end
    end
    H(i3,i2,i1) = H(i3,i2,i1)+18400*Frc(i1)/0.0004302099943161011;
    p = p+1;
    fprintf('Complete with Fraction %d of %d\n',p,LF)
end
SA = S*0.000238845896627;
HA = H*0.0004302099943161011;
HA2D = mean(HA);
disp('entropy and enthalpy value calculation complete');


for itype = 1:length(type)
    typeS = type{itype};
    
    % generate tables
    switch(typeS)
        case('PT2S')
            disp('generating pt2sc_TMATS.c file');
            % generate pt2sc_TMATS.h file
            OldFolder = cd('AlternateTables');
            fid = fopen('pt2sc_TMATS.c','w');
            
            fprintf(fid,'/*	T-MATS -- pt2sc_TMATS.c */\n\n');
            % send stream definitions to file
            fprintf(fid,'/* Stream 1 Name: ');
            for i = 1:length(Name1)
                fprintf(fid,'%s, ',char(Name1(i)));
            end
            fprintf(fid,'*/\n/* Stream 1 Species: ');
            for i = 1:length(Species1);
                fprintf(fid,'%f, ',Species1(i));
            end
            fprintf(fid,'*/\n/* Stream 2 Name: ');
            for i = 1:length(Name2)
                fprintf(fid,'%s, ',char(Name2(i)));
            end
            fprintf(fid,'*/\n/* Stream 2 Species: ');
            for i = 1:length(Species2);
                fprintf(fid,'%f, ',Species2(i));
            end
            fprintf(fid,'*/\n\n');
            
            fprintf(fid,'#include <stdio.h>\n');
            fprintf(fid,'#include <stdlib.h>\n');
            fprintf(fid,'#include "simstruc.h"\n');
            fprintf(fid,'#include <math.h>\n');
            
            fprintf(fid,'double pt2sc(double Pressure, double Temperature, double FAR)\n{\n');
            ThreeD_D(fid);
            
            fprintf(fid,'/*generate vector length constants */\n');
            fprintf(fid,'const int Lx = %i;\n',LP);
            fprintf(fid,'const int Ly = %i;\n',LT);
            fprintf(fid,'const int Lz = %i;\n\n',LF);
            
            % generate Pressure Axis
            fprintf(fid,'/* X or Pressure axis definition */\n');
            fprintf(fid,'const double X[%i] = {',LP);
            fprintf(fid,'%d', PressA(1));
            if LP > 1
                n = 0;
                for i1 = 2:LP
                    fprintf(fid,',%d', PressA(i1));
                    if n > 9
                        fprintf(fid,'\n');
                        n = 0;
                    end
                    n = n+1;
                end
            end
            fprintf(fid,'};\n\n');
            
            % generate Temperature Axis
            fprintf(fid,'/* Y or Temperature axis definition */\n');
            fprintf(fid,'const double Y[%i] = {',LT);
            fprintf(fid,'%d', TempA(1));
            if LT > 1
                n = 0;
                for i1 = 2:LT
                    fprintf(fid,',%d', TempA(i1));
                    if n > 9
                        fprintf(fid,'\n');
                        n = 0;
                    end
                    n = n+1;
                end
            end
            fprintf(fid,'};\n\n');
            
            % generate Fractional  Axis
            fprintf(fid,'/* Z or fractional axis definition */\n');
            fprintf(fid,'const double Z[%i] = {',LF);
            fprintf(fid,'%f', Frc(1));
            if LF > 1
                n = 0;
                for i1 = 2:LF
                    fprintf(fid,',%f', Frc(i1));
                    if n > 9
                        fprintf(fid,'\n');
                        n = 0;
                    end
                    n = n+1;
                end
            end
            fprintf(fid,'};\n\n');
            
            % generate 3D lookup table M: X,Y,Z
            fprintf(fid,'/* M or 3D lookup table of S: X,Y,Z */\n');
            fprintf(fid,'const double M[%i] = {',LT*LP*LF);
            for i3 = 1:LF
                for i2 = 1:LT
                    for i1 = 1:LP
                        if i1*i2*i3 == 1
                            fprintf(fid,'%f', SA(1,1,1));
                        else
                            fprintf(fid,',%f', SA(i1,i2,i3));
                        end
                    end
                    fprintf(fid,'   /* entries 1,%i,%i to %i,%i,%i */\n',i2,i3,LP,i2,i3);
                end
                fprintf(fid,'\n');
            end
            fprintf(fid,'};\n\n');
            
            % set table lookup values
            fprintf(fid,'/* Pressure */\n');
            fprintf(fid,'xi = Pressure;\n');
            fprintf(fid,'/* Temperature */\n');
            fprintf(fid,'yi = Temperature;\n');
            fprintf(fid,'/* FAR */\n');
            fprintf(fid,'zi = FAR;\n');
            
            % write 3-D lookup function
            ThreeD(fid);
            
            fprintf(fid,'return P;\n');
            fprintf(fid,'}\n');
            
            
            fclose(fid);
            disp('pt2sc_TMATS.c created, Table generation complete');
            cd(OldFolder);
            
        case('SP2T')
            %% SP2T
            disp('generating sp2tc_TMATS.c file');
            
            % generate sp2tc_TMATS.h file
            OldFolder = cd('AlternateTables');
            fid = fopen('sp2tc_TMATS.c','w');
            fprintf(fid,'/*	T-MATS -- sp2tc_TMATS.c */\n\n');
            % send stream definitions to file
            fprintf(fid,'/* Stream 1 Name: ');
            for i = 1:length(Name1)
                fprintf(fid,'%s, ',char(Name1(i)));
            end
            fprintf(fid,'*/\n/* Stream 1 Species: ');
            for i = 1:length(Species1);
                fprintf(fid,'%f, ',Species1(i));
            end
            fprintf(fid,'*/\n/* Stream 2 Name: ');
            for i = 1:length(Name2)
                fprintf(fid,'%s, ',char(Name2(i)));
            end
            fprintf(fid,'*/\n/* Stream 2 Species: ');
            for i = 1:length(Species2);
                fprintf(fid,'%f, ',Species2(i));
            end
            fprintf(fid,'*/\n\n');
            
            fprintf(fid,'#include <stdio.h>\n');
            fprintf(fid,'#include <stdlib.h>\n');
            fprintf(fid,'#include "simstruc.h"\n');
            fprintf(fid,'#include <math.h>\n');
            
            fprintf(fid,'double sp2tc(double Entropy, double Pressure, double FAR)\n{\n');
            ThreeD_D(fid);
            
            fprintf(fid,'/*generate vector length constants */\n');
            fprintf(fid,'const int Lx = %i;\n',LP);
            fprintf(fid,'const int Ly = %i;\n',LT);
            fprintf(fid,'const int Lz = %i;\n\n',LF);
            
            % generate Pressure Axis
            fprintf(fid,'/* X or Pressure axis definition */\n');
            fprintf(fid,'const double X[%i] = {',LP);
            fprintf(fid,'%d', PressA(1));
            if LP > 1
                n = 0;
                for i1 = 2:LP
                    fprintf(fid,',%d', PressA(i1));
                    if n > 9
                        fprintf(fid,'\n');
                        n = 0;
                    end
                    n = n+1;
                end
            end
            fprintf(fid,'};\n\n');
            
            % generate Temperature Axis
            fprintf(fid,'/* Y or Temperature axis definition */\n');
            fprintf(fid,'const double Y[%i] = {',LT);
            fprintf(fid,'%d', TempA(1));
            if LT > 1
                n = 0;
                for i1 = 2:LT
                    fprintf(fid,',%d', TempA(i1));
                    if n > 9
                        fprintf(fid,'\n');
                        n = 0;
                    end
                    n = n+1;
                end
            end
            fprintf(fid,'};\n\n');
            
            % generate Fractional  Axis
            fprintf(fid,'/* Z or fractional axis definition */\n');
            fprintf(fid,'const double Z[%i] = {',LF);
            fprintf(fid,'%f', Frc(1));
            if LF > 1
                n = 0;
                for i1 = 2:LF
                    fprintf(fid,',%f', Frc(i1));
                    if n > 9
                        fprintf(fid,'\n');
                        n = 0;
                    end
                    n = n+1;
                end
            end
            fprintf(fid,'};\n\n');
            
            % generate 3D lookup table M: X,Y,Z
            fprintf(fid,'/* M or 3D lookup table of S: X,Y,Z */\n');
            fprintf(fid,'const double M[%i] = {',LT*LP*LF);
            for i3 = 1:LF
                for i2 = 1:LT
                    for i1 = 1:LP
                        if i1*i2*i3 == 1
                            fprintf(fid,'%f', SA(1,1,1));
                        else
                            fprintf(fid,',%f', SA(i1,i2,i3));
                        end
                    end
                    fprintf(fid,'   /* entries 1,%i,%i to %i,%i,%i */\n',i2,i3,LP,i2,i3);
                end
                fprintf(fid,'\n');
            end
            fprintf(fid,'};\n\n');
            
            % Temperature Guess
            fprintf(fid,'double Sg1, Tg1, dTdS, Tfin;\n');
            fprintf(fid,'int IterMax = 10;\n int iter = 0;\n double Sg = 1e3;\n double Stol = 1e-4;\n\n');
            
            % Set Table values
            fprintf(fid,'/* Pressure [psia]*/\n');
            fprintf(fid,'xi = Pressure;\n');
            fprintf(fid,'/* Temperature guess [degR]*/\n');
            fprintf(fid,'yi = 1000;\n');
            fprintf(fid,'/* FAR */\n');
            fprintf(fid,'zi = FAR;\n');
            
            % start while loop
            fprintf(fid,'while ((fabs(Entropy - P) >= Stol && iter < IterMax) || (iter < 2)){\n');
            
            % add 3-D table lookup
            ThreeD(fid)
            
            % compare guess with actual
            fprintf(fid,'if (iter==0){\n Sg1 = P;\nTg1 = yi;\nyi = Tg1+50;\n}\n');
            fprintf(fid,'else{\ndTdS = (yi-Tg1)/(P-Sg1);\nTg1 = yi;\nSg1 = P;\n');
            fprintf(fid,'yi = Tg1 + (Entropy-P)*dTdS;\n\n');
            fprintf(fid,'if (yi > Y[Ly-1]){\nyi = Y[Ly-1];}\n');
            fprintf(fid,'if (yi < Y[0]){\nyi = Y[0];}\n');
            fprintf(fid,'else if (!(yi >= Y[0])){\nyi = Y[0];}\n\n');
            fprintf(fid,'}\n++iter;\n}\n');
            fprintf(fid,'if (iter >= IterMax)\n');
            fprintf(fid,'printf("Warning in Entropy and Pressure to Temperature. Correct temperature could not be found\\n");\n');
            
            fprintf(fid,'Tfin = yi;\nreturn Tfin;\n }');
            fclose(fid);
            disp('sp2tc_TMATS.c created, Table generation complete');
            cd(OldFolder);
            
        case('T2H')
            %% T2H
            disp('generating t2hc_TMATS.c file');
            
            % generate t2hc_TMATS.c file
            OldFolder = cd('AlternateTables');
            fid = fopen('t2hc_TMATS.c','w');
            fprintf(fid,'/*	T-MATS -- t2hc_TMATS.c */\n\n');
            % send stream definitions to file
            fprintf(fid,'/* Stream 1 Name: ');
            for i = 1:length(Name1)
                fprintf(fid,'%s, ',char(Name1(i)));
            end
            fprintf(fid,'*/\n/* Stream 1 Species: ');
            for i = 1:length(Species1);
                fprintf(fid,'%f, ',Species1(i));
            end
            fprintf(fid,'*/\n/* Stream 2 Name: ');
            for i = 1:length(Name2)
                fprintf(fid,'%s, ',char(Name2(i)));
            end
            fprintf(fid,'*/\n/* Stream 2 Species: ');
            for i = 1:length(Species2);
                fprintf(fid,'%f, ',Species2(i));
            end
            fprintf(fid,'*/\n\n');
            
            fprintf(fid,'#include <stdio.h>\n');
            fprintf(fid,'#include <stdlib.h>\n');
            fprintf(fid,'#include "simstruc.h"\n');
            fprintf(fid,'#include <math.h>\n');
            
            fprintf(fid,'double t2hc(double Temperature, double FAR)\n{\n');
            TwoD_D(fid);
            
            fprintf(fid,'/*generate vector length constants */\n\n');
            fprintf(fid,'const int Lx = %i;\n',LT);
            fprintf(fid,'const int Ly = %i;\n\n',LF);
            
            % generate Temperature Axis
            fprintf(fid,'/* X or Temperature axis definition */\n');
            fprintf(fid,'const double X[%i] = {',LT);
            fprintf(fid,'%d', TempA(1));
            if LT > 1
                n = 0;
                for i1 = 2:LT
                    fprintf(fid,',%d', TempA(i1));
                    if n > 9
                        fprintf(fid,'\n');
                        n = 0;
                    end
                    n = n+1;
                end
            end
            fprintf(fid,'};\n\n');
            
            % generate Fractional  Axis
            fprintf(fid,'/* Y or fractional axis definition */\n');
            fprintf(fid,'const double Y[%i] = {',LF);
            fprintf(fid,'%f', Frc(1));
            if LF > 1
                n = 0;
                for i1 = 2:LF
                    fprintf(fid,',%f', Frc(i1));
                    if n > 9
                        fprintf(fid,'\n');
                        n = 0;
                    end
                    n = n+1;
                end
            end
            fprintf(fid,'};\n\n');
            
            % generate 2D lookup table M: X,Y
            fprintf(fid,'/* M or 2D lookup table of Enthalpy: X,Y */\n');
            fprintf(fid,'const double M[%i] = {',LT*LF);
            for i2 = 1:LF
                for i1 = 1:LT
                    if i1*i2 == 1
                        fprintf(fid,'%f', HA2D(1,1,1));
                    else
                        fprintf(fid,',%f', HA2D(1,i1,i2));
                    end
                end
                fprintf(fid,'   /* entries 1,%i to %i,%i */\n',i2,LT,i2);
            end
            fprintf(fid,'};\n\n');
            
            % set table lookup values
            fprintf(fid,'/* Temperature */\n');
            fprintf(fid,'xi = Temperature;\n');
            fprintf(fid,'/* FAR */\n');
            fprintf(fid,'yi = FAR;\n');
            
            % write 3-D lookup function
            TwoD(fid);
            
            fprintf(fid,'return P;\n');
            fprintf(fid,'}\n');
            
            fclose(fid);
            disp('t2hc_TMATS.h created, Table generation complete');
            cd(OldFolder);
            
        case('H2T')
            %% H2T
            disp('generating h2tc_TMATS.c file');
            
            % generate h2tc_TMATS.c file
            OldFolder = cd('AlternateTables');
            fid = fopen('h2tc_TMATS.c','w');
            fprintf(fid,'/*	T-MATS -- h2tc_TMATS.c */\n\n');
            % send stream definitions to file
            fprintf(fid,'/* Stream 1 Name: ');
            for i = 1:length(Name1)
                fprintf(fid,'%s, ',char(Name1(i)));
            end
            fprintf(fid,'*/\n/* Stream 1 Species: ');
            for i = 1:length(Species1);
                fprintf(fid,'%f, ',Species1(i));
            end
            fprintf(fid,'*/\n/* Stream 2 Name: ');
            for i = 1:length(Name2)
                fprintf(fid,'%s, ',char(Name2(i)));
            end
            fprintf(fid,'*/\n/* Stream 2 Species: ');
            for i = 1:length(Species2);
                fprintf(fid,'%f, ',Species2(i));
            end
            fprintf(fid,'*/\n\n');
            
            fprintf(fid,'#include <stdio.h>\n');
            fprintf(fid,'#include <stdlib.h>\n');
            fprintf(fid,'#include "simstruc.h"\n');
            fprintf(fid,'#include <math.h>\n');
            
            fprintf(fid,'double h2tc(double Enthalpy, double FAR)\n{\n');
            TwoD_D(fid);
            
            fprintf(fid,'/*generate vector length constants */\n\n');
            fprintf(fid,'const int Lx = %i;\n',LT);
            fprintf(fid,'const int Ly = %i;\n\n',LF);
            
            % generate Temperature Axis
            fprintf(fid,'/* X or Temperature axis definition */\n');
            fprintf(fid,'const double X[%i] = {',LT);
            fprintf(fid,'%d', TempA(1));
            if LT > 1
                n = 0;
                for i1 = 2:LT
                    fprintf(fid,',%d', TempA(i1));
                    if n > 9
                        fprintf(fid,'\n');
                        n = 0;
                    end
                    n = n+1;
                end
            end
            fprintf(fid,'};\n\n');
            
            % generate Fractional  Axis
            fprintf(fid,'/* Y or fractional axis definition */\n');
            fprintf(fid,'const double Y[%i] = {',LF);
            fprintf(fid,'%f', Frc(1));
            if LF > 1
                n = 0;
                for i1 = 2:LF
                    fprintf(fid,',%f', Frc(i1));
                    if n > 9
                        fprintf(fid,'\n');
                        n = 0;
                    end
                    n = n+1;
                end
            end
            fprintf(fid,'};\n\n');
            
            % generate 2D lookup table M: X,Y
            fprintf(fid,'/* M or 2D lookup table of Enthalpy: X,Y */\n');
            fprintf(fid,'const double M[%i] = {',LT*LF);
            for i2 = 1:LF
                for i1 = 1:LT
                    if i1*i2 == 1
                        fprintf(fid,'%f', HA2D(1,1,1));
                    else
                        fprintf(fid,',%f', HA2D(1,i1,i2));
                    end
                end
                fprintf(fid,'   /* entries 1,%i to %i,%i */\n',i2,LT,i2);
            end
            fprintf(fid,'};\n\n');
            
            % Temperature Guess
            fprintf(fid,'double Hg1, Tg1, dTdH, Tfin;\n');
            fprintf(fid,'int IterMax = 10;\n int iter = 0;\n double Hg = 1e3;\n double Htol = 1e-4;\n\n');
            
            % set guess table lookup values
            fprintf(fid,'/* Temperature */\n');
            fprintf(fid,'xi = 1000;\n');
            fprintf(fid,'/* FAR */\n');
            fprintf(fid,'yi = FAR;\n');
            
            % start while loop
            fprintf(fid,'while ((fabs(Enthalpy - P) >= Htol && iter < IterMax) || (iter < 2)){\n');
            
            % add 2-D table lookup
            TwoD(fid)
            
            % compare guess with actual
            fprintf(fid,'if (iter==0){\n Hg1 = P;\nTg1 = xi;\nxi = Tg1+50;\n}\n');
            fprintf(fid,'else{\ndTdH = (xi-Tg1)/(P-Hg1);\nTg1 = xi;\nHg1 = P;\n');
            fprintf(fid,'xi = Tg1 + (Enthalpy-P)*dTdH;\n\n');
            fprintf(fid,'if (xi > X[Lx-1]){\nxi = X[Lx-1];}\n');
            fprintf(fid,'else if (xi < X[0]){\nxi = X[0];}\n');
            fprintf(fid,'else if (!(xi >= X[0])){\nxi = X[0];}\n\n');
            fprintf(fid,'}\n++iter;\n}\n');
            fprintf(fid,'if (iter >= IterMax)\n');
            fprintf(fid,'printf("Warning in Enthalpy to Temperature. Correct temperature could not be found\\n");\n');
            fprintf(fid,'Tfin = xi;\nreturn Tfin;\n }');
            
            fclose(fid);
            disp('h2tc_TMATS.c created, Table generation complete');
            cd(OldFolder);
    end
end
cleanup

function ThreeD_D(fid)
OldFolder = cd('FileTemplates');
text = fileread('3D_append_Define.c');
fprintf(fid,text);
cd(OldFolder);

function ThreeD(fid)
OldFolder = cd('FileTemplates');
text = fileread('3D_append.c');
fprintf(fid,text);
cd(OldFolder);

function TwoD_D(fid)
OldFolder = cd('FileTemplates');
text = fileread('2D_append_Define.c');
fprintf(fid,text);
cd(OldFolder);

function TwoD(fid)
OldFolder = cd('FileTemplates');
text = fileread('2D_append.c');
fprintf(fid,text);
cd(OldFolder);














