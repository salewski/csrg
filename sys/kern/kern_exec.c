/*
 * Copyright (c) 1982, 1986, 1989, 1991 Regents of the University of California.
 * All rights reserved.  The Berkeley software License Agreement
 * specifies the terms and conditions for redistribution.
 *
 *	@(#)kern_exec.c	7.37 (Berkeley) 03/25/91
 */

#include "param.h"
#include "systm.h"
#include "filedesc.h"
#include "kernel.h"
#include "proc.h"
#include "mount.h"
#include "malloc.h"
#include "vnode.h"
#include "seg.h"
#include "file.h"
#include "acct.h"
#include "exec.h"
#include "ktrace.h"
#include "resourcevar.h"

#include "machine/reg.h"

#include "mman.h"
#include "vm/vm.h"
#include "vm/vm_param.h"
#include "vm/vm_map.h"
#include "vm/vm_kern.h"
#include "vm/vm_pager.h"

#include "signalvar.h"
#include "kinfo_proc.h"
#include "user.h"			/* for pcb, sigc */

#ifdef HPUXCOMPAT
#include "hp300/hpux/hpux_exec.h"
#endif

#ifdef COMPAT_43
execv(p, uap, retval)
	struct proc *p;
	struct args {
		char	*fname;
		char	**argp;
		char	**envp;
	} *uap;
	int *retval;
{

	uap->envp = NULL;
	return (execve(p, uap, retval));
}
#endif

/*
 * exec system call
 */
