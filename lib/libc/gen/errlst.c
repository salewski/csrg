/* @(#)errlst.c	4.2 (Berkeley) 09/25/81 */
char	*sys_errlist[] = {
	"Error 0",
	"Not owner",
	"No such file or directory",
	"No such process",
	"Interrupted system call",
	"I/O error",
	"No such device or address",
	"Arg list too long",
	"Exec format error",
	"Bad file number",
	"No children",
	"No more processes",
	"Not enough core",
	"Permission denied",
	"Bad address",
	"Block device required",
	"Mount device busy",
	"File exists",
	"Cross-device link",
	"No such device",
	"Not a directory",
	"Is a directory",
	"Invalid argument",
	"File table overflow",
	"Too many open files",
	"Not a typewriter",
	"Text file busy",
	"File too large",
	"No space left on device",
	"Illegal seek",
	"Read-only file system",
	"Too many links",
	"Broken pipe",
	"Argument too large",
	"Result too large",
	/* bbnnet errors */
	"User status available (not an error)",
	"Open attempted when net not initialized",
	"Too many net connections",
	"No more net buffer space",
	"Error from network",
	"Error in raw link or protocol range",
	"Unable to open connection",
	"Unable to send on raw connection",
};
int	sys_nerr = { sizeof sys_errlist/sizeof sys_errlist[0] };
