.\" Copyright (c) 1983, 1986 The Regents of the University of California.
.\" All rights reserved.
.\"
.\" Redistribution and use in source and binary forms are permitted
.\" provided that the above copyright notice and this paragraph are
.\" duplicated in all such forms and that any documentation,
.\" advertising materials, and other materials related to such
.\" distribution and use acknowledge that the software was developed
.\" by the University of California, Berkeley.  The name of the
.\" University may not be used to endorse or promote products derived
.\" from this software without specific prior written permission.
.\" THIS SOFTWARE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR
.\" IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
.\" WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
.\"
.\"	@(#)4.t	6.4 (Berkeley) 03/07/89
.\"
.nr H2 1
.\".ds RH "Address representation
.br
.ne 2i
.NH
\s+2Internal address representation\s0
.PP
Common to all portions of the system are two data structures.
These structures are used to represent
addresses and various data objects.
Addresses, internally are described by the \fIsockaddr\fP structure,
.DS
._f
struct sockaddr {
	short	sa_family;	/* data format identifier */
	char	sa_data[14];	/* address */
};
.DE
All addresses belong to one or more \fIaddress families\fP
which define their format and interpretation.
The \fIsa_family\fP field indicates the address family to which the address
belongs, and the \fIsa_data\fP field contains the actual data value.
The size of the data field, 14 bytes, was selected based on a study
of current address formats.*
Specific address formats use private structure definitions
that define the format of the data field.
The system interface supports larger address structures,
although address-family-independent support facilities, for example routing
and raw socket interfaces, provide only 14 bytes for address storage.
Protocols that do not use those facilities (e.g, the current Unix domain)
may use larger data areas.
.FS
* Later versions of the system may support variable length addresses.
.FE
