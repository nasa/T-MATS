function [effMap,WcMap] = PRmapFile(MapFile, NcMapIn, PRmap  )
% Read turbine map file
% [effMap,WcMap] = PRmapFile(MapFile, NcMapIn, PRmap)
    tempparent = gcb();
    lengtht = length( tempparent );
    ii = 1;
    while  ii < (lengtht)

        if tempparent(ii)== '/'
            tempparent(ii)= '_';
        end
       if tempparent(ii)== '\o';
            tempparent(ii)= '';
        end
    ii= ii + 1;
    end
    tempparent(ii) = '';
    tempPR = sprintf( '%s_PRArray', tempparent );
    tempN = sprintf( '%s_NcorrArray', tempparent );  
    tempWflow = sprintf( '%s_WflowArray', tempparent ); 
    tempEff = sprintf( '%s_EffArray', tempparent ); 
    check = exist( tempPR );
    
    if  check==0
       fileID = fopen(MapFile);
       tline = fgetl(fileID);
       read = sscanf( tline(1:end),'%i' );
       Rnum = read(1);
       Nnum = read(2);
       tline = fgetl(fileID);
       PR = sscanf(tline(1:end), '%f %f');   
       N =[];
       Wc = zeros( Nnum, Rnum );
       eff = zeros( Nnum, Rnum );
       for i = 1:Nnum 
          tline = fgetl(fileID);
          Ntemp = sscanf(tline(1:end), '%f');
          N = [N Ntemp];
          tline = fgetl(fileID);
          WcTemp = sscanf(tline(1:end), '%f',Rnum);
          tline = fgetl(fileID);
          effTemp = sscanf(tline(1:end), '%f', Rnum);
          Wc(i,:) = WcTemp;
          eff(i,:) = effTemp;
       end
       assignin( 'base', tempPR, PR );
       assignin( 'base', tempN, N );
       assignin( 'base', tempWflow, Wc );       
       assignin( 'base', tempEff, eff );
       assignin( 'base', tempPR, PR );       
    end
    PR = evalin( 'base', tempPR );
    N = evalin( 'base', tempN );
    eff = evalin( 'base', tempEff );
    Wc = evalin( 'base', tempWflow );
     
    effMap = interp2(PR,N,eff,PRmap,NcMapIn );    
    WcMap = interp2(PR,N,Wc,PRmap,NcMapIn ); 
    
   
 
