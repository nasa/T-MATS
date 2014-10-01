function [effMap,PRmap,WcMap, stallMargin] = RlineMap(MapFile, NcMapIn, RlineIn )
    % determine the name of this instance of the map
    tempparent = gcb();
    
    % clean up the name for characters that dont work
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
%     tempparent(ii) = '';
    
    %create names to store the map values
    tempR = sprintf( '%s_RlineArray', tempparent );
    tempN = sprintf( '%s_NcorrArray', tempparent );  
    tempWflow = sprintf( '%s_WflowArray', tempparent ); 
    tempEff = sprintf( '%s_EffArray', tempparent ); 
    tempPR = sprintf( '%s_PRarray', tempparent );
    
    % if the variable does not exist, the map has not been read in
    try
        TMATSC_getV(tempR,'');
        check = 1;
    catch
        check = 0;
    end
    
    if  check==0
       % read in the map
       fileID = fopen(MapFile);
       tline = fgetl(fileID);
       read = sscanf( tline(1:end),'%i' );
       Rnum = read(1);
       Nnum = read(2);
       tline = fgetl(fileID);
       Rlines = sscanf(tline(1:end), '%f %f');
       N =[];
       Wc = zeros( Nnum, Rnum );
       eff = zeros( Nnum, Rnum );
       PR = zeros( Nnum, Rnum );
       for i = 1:Nnum 
          tline = fgetl(fileID);       
          Ntemp = sscanf(tline(1:end), '%f');
          N = [N Ntemp];
          tline = fgetl(fileID);
          WcTemp = sscanf(tline(1:end), '%f',Rnum);
          tline = fgetl(fileID);
          effTemp = sscanf(tline(1:end), '%f', Rnum);
          tline = fgetl(fileID);
          PRtemp = sscanf(tline(1:end), '%f', Rnum);
          Wc(i,:) = WcTemp;
          eff(i,:) = effTemp;
          PR(i,:)=PRtemp;
       end
       % store the map in the workspace
       assignin( 'base', tempR, Rlines );
       assignin( 'base', tempN, N );
       assignin( 'base', tempWflow, Wc );       
       assignin( 'base', tempEff, eff );
       assignin( 'base', tempPR, PR );       
    end
 
    % grab the map value from the workspace
    Rline = evalin( 'base', tempR );
    N = evalin( 'base', tempN );
    eff = evalin( 'base', tempEff );
    Wc = evalin( 'base', tempWflow );
    PR = evalin( 'base', tempPR );

    % interpolate the map
    PRmap = interp2(Rline,N,PR,RlineIn,NcMapIn ); 
    effMap = interp2(Rline,N,eff,RlineIn,NcMapIn );    
    WcMap = interp2(Rline,N,Wc,RlineIn,NcMapIn ); 

    %determine the stall margin
    PRstall = interp2(Rline,N,PR,1.0,NcMapIn ); 
    stallMargin = ( PRstall - PRmap )/ PRmap * 100.;
    
  
 
