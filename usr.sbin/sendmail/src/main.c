/*
 * Copyright (c) 1983 Eric P. Allman
 * Copyright (c) 1988 Regents of the University of California.
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
"@(#) Copyright (c) 1988 Regents of the University of California.\n\
 All rights reserved.\n";
#endif /* not lint */

#ifndef lint
static char sccsid[] = "@(#)main.c	6.24 (Berkeley) 02/21/93";
#endif /* not lint */

#define	_DEFINE

#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <signal.h>
#include <sgtty.h>
#include "sendmail.h"
#ifdef NAMED_BIND
#include <arpa/nameser.h>
#include <resolv.h>
#endif

# ifdef lint
char	edata, end;
# endif lint

/*
**  SENDMAIL -- Post mail to a set of destinations.
**
**	This is the basic mail router.  All user mail programs should
**	call this routine to actually deliver mail.  Sendmail in
**	turn calls a bunch of mail servers that do the real work of
**	delivering the mail.
**
**	Sendmail is driven by tables read in from /usr/lib/sendmail.cf
**	(read by readcf.c).  Some more static configuration info,
**	including some code that you may want to tailor for your
**	installation, is in conf.c.  You may also want to touch
**	daemon.c (if you have some other IPC mechanism), acct.c
**	(to change your accounting), names.c (to adjust the name
**	server mechanism).
**
**	Usage:
**		/usr/lib/sendmail [flags] addr ...
**
**		See the associated documentation for details.
**
**	Author:
**		Eric Allman, UCB/INGRES (until 10/81)
**			     Britton-Lee, Inc., purveyors of fine
**				database computers (from 11/81)
**			     Now back at UCB at the Mammoth project.
**		The support of the INGRES Project and Britton-Lee is
**			gratefully acknowledged.  Britton-Lee in
**			particular had absolutely nothing to gain from
**			my involvement in this project.
*/


int		NextMailer;	/* "free" index into Mailer struct */
char		*FullName;	/* sender's full name */
ENVELOPE	BlankEnvelope;	/* a "blank" envelope */
ENVELOPE	MainEnvelope;	/* the envelope around the basic letter */
ADDRESS		NullAddress =	/* a null address */
		{ "", "", NULL, "" };
char		*UserEnviron[MAXUSERENVIRON + 1];
				/* saved user environment */

/*
**  Pointers for setproctitle.
**	This allows "ps" listings to give more useful information.
**	These must be kept out of BSS for frozen configuration files
**		to work.
*/

# ifdef SETPROCTITLE
char		**Argv = NULL;		/* pointer to argument vector */
char		*LastArgv = NULL;	/* end of argv */
# endif /* SETPROCTITLE */

/*
**  The file in which to log raw recipient information.
**	This is logged before aliasing, forwarding, and so forth so we
**	can see how our addresses are being used.  For example, this
**	would give us the names of aliases (instead of what they alias
**	to), the pre-MX hostnames, and so forth.
**
**	This is specified on the command line, not in the config file,
**	and is therefore really only useful for logging SMTP RCPTs.
*/

char		*RcptLogFile = NULL;	/* file name */

static void	obsolete();

#ifdef DAEMON
#ifndef SMTP
ERROR %%%%   Cannot have daemon mode without SMTP   %%%% ERROR
#endif /* SMTP */
#endif /* DAEMON */

#define MAXCONFIGLEVEL	4	/* highest config version level known */

