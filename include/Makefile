#
# Copyright (c) 1980 Regents of the University of California.
# All rights reserved.  The Berkeley software License Agreement
# specifies the terms and conditions for redistribution.
#
#	@(#)Makefile	5.16.1 (2.11BSD GTE) 1997/9/22
#
# Doing a ``make includes'' builds ${DESTDIR}/usr/include
#
# Define SHARED to indicate whether you want
# symbolic links to the system source (``symlinks''),
# or a separate copy (``copies'').
# (latter useful in environments where it's
# not possible to keep /sys publicly readable)
#
# The ``rm -rf''s used below are safe because rm doesn't
# follow symbolic links.
#
TOPSRC!=cd ..; pwd
include ${TOPSRC}/share/mk/sys.mk

STD=	a.out.h alloca.h ar.h assert.h \
	ctype.h curses.h \
	fcntl.h float.h fstab.h \
	grp.h kmem.h lastlog.h limits.h \
	math.h mtab.h ndbm.h nlist.h \
	paths.h psout.h pwd.h \
	ranlib.h regexp.h \
	setjmp.h sgtty.h \
	stdarg.h stddef.h stdint.h stdio.h stdlib.h \
	string.h strings.h struct.h syscall.h sysexits.h \
	term.h time.h ttyent.h tzfile.h \
	unistd.h utmp.h vmf.h

LINKS=	errno.h signal.h syslog.h

SUBDIRS=arpa

# XXX This is temporary
SUBDIRS+=smallc smallc/sys

MACHDEP=${MACHINE}
NETDIRS=netinet
SYSDIRS=${NETDIRS} ${MACHDEP}

#SHARED=	symlinks
SHARED=	copies
TAGSFILE=tags

all:

includes: ${SHARED}
	-for i in ${STD}; do \
		cmp -s $$i ${DESTDIR}/usr/include/$$i || \
		    ${INSTALL} -c -m 444 $$i ${DESTDIR}/usr/include/$$i; \
	done
	-for i in ${SUBDIRS}; do \
		if [ ! -d ${DESTDIR}/usr/include/$$i ]; \
		then \
			mkdir ${DESTDIR}/usr/include/$$i; \
		fi; \
		(cd $$i; for j in *.[ih]; do \
			cmp -s $$j ${DESTDIR}/usr/include/$$i/$$j || \
			${INSTALL} -c -m 444 $$j ${DESTDIR}/usr/include/$$i/$$j; \
		done); \
	done
# XXX	-cmp -s Makefile.install ${DESTDIR}/usr/include/Makefile || \
# XXX	    ${INSTALL} -c -m 444 Makefile.install ${DESTDIR}/usr/include/Makefile
	-for i in ${LINKS}; do \
		rm -f ${DESTDIR}/usr/include/$$i; \
		ln -s sys/$$i ${DESTDIR}/usr/include/$$i; \
	done
	rm -f ${DESTDIR}/usr/include/machine
	ln -s ${MACHINE} ${DESTDIR}/usr/include/machine
# XXX	rm -f ${DESTDIR}/usr/include/frame.h
# XXX	ln -s machine/frame.h ${DESTDIR}/usr/include/frame.h

# XXX symlinks:
# XXX 	-for i in ${SYSDIRS}; do \
# XXX 		rm -rf ${DESTDIR}/usr/include/$$i; \
# XXX 		if [ ! -s ${DESTDIR}/usr/include/$$i ]; \
# XXX 		then \
# XXX 			ln -s ../../sys/$$i ${DESTDIR}/usr/include/$$i; \
# XXX 		else \
# XXX 			echo ${DESTDIR}/usr/include/$$i not removed; \
# XXX 		fi; \
# XXX 	done
# XXX 	rm -rf ${DESTDIR}/usr/include/sys
# XXX 	-if [ ! -s ${DESTDIR}/usr/include/sys ]; \
# XXX 	then \
# XXX 		ln -s ../../sys/h ${DESTDIR}/usr/include/sys; \
# XXX 	else \
# XXX 		echo ${DESTDIR}/usr/include/sys not removed; \
# XXX 	fi

copies:
	if [ -z "${DESTDIR}" ] || [ "${DESTDIR}" = "/" ]; \
	then \
		echo "DESTDIR must be set and must not be /"; \
		exit 1; \
	fi; \
	if [ -z "${DESTDIR}" ] || [ "${DESTDIR}" = "${TOPSRC}" ]; \
	then \
		echo "DESTDIR is same as TOPSRC: skip copying SYSDIRS"; \
	else \
		if [ ! -d ${DESTDIR}/usr/include ]; \
		then \
			mkdir ${DESTDIR}/usr/include; \
		fi; \
		for i in ${SYSDIRS}; do \
			rm -rf ${DESTDIR}/usr/include/$$i; \
			tar cf - $$i/*.h | \
			    (cd ${DESTDIR}/usr/include; tar xpf -); \
		done; \
		rm -rf ${DESTDIR}/usr/include/sys; \
		mkdir ${DESTDIR}/usr/include/sys; \
		chmod 775 ${DESTDIR}/usr/include/sys; \
		(cd ../sys/include; tar cf - *.h | \
		    (cd ${DESTDIR}/usr/include/sys; tar xpf -)); \
	fi

# XXX tags:
# XXX 	cwd=${TOPSRC}/include; \
# XXX 	for i in ${STD} ${LINKS}; do \
# XXX 		ctags -a -f ${TAGSFILE} $$cwd/$$i; \
# XXX 	done

clean:
	rm -f tags
