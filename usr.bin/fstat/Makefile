#	@(#)Makefile	8.1 (Berkeley) 06/06/93

PROG=	fstat
CFLAGS+=-I/sys
DPADD=	${LIBKVM}
LDADD=	-lkvm
BINGRP=	kmem
BINMODE=2555

.include <bsd.prog.mk>
