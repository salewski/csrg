/*-
 * Copyright (c) 1993 The Regents of the University of California.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *	This product includes software developed by the University of
 *	California, Berkeley and its contributors.
 * 4. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#ifndef lint
char copyright[] =
"@(#) Copyright (c) 1993 The Regents of the University of California.\n\
 All rights reserved.\n";
#endif /* not lint */

#ifndef lint
static char sccsid[] = "@(#)uname.c	5.2 (Berkeley) 03/05/93";
#endif /* not lint */

#include <sys/types.h>
#include <sys/sysctl.h>

#include <err.h>
#include <stdio.h>
#include <stdlib.h>

void usage __P((void));

int
main(argc, argv)
	int argc;
	char *argv[];
{
#define	MFLAG	0x01
#define	NFLAG	0x02
#define	RFLAG	0x04
#define	SFLAG	0x08
#define	VFLAG	0x10
	u_int flags;
	int ch, name[2];
	size_t len, tlen;
	char *p, *prefix, buf[1024];

	flags = 0;
	while ((ch = getopt(argc, argv, "amnrsv")) != EOF)
		switch(ch) {
		case 'a':
			flags |= (MFLAG | NFLAG | RFLAG | SFLAG | VFLAG);
			break;
		case 'm':
			flags |= MFLAG;
			break;
		case 'n':
			flags |= NFLAG;
			break;
		case 'r':
			flags |= RFLAG;
			break;
		case 's':
			flags |= SFLAG;
			break;
		case 'v':
			flags |= VFLAG;
			break;
		case '?':
		default:
			usage();
		}

	argc -= optind;
	argv += optind;

	if (argc)
		usage();

	if (!flags)
		flags |= SFLAG;

	prefix = "";

	if (flags & SFLAG) {
		name[0] = CTL_KERN;
		name[1] = KERN_OSTYPE;
		len = sizeof(buf);
		if (sysctl(name, 2, &buf, &len, NULL, 0) == -1)
			err(1, "sysctl");
		(void)printf("%s%.*s", prefix, len, buf);
		prefix = " ";
	}
	if (flags & NFLAG) {
		name[0] = CTL_KERN;
		name[1] = KERN_HOSTNAME;
		len = sizeof(buf);
		if (sysctl(name, 2, &buf, &len, NULL, 0) == -1)
			err(1, "sysctl");
		(void)printf("%s%.*s", prefix, len, buf);
		prefix = " ";
	}
	if (flags & RFLAG) {
		name[0] = CTL_KERN;
		name[1] = KERN_OSRELEASE;
		len = sizeof(buf);
		if (sysctl(name, 2, &buf, &len, NULL, 0) == -1)
			err(1, "sysctl");
		(void)printf("%s%.*s", prefix, len, buf);
		prefix = " ";
	}
	if (flags & VFLAG) {
		name[0] = CTL_KERN;
		name[1] = KERN_VERSION;
		len = sizeof(buf);
		if (sysctl(name, 2, &buf, &len, NULL, 0) == -1)
			err(1, "sysctl");
		for (p = buf, tlen = len; tlen--; ++p)
			if (*p == '\n' || *p == '\t')
				*p = ' ';
		(void)printf("%s%.*s", prefix, len, buf);
		prefix = " ";
	}
	if (flags & MFLAG) {
		name[0] = CTL_HW;
		name[1] = HW_MACHINE;
		len = sizeof(buf);
		if (sysctl(name, 2, &buf, &len, NULL, 0) == -1)
			err(1, "sysctl");
		(void)printf("%s%.*s", prefix, len, buf);
		prefix = " ";
	}
	(void)printf("\n");
	exit (0);
}

void
usage()
{
	(void)fprintf(stderr, "usage: uname [-amnrsv]\n");
	exit(1);
}
