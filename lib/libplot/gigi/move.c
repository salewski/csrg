#ifndef lint
static char sccsid[] = "@(#)move.c	4.1 (Berkeley) 11/10/83";
#endif

#include "gigi.h"

move(xi,yi)
int xi,yi;
{
	currentx = xsc(xi);
	currenty = ysc(yi);
	printf("P[%d,%d]",currentx, currenty);
}
