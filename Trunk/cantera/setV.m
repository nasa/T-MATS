function setV( var, parent, value  )
    temp = sprintf( '%s%s', parent, var ); 
    assignin( 'base', temp, value );