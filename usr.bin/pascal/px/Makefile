#	@(#)Makefile	8.1 (Berkeley) 06/06/93

PROG=	px
SRCDIR=	${.CURDIR}/../src
CFLAGS+=-I. -I${.CURDIR}/../libpc -I${SRCDIR}
.PATH:	${SRCDIR}
SRCS=	int.c interp.c utilities.c Version.c
DPADD=	${LIBPC} ${LIBM} ${LIBCOMPAT}
LDADD=	-lpc -lm -lcompat
CLEANFILES+=h02opcs.h opc version Version.c

h02opcs.h: opc ${SRCDIR}/OPnames.h
	./opc > ${.TARGET}

opc: opc.c
	${CC} ${CFLAGS} ${.ALLSRC} -o ${.TARGET}

Version.c: version
	./version > ${.TARGET}

version: version.c
	${CC} ${CFLAGS} ${.CURDIR}/version.c -o ${.TARGET}

.include <bsd.prog.mk>

${OBJS} .depend: h02opcs.h