main(argc, argv, envp)
	int argc;
	char **argv;
	char **envp;
{
	register char *p;
	register char *q;
	char **av;
	char *locname;
	extern int finis();
	extern char Version[];
	char *ep, *from;
	typedef int (*fnptr)();
	STAB *st;
	register int i;
	int j;
	bool readconfig = TRUE;
	bool queuemode = FALSE;		/* process queue requests */
	bool nothaw;
	bool safecf = TRUE;
	static bool reenter = FALSE;
	char *argv0 = argv[0];
	char jbuf[MAXHOSTNAMELEN];	/* holds MyHostName */
	extern int DtableSize;
	extern int optind;
	extern bool safefile();
	extern time_t convtime();
	extern putheader(), putbody();
	extern ENVELOPE *newenvelope();
	extern void intsig();
	extern char **myhostname();
	extern char *arpadate();
	extern char *optarg;
	extern char **environ;

	/*
	**  Check to see if we reentered.
	**	This would normally happen if e_putheader or e_putbody
	**	were NULL when invoked.
	*/

	if (reenter)
	{
		syserr("main: reentered!");
		abort();
	}
	reenter = TRUE;
	extern ADDRESS *recipient();
	bool canrename;

#ifndef SYS5TZ
	/* enforce use of kernel-supplied time zone information */
	unsetenv("TZ");
#endif

	/* in 4.4BSD, the table can be huge; impose a reasonable limit */
	DtableSize = getdtablesize();
	if (DtableSize > 256)
		DtableSize = 256;

	/*
	**  Be sure we have enough file descriptors.
	**	But also be sure that 0, 1, & 2 are open.
	*/

	i = open("/dev/null", O_RDWR);
	while (i >= 0 && i < 2)
		i = dup(i);

	i = DtableSize;
	while (--i > 0)
	{
		if (i != STDIN_FILENO && i != STDOUT_FILENO && i != STDERR_FILENO)
			(void) close(i);
	}
	errno = 0;

#ifdef LOG_MAIL
	openlog("sendmail", LOG_PID, LOG_MAIL);
#else 
	openlog("sendmail", LOG_PID);
#endif 

	/*
	**  Set default values for variables.
	**	These cannot be in initialized data space.
	*/

	setdefaults();

	/* set up the blank envelope */
	BlankEnvelope.e_puthdr = putheader;
	BlankEnvelope.e_putbody = putbody;
	BlankEnvelope.e_xfp = NULL;
	STRUCTCOPY(NullAddress, BlankEnvelope.e_from);
	STRUCTCOPY(BlankEnvelope, MainEnvelope);
	CurEnv = &MainEnvelope;

	/* Handle any non-getoptable constructions. */
	obsolete(argv);

	/*
	**  Do a quick prescan of the argument list.
	**	We do this to find out if we can potentially thaw the
	**	configuration file.  If not, we do the thaw now so that
	**	the argument processing applies to this run rather than
	**	to the run that froze the configuration.
	*/
	nothaw = FALSE;
#define OPTIONS		"b:C:cd:e:F:f:h:Iimno:p:q:R:r:sTtv"
	while ((j = getopt(argc, argv, OPTIONS)) != EOF)
	{
		switch (j)
		{
		  case 'b':
			if (optarg[0] == 'z' && optarg[1] == '\0')
				nothaw = TRUE;
			break;

		  case 'C':
			ConfFile = optarg;
			(void) setgid(getrgid());
			(void) setuid(getruid());
			safecf = FALSE;
			nothaw = TRUE;
			break;

		  case 'd':
			tTsetup(tTdvect, sizeof tTdvect, "0-99.1");
			tTflag(optarg);
			setbuf(stdout, (char *) NULL);
			printf("Version %s\n", Version);
			break;
		}
	}

	InChannel = stdin;
	OutChannel = stdout;

	if (!nothaw)
		readconfig = !thaw(FreezeFile, argv0);

# ifdef SETPROCTITLE
	/*
	**  Move the environment so setproctitle can use the space at
	**  the top of memory.
	*/

	for (i = j = 0; j < MAXUSERENVIRON && (p = envp[i]) != NULL; i++)
	{
		if (strncmp(p, "FS=", 3) == 0 || strncmp(p, "LD_", 3) == 0)
			continue;
		UserEnviron[j++] = newstr(p);
	}
	UserEnviron[j] = NULL;
	environ = UserEnviron;

	/*
	**  Save start and extent of argv for setproctitle.
	*/

	Argv = argv;
	if (i > 0)
		LastArgv = envp[i - 1] + strlen(envp[i - 1]);
	else
		LastArgv = argv[argc - 1] + strlen(argv[argc - 1]);
# endif /* SETPROCTITLE */

	if (signal(SIGINT, SIG_IGN) != SIG_IGN)
		(void) signal(SIGINT, intsig);
	if (signal(SIGHUP, SIG_IGN) != SIG_IGN)
		(void) signal(SIGHUP, intsig);
	(void) signal(SIGTERM, intsig);
	(void) signal(SIGPIPE, SIG_IGN);
	OldUmask = umask(022);
	OpMode = MD_DELIVER;
	FullName = getenv("NAME");

	errno = 0;
	from = NULL;

	if (readconfig)
	{
		/* initialize some macros, etc. */
		initmacros();

		/* hostname */
		av = myhostname(jbuf, sizeof jbuf);
		if (jbuf[0] != '\0')
		{
			struct	utsname	utsname;
			extern char *strchr();

			if (tTd(0, 4))
				printf("canonical name: %s\n", jbuf);
			p = newstr(jbuf);
			define('w', p, CurEnv);

			q = strchr(jbuf, '.');
			if (q != NULL)
			{
				*q++ = '\0';
				p = newstr(jbuf);
				define('m', q, CurEnv);
			}
			setclass('w', p);

			if (uname(&utsname) >= 0)
				p = utsname.nodename;
			else
			{
				makelower(jbuf);
				p = jbuf;
			}
			if (tTd(0, 4))
				printf("UUCP nodename: %s\n", p);
			p = newstr(p);
			define('k', p, CurEnv);
			setclass('w', p);
		}
		while (av != NULL && *av != NULL)
		{
			if (tTd(0, 4))
				printf("\ta.k.a.: %s\n", *av);
			setclass('w', *av++);
		}

		/* version */
		define('v', Version, CurEnv);
	}

	/* current time */
	define('b', arpadate((char *) NULL), CurEnv);

	/*
	** Crack argv.
	*/

	av = argv;
	p = strrchr(*av, '/');
	if (p++ == NULL)
		p = *av;
	if (strcmp(p, "newaliases") == 0)
		OpMode = MD_INITALIAS;
	else if (strcmp(p, "mailq") == 0)
		OpMode = MD_PRINT;
	else if (strcmp(p, "smtpd") == 0)
		OpMode = MD_DAEMON;

	optind = 1;
	while ((j = getopt(argc, argv, OPTIONS)) != EOF)
	{
		switch (j)
		{
		  case 'b':	/* operations mode */
			switch (j = *optarg)
			{
			  case MD_DAEMON:
# ifdef DAEMON
				if (getuid() != 0) {
					usrerr("Permission denied");
					exit (EX_USAGE);
				}
				(void) unsetenv("HOSTALIASES");
# else
				usrerr("Daemon mode not implemented");
				ExitStat = EX_USAGE;
				break;
# endif /* DAEMON */
			  case MD_SMTP:
# ifndef SMTP
				usrerr("I don't speak SMTP");
				ExitStat = EX_USAGE;
				break;
# endif /* SMTP */
			  case MD_DELIVER:
			  case MD_VERIFY:
			  case MD_TEST:
			  case MD_INITALIAS:
			  case MD_PRINT:
			  case MD_FREEZE:
				OpMode = j;
				break;

			  default:
				usrerr("Invalid operation mode %c", j);
				ExitStat = EX_USAGE;
				break;
			}
			break;

		  case 'C':	/* select configuration file (already done) */
			break;

		  case 'd':	/* debugging -- redo in case frozen */
			tTsetup(tTdvect, sizeof tTdvect, "0-99.1");
			tTflag(optarg);
			setbuf(stdout, (char *) NULL);
			break;

		  case 'f':	/* from address */
		  case 'r':	/* obsolete -f flag */
			if (from != NULL)
			{
				usrerr("More than one \"from\" person");
				ExitStat = EX_USAGE;
				break;
			}
			from = newstr(optarg);
			break;

		  case 'F':	/* set full name */
			FullName = newstr(optarg);
			break;

		  case 'h':	/* hop count */
			CurEnv->e_hopcount = strtol(optarg, &ep, 10);
			if (*ep)
			{
				usrerr("Bad hop count (%s)", optarg);
				ExitStat = EX_USAGE;
				break;
			}
			break;
		
		  case 'n':	/* don't alias */
			NoAlias = TRUE;
			break;

		  case 'o':	/* set option */
			setoption(*optarg, optarg + 1, FALSE, TRUE);
			break;

		  case 'p':	/* set protocol */
			q = strchr(optarg, ':');
			if (q != NULL)
				*q++ = '\0';
			if (*optarg != '\0')
				define('r', newstr(optarg), CurEnv);
			if (q != NULL && *q != '\0')
				define('s', newstr(q), CurEnv);
			break;

		  case 'q':	/* run queue files at intervals */
# ifdef QUEUE
			(void) unsetenv("HOSTALIASES");
			FullName = NULL;
			queuemode = TRUE;
			QueueIntvl = convtime(optarg);
# else /* QUEUE */
			usrerr("I don't know about queues");
			ExitStat = EX_USAGE;
# endif /* QUEUE */
			break;

		  case 't':	/* read recipients from message */
			GrabTo = TRUE;
			break;

			/* compatibility flags */
		  case 'c':	/* connect to non-local mailers */
		  case 'i':	/* don't let dot stop me */
		  case 'm':	/* send to me too */
		  case 'T':	/* set timeout interval */
		  case 'v':	/* give blow-by-blow description */
			setoption(j, "T", FALSE, TRUE);
			break;

		  case 'e':	/* error message disposition */
			setoption(j, optarg, FALSE, TRUE);
			break;

		  case 's':	/* save From lines in headers */
			setoption('f', "T", FALSE, TRUE);
			break;

# ifdef DBM
		  case 'I':	/* initialize alias DBM file */
			OpMode = MD_INITALIAS;
			break;
# endif /* DBM */

		  case 'R':	/* log raw recipient info */
			RcptLogFile = newstr(optarg);
			break;

		  default:
			ExitStat = EX_USAGE;
			finis();
			break;
		}
	}
	av += optind;

#ifdef NAMED_BIND
	if (tTd(8, 8))
		_res.options |= RES_DEBUG;
#endif

	/*
	**  Do basic initialization.
	**	Read system control file.
	**	Extract special fields for local use.
	*/

	if (OpMode == MD_FREEZE || readconfig)
		readcf(ConfFile, safecf, CurEnv);

#ifdef SYS5TZ
	/* Enforce use of local time (null string overrides this) */
	if (TimeZoneSpec == NULL)
		unsetenv("TZ");
	else if (TimeZoneSpec[0] != '\0')
	{
		p = xalloc(strlen(TimeZoneSpec) + 4);
		(void) strcpy(p, "TZ=");
		(void) strcat(p, TimeZoneSpec);
		putenv(p);
	}
#endif

	if (ConfigLevel > MAXCONFIGLEVEL)
	{
		syserr("Warning: .cf version level (%d) exceeds program functionality (%d)",
			ConfigLevel, MAXCONFIGLEVEL);
	}
# ifdef QUEUE
	if (queuemode && getuid() != 0)
	{
		struct stat stbuf;

		/* check to see if we own the queue directory */
		if (stat(QueueDir, &stbuf) < 0)
			syserr("main: cannot stat %s", QueueDir);
		if (stbuf.st_uid != getuid())
		{
			/* nope, really a botch */
			usrerr("Permission denied");
			exit (EX_NOPERM);
		}
	}
# endif /* QUEUE */
	switch (OpMode)
	{
	  case MD_FREEZE:
		/* this is critical to avoid forgeries of the frozen config */
		(void) setgid(getgid());
		(void) setuid(getuid());

		/* freeze the configuration */
		freeze(FreezeFile);
		exit(EX_OK);

	  case MD_INITALIAS:
		Verbose = TRUE;
		break;

	  case MD_DAEMON:
		/* remove things that don't make sense in daemon mode */
		FullName = NULL;
		break;
	}

	/* do heuristic mode adjustment */
	if (Verbose)
	{
		/* turn off noconnect option */
		setoption('c', "F", TRUE, FALSE);

		/* turn on interactive delivery */
		setoption('d', "", TRUE, FALSE);
	}

	/* our name for SMTP codes */
	expand("\201j", jbuf, &jbuf[sizeof jbuf - 1], CurEnv);
	MyHostName = jbuf;

	/* the indices of built-in mailers */
	st = stab("local", ST_MAILER, ST_FIND);
	if (st == NULL)
		syserr("No local mailer defined");
	else
		LocalMailer = st->s_mailer;

	st = stab("prog", ST_MAILER, ST_FIND);
	if (st == NULL)
		syserr("No prog mailer defined");
	else
		ProgMailer = st->s_mailer;

	st = stab("*file*", ST_MAILER, ST_FIND);
	if (st == NULL)
		syserr("No *file* mailer defined");
	else
		FileMailer = st->s_mailer;

	st = stab("*include*", ST_MAILER, ST_FIND);
	if (st == NULL)
		syserr("No *include* mailer defined");
	else
		InclMailer = st->s_mailer;


	/* operate in queue directory */
	if (chdir(QueueDir) < 0)
	{
		syserr("cannot chdir(%s)", QueueDir);
		exit(EX_SOFTWARE);
	}

	/*
	**  Do operation-mode-dependent initialization.
	*/

	switch (OpMode)
	{
	  case MD_PRINT:
		/* print the queue */
#ifdef QUEUE
		dropenvelope(CurEnv);
		printqueue();
		exit(EX_OK);
#else /* QUEUE */
		usrerr("No queue to print");
		finis();
#endif /* QUEUE */

	  case MD_INITALIAS:
		/* initialize alias database */
		initaliases(AliasFile, TRUE, CurEnv);
		exit(EX_OK);

	  case MD_DAEMON:
		/* don't open alias database -- done in srvrsmtp */
		break;

	  default:
		/* open the alias database */
		initaliases(AliasFile, FALSE, CurEnv);
		break;
	}

	if (tTd(0, 15))
	{
		/* print configuration table (or at least part of it) */
		printrules();
		for (i = 0; i < MAXMAILERS; i++)
		{
			register struct mailer *m = Mailer[i];
			int j;

			if (m == NULL)
				continue;
			printf("mailer %d (%s): P=%s S=%d/%d R=%d/%d M=%ld F=", i, m->m_name,
				m->m_mailer, m->m_se_rwset, m->m_sh_rwset,
				m->m_re_rwset, m->m_rh_rwset, m->m_maxsize);
			for (j = '\0'; j <= '\177'; j++)
				if (bitnset(j, m->m_flags))
					(void) putchar(j);
			printf(" E=");
			xputs(m->m_eol);
			if (m->m_argv != NULL)
			{
				char **a = m->m_argv;

				printf(" A=");
				while (*a != NULL)
				{
					if (a != m->m_argv)
						printf(" ");
					xputs(*a++);
				}
			}
			printf("\n");
		}
	}

	/*
	**  Switch to the main envelope.
	*/

	CurEnv = newenvelope(&MainEnvelope);
	MainEnvelope.e_flags = BlankEnvelope.e_flags;

	/*
	**  If test mode, read addresses from stdin and process.
	*/

	if (OpMode == MD_TEST)
	{
		char buf[MAXLINE];

		if (isatty(fileno(stdin)))
			Verbose = TRUE;

		if (Verbose)
		{
			printf("ADDRESS TEST MODE (ruleset 3 NOT automatically invoked)\n");
			printf("Enter <ruleset> <address>\n");
		}
		for (;;)
		{
			register char **pvp;
			char *q;
			extern char *DelimChar;
			extern bool invalidaddr();

			if (Verbose)
				printf("> ");
			(void) fflush(stdout);
			if (fgets(buf, sizeof buf, stdin) == NULL)
				finis();
			if (!Verbose)
				printf("> %s", buf);
			if (buf[0] == '#')
				continue;
			for (p = buf; isascii(*p) && isspace(*p); p++)
				continue;
			q = p;
			while (*p != '\0' && !(isascii(*p) && isspace(*p)))
				p++;
			if (*p == '\0')
			{
				printf("No address!\n");
				continue;
			}
			*p = '\0';
			if (invalidaddr(p + 1))
				continue;
			do
			{
				extern char **prescan();
				char pvpbuf[PSBUFSIZE];

				pvp = prescan(++p, ',', pvpbuf);
				if (pvp == NULL)
					continue;
				p = q;
				while (*p != '\0')
				{
					rewrite(pvp, atoi(p));
					while (*p != '\0' && *p++ != ',')
						continue;
				}
			} while (*(p = DelimChar) != '\0');
		}
	}

# ifdef QUEUE
	/*
	**  If collecting stuff from the queue, go start doing that.
	*/

	if (queuemode && OpMode != MD_DAEMON && QueueIntvl == 0)
	{
		runqueue(FALSE);
		finis();
	}
# endif /* QUEUE */

	/*
	**  If a daemon, wait for a request.
	**	getrequests will always return in a child.
	**	If we should also be processing the queue, start
	**		doing it in background.
	**	We check for any errors that might have happened
	**		during startup.
	*/

	if (OpMode == MD_DAEMON || QueueIntvl != 0)
	{
		if (!tTd(0, 1))
		{
			FILE *pidf;

			/* put us in background */
			i = fork();
			if (i < 0)
				syserr("daemon: cannot fork");
			if (i != 0)
				exit(0);

			if (OpMode == MD_DAEMON)
			{
				pidf = fopen(PidFile, "w");
				if (pidf != NULL)
				{
					fprintf(pidf, "%d\n", getpid());
					fclose(pidf);
				}
			}

			/* disconnect from our controlling tty */
			disconnect(TRUE);
		}

# ifdef QUEUE
		if (queuemode)
		{
			runqueue(TRUE);
			if (OpMode != MD_DAEMON)
				for (;;)
					pause();
		}
# endif /* QUEUE */
		dropenvelope(CurEnv);

#ifdef DAEMON
		getrequests();

		/* at this point we are in a child: reset state */
		OpMode = MD_SMTP;
		(void) newenvelope(CurEnv);
		openxscript(CurEnv);
#endif /* DAEMON */
	}
	
# ifdef SMTP
	/*
	**  If running SMTP protocol, start collecting and executing
	**  commands.  This will never return.
	*/

	if (OpMode == MD_SMTP)
		smtp(CurEnv);
# endif /* SMTP */

	/*
	**  Do basic system initialization and set the sender
	*/

	/* make sendmail immune from process group signals */
	(void) setpgrp(0, getpid());

	initsys(CurEnv);
	setsender(from, CurEnv);

	if (*av == NULL && !GrabTo)
	{
		usrerr("Recipient names must be specified");

		/* collect body for UUCP return */
		if (OpMode != MD_VERIFY)
			collect(FALSE, CurEnv);
		finis();
	}
	if (OpMode == MD_VERIFY)
		SendMode = SM_VERIFY;

	/*
	**  Scan argv and deliver the message to everyone.
	*/

	sendtoargv(av, CurEnv);

	/* if we have had errors sofar, arrange a meaningful exit stat */
	if (Errors > 0 && ExitStat == EX_OK)
		ExitStat = EX_USAGE;

	/*
	**  Read the input mail.
	*/

	CurEnv->e_to = NULL;
	if (OpMode != MD_VERIFY || GrabTo)
		collect(FALSE, CurEnv);
	errno = 0;

	/* collect statistics */
	if (OpMode != MD_VERIFY)
		markstats(CurEnv, (ADDRESS *) NULL);

	if (tTd(1, 1))
		printf("From person = \"%s\"\n", CurEnv->e_from.q_paddr);

	/*
	**  Actually send everything.
	**	If verifying, just ack.
	*/

	CurEnv->e_from.q_flags |= QDONTSEND;
	if (tTd(1, 5))
	{
		printf("main: QDONTSEND ");
		printaddr(&CurEnv->e_from, FALSE);
	}
	CurEnv->e_to = NULL;
	sendall(CurEnv, SM_DEFAULT);

	/*
	** All done.
	*/

	finis();
}
/*
**  FINIS -- Clean up and exit.
**
**	Parameters:
**		none
**
**	Returns:
**		never
**
**	Side Effects:
**		exits sendmail
*/

finis()
{
	if (tTd(2, 1))
		printf("\n====finis: stat %d e_flags %o\n", ExitStat, CurEnv->e_flags);

	/* clean up temp files */
	CurEnv->e_to = NULL;
	dropenvelope(CurEnv);

	/* flush any cached connections */
	mci_flush(TRUE, NULL);

	/* post statistics */
	poststats(StatFile);

	/* and exit */
# ifdef LOG
	if (LogLevel > 78)
		syslog(LOG_DEBUG, "finis, pid=%d", getpid());
# endif /* LOG */
	if (ExitStat == EX_TEMPFAIL)
		ExitStat = EX_OK;
	exit(ExitStat);
}
/*
**  INTSIG -- clean up on interrupt
**
**	This just arranges to exit.  It pessimises in that it
**	may resend a message.
**
**	Parameters:
**		none.
**
**	Returns:
**		none.
**
**	Side Effects:
**		Unlocks the current job.
*/

void
intsig()
{
	FileName = NULL;
	unlockqueue(CurEnv);
	exit(EX_OK);
}
/*
**  INITMACROS -- initialize the macro system
**
**	This just involves defining some macros that are actually
**	used internally as metasymbols to be themselves.
**
**	Parameters:
**		none.
**
**	Returns:
**		none.
**
**	Side Effects:
**		initializes several macros to be themselves.
*/

