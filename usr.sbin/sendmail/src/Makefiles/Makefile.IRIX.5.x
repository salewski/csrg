#
#  This Makefile is designed to work on the old "make" program.  It does
#  not use the obj subdirectory.  It also does not install documentation
#  automatically -- think of it as a quick start for sites that have the
#  old make program (I recommend that you get and port the new make if you
#  are going to be doing any signficant work on sendmail).
#
#  Tested on IRIX 5.3 by Kari E. Hurtta <Kari.Hurtta@fmi.fi>.
#
#	@(#)Makefile.IRIX.5.x	8.4 (Berkeley) 06/20/95
#
SHELL=	/bin/sh

# use O=-O (usual) or O=-g (debugging)
O=	-O

# define the database mechanisms available for map & alias lookups:
#	-DNDBM -- use new DBM
#	-DNEWDB -- use new Berkeley DB (requires -ldb)
#	-DNIS -- include NIS support (requires -lsun)
# The really old (V7) DBM library is no longer supported.
# See READ_ME for a description of how these flags interact.
#
DBMDEF=	-DNDBM -DNIS

# environment definitions (e.g., -D_AIX3)
ENVDEF= -DIRIX5

# see also conf.h for additional compilation flags

# include directories
INCDIRS=

# library directories
LIBDIRS=

# libraries required on your system
LIBS= -lmld -lmalloc -lsun

# location of sendmail binary (usually /usr/sbin or /usr/lib)
BINDIR=	${DESTDIR}/usr/lib

# location of sendmail.st file (usually /var/log or /usr/lib)
STDIR=	${DESTDIR}/usr/lib

# location of sendmail.hf file (usually /usr/share/misc or /usr/lib)
HFDIR=	${DESTDIR}/usr/lib

# additional .o files needed
OBJADD=

###################  end of user configuration flags  ######################

CFLAGS=	-I. $O ${INCDIRS} ${DBMDEF} ${ENVDEF}

OBJS=	alias.o arpadate.o clock.o collect.o conf.o convtime.o daemon.o \
	deliver.o domain.o envelope.o err.o headers.o macro.o main.o \
	map.o mci.o mime.o parseaddr.o queue.o readcf.o recipient.o \
	savemail.o srvrsmtp.o stab.o stats.o sysexits.o \
	trace.o udb.o usersmtp.o util.o version.o ${OBJADD}

LINKS=	${DESTDIR}/usr/bsd/newaliases ${DESTDIR}/usr/bsd/mailq
BINOWN=	root
BINGRP=	sys
BINMODE=6555

ALL=	sendmail aliases.0 mailq.0 newaliases.0 sendmail.0

all: ${ALL}

sendmail: ${BEFORE} ${OBJS}
	${CC} -o sendmail ${OBJS} ${LIBDIRS} ${LIBS}

#NROFF=	nroff -h
NROFF=	groff -Tascii
MANDOC=	-mandoc

aliases.0: aliases.5
	${NROFF} ${MANDOC} aliases.5 > aliases.0

mailq.0: mailq.1
	${NROFF} ${MANDOC} mailq.1 > mailq.0

newaliases.0: newaliases.1
	${NROFF} ${MANDOC} newaliases.1 > newaliases.0

sendmail.0: sendmail.8
	${NROFF} ${MANDOC} sendmail.8 > sendmail.0

install: install-sendmail install-docs

install-sendmail: sendmail
	install -u ${BINOWN} -g ${BINGRP} -m ${BINMODE} -f ${BINDIR} sendmail
	for i in ${LINKS}; do rm -f $$i; ln -s ${BINDIR}/sendmail $$i; done
	cp /dev/null ${STDIR}/sendmail.st
	chmod 644 ${STDIR}/sendmail.st
	chown ${BINOWN} ${STDIR}/sendmail.st
	chgrp ${BINGRP} ${STDIR}/sendmail.st
	install -u ${BINOWN} -g ${BINGRP} -m 444 -f ${HFDIR} sendmail.hf

# doesn't actually install them -- you may want to install pre-nroff versions
install-docs: aliases.0 mailq.0 newaliases.0 sendmail.0

clean:
	rm -f ${OBJS} sendmail aliases.0 mailq.0 newaliases.0 sendmail.0

# dependencies
#   gross overkill, and yet still not quite enough....
${OBJS}: sendmail.h conf.h

depend:
