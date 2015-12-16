
	/*********************************/
	/*check for out of bounds errors */
	/*********************************/

	/* Check to see if zi is outside of X vector */
	if (xi < X[0]){
        errValue = 1;
		xi = X[0];
    }
    else if (xi > X[Lx-1]){
        errValue = 1;
		xi = X[Lx-1];
    }
	else if (!(xi >= X[0]))
	{
		/* xi must be NaN to have failed by < X[0]  and >= X[0] */
		errValue = 1;
		xi = X[0];
	}

	/* Check to see if zi is outside of Y vector */
	if (yi < Y[0]){
        errValue = 1;
		yi = Y[0];
    }
    else if (yi > Y[Ly-1]){
        errValue = 1;
		yi = Y[Ly-1];
    }
	else if (!(yi >= Y[0]))
	{
		/* yi must be NaN to have failed by < Y[0]  and >= Y[0] */
		errValue = 1;
		yi = Y[0];
	}

    /* Search for starting point */
	i = Lx-2;
	j = Ly-2;
	while (i >= 0){
		if (xi >= X[i]){
			ii = i;
			break;
		}
		else
			i = i - 1;
    }

	while (j >= 0){
		if (yi >= Y[j]){
			jj = j;
			break;
		}
		else
			j = j - 1;
    }


	/*-------------------------------------------------------------- */

	Lxy = Lx*Ly;

	/* JL
	Find the values at the vertices of the bounding cube

	Assume M is structured as follows:
	First increment x values with others constant
	Then increment y once, and then increment x values
	When done with y values, finally increment z once
	Repeat until z is done */
	v00 = M[jj*Lx + ii];
	v10 = M[jj*Lx + (ii+1)];
	v01 = M[(jj+1)*Lx + ii];
	v11 = M[(jj+1)*Lx + (ii+1)];

	/* Normalize/translate coords xi,yi so that they go from 0 to 1 within bounding square */
	xx = (xi-X[ii])/(X[ii+1]-X[ii]);
	yy = (yi-Y[jj])/(Y[jj+1]-Y[jj]);
	/* 2D interpolation */
	P = v00*(1-xx)*(1-yy) + v10*xx*(1-yy) + v01*(1-xx)*yy + v11*xx*yy;

        if (errValue == 1)
            printf("Warning in Temperature to Enthalpy lookup, lookup tables should be expanded\\n");

    errValue = 0;

