#
# Copyright (c) 1987 Regents of the University of California.
# All rights reserved.
#
# Redistribution and use in source and binary forms are permitted
# provided that the above copyright notice and this paragraph are
# duplicated in all such forms and that any documentation,
# advertising materials, and other materials related to such
# distribution and use acknowledge that the software was developed
# by the University of California, Berkeley.  The name of the
# University may not be used to endorse or promote products derived
# from this software without specific prior written permission.
# THIS SOFTWARE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR
# IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
# WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE.
#
#	@(#)Makefile	5.4.3 (2.11BSD GTE) 1996/3/20
#
TOPSRC!=cd ../../..; pwd
include ${TOPSRC}/share/mk/sys.mk

MDIR=	/usr/share/man/cat5
SRCS=	L-devices.5 L-dialcodes.5 L.aliases.5 L.cmds.5 L.sys.5 \
	USERFILE.5 a.out.5 acct.5 aliases.5 core.5 dbx.5 \
	dir.5 disktab.5 dtab.5 dump.5 fs.5 fstab.5 gettytab.5 \
	group.5 hosts.5 intro.5 map3270.5 networks.5 passwd.5 \
	phones.5 plot.5 printcap.5 protocols.5 remote.5 resolver.5 \
	services.5 shells.5 stack.5 tar.5 termcap.5 tp.5 ttys.5 types.5 \
	syserrlst.5 \
	tzfile.5 utmp.5 uuencode.5 vfont.5 vgrindefs.5
OBJS=	L-devices.0 L-dialcodes.0 L.aliases.0 L.cmds.0 L.sys.0 \
	USERFILE.0 a.out.0 acct.0 aliases.0 core.0 dbx.0 \
	dir.0 disktab.0 dtab.0 dump.0 fs.0 fstab.0 gettytab.0 \
	group.0 hosts.0 intro.0 map3270.0 networks.0 passwd.0 \
	phones.0 plot.0 printcap.0 protocols.0 remote.0 resolver.0 \
	services.0 shells.0 stack.0 tar.0 termcap.0 tp.0 ttys.0 types.0 \
	syserrlst.0 \
	tzfile.0 utmp.0 uuencode.0 vfont.0 vgrindefs.0

.SUFFIXES: .5 .0

.5.0:
	${MANROFF} $*.5 > $*.0

all: _make_01

_make_01: ${OBJS}

clean: FRC
	rm -f ${OBJS}

install: _make_01
	cp ${OBJS} ${DESTDIR}${MDIR}
	ln -f ${DESTDIR}${MDIR}/fs.0 ${DESTDIR}${MDIR}/inode.0
	ln -f ${DESTDIR}${MDIR}/dump.0 ${DESTDIR}${MDIR}/dumpdates.0
	ln -f ${DESTDIR}${MDIR}/utmp.0 ${DESTDIR}${MDIR}/wtmp.0

FRC:
