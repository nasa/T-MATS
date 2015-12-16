
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
		// xi must be NaN to have failed by < X[0]  and >= X[0]
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
		// yi must be NaN to have failed by < Y[0]  and >= Y[0]
		errValue = 1;
		yi = Y[0];
	}

	/* Check to see if zi is outside of Z vector */
	if (zi < Z[0]){
        errValue = 1;
		zi = Z[0];
    }
    else if (zi > Z[Lz-1]){
        errValue = 1;
		zi = Z[Lz-1];
    }
	else if (!(zi >= Z[0]))
	{
		// zi must be NaN to have failed by < Z[0]  and >= Z[0]
		errValue = 1;
		zi = Z[0];
	}


	/* Search for starting point. Point 0,0,0 of a bounding cube  */

	/*JL: */
	/* Faster search routine which assumes equal spacing in X, Y, Z
	inc_X = (X[Lx-1]-X[0])/(Lx-1);
	inc_Y = (Y[Ly-1]-Y[0])/(Ly-1);
	inc_Z = (Z[Lz-1]-Z[0])/(Lz-1);
	ii = floor((xi-X[0])/inc_X);
	jj = floor((yi-Y[0])/inc_Y);
	kk = floor((zi-Z[0])/inc_Z); */

	/*JL: Brute force search routine */
	i = Lx-2;
	j = Ly-2;
	k = Lz-2;
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

	while (k >= 0){
		if (zi >= Z[k]){
			kk = k;
			break;
		}
		else
			k = k - 1;
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
	v000 = M[kk*Lxy + jj*Lx + ii];
	v100 = M[kk*Lxy + jj*Lx + (ii+1)];
	v010 = M[kk*Lxy + (jj+1)*Lx + ii];
	v001 = M[(kk+1)*Lxy + jj*Lx + ii];
	v101 = M[(kk+1)*Lxy + jj*Lx + (ii+1)];
	v011 = M[(kk+1)*Lxy + (jj+1)*Lx + ii];
	v110 = M[kk*Lxy + (jj+1)*Lx + (ii+1)];
	v111 = M[(kk+1)*Lxy + (jj+1)*Lx + (ii+1)];

	/* Normalize/translate coords xi,yi,zi so that they go from 0 to 1 within bounding cube */
	xx = (xi-X[ii])/(X[ii+1]-X[ii]);
	yy = (yi-Y[jj])/(Y[jj+1]-Y[jj]);
	zz = (zi-Z[kk])/(Z[kk+1]-Z[kk]);
	/* 3D interpolation */
	P = v000*(1-xx)*(1-yy)*(1-zz) + v100*xx*(1-yy)*(1-zz) + v010*(1-xx)*yy*(1-zz) + v001*(1-xx)*(1-yy)*zz + v101*xx*(1-yy)*zz + v011*(1-xx)*yy*zz + v110*xx*yy*(1-zz) + v111*xx*yy*zz;

        if (errValue == 1)
            printf("Warning in Pressure and Temperature to Entropy lookup, lookup tables should be expanded\\n");

    errValue = 0;
