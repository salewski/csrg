/*
 * Copyright (c) 1983 Regents of the University of California.
 * All rights reserved.  The Berkeley software License Agreement
 * specifies the terms and conditions for redistribution.
 */

#ifdef SYSLIBC_SCCS
_sccsid:.asciz	"@(#)getpid.s	5.3 (Berkeley) 03/09/86"
#endif SYSLIBC_SCCS

#include "SYS.h"

SYSCALL(getpid)
	ret		# pid = getpid();
