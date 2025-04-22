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
#	@(#)Makefile	5.3 (Berkeley) 7/9/88
#
TOPSRC!=cd ../..; pwd
include ${TOPSRC}/share/mk/sys.mk

DIRS=	man1 man2 man3 man4 man5 man6 man7 man8 man9
CDIRS=	cat1 cat2 cat3 cat4 cat5 cat6 cat7 cat8 cat9
LCDIRS=	cat1 cat2 cat3 cat4 cat5 cat6 cat7 cat8 cat9

all:
	for i in ${DIRS}; do \
		${MAKE} -C $$i; \
	done

whatis.db:
	cp /dev/null whatis_tmp
	for dirext in ${DESTDIR}/usr/share/man/cat* ; do \
	echo $$dirext; \
	for file in `find $$dirext -type f -name '*.0' -print`; do \
		sed -n -f makewhatis.sed $$file; \
	done >> whatis_tmp; \
	done
	sort -u whatis_tmp > whatis.db
	-rm -f whatis_tmp

install: FRC
	for i in ${DIRS}; do \
		${MAKE} -C $$i install; \
	done
	${MAKE} whatis.db
	cp whatis.db ${DESTDIR}/usr/share/man/whatis
	cp man.template ${DESTDIR}/usr/share/man/man.template
#	cp makewhatis.sed ${DESTDIR}/usr/share/man/makewhatis.sed
#	${INSTALL} -c -m 755 manroff ${DESTDIR}/usr/share/man/manroff

clean: FRC
	for i in ${DIRS}; do \
		${MAKE} -C $$i clean; \
	done
	rm -f whatis_tmp whatis.db

depend:
lint:
tags:
FRC:
