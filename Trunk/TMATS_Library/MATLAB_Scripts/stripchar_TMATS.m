function [namer] = stripchar_TMATS( name )
lengtht = length( name );
ii = 1;
while  ii < (lengtht)
    if strcmp(name(ii), '/')
        name(ii)= '_';
    elseif strcmp(name(ii), '\')
        name(ii)= '_';
    elseif strcmp(name(ii), '\o')
        name(ii)= '';
    elseif strcmp(name(ii), ' ')
        name(ii)= '_';
    elseif strcmp(name(ii), sprintf('\n'))
        name(ii)= '_';
    end
    ii= ii + 1;
end;
namer = name;