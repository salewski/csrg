/*
 * Copyright (c) 1980 Regents of the University of California.
 * All rights reserved.  The Berkeley software License Agreement
 * specifies the terms and conditions for redistribution.
 *
 *	@(#)dump.h	5.14 (Berkeley) 03/07/91
 */

#define MAXINOPB	(MAXBSIZE / sizeof(struct dinode))
#define MAXNINDIR	(MAXBSIZE / sizeof(daddr_t))

/*
 * Dump maps used to describe what is to be dumped.
 */
int	mapsize;	/* size of the state maps */
char	*usedinomap;	/* map of allocated inodes */
char	*dumpdirmap;	/* map of directories to be dumped */
char	*dumpinomap;	/* map of files to be dumped */
/*
 * Map manipulation macros.
 */
#define	SETINO(ino, map) \
	map[(u_int)((ino) - 1) / NBBY] |=  1 << ((u_int)((ino) - 1) % NBBY)
#define	CLRINO(ino, map) \
	map[(u_int)((ino) - 1) / NBBY] &=  ~(1 << ((u_int)((ino) - 1) % NBBY))
#define	TSTINO(ino, map) \
	(map[(u_int)((ino) - 1) / NBBY] &  (1 << ((u_int)((ino) - 1) % NBBY)))

/*
 *	All calculations done in 0.1" units!
 */
char	*disk;		/* name of the disk file */
char	*tape;		/* name of the tape file */
char	*dumpdates;	/* name of the file containing dump date information*/
char	*temp;		/* name of the file for doing rewrite of dumpdates */
char	lastlevel;	/* dump level of previous dump */
char	level;		/* dump level of this dump */
int	uflag;		/* update flag */
int	diskfd;		/* disk file descriptor */
int	tapefd;		/* tape file descriptor */
int	pipeout;	/* true => output to standard output */
ino_t	curino;		/* current inumber; used globally */
int	newtape;	/* new tape flag */
int	density;	/* density in 0.1" units */
long	tapesize;	/* estimated tape size, blocks */
long	tsize;		/* tape size in 0.1" units */
long	asize;		/* number of 0.1" units written on current tape */
int	etapes;		/* estimated number of tapes */

int	notify;		/* notify operator flag */
int	blockswritten;	/* number of blocks written on current tape */
int	tapeno;		/* current tape number */
time_t	tstart_writing;	/* when started writing the first tape block */
char	*processname;
struct	fs *sblock;	/* the file system super block */
char	buf[MAXBSIZE];
long	dev_bsize;	/* block size of underlying disk device */
int	dev_bshift;	/* log2(dev_bsize) */
int	tp_bshift;	/* log2(TP_BSIZE) */

/* operator interface functions */
void	broadcast();
void	lastdump();
void	msg();
void	msgtail();
int	query();
void	set_operators();
void	timeest();

/* mapping rouintes */
long	blockest();
int	mapfiles();
int	mapdirs();

/* file dumping routines */
void	dirdump();
void	blksout();
void	dumpmap();
void	writeheader();
void	bread();

/* tape writing routines */
int	alloctape();
void	writerec();
void	dumpblock();
void	flushtape();
void	trewind();
void	close_rewind();
void	startnewtape();

void	dumpabort();
void	Exit();
void	getfstab();
void	quit();

char	*rawname();
struct dinode *getino();

void	interrupt();		/* in case operator bangs on console */

/*
 *	Exit status codes
 */
#define	X_FINOK		0	/* normal exit */
#define	X_REWRITE	2	/* restart writing from the check point */
#define	X_ABORT		3	/* abort all of dump; don't attempt checkpointing*/

#define	OPGRENT	"operator"		/* group entry to notify */
#define DIALUP	"ttyd"			/* prefix for dialups */

struct	fstab	*fstabsearch();	/* search in fs_file and fs_spec */

/*
 *	The contents of the file _PATH_DUMPDATES is maintained both on
 *	a linked list, and then (eventually) arrayified.
 */
struct dumpdates {
	char	dd_name[MAXNAMLEN+3];
	char	dd_level;
	time_t	dd_ddate;
};
struct dumptime {
	struct	dumpdates dt_value;
	struct	dumptime *dt_next;
};
struct	dumptime *dthead;	/* head of the list version */
int	nddates;		/* number of records (might be zero) */
int	ddates_in;		/* we have read the increment file */
struct	dumpdates **ddatev;	/* the arrayfied version */
void	initdumptimes();
void	getdumptime();
void	putdumptime();
#define	ITITERATE(i, ddp) \
	for (ddp = ddatev[i = 0]; i < nddates; ddp = ddatev[++i])

/*
 *	We catch these interrupts
 */
void	sighup();
void	sigquit();
void	sigill();
void	sigtrap();
void	sigfpe();
void	sigkill();
void	sigbus();
void	sigsegv();
void	sigsys();
void	sigalrm();
void	sigterm();

/*
 * Compatibility with old systems.
 */
#ifndef __STDC__
#include <sys/file.h>
#define _PATH_FSTAB	"/etc/fstab"
typedef int (*sig_t)();
extern char *strdup();
extern char *ctime();
extern int errno;
#endif
