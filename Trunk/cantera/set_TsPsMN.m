% set flow conditions based on Ts, Ps, and MN
function [outFlow] = set_TsPsMN( Ts, Ps, MN, comp )

canteraload;

tempcomp = '';
W = 1;
Tt = 2;
Pt = 3;
ht = 4;
entropy = 11;
rhot = 12;
Tsind = 13;
Psind = 14;
hsind = 15;
rhosind = 16;
Vflowind = 17;
MNind = 18;
area = 19;

 %write out a mass composition array for cantera
   tempcomp = '';
   compname = {'', '', '', '', '', '', '', '', '', '', '', ''};
   fract = [0 0 0 0 0 0 0 0 0 0 0 0]
   numcurrent = 0;
   for cName = 1:6
       for cSpecies = 1:6
           if Species{cName,cSpecies}*comp(cName) > 0.00001
               numcurrent = numcurrent+1;
               fract(numcurrent)=Species{cName,cSpecies}*comp(cName);
               fprintf('%g\n',numcurrent );
               
               compname{numcurrent} = Name{cName,cSpecies};
               tempcomp = sprintf( '%s %s:%f', tempcomp, Name{cName,cSpecies},Species{cName,cSpecies}*comp(cName) );
           end
       end 
   end
   
   for count1 = numcurrent:1;
      for  cont2 = count1-1:1
          if strcmp( compname(count2),compname(cont1) )
              fract(count2)=fract(count2)+fract(count1);
          else
              tempcomp = sprintf( '%s %s:%f', tempcomp,compname(count1),fract(count1) );
          end
      end
   end
   
   set( fs, 'Y',tempcomp);

% set the static state based on temperature and pressure
set( fs, 'T',  Ts*5./9.,'P',Ps*6894.75729 );
equilibrate( fs, 'TP' );

% determine static values
hs = enthalpy_mass( fs )*.0004302099943161011;
rhos = density( fs )*.0624;
MMW = meanMolecularWeight(fs );
gams =  cp_mass( fs )/ cv_mass( fs );
s = entropy_mass( fs ) *0.000238845896627;
   
% calculate total enthalpy
Vson = sqrt( gams*Ts*1544/MMW*32.174);
Vflow = MN*Vson;
ht = hs + (Vflow)^2/(  778.169 * 32.1740 * 2 );

% iterate on total pressure until the entropy of the total state
% matches the static state
Pt = Ps*( 1+( gams - 1 ) /2.* MN^2 )^( gams /( gams -1 ));
PtLast = -9999;
lasterr=1;
count = 0;
err=0; 
while  abs (lasterr)>.0000001 & count < 50 
     set( fs, 'H', ht/0.0004302099943161011, 'P',Pt*6894.75729 );
     equilibrate( fs, 'HP' );
     root = s - entropy_mass( fs )*0.000238845896627;
     count = count + 1;
     Tt = temperature( fs )*9./5.;
     [Pt, PtLast, lasterr] = iterSecant( root, Pt, PtLast, lasterr, .1 );
end

% load the values into the output
rhot = density( fs )*.0624;
gamt =  cp_mass( fs )/ cv_mass( fs );
area = 0;
outFlow(2) = Tt;
outFlow(3) = Pt;
outFlow(4) = ht;
outFlow(5) = comp(1);
outFlow(6) = comp(2);
outFlow(7) = comp(3);
outFlow(8) = comp(4);
outFlow(9) = comp(5);
outFlow(10) = comp(6);
outFlow(11) = s;
outFlow(12) = rhot;
outFlow(13) = Ts;
outFlow(14) = Ps;
outFlow(15) = hs;
outFlow(16) = rhos;
outFlow(17) = Vflow;
outFlow(18) = MN;
outFlow(19) = area;
outFlow(20) = gamt;
outFlow(21) = gams;

outFlow(22) = 0;
outFlow(23) = 0;
outFlow(24) = 0;
outFlow(25) = 0;
fclose( 'all' );