struct metamac	MetaMacros[] =
{
	/* LHS pattern matching characters */
	'*', MATCHZANY,		'+', MATCHANY,		'-', MATCHONE,
	'=', MATCHCLASS,	'~', MATCHNCLASS,

	/* these are RHS metasymbols */
	'#', CANONNET,		'@', CANONHOST,		':', CANONUSER,
	'>', CALLSUBR,
	'{', MATCHLOOKUP,		'}', MATCHELOOKUP,

	/* the conditional operations */
	'?', CONDIF,		'|', CONDELSE,		'.', CONDFI,

	/* and finally the hostname lookup characters */
	'[', HOSTBEGIN,		']', HOSTEND,
	'(', LOOKUPBEGIN,	')', LOOKUPEND,

	'\0'
};

initmacros()
{
	register struct metamac *m;
	char buf[5];
	register int c;

	for (m = MetaMacros; m->metaname != '\0'; m++)
	{
		buf[0] = m->metaval;
		buf[1] = '\0';
		define(m->metaname, newstr(buf), CurEnv);
	}
	buf[0] = MATCHREPL;
	buf[2] = '\0';
	for (c = '0'; c <= '9'; c++)
	{
		buf[1] = c;
		define(c, newstr(buf), CurEnv);
	}
}
/*
**  FREEZE -- freeze BSS & allocated memory
**
**	This will be used to efficiently load the configuration file.
**
**	Parameters:
**		freezefile -- the name of the file to freeze to.
**
**	Returns:
**		none.
**
**	Side Effects:
**		Writes BSS and malloc'ed memory to freezefile
*/

