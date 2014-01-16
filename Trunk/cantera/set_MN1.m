% set the static conditions based on input pressure
function [outFlow] = set_MN1( flow1   )


canteraload;

W = 1;
Tt = 2;
Pt = 3;
ht = 4;
entropy = 11;
rhot = 12;
Ts = 13;
Ps = 14;
hs = 15;
rhos = 16;
Vflow = 17;
MNin = 18;
area = 19;
gamt = 20;
gams = 21;

outFlow(1) = flow1(1);
outFlow(2) = flow1(2);
outFlow(3) = flow1(3);
outFlow(4) = flow1(4);
outFlow(5) = flow1(5);
outFlow(6) = flow1(6);
outFlow(7) = flow1(7);
outFlow(8) = flow1(8);
outFlow(9) = flow1(9);
outFlow(10) = flow1(10);
outFlow(11) = flow1(11);
outFlow(12) = flow1(rhot);
outFlow(20) = flow1(gamt);



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


PsLast = -999;
lasterr=1;
count = 0; 


PsI = outFlow(Pt)/(1+ (outFlow(gamt)-1)/2)^(outFlow(gamt)/(outFlow(gamt)-1));

while  abs (lasterr)>.000000001 & count < 50

   % set the conditions based on entropy and input pressure   
   set( fs, 'S',outFlow(entropy)/0.000238845896627, 'P', PsI*6894.75729,'Y',tempcomp);
   equilibrate( fs, 'SP' );

   % set static conditions
   outFlow(gams) = cp_mass( fs )/ cv_mass( fs );
   outFlow(Ts)= temperature( fs )*9./5.;
   outFlow(hs) = enthalpy_mass( fs )*0.0004302099943161011;
   outFlow(rhos) = density( fs )*.0624;
   outFlow(Ps) = PsI;
   MMW = meanMolecularWeight(fs );
   Vson = sqrt( outFlow(gams)*outFlow(Ts)*5./9.*8314.4621/ MMW )*3.28084;
   outFlow(Vflow) =   sqrt(  778.169 * 32.1740 * 2. * ( outFlow(ht) - outFlow(hs) ));
   outFlow(MNin) = outFlow(Vflow)/Vson ;
   root = 1 - outFlow(Vflow)/Vson;  
   [PsI, PsLast, lasterr] = iterSecant( root, PsI, PsLast, lasterr, .1 );
end

outFlow(25) = 0;
fclose( 'all' );
