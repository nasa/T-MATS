% set a the flow conditions based on h and p
function [outFlow] = TMATSC_set_hP( flow1, h, P  )



TMATSC_canteraload

% load in what we know
outFlow(1) = flow1(1);
outFlow(2) = flow1(2);
outFlow(3) = P;
outFlow(4) = h;
outFlow(5) = flow1(5);
outFlow(6) = flow1(6);
outFlow(7) = flow1(7);
outFlow(8) = flow1(8);
outFlow(9) = flow1(9);
outFlow(10) = flow1(10);
   


 %write out a mass composition array for cantera
   tempcomp = '';
   compname = {'', '', '', '', '', '', '', '', '', '', '', ''};
   fract = [0 0 0 0 0 0 0 0 0 0 0 0];
   numcurrent = 0;
   for cName = 1:6
       for cSpecies = 1:6
           if Species{cName,cSpecies}*outFlow(cName+4) > 0.00001
               numcurrent = numcurrent+1;
               fract(numcurrent)=Species{cName,cSpecies}*outFlow(cName+4);
               
               compname{numcurrent} = Name{cName,cSpecies};
               tempcomp = sprintf( '%s %s:%f', tempcomp, Name{cName,cSpecies},Species{cName,cSpecies}*outFlow(cName+4) );
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

% iterate on temperature until it matches the input enthalpy
Ttlast = -999;
lasterr=1;
count = 0; 
TtOut = outFlow(2);
while  abs (lasterr)>.000000001 && count < 50
   set(fs,'Y',tempcomp);
   set(fs, 'T', TtOut*5./9.,'P',P*6894.75729 );
   equilibrate(fs, 'TP');
   count= count+1;
   ht = enthalpy_mass( fs )*.0004302099943161011;
   root = ht-h;  
   [TtOut, Ttlast, lasterr] = TMATSC_iterSecant( root, TtOut, Ttlast, lasterr, .1 );  
end

% set the additional values that we do not know
s = entropy_mass( fs )*0.000238845896627;
rhot = density( fs )*.0624;
gamt =  cp_mass( fs )/ cv_mass( fs );
outFlow(2) = TtOut;
outFlow(11) = s;
outFlow(12) = rhot;
outFlow(20) = gamt;
outFlow(25) = 0;

fclose( 'all' );