/* ARGSUSED */
execve(p, uap, retval)
	register struct proc *p;
	register struct args {
		char	*fname;
		char	**argp;
		char	**envp;
	} *uap;
	int *retval;
{
	register struct ucred *cred = p->p_ucred;
	register struct nameidata *ndp;
	register struct filedesc *fdp = p->p_fd;
	int na, ne, ucp, ap, cc;
	register char *cp;
	register int nc;
	unsigned len;
	int indir, uid, gid;
	char *sharg;
	struct vnode *vp;
	int resid, error, flags = 0;
	vm_offset_t execargs;
	struct vattr vattr;
	char cfname[MAXCOMLEN + 1];
	char cfarg[MAXINTERP];
	union {
		char	ex_shell[MAXINTERP];	/* #! and interpreter name */
		struct	exec ex_exec;
#ifdef HPUXCOMPAT
		struct	hpux_exec ex_hexec;
#endif
	} exdata;
#ifdef HPUXCOMPAT
	struct hpux_exec hhead;
#endif
	struct nameidata nd;
#ifdef SECSIZE
	extern long argdbsize;			/* XXX */
#endif SECSIZE

	ndp = &nd;
  start:
	ndp->ni_nameiop = LOOKUP | FOLLOW | LOCKLEAF;
	ndp->ni_segflg = UIO_USERSPACE;
	ndp->ni_dirp = uap->fname;
	if (error = namei(ndp, p))
		return (error);
	vp = ndp->ni_vp;
	indir = 0;
	uid = cred->cr_uid;
	gid = cred->cr_gid;
	if (error = VOP_GETATTR(vp, &vattr, cred))
		goto bad;
	if (vp->v_mount->mnt_flag & MNT_NOEXEC) {
		error = EACCES;
		goto bad;
	}
	if ((vp->v_mount->mnt_flag & MNT_NOSUID) == 0) {
		if (vattr.va_mode & VSUID)
			uid = vattr.va_uid;
		if (vattr.va_mode & VSGID)
			gid = vattr.va_gid;
	}

  again:
	if (error = VOP_ACCESS(vp, VEXEC, cred))
		goto bad;
	if ((p->p_flag & STRC) && (error = VOP_ACCESS(vp, VREAD, cred)))
		goto bad;
	if (vp->v_type != VREG ||
	    (vattr.va_mode & (VEXEC|(VEXEC>>3)|(VEXEC>>6))) == 0) {
		error = EACCES;
		goto bad;
	}

	/*
	 * Read in first few bytes of file for segment sizes, magic number:
	 *	OMAGIC = plain executable
	 *	NMAGIC = RO text
	 *	ZMAGIC = demand paged RO text
	 * Also an ASCII line beginning with #! is
	 * the file name of a ``shell'' and arguments may be prepended
	 * to the argument list if given here.
	 *
	 * SHELL NAMES ARE LIMITED IN LENGTH.
	 *
	 * ONLY ONE ARGUMENT MAY BE PASSED TO THE SHELL FROM
	 * THE ASCII LINE.
	 */
	exdata.ex_shell[0] = '\0';	/* for zero length files */
	error = vn_rdwr(UIO_READ, vp, (caddr_t)&exdata, sizeof (exdata),
	    (off_t)0, UIO_SYSSPACE, (IO_UNIT|IO_NODELOCKED), cred, &resid);
	if (error)
		goto bad;
#ifndef lint
	if (resid > sizeof(exdata) - sizeof(exdata.ex_exec) &&
	    exdata.ex_shell[0] != '#') {
		error = ENOEXEC;
		goto bad;
	}
#endif
#if defined(hp300)
	switch ((int)exdata.ex_exec.a_mid) {

	/*
	 * An ancient hp200 or hp300 binary, shouldn't happen anymore.
	 * Mark as invalid.
	 */
	case MID_ZERO:
		exdata.ex_exec.a_magic = 0;
		break;

	/*
	 * HP200 series has a smaller page size so we cannot
	 * demand-load or even write protect text, so we just
	 * treat as OMAGIC.
	 */
	case MID_HP200:
		exdata.ex_exec.a_magic = OMAGIC;
		break;

	case MID_HP300:
		break;

#ifdef HPUXCOMPAT
	case MID_HPUX:
		/*
		 * Save a.out header.  This is eventually saved in the pcb,
		 * but we cannot do that yet in case the exec fails before
		 * the image is overlayed.
		 */
		bcopy((caddr_t)&exdata.ex_hexec,
		      (caddr_t)&hhead, sizeof hhead);
		/*
		 * If version number is 0x2bad this is a native BSD
		 * binary created via the HPUX SGS.  Should not be
		 * treated as an HPUX binary.
		 */
		if (exdata.ex_hexec.ha_version != BSDVNUM)
			flags |= SHPUX;
		/*
		 * Shuffle important fields to their BSD locations.
		 * Note that the order in which this is done is important.
		 */
		exdata.ex_exec.a_text = exdata.ex_hexec.ha_text;
		exdata.ex_exec.a_data = exdata.ex_hexec.ha_data;
		exdata.ex_exec.a_bss = exdata.ex_hexec.ha_bss;
		exdata.ex_exec.a_entry = exdata.ex_hexec.ha_entry;
		/*
		 * For ZMAGIC files, make sizes consistant with those
		 * generated by BSD ld.
		 */
		if (exdata.ex_exec.a_magic == ZMAGIC) {
			exdata.ex_exec.a_text = 
				ctob(btoc(exdata.ex_exec.a_text));
			nc = exdata.ex_exec.a_data + exdata.ex_exec.a_bss;
			exdata.ex_exec.a_data =
				ctob(btoc(exdata.ex_exec.a_data));
			nc -= (int)exdata.ex_exec.a_data;
			exdata.ex_exec.a_bss = (nc < 0) ? 0 : nc;
		}
		break;
#endif
	}
#endif
	switch ((int)exdata.ex_exec.a_magic) {

	case OMAGIC:
		exdata.ex_exec.a_data += exdata.ex_exec.a_text;
		exdata.ex_exec.a_text = 0;
		break;

	case ZMAGIC:
		flags |= SPAGV;
	case NMAGIC:
		if (exdata.ex_exec.a_text == 0) {
			error = ENOEXEC;
			goto bad;
		}
		break;

	default:
		if (exdata.ex_shell[0] != '#' ||
		    exdata.ex_shell[1] != '!' ||
		    indir) {
			error = ENOEXEC;
			goto bad;
		}
		for (cp = &exdata.ex_shell[2];; ++cp) {
			if (cp >= &exdata.ex_shell[MAXINTERP]) {
				error = ENOEXEC;
				goto bad;
			}
			if (*cp == '\n') {
				*cp = '\0';
				break;
			}
			if (*cp == '\t')
				*cp = ' ';
		}
		cp = &exdata.ex_shell[2];
		while (*cp == ' ')
			cp++;
		ndp->ni_dirp = cp;
		while (*cp && *cp != ' ')
			cp++;
		cfarg[0] = '\0';
		if (*cp) {
			*cp++ = '\0';
			while (*cp == ' ')
				cp++;
			if (*cp)
				bcopy((caddr_t)cp, (caddr_t)cfarg, MAXINTERP);
		}
		indir = 1;
		vput(vp);
		ndp->ni_nameiop = LOOKUP | FOLLOW | LOCKLEAF;
		ndp->ni_segflg = UIO_SYSSPACE;
		if (error = namei(ndp, p))
			return (error);
		vp = ndp->ni_vp;
		if (error = VOP_GETATTR(vp, &vattr, cred))
			goto bad;
		bcopy((caddr_t)ndp->ni_dent.d_name, (caddr_t)cfname,
		    MAXCOMLEN);
		cfname[MAXCOMLEN] = '\0';
		uid = cred->cr_uid;	/* shell scripts can't be setuid */
		gid = cred->cr_gid;
		goto again;
	}

	/*
	 * Collect arguments on "file" in swap space.
	 */
	na = 0;
	ne = 0;
	nc = 0;
	cc = NCARGS;
	execargs = kmem_alloc_wait(exec_map, NCARGS);
	cp = (char *) execargs;
	/*
	 * Copy arguments into file in argdev area.
	 */
	if (uap->argp) for (;;) {
		ap = NULL;
		sharg = NULL;
		if (indir && na == 0) {
			sharg = cfname;
			ap = (int)sharg;
			uap->argp++;		/* ignore argv[0] */
		} else if (indir && (na == 1 && cfarg[0])) {
			sharg = cfarg;
			ap = (int)sharg;
		} else if (indir && (na == 1 || na == 2 && cfarg[0]))
			ap = (int)uap->fname;
		else if (uap->argp) {
			ap = fuword((caddr_t)uap->argp);
			uap->argp++;
		}
		if (ap == NULL && uap->envp) {
			uap->argp = NULL;
			if ((ap = fuword((caddr_t)uap->envp)) != NULL)
				uap->envp++, ne++;
		}
		if (ap == NULL)
			break;
		na++;
		if (ap == -1) {
			error = EFAULT;
			goto bad;
		}
		do {
			if (nc >= NCARGS-1) {
				error = E2BIG;
				break;
			}
			if (sharg) {
				error = copystr(sharg, cp, (unsigned)cc, &len);
				sharg += len;
			} else {
				error = copyinstr((caddr_t)ap, cp, (unsigned)cc,
				    &len);
				ap += len;
			}
			cp += len;
			nc += len;
			cc -= len;
		} while (error == ENAMETOOLONG);
		if (error)
			goto bad;
	}
	nc = (nc + NBPW-1) & ~(NBPW-1);
	error = getxfile(p, vp, &exdata.ex_exec, flags, nc + (na+4)*NBPW,
	    uid, gid);
	if (error)
		goto bad;
	vput(vp);
	vp = NULL;

#ifdef HPUXCOMPAT
	/*
	 * We are now committed to the exec so we can save the exec
	 * header in the pcb where we can dump it if necessary in core()
	 */
	if (u.u_pcb.pcb_flags & PCB_HPUXBIN)
		bcopy((caddr_t)&hhead,
		      (caddr_t)u.u_pcb.pcb_exec, sizeof hhead);
#endif

	/*
	 * Copy back arglist.
	 */
	ucp = USRSTACK - sizeof(u.u_pcb.pcb_sigc) - nc - NBPW;
	ap = ucp - na*NBPW - 3*NBPW;
	p->p_regs[SP] = ap;
	(void) suword((caddr_t)ap, na-ne);
	nc = 0;
	cp = (char *) execargs;
	cc = NCARGS;
	for (;;) {
		ap += NBPW;
		if (na == ne) {
			(void) suword((caddr_t)ap, 0);
			ap += NBPW;
		}
		if (--na < 0)
			break;
		(void) suword((caddr_t)ap, ucp);
		do {
			error = copyoutstr(cp, (caddr_t)ucp, (unsigned)cc,
			    &len);
			ucp += len;
			cp += len;
			nc += len;
			cc -= len;
		} while (error == ENAMETOOLONG);
		if (error == EFAULT)
			panic("exec: EFAULT");
	}
	(void) suword((caddr_t)ap, 0);

	execsigs(p);

	for (nc = fdp->fd_lastfile; nc >= 0; --nc) {
		if (fdp->fd_ofileflags[nc] & UF_EXCLOSE) {
			(void) closef(fdp->fd_ofiles[nc], p);
			fdp->fd_ofiles[nc] = NULL;
			fdp->fd_ofileflags[nc] = 0;
			if (nc < fdp->fd_freefile)
				fdp->fd_freefile = nc;
		}
		fdp->fd_ofileflags[nc] &= ~UF_MAPPED;
	}
	/*
	 * Adjust fd_lastfile to account for descriptors closed above.
	 * Don't decrement fd_lastfile past 0, as it's unsigned.
	 */
	while (fdp->fd_lastfile > 0 && fdp->fd_ofiles[fdp->fd_lastfile] == NULL)
		fdp->fd_lastfile--;
	setregs(exdata.ex_exec.a_entry, retval);
	/*
	 * Install sigcode at top of user stack.
	 */
	copyout((caddr_t)u.u_pcb.pcb_sigc,
		(caddr_t)(USRSTACK - sizeof(u.u_pcb.pcb_sigc)),
		sizeof(u.u_pcb.pcb_sigc));
	/*
	 * Remember file name for accounting.
	 */
	p->p_acflag &= ~AFORK;
	if (indir)
		bcopy((caddr_t)cfname, (caddr_t)p->p_comm, MAXCOMLEN);
	else {
		if (ndp->ni_dent.d_namlen > MAXCOMLEN)
			ndp->ni_dent.d_namlen = MAXCOMLEN;
		bcopy((caddr_t)ndp->ni_dent.d_name, (caddr_t)p->p_comm,
		    (unsigned)(ndp->ni_dent.d_namlen + 1));
	}
bad:
	if (execargs)
		kmem_free_wakeup(exec_map, execargs, NCARGS);
#endif SECSIZE
	if (vp)
		vput(vp);
	return (error);
}

