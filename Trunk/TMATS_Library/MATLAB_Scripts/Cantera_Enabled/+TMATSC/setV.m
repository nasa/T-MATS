function setV( var, parent, value  )
% set variable to the workspace
% [val] = setV(var,parent,value)
    temp = sprintf( '%s%s', parent, var ); 
    assignin( 'base', temp, value );