function [outFlow] = TMATSC_set_SP( flow1, S, P  )

   TMATSC_canteraload;   
   
   outFlow(1) = flow1(1);
   outFlow(2) = flow1(2);
   outFlow(3) = P;
   outFlow(4) = flow1(4);
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
set(fs, 'S', S/0.000238845896627,'P',P*6894.75729 );
equilibrate(fs, 'SP');
outFlow(2)=temperature(fs)*9./5.;
outFlow(4)=enthalpy_mass(fs)*.0004302099943161011;
outFlow(12) = density( fs )*.0624;
outFlow(20) = cp_mass( fs )/ cv_mass( fs );

s = entropy_mass( fs )*0.000238845896627;

outFlow(11) = s;
outFlow(25) = 0;
fclose( 'all' );
