#
# Copyright (c) 1987 Regents of the University of California.
# All rights reserved.  The Berkeley software License Agreement
# specifies the terms and conditions for redistribution.
#
#	crt0	Normal C run time startoff
#
TOPSRC!=cd ../../..; pwd
include ${TOPSRC}/target.mk

COPTS!=if [ x"${MACHINE_ARCH}" = x"mips" ] ; then \
		echo "-Wa,-x -B${TOPSRC}/lib/libc_aout/" ; \
	else \
		echo "" ; \
	fi

CFLAGS=		-Os ${DEFS} -Wall ${COPTS}

SRCS=		${TOPSRC}/lib/startup-${MACHINE_ARCH}/crt0.c
OBJS=		../crt0.o

all:		${OBJS}

${OBJS}:	${SRCS}
		${CC} ${DEFS} ${CFLAGS} -c ${SRCS} -o $@

install:	${OBJS}
		${INSTALLDIR} ${DESTDIR}/usr/lib
		${INSTALL} ${OBJS} ${DESTDIR}/usr/lib/crt0.o

clean:
		rm -f ${OBJS} *~
