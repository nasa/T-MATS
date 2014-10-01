function [val] = getV( var, parent  )
    temp = sprintf( '%s%s', parent, var );
    val = evalin( 'base', temp );
