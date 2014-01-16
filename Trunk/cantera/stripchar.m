function [namer] = stripchar( name )
    lengtht = length( name );
    ii = 1;
    while  ii < (lengtht)
        if name(ii)== '/'
            name(ii)= '_';
        end
       if name(ii)== '\o';
            name(ii)= '';
        end
    ii= ii + 1;
    end;
    namer = name;