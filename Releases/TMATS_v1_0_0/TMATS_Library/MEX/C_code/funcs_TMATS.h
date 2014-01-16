#ifndef TMATS_FUNCS_H
#define TMATS_FUNCS_H

/*		T-MATS -- funcs_TMATS.h
 * % *************************************************************************
 * % written by Jeffryes Chapman bsed on work by Ryan May
 * % NASA Glenn Research Center, Cleveland, OH
 * % February 14th, 2013
 * %
 * %  This is a file that contains various constants for use in the T-MATS C-code
 * % *************************************************************************/

/* create absolute value function for doubles */
double abs_D(double a)
{
    double b;
    b = a;
    if(a < 0)
        b = -a;
    return b;
}



#endif  /* TMATS_FUNCS_H */
