#
# Copyright (c) 1980 Regents of the University of California.
# All rights reserved.  The Berkeley software License Agreement
# specifies the terms and conditions for redistribution.
#
#	@(#)Makefile	5.2.4 (2.11BSD GTE) 1997/9/22
#
# reorder gives an editor command for most common terminals
# (in reverse order from n'th to 1'st most commonly used)
# to move them to the front of termcap
#
TOPSRC!=cd ../../..; pwd
include ${TOPSRC}/share/mk/sys.mk

termcap.full: reorder termcap.src
	ex - termcap.src < reorder

install: termcap.small termcap.full
	rm -f ${DESTDIR}/usr/share/misc/termcap
	${INSTALL} -c -m 444 termcap.small ${DESTDIR}/usr/share/misc/termcap
#	${INSTALL} -c -m 444 termcap.full ${DESTDIR}/etc/termcap.full

tabset: FRC
	rm -fr ${DESTDIR}/usr/share/tabset
	mkdir ${DESTDIR}/usr/share/tabset
	chmod 755 ${DESTDIR}/usr/share/tabset
	${INSTALL} -c -m 444 tabset/* ${DESTDIR}/usr/share/tabset

clean:
	rm -f termcap.full

FRC:
