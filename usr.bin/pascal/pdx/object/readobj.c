/* Copyright (c) 1982 Regents of the University of California */

static char sccsid[] = "@(#)readobj.c 1.2 01/19/82";

/*
 * Read in the namelist from the obj file.
 */

#include "defs.h"
#include "sym.h"
#include "symtab.h"
#include "object.h"
#include "objfmt.h"
#include "mappings.h"
#include "mappings/filetab.h"
#include "mappings/linetab.h"

#define MAXSYMNO 2000

char *objname = "obj";

LOCAL SYM *sym[MAXSYMNO];

readobj(file)
char *file;
{
	register FILE *fp;
	struct pxhdr hdr;

	if ((fp = fopen(file, "r")) == NIL) {
		panic("can't open %s", file);
	}
	fseek(fp, (long) (HEADER_BYTES - sizeof(struct pxhdr)), 0);
	get(fp, hdr);
	objsize = hdr.objsize;
	fseek(fp, (long) objsize, 1);
	if (get(fp, nlhdr) != 1) {
		panic("readobj:  get failed");
	}
	stringtab = alloc(nlhdr.stringsize, char);
	fread(stringtab, sizeof(char), nlhdr.stringsize, fp);
	readsyms(fp);
	readfiles(fp);
	readlines(fp);
	fclose(fp);
}

/*
 * allocate and read in file name information table
 */

LOCAL readfiles(fp)
register FILE *fp;
{
	register int i;
	register FILETAB *ftp;
	FILETAB temp;
	ADDRESS prevaddr;

	filetab = alloc(nlhdr.nfiles, FILETAB);
	ftp = &filetab[0];
	prevaddr = 0;
	for (i = 0; i < nlhdr.nfiles; i++) {
		fread(&temp, sizeof(FILETAB), 1, fp);
		if (temp.addr != prevaddr) {
			ftp++;
		}
		*ftp = temp;
		ftp->filename += (int) stringtab;
		prevaddr = ftp->addr;
	}
	nlhdr.nfiles = (ftp - &filetab[0]) + 1;
	skimsource(filetab[0].filename);
	dotpfile = filetab[0].filename;
}

/*
 * allocate and read in line number information table
 */

LOCAL readlines(fp)
FILE *fp;
{
	register LINENO oline;
	register ADDRESS oaddr;
	register LINETAB *lp;
	FILETAB *ftp;
	short lineincr;

	if (nlhdr.nlines == 0) {
		return;
	}
	linetab = alloc(nlhdr.nlines, LINETAB);
	for (lp = &linetab[0]; lp < &linetab[nlhdr.nlines]; lp++) {
		lp->line = 0;
	}
	for (ftp = &filetab[0]; ftp < &filetab[nlhdr.nfiles]; ftp++) {
		linetab[ftp->lineindex].line = ftp->line;
	}
	oline = 0;
	oaddr = 0;
	for (lp = &linetab[0]; lp < &linetab[nlhdr.nlines]; lp++) {
		if (lp->line != 0) {
			oline = lp->line;
		}
		lineincr = (unsigned) getc(fp);
		if (lineincr > 127) {
			lineincr -= 256;
		}
		oline += lineincr;
		oaddr += (unsigned) getc(fp);
		lp->line = oline;
		lp->addr = oaddr;
	}
}

/*
 * read in the symbols
 */

readsyms(fp)
FILE *fp;
{
	register int i;
	int symno;

	symtab = st_creat(nlhdr.nsyms);
	for (i = 0; i < nlhdr.nsyms; i++) {
		symno = getw(fp);
		if (symno >= MAXSYMNO) {
			panic("symbol number too large");
		}
		sym[symno] = readsym(fp);
	}
	if (backpatch() != 0) {
		panic("patchlist not empty after reading namelist");
	}
	if (program == NIL) {
		panic("no program");
	}
	maketypes();
}

typedef struct patchinfo {
	SYM **patchsym;
	struct patchinfo *next_patch;
} PATCH;

LOCAL PATCH *phead;

/*
 * Go through patchlist looking for symbol numbers for which the
 * sym array now has a non-NIL entry.
 *
 * Afterwards, zap the sym array.
 */

int backpatch()
{
	register PATCH *p, *last, *next;
	register SYM *s, **t;
	int count;

	last = NIL;
	count = 0;
	for (p = phead; p != NIL; p = next) {
		next = p->next_patch;
		t = p->patchsym;
		if ((s = sym[(int) *t]) != NIL) {
			*t = s;
			if (last == NIL) {
				phead = next;
			} else {
				last->next_patch = next;
			}
			dispose(p);
		} else {
			last = p;
			count++;
		}
	}
	for (t = &sym[0]; t < &sym[MAXSYMNO]; t++) {
		*t = NIL;
	}
	return(count);
}

/*
 * Check to see if the given pointer (really symbol number) should
 * be added to the patch list.  The argument is double indirect
 * to do call by reference passing.
 */

chkpatch(p)
SYM **p;
{
	register SYM *s, *t;
	register PATCH *patch;

	if ((s = *p) != NIL) {
		if ((t = sym[(int) s]) != NIL) {
			*p = t;
		} else {
			patch = alloc(1, PATCH);
			patch->patchsym = p;
			patch->next_patch = phead;
			phead = patch;
		}
	}
}

/*
 * Free all the object information.
 */

objfree()
{
	register int i;

	st_destroy(symtab);
	dispose(stringtab);
	dispose(filetab);
	dispose(linetab);
	clrfunctab();
	for (i = 0; i < MAXSYMNO; i++) {
		sym[i] = NIL;
	}
}
