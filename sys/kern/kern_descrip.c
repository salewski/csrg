/*	kern_descrip.c	5.8	82/09/08	*/

#include "../h/param.h"
#include "../h/systm.h"
#include "../h/dir.h"
#include "../h/user.h"
#include "../h/kernel.h"
#include "../h/inode.h"
#include "../h/proc.h"
#include "../h/conf.h"
#include "../h/file.h"
#include "../h/inline.h"
#include "../h/socket.h"
#include "../h/socketvar.h"
#include "../h/mount.h"

#include "../h/descrip.h"

/*
 * Descriptor management.
 */

/*
 * TODO:
 *	getf should be renamed
 *	ufalloc side effects are gross
 */

/*
 * System calls on descriptors.
 */
getdtablesize()
{

	u.u_r.r_val1 = NOFILE;
}

getdprop()
{
	register struct a {
		int	d;
		struct	dtype *dtypeb;
	} *uap = (struct a *)u.u_ap;
	register struct file *fp;
	struct dtype adtype;

	fp = getf(uap->d);
	if (fp == 0)
		return;
	adtype.dt_type = 0;		/* XXX */
	adtype.dt_protocol = 0;		/* XXX */
	if (copyout((caddr_t)&adtype, (caddr_t)uap->dtypeb,
	    sizeof (struct dtype)) < 0) {
		u.u_error = EFAULT;
		return;
	}
}

getdopt()
{

}

setdopt()
{

}

dup()
{
	register struct a {
		int	i;
	} *uap = (struct a *) u.u_ap;
	struct file *fp;
	int j;

	if (uap->i &~ 077) { uap->i &= 077; dup2(); return; }	/* XXX */

	fp = getf(uap->i);
	if (fp == 0)
		return;
	j = ufalloc();
	if (j < 0)
		return;
	dupit(j, fp, u.u_pofile[uap->i] & (RDLOCK|WRLOCK));
}

dup2()
{
	register struct a {
		int	i, j;
	} *uap = (struct a *) u.u_ap;
	register struct file *fp;

	fp = getf(uap->i);
	if (fp == 0)
		return;
	if (uap->j < 0 || uap->j >= NOFILE) {
		u.u_error = EBADF;
		return;
	}
	u.u_r.r_val1 = uap->j;
	if (uap->i == uap->j)
		return;
	if (u.u_ofile[uap->j]) {
		closef(u.u_ofile[uap->j], 0, u.u_pofile[uap->j]);
		if (u.u_error)
			return;
		/* u.u_ofile[uap->j] = 0; */
		/* u.u_pofile[uap->j] = 0; */
	}
	dupit(uap->j, fp, u.u_pofile[uap->i] & (RDLOCK|WRLOCK));
}

dupit(fd, fp, lockflags)
	int fd;
	register struct file *fp;
	register int lockflags;
{

	u.u_ofile[fd] = fp;
	u.u_pofile[fd] = lockflags;
	fp->f_count++;
	if (lockflags&RDLOCK)
		fp->f_inode->i_rdlockc++;
	if (lockflags&WRLOCK)
		fp->f_inode->i_wrlockc++;
}

close()
{
	register struct a {
		int	i;
	} *uap = (struct a *)u.u_ap;
	register struct file *fp;

	fp = getf(uap->i);
	if (fp == 0)
		return;
	closef(fp, 0, u.u_pofile[uap->i]);
	/* WHAT IF u.u_error ? */
	u.u_ofile[uap->i] = NULL;
	u.u_pofile[uap->i] = 0;
}

wrap()
{
	register struct a {
		int	d;
		struct	dtype *dtypeb;
	} *uap = (struct a *)u.u_ap;
	register struct file *fp;
	struct dtype adtype;

	fp = getf(uap->d);
	if (fp == 0)
		return;
	if (copyin((caddr_t)uap->dtypeb, (caddr_t)&adtype,
	    sizeof (struct dtype)) < 0) {
		u.u_error = EFAULT;
		return;
	}
	/* DO WRAP */
}

int	unselect();
int	nselcoll;
/*
 * Select system call.
 */