union frz
{
	char		frzpad[BUFSIZ];	/* insure we are on a BUFSIZ boundary */
	struct
	{
		time_t	frzstamp;	/* timestamp on this freeze */
		char	*frzbrk;	/* the current break */
		char	*frzedata;	/* address of edata */
		char	*frzend;	/* address of end */
		char	frzver[252];	/* sendmail version */
	} frzinfo;
};

#ifdef __hpux
#define BRK_TYPE        int
#define SBRK_TYPE       void *
#else
#define BRK_TYPE        char *
#define SBRK_TYPE       char *
#endif

freeze(freezefile)
	char *freezefile;
{
	int f;
	union frz fhdr;
	extern SBRK_TYPE sbrk();
	extern char edata, end;
	extern char Version[];

	if (freezefile == NULL)
		return;

	/* try to open the freeze file */
	f = creat(freezefile, FileMode);
	if (f < 0)
	{
		syserr("Cannot freeze %s", freezefile);
		errno = 0;
		return;
	}

	/* build the freeze header */
	fhdr.frzinfo.frzstamp = curtime();
	fhdr.frzinfo.frzbrk = sbrk(0);
	fhdr.frzinfo.frzedata = &edata;
	fhdr.frzinfo.frzend = &end;
	(void) strcpy(fhdr.frzinfo.frzver, Version);

	/* write out the freeze header */
	if (write(f, (char *) &fhdr, sizeof fhdr) != sizeof fhdr ||
	    write(f, (char *) &edata, (int) (fhdr.frzinfo.frzbrk - &edata)) !=
					(int) (fhdr.frzinfo.frzbrk - &edata))
	{
		syserr("Cannot freeze %s", freezefile);
	}

	/* fine, clean up */
	(void) close(f);
}
/*
**  THAW -- read in the frozen configuration file.
**
**	Parameters:
**		freezefile -- the name of the file to thaw from.
**		binfile -- the name of the sendmail binary (ok to guess).
**
**	Returns:
**		TRUE if it successfully read the freeze file.
**		FALSE otherwise.
**
**	Side Effects:
**		reads freezefile in to BSS area.
*/