/*
 * Read in and set up memory for executed file.
 */
getxfile(p, vp, ep, flags, nargc, uid, gid)
	register struct proc *p;
	register struct vnode *vp;
	register struct exec *ep;
	int flags, nargc, uid, gid;
{
	segsz_t ts, ds, ss;
	register struct ucred *cred = p->p_ucred;
	off_t toff;
	int error = 0;
	vm_offset_t addr;
	vm_size_t size;
	struct vmspace *vm = p->p_vmspace;

#ifdef HPUXCOMPAT
	if (ep->a_mid == MID_HPUX) {
		if (flags & SPAGV)
			toff = CLBYTES;
		else
			toff = sizeof (struct hpux_exec);
	} else
#endif
	if (flags & SPAGV)
		toff = CLBYTES;
	else
		toff = sizeof (struct exec);
	if (ep->a_text != 0 && (vp->v_flag & VTEXT) == 0 &&
	    vp->v_usecount != 1) {
		register struct file *fp;

		for (fp = file; fp < fileNFILE; fp++) {
			if (fp->f_type == DTYPE_VNODE &&
			    fp->f_count > 0 &&
			    (struct vnode *)fp->f_data == vp &&
			    (fp->f_flag & FWRITE)) {
				return (ETXTBSY);
			}
		}
	}

	/*
	 * Compute text and data sizes and make sure not too large.
	 * NB - Check data and bss separately as they may overflow 
	 * when summed together.
	 */
	ts = clrnd(btoc(ep->a_text));
	ds = clrnd(btoc(ep->a_data + ep->a_bss));
	ss = clrnd(SSIZE + btoc(nargc + sizeof(u.u_pcb.pcb_sigc)));

	/*
	 * If we're sharing the address space, allocate a new space
	 * and release our reference to the old one.  Otherwise,
	 * empty out the existing vmspace.
	 */
	if (vm->vm_refcnt > 1) {
		p->p_vmspace = vmspace_alloc(vm_map_min(&vm->vm_map),
		    vm_map_max(&vm->vm_map), 1);
		vmspace_free(vm);
		vm = p->p_vmspace;
	} else {
#ifdef SYSVSHM
		if (vm->vm_shm)
			shmexit(p);
#endif
		(void) vm_map_remove(&vm->vm_map, vm_map_min(&vm->vm_map),
		    vm_map_max(&vm->vm_map));
	}
	/*
	 * If parent is waiting for us to exec or exit,
	 * SPPWAIT will be set; clear it and wakeup parent.
	 */
	if (p->p_flag & SPPWAIT) {
		p->p_flag &= ~SPPWAIT;
		wakeup((caddr_t) p->p_pptr);
	}
#ifdef hp300
	u.u_pcb.pcb_flags &= ~(PCB_AST|PCB_HPUXMMAP|PCB_HPUXBIN);
#ifdef HPUXCOMPAT
	/* remember that we were loaded from an HPUX format file */
	if (ep->a_mid == MID_HPUX)
		u.u_pcb.pcb_flags |= PCB_HPUXBIN;
#endif
#endif
	p->p_flag &= ~(SPAGV|SSEQL|SUANOM|SHPUX);
	p->p_flag |= flags | SEXEC;
	addr = VM_MIN_ADDRESS;
	if (vm_allocate(&vm->vm_map, &addr, round_page(ctob(ts + ds)), FALSE)) {
		uprintf("Cannot allocate text+data space\n");
		error = ENOMEM;			/* XXX */
		goto badmap;
	}
	size = round_page(MAXSSIZ);		/* XXX */
	addr = trunc_page(VM_MAX_ADDRESS - size);
	if (vm_allocate(&vm->vm_map, &addr, size, FALSE)) {
		uprintf("Cannot allocate stack space\n");
		error = ENOMEM;			/* XXX */
		goto badmap;
	}
	vm->vm_maxsaddr = (caddr_t)addr;
	vm->vm_taddr = (caddr_t)VM_MIN_ADDRESS;
	vm->vm_daddr = (caddr_t)(VM_MIN_ADDRESS + ctob(ts));

	if ((flags & SPAGV) == 0) {
		/*
		 * Read in data segment.
		 */
		(void) vn_rdwr(UIO_READ, vp, vm->vm_daddr, (int) ep->a_data,
			(off_t)(toff + ep->a_text), UIO_USERSPACE,
			(IO_UNIT|IO_NODELOCKED), cred, (int *)0);
		/*
		 * Read in text segment if necessary (0410),
		 * and read-protect it.
		 */
		if (ep->a_text > 0) {
			error = vn_rdwr(UIO_READ, vp, vm->vm_taddr,
				(int)ep->a_text, toff, UIO_USERSPACE,
				(IO_UNIT|IO_NODELOCKED), cred, (int *)0);
			(void) vm_map_protect(&vm->vm_map, VM_MIN_ADDRESS,
				VM_MIN_ADDRESS + trunc_page(ep->a_text),
				VM_PROT_READ|VM_PROT_EXECUTE, FALSE);
		}
	} else {
		/*
		 * Allocate a region backed by the exec'ed vnode.
		 */
		addr = VM_MIN_ADDRESS;
		size = round_page(ep->a_text + ep->a_data);
		error = vm_mmap(&vm->vm_map, &addr, size, VM_PROT_ALL,
			MAP_FILE|MAP_COPY|MAP_FIXED,
			(caddr_t)vp, (vm_offset_t)toff);
		(void) vm_map_protect(&vm->vm_map, addr,
			addr + trunc_page(ep->a_text),
			VM_PROT_READ|VM_PROT_EXECUTE, FALSE);
		vp->v_flag |= VTEXT;
	}
badmap:
	if (error) {
		printf("pid %d: VM allocation failure\n", p->p_pid);
		uprintf("sorry, pid %d was killed in exec: VM allocation\n",
			p->p_pid);
		psignal(p, SIGKILL);
		p->p_flag |= SULOCK;
		return(error);
	}

	/*
	 * set SUID/SGID protections, if no tracing
	 */
	if ((p->p_flag&STRC)==0) {
		if (uid != cred->cr_uid || gid != cred->cr_gid) {
			p->p_ucred = cred = crcopy(cred);
			/*
			 * If process is being ktraced, turn off - unless
			 * root set it.
			 */
			if (p->p_tracep && !(p->p_traceflag & KTRFAC_ROOT)) {
				vrele(p->p_tracep);
				p->p_tracep = NULL;
				p->p_traceflag = 0;
			}
		}
		cred->cr_uid = uid;
		cred->cr_gid = gid;
	} else
		psignal(p, SIGTRAP);
	p->p_cred->p_svuid = cred->cr_uid;
	p->p_cred->p_svgid = cred->cr_gid;
	vm->vm_tsize = ts;
	vm->vm_dsize = ds;
	vm->vm_ssize = ss;
	p->p_stats->p_prof.pr_scale = 0;
#if defined(tahoe)
	u.u_pcb.pcb_savacc.faddr = (float *)NULL;
#endif
	return (0);
}