select()
{
	register struct uap  {
		long	*ibits;
		long	*obits;
		struct	timeval *tv;
	} *uap = (struct uap *)u.u_ap;
	int ibits[3], obits[3];
	struct timeval atv;
	int s, tsel, ncoll, rem;
	label_t lqsave;

	if (uap->tv) {
		if (copyin((caddr_t)uap->tv, (caddr_t)&atv, sizeof (atv))) {
			u.u_error = EFAULT;
			return;
		}
		if (itimerfix(&atv)) {
			u.u_error = EINVAL;
			return;
		}
		s = spl7(); timevaladd(&atv, &time); splx(s);
	}
	if (copyin((caddr_t)uap->ibits, (caddr_t)ibits, sizeof (ibits))) {
		u.u_error = EFAULT;
		return;
	}
retry:
	ncoll = nselcoll;
	u.u_procp->p_flag |= SSEL;
	u.u_r.r_val1 = selscan(ibits, obits);
	if (u.u_error)
		return;
	if (u.u_r.r_val1)
		goto done;
	s = spl6();
	if (uap->tv && timercmp(&atv, &time, >=)) {
		splx(s);
		goto done;
	}
	if ((u.u_procp->p_flag & SSEL) == 0 || nselcoll != ncoll) {
		u.u_procp->p_flag &= ~SSEL;
		splx(s);
		goto retry;
	}
	u.u_procp->p_flag &= ~SSEL;
	if (uap->tv) {
		lqsave = u.u_qsave;
		if (setjmp(&u.u_qsave)) {
			untimeout(unselect, u.u_procp);
			u.u_error = EINTR;
			splx(s);
			return;
		}
		timeout(unselect, u.u_procp, hzto(&atv));
	}
	sleep((caddr_t)&selwait, PZERO+1);
	if (uap->tv) {
		u.u_qsave = lqsave;
		untimeout(unselect, u.u_procp);
	}
	splx(s);
	goto retry;
done:
	if (copyout((caddr_t)obits, (caddr_t)uap->obits, sizeof (obits))) {
		u.u_error = EFAULT;
		return;
	}
}

unselect(p)
	register struct proc *p;
{
	register int s = spl6();

	switch (p->p_stat) {

	case SSLEEP:
		setrun(p);
		break;

	case SSTOP:
		unsleep(p);
		break;
	}
	splx(s);
}

selscan(ibits, obits)
	int *ibits, *obits;
{
	register int which, bits, i;
	int flag;
	struct file *fp;
	int able;
	struct inode *ip;
	int n = 0;

	for (which = 0; which < 3; which++) {
		bits = ibits[which];
		obits[which] = 0;
		switch (which) {

		case 0:
			flag = FREAD; break;

		case 1:
			flag = FWRITE; break;

		case 2:
			flag = 0; break;
		}
		while (i = ffs(bits)) {
			bits &= ~(1<<(i-1));
			fp = u.u_ofile[i-1];
			if (fp == NULL) {
				u.u_error = EBADF;
				break;
			}
			if (fp->f_type == DTYPE_SOCKET)
				able = soselect(fp->f_socket, flag);
			else {
				ip = fp->f_inode;
				switch (ip->i_mode & IFMT) {

				case IFCHR:
					able =
					    (*cdevsw[major(ip->i_rdev)].d_select)
						(ip->i_rdev, flag);
					break;

				case IFBLK:
				case IFREG:
				case IFDIR:
					able = 1;
					break;
				}

			}
			if (able) {
				obits[which] |= (1<<(i-1));
				n++;
			}
		}
	}
	return (n);
}

/*ARGSUSED*/
seltrue(dev, flag)
	dev_t dev;
	int flag;
{

	return (1);
}

selwakeup(p, coll)
	register struct proc *p;
	int coll;
{
	int s;

	if (coll) {
		nselcoll++;
		wakeup((caddr_t)&selwait);
	}
	if (p) {
		if (p->p_wchan == (caddr_t)&selwait)
			setrun(p);
		else {
			s = spl6();
			if (p->p_flag & SSEL)
				p->p_flag &= ~SSEL;
			splx(s);
		}
	}
}