thaw(freezefile, binfile)
	char *freezefile;
	char *binfile;
{
	int f;
	register char *p;
	union frz fhdr;
	char hbuf[60];
	struct stat fst, sst;
	extern char edata, end;
	extern char Version[];
	extern char **myhostname();
	extern char *macvalue();
	extern BRK_TYPE brk();

	if (freezefile == NULL)
		return (FALSE);

	/* open the freeze file */
	f = open(freezefile, 0);
	if (f < 0)
	{
		errno = 0;
		return (FALSE);
	}

	if (fstat(f, &fst) < 0 || stat(ConfFile, &sst) < 0 ||
	    fst.st_mtime < sst.st_mtime)
	{
		syslog(LOG_WARNING, "Freeze file older than config file");
		(void) close(f);
		return (FALSE);
	}

	if (strchr(binfile, '/') != NULL && stat(binfile, &sst) == 0 &&
	    fst.st_mtime < sst.st_mtime)
	{
		syslog(LOG_WARNING, "Freeze file older than binary file");
		(void) close(f);
		return (FALSE);
	}

	/* read in the header */
	if (read(f, (char *) &fhdr, sizeof fhdr) < sizeof fhdr)
	{
		syslog(LOG_WARNING, "Cannot read frozen config file");
		(void) close(f);
		return (FALSE);
	}
	if (fhdr.frzinfo.frzedata != &edata ||
	    fhdr.frzinfo.frzend != &end ||
	    strcmp(fhdr.frzinfo.frzver, Version) != 0)
	{
		syslog(LOG_WARNING, "Wrong version of frozen config file");
		(void) close(f);
		return (FALSE);
	}

	/* arrange to have enough space */
	if (brk(fhdr.frzinfo.frzbrk) == (caddr_t) -1)
	{
		syserr("Cannot break to %x", fhdr.frzinfo.frzbrk);
		(void) close(f);
		return (FALSE);
	}

	/* now read in the freeze file */
	if (read(f, (char *) &edata, (int) (fhdr.frzinfo.frzbrk - &edata)) !=
					(int) (fhdr.frzinfo.frzbrk - &edata))
	{
		syserr("Cannot read frozen config file");
		/* oops!  we have trashed memory..... */
		(void) write(2, "Cannot read freeze file\n", 24);
		_exit(EX_SOFTWARE);
	}

	(void) close(f);

	/* verify that the host name was correct on the freeze */
	(void) myhostname(hbuf, sizeof hbuf);
	p = macvalue('w', CurEnv);
	if (p == NULL)
		p = "";
	if (strcmp(hbuf, macvalue('w', CurEnv)) == 0)
		return (TRUE);
	syslog(LOG_WARNING, "Hostname changed since freeze (%s => %s)",
		p, hbuf);
	return (FALSE);
}
/*
**  DISCONNECT -- remove our connection with any foreground process
**
**	Parameters:
**		fulldrop -- if set, we should also drop the controlling
**			TTY if possible -- this should only be done when
**			setting up the daemon since otherwise UUCP can
**			leave us trying to open a dialin, and we will
**			wait for the carrier.
**
**	Returns:
**		none
**
**	Side Effects:
**		Trys to insure that we are immune to vagaries of
**		the controlling tty.
*/

