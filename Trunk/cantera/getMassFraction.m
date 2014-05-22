% set a the flow conditions based on h and p
function [fraction] = getMassFraction( flow1, element )



canteraload


   


 %write out a mass composition array for cantera
   tempcomp = '';
   compname = {'', '', '', '', '', '', '', '', '', '', '', ''};
   fract = [0 0 0 0 0 0 0 0 0 0 0 0]
   numcurrent = 0;
   for cName = 1:6
       for cSpecies = 1:6
           if Species{cName,cSpecies}*flow1(cName+4) > 0.00001
               numcurrent = numcurrent+1;
               fract(numcurrent)=Species{cName,cSpecies}*flow1(cName+4);
               fprintf('%g\n',numcurrent );
               
               compname{numcurrent} = Name{cName,cSpecies};
               tempcomp = sprintf( '%s %s:%f', tempcomp, Name{cName,cSpecies},Species{cName,cSpecies}*flow1(cName+4) );
           end
       end 
   end
   
   if ( numcurrent > 1 )
   for count1 = numcurrent:1;
      for  cont2 = count1-1:1
          if strcmp( compname(count2),compname(cont1) )
              fract(count2)=fract(count2)+fract(count1);
          else
              tempcomp = sprintf( '%s %s:%f', tempcomp,compname(count1),fract(count1) );
          end
      end
   end
   end
   
   set( fs, 'Y',tempcomp);

% iterate on temperature until it matches the input enthalpy
Ttlast = -999;
lasterr=1;
count = 0; 
TtOut = flow1(2);
h = flow1(4);
while  abs (lasterr)>.000000001 & count < 50
   set(fs,'Y',tempcomp);
   set(fs, 'T', TtOut*5./9.,'P',flow1(3)*6894.75729 );
   equilibrate(fs, 'TP');
   count= count+1;
   ht = enthalpy_mass( fs )*.0004302099943161011;
   root = ht-h;  
   [TtOut, Ttlast, lasterr] = iterSecant( root, TtOut, Ttlast, lasterr, .1 );  
end

fraction = massFraction( fs, element )

fclose( 'all' );

