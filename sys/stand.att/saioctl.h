/*
 * Copyright (c) 1988 Regents of the University of California.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms are permitted
 * provided that this notice is preserved and that due credit is given
 * to the University of California at Berkeley. The name of the University
 * may not be used to endorse or promote products derived from this
 * software without specific prior written permission. This software
 * is provided ``as is'' without express or implied warranty.
 *
 *	@(#)saioctl.h	7.2 (Berkeley) 03/02/88
 */

/* ioctl's -- for disks just now */
#define	SAIOHDR		(('d'<<8)|1)	/* next i/o includes header */
#define	SAIOCHECK	(('d'<<8)|2)	/* next i/o checks data */
#define	SAIOHCHECK	(('d'<<8)|3)	/* next i/o checks header & data */
#define	SAIONOBAD	(('d'<<8)|4)	/* inhibit bad sector forwarding */
#define	SAIODOBAD	(('d'<<8)|5)	/* enable bad sector forwarding */
#define	SAIOECCLIM	(('d'<<8)|6)	/* set limit to ecc correction, bits */
#define	SAIOECCUNL	(('d'<<8)|7)	/* use standard ecc procedures */
#define	SAIORETRIES	(('d'<<8)|8)	/* set retry count for unit */
#define	SAIODEVDATA	(('d'<<8)|9)	/* get pointer to pack label */
#define	SAIOSSI		(('d'<<8)|10)	/* set skip sector inhibit */
#define	SAIONOSSI	(('d'<<8)|11)	/* inhibit skip sector handling */
#define	SAIOSSDEV	(('d'<<8)|12)	/* is device skip sector type? */
#define	SAIODEBUG	(('d'<<8)|13)	/* enable/disable debugging */
#define	SAIOGBADINFO	(('d'<<8)|14)	/* get bad-sector table */