disconnect(fulldrop)
	bool fulldrop;
{
	int fd;

	if (tTd(52, 1))
		printf("disconnect: In %d Out %d\n", fileno(InChannel),
						fileno(OutChannel));
	if (tTd(52, 5))
	{
		printf("don't\n");
		return;
	}

	/* be sure we don't get nasty signals */
	(void) signal(SIGHUP, SIG_IGN);
	(void) signal(SIGINT, SIG_IGN);
	(void) signal(SIGQUIT, SIG_IGN);

	/* we can't communicate with our caller, so.... */
	HoldErrs = TRUE;
	ErrorMode = EM_MAIL;
	Verbose = FALSE;

	/* all input from /dev/null */
	if (InChannel != stdin)
	{
		(void) fclose(InChannel);
		InChannel = stdin;
	}
	(void) freopen("/dev/null", "r", stdin);

	/* output to the transcript */
	if (OutChannel != stdout)
	{
		(void) fclose(OutChannel);
		OutChannel = stdout;
	}
	if (CurEnv->e_xfp == NULL)
		CurEnv->e_xfp = fopen("/dev/null", "w");
	(void) fflush(stdout);
	(void) close(1);
	(void) close(2);
	while ((fd = dup(fileno(CurEnv->e_xfp))) < 2 && fd > 0)
		continue;

	/* drop our controlling TTY completely if possible */
	if (fulldrop)
	{
#ifdef SYSTEM5
		(void) setpgrp();
#else
		(void) setsid();
#endif
#ifdef TIOCNOTTY
		fd = open("/dev/tty", 2);
		if (fd >= 0)
		{
			(void) ioctl(fd, (int) TIOCNOTTY, (char *) 0);
			(void) close(fd);
		}
		(void) setpgrp(0, 0);
#endif /* TIOCNOTTY */
		errno = 0;
	}

# ifdef LOG
	if (LogLevel > 71)
		syslog(LOG_DEBUG, "in background, pid=%d", getpid());
# endif /* LOG */

	errno = 0;
}

static void
obsolete(argv)
	char *argv[];
{
	char *ap;

	while (ap = *++argv)
	{
		/* Return if "--" or not an option of any form. */
		if (ap[0] != '-' || ap[1] == '-')
			return;

		/* If -C doesn't have an argument, use sendmail.cf. */
#define	__DEFPATH	"sendmail.cf"
		if (ap[1] == 'C' && ap[2] == '\0' &&
		    (argv[1] == NULL || argv[1][0] == '-'))
		{
			*argv = xalloc(sizeof(__DEFPATH) + 2);
			argv[0][0] = '-';
			argv[0][1] = 'C';
			(void)strcpy(&argv[0][2], __DEFPATH);
		}

		/* If -q doesn't have an argument, run it once. */
		if (ap[1] == 'q' && ap[2] == '\0' &&
		    (argv[1] == NULL || argv[1][0] == '-'))
			*argv = "-q0";

		/* if -d doesn't have an argument, use 0-99.1 */
		if (ap[1] == 'd' && ap[2] == '\0' &&
		    (argv[1] == NULL || argv[1][0] == '-'))
			*argv = "-d0-99.1";
	}
}
