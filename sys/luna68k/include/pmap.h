/* 
 * Copyright (c) 1987 Carnegie-Mellon University
 * Copyright (c) 1992 OMRON Corporation.
 * Copyright (c) 1991, 1992, 1993
 *	The Regents of the University of California.  All rights reserved.
 *
 * This code is derived from software contributed to Berkeley by
 * the Systems Programming Group of the University of Utah Computer
 * Science Department.
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
 *
 * from: hp300/include/pmap.h	7.13 (Berkeley) 6/2/93
 *
 *	@(#)pmap.h	8.2 (Berkeley) 12/06/93
 */

#ifndef	_PMAP_MACHINE_
#define	_PMAP_MACHINE_

#define LUNA_PAGE_SIZE	NBPG
#if defined(LUNA2)
#define LUNA_SEG_SIZE	(mmutype == MMU_68040 ? 0x40000 : NBSEG)
#else
#define LUNA_SEG_SIZE	NBSEG
#endif

#define luna_trunc_seg(x)	(((unsigned)(x)) & ~(LUNA_SEG_SIZE-1))
#define luna_round_seg(x)	luna_trunc_seg((unsigned)(x) + LUNA_SEG_SIZE-1)

/*
 * Pmap stuff
 */
struct pmap {
	struct pte		*pm_ptab;	/* KVA of page table */
	struct ste		*pm_stab;	/* KVA of segment table */
	int			pm_stchanged;	/* ST changed */
	int			pm_stfree;	/* 040: free lev2 blocks */
	struct ste		*pm_stpa;	/* 040: ST phys addr */
	short			pm_sref;	/* segment table ref count */
	short			pm_count;	/* pmap reference count */
	simple_lock_data_t	pm_lock;	/* lock on pmap */
	struct pmap_statistics	pm_stats;	/* pmap statistics */
	long			pm_ptpages;	/* more stats: PT pages */
};

typedef struct pmap	*pmap_t;

extern struct pmap	kernel_pmap_store;

#define kernel_pmap	(&kernel_pmap_store)
#define	active_pmap(pm) \
	((pm) == kernel_pmap || (pm) == curproc->p_vmspace->vm_map.pmap)

/*
 * On the 040 we keep track of which level 2 blocks are already in use
 * with the pm_stfree mask.  Bits are arranged from LSB (block 0) to MSB
 * (block 31).  For convenience, the level 1 table is considered to be
 * block 0.
 *
 * MAX[KU]L2SIZE control how many pages of level 2 descriptors are allowed.
 * for the kernel and users.  8 implies only the initial "segment table"
 * page is used.  WARNING: don't change MAXUL2SIZE unless you can allocate
 * physically contiguous pages for the ST in pmap.c!
 */
#define	MAXKL2SIZE	16
#define MAXUL2SIZE	8
#define l2tobm(n)	(1 << (n))
#define	bmtol2(n)	(ffs(n) - 1)

/*
 * Macros for speed
 */
#define PMAP_ACTIVATE(pmapp, pcbp, iscurproc) \
	if ((pmapp) != NULL && (pmapp)->pm_stchanged) { \
		(pcbp)->pcb_ustp = luna_btop((vm_offset_t)(pmapp)->pm_stpa); \
		if (iscurproc) \
			loadustp((pcbp)->pcb_ustp); \
		(pmapp)->pm_stchanged = FALSE; \
	}
#define PMAP_DEACTIVATE(pmapp, pcbp)

/*
 * For each vm_page_t, there is a list of all currently valid virtual
 * mappings of that page.  An entry is a pv_entry_t, the list is pv_table.
 */
typedef struct pv_entry {
	struct pv_entry	*pv_next;	/* next pv_entry */
	struct pmap	*pv_pmap;	/* pmap where mapping lies */
	vm_offset_t	pv_va;		/* virtual address for mapping */
	struct ste	*pv_ptste;	/* non-zero if VA maps a PT page */
	struct pmap	*pv_ptpmap;	/* if pv_ptste, pmap for PT page */
	int		pv_flags;	/* flags */
} *pv_entry_t;

#define	PV_CI		0x01	/* header: all entries are cache inhibited */
#define PV_PTPAGE	0x02	/* header: entry maps a page table page */

#ifdef	KERNEL

pv_entry_t	pv_table;		/* array of entries, one per page */

#define pa_index(pa)		atop(pa - vm_first_phys)
#define pa_to_pvh(pa)		(&pv_table[pa_index(pa)])

#define	pmap_resident_count(pmap)	((pmap)->pm_stats.resident_count)
#define	pmap_wired_count(pmap)		((pmap)->pm_stats.wired_count)

extern	struct pte *Sysmap;
extern	char *vmmap;			/* map for mem, dumps, etc. */

/*
 * definitions LUNA IO space mapping
 */
struct physmap {
	int pm_phys;
	int pm_size;
	int pm_cache;
} ;

#endif /* KERNEL */

#endif /* _PMAP_MACHINE_ */
