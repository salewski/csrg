/*
 * Copyright (c) 1982 Regents of the University of California.
 * All rights reserved.  The Berkeley software License Agreement
 * specifies the terms and conditions for redistribution.
 */

#ifndef lint
static char sccsid[] = "@(#)table.c	5.1 (Berkeley) 11/26/86";
#endif not lint

# define	DEBUG

/*
 * @(#)table.c	1.1 (Berkeley) 4/1/82
 */

# include	"mille.h"

main() {

	reg int	i, j, count;

	printf("   %16s -> %5s %5s %4s %s\n", "Card", "cards", "count", "need", "opposite");
	for (i = 0; i < NUM_CARDS - 1; i++) {
		for (j = 0, count = 0; j < DECK_SZ; j++)
			if (Deck[j] == i)
				count++;
		printf("%2d %16s -> %5d %5d %4d %s\n", i, C_name[i], Numcards[i], count, Numneed[i], C_name[opposite(i)]);
	}
}