revoke()
{

	/* XXX */
}

/*
 * Allocate a user file descriptor.
 */
ufalloc()
{
	register i;

	for (i=0; i<NOFILE; i++)
		if (u.u_ofile[i] == NULL) {
			u.u_r.r_val1 = i;
			u.u_pofile[i] = 0;
			return (i);
		}
	u.u_error = EMFILE;
	return (-1);
}

struct	file *lastf;
/*
 * Allocate a user file descriptor
 * and a file structure.
 * Initialize the descriptor
 * to point at the file structure.
 */
struct file *
falloc()
{
	register struct file *fp;
	register i;

	i = ufalloc();
	if (i < 0)
		return (NULL);
	if (lastf == 0)
		lastf = file;
	for (fp = lastf; fp < fileNFILE; fp++)
		if (fp->f_count == 0)
			goto slot;
	for (fp = file; fp < lastf; fp++)
		if (fp->f_count == 0)
			goto slot;
	tablefull("file");
	u.u_error = ENFILE;
	return (NULL);
slot:
	u.u_ofile[i] = fp;
	fp->f_count++;
	fp->f_offset = 0;
	fp->f_inode = 0;
	lastf = fp + 1;
	return (fp);
}
/*
 * Convert a user supplied file descriptor into a pointer
 * to a file structure.  Only task is to check range of the descriptor.
 * Critical paths should use the GETF macro, defined in inline.h.
 */
struct file *
getf(f)
	register int f;
{
	register struct file *fp;

	if ((unsigned)f >= NOFILE || (fp = u.u_ofile[f]) == NULL) {
		u.u_error = EBADF;
		return (NULL);
	}
	return (fp);
}

/*
 * Internal form of close.
 * Decrement reference count on
 * file structure.
 * Also make sure the pipe protocol
 * does not constipate.
 *
 * Decrement reference count on the inode following
 * removal to the referencing file structure.
 * Call device handler on last close.
 * Nouser indicates that the user isn't available to present
 * errors to.
 *
 * Handling locking at this level is RIDICULOUS.
 */
closef(fp, nouser, flags)
	register struct file *fp;
	int nouser, flags;
{
	register struct inode *ip;
	register struct mount *mp;
	int flag, mode;
	dev_t dev;
	register int (*cfunc)();

	if (fp == NULL)
		return;
	if (fp->f_count > 1) {
		fp->f_count--;
		return;
	}
	if (fp->f_type == DTYPE_SOCKET) {
		u.u_error = 0;			/* XXX */
		soclose(fp->f_socket, nouser);
		if (nouser == 0 && u.u_error)
			return;
		fp->f_socket = 0;
		fp->f_count = 0;
		return;
	}
	flag = fp->f_flag;
	ip = fp->f_inode;
	dev = (dev_t)ip->i_rdev;
	mode = ip->i_mode & IFMT;
	flags &= RDLOCK|WRLOCK;			/* conservative */
	if (flags)
		funlocki(ip, flags);
	ilock(ip);
	iput(ip);
	fp->f_count = 0;

	switch (mode) {

	case IFCHR:
		cfunc = cdevsw[major(dev)].d_close;
		break;

	case IFBLK:
		/*
		 * We don't want to really close the device if it is mounted
		 */
		for (mp = mount; mp < &mount[NMOUNT]; mp++)
			if (mp->m_bufp != NULL && mp->m_dev == dev)
				return;
		cfunc = bdevsw[major(dev)].d_close;
		break;

	default:
		return;
	}
	for (fp = file; fp < fileNFILE; fp++) {
		if (fp->f_type == DTYPE_SOCKET)		/* XXX */
			continue;
		if (fp->f_count && (ip = fp->f_inode) &&
		    ip->i_rdev == dev && (ip->i_mode&IFMT) == mode)
			return;
	}
	if (mode == IFBLK) {
		/*
		 * On last close of a block device (that isn't mounted)
		 * we must invalidate any in core blocks
		 */
		bflush(dev);
		binval(dev);
	}
	(*cfunc)(dev, flag, fp);
}
