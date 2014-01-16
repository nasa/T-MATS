
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%  add two flows together
%  return the values to a thirds
%  assume that the pressure remains and the same pressure as stream1
%  mass average the composition and enthalpy
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

function [outFlow] = add( flow1, flow2 )

   canteraload
   % add the weight flows
   outFlow(1) = flow1(1) + flow2(1);
  
   %keep the pressure at the primary flow pressure
   outFlow(3) = flow1(3);
   
   %average the composition values
   outFlow(4) = (flow1(1)*flow1(4)+flow2(1)*flow2(4))/outFlow(1);
   outFlow(5) = (flow1(1)*flow1(5)+flow2(1)*flow2(5))/outFlow(1);
   outFlow(6) = (flow1(1)*flow1(6)+flow2(1)*flow2(6))/outFlow(1);
   outFlow(7) = (flow1(1)*flow1(7)+flow2(1)*flow2(7))/outFlow(1);
   outFlow(8) = (flow1(1)*flow1(8)+flow2(1)*flow2(8))/outFlow(1);
   outFlow(9) = (flow1(1)*flow1(9)+flow2(1)*flow2(9))/outFlow(1);
   outFlow(10) = (flow1(1)*flow1(10)+flow2(1)*flow2(10))/outFlow(1);
   
   %write out a mass composition array for cantera
   tempcomp = '';
   compname = {'', '', '', '', '', '', '', '', '', '', '', ''};
   fract = [0 0 0 0 0 0 0 0 0 0 0 0]
   numcurrent = 0;
   for cName = 1:6
       for cSpecies = 1:6
           if Species{cName,cSpecies}*outFlow(cName+4) > 0.00001
               numcurrent = numcurrent+1;
               fract(numcurrent)=Species{cName,cSpecies}*outFlow(cName+4);
               fprintf('%g\n',numcurrent );
               
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
   
   %iterate on temperature until you get the desired enthalpy
   Ttlast = -999;
   lasterr=1;
   count = 0; 
   TtOut = (flow1(1)*flow1(2) + flow2(1)*flow2(2))/( outFlow(1) );
   htOut = outFlow(4);
   PtOut = outFlow(3);
   while  abs (lasterr)>.000000001 & count < 50
      set(fs,'Y',tempcomp);
      set(fs, 'T', TtOut*5./9.,'P',PtOut*6894.75729 );
      equilibrate(fs, 'TP');
      ht = enthalpy_mass( fs )*.0004302099943161011;
      root = ht-htOut;  
      [TtOut, Ttlast, lasterr] = iterSecant( root, TtOut, Ttlast, lasterr, .1 );  
   end

   %set enthalpy an temperature in output
   outFlow(2) = TtOut;
   s = entropy_mass( fs )*0.000238845896627;
   outFlow(11) = s;
   %size the array to 25
   outFlow(25) = 0;
  
   fclose( 'all' );
