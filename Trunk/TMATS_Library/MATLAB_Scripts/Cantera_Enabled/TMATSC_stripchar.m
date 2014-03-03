function [namer] = TMATSC_stripchar( name )
lengtht = length( name );
ii = 1;
while  ii < (lengtht)
    if strcmp(name(ii), '/')
        name(ii)= '_';
    elseif strcmp(name(ii), '\')
        name(ii)= '_';
    elseif strcmp(name(ii), '\o')
        name(ii)= '';
    end
    ii= ii + 1;
end;
namer = name;