# @(#)Makefile	1.6 Makefile 1997/12/28
TOPSRC!=cd ../..; pwd
include ${TOPSRC}/share/mk/sys.mk

# If you want something other than Western United States time used on your
# system, change the line below (after finding the zone you want in the
# time zone files, or adding it to a time zone file).
# Alternately, if you discover you've got the wrong time zone, you can just
#	install -c -m 444 /usr/share/zoneinfo/right_zone /etc/localtime

LOCALTIME=	US/Pacific

# If you want code inspired by certain emerging standards, add
#	-DSTD_INSPIRED
# to the end of the "CFLAGS=" line.
#
# If you want to handle solar-time-based time zones, remove the
# "#define NOSOLAR" from the include file usr/include/tzfile.h.
# (and add solar87 to the DATA= line below).

CFLAGS= -O
LINTFLAGS=	-phbaxc
CC=		${HOST_CC}
CFLAGS+=	-Wall -idirafter ${TOPSRC}/include

# libbsd-dev package on Linux in overlay mode.
CFLAGS+=	${_LIBBSD_CFLAGS}
LIBS+=		${_LIBBSD_LIBS}

TZCSRCS=	zic.c scheck.c ialloc.c
TZCOBJS=	zic.o scheck.o ialloc.o
TZDSRCS=	zdump.c ialloc.c
TZDOBJS=	zdump.o ialloc.o
SOURCES=	zic.c zdump.c scheck.c ialloc.c
DATA=		asia australasia europe etcetera northamerica \
		pacificnew systemv
ZICMAN=		zic.0
ZICMANSRC=	zic.8
ZDUMAN=		zdump.0
ZDUMANSRC=	zdump.8
MAN=		$(ZICMAN) $(ZDUMAN)
MANSRC=		$(ZICMANSRC) $(ZDUMAN)
SHARDIR=	${DESTDIR}/usr/share/zoneinfo
MANDIR=		${DESTDIR}/usr/share/man/cat8

all:		zdump zic ${ZICMAN} ${ZDUMAN}

install:	zic $(DATA) $(MAN)
		(umask 22; ./zic -v -d ${SHARDIR} $(DATA))
#		${INSTALL} -c ${ZICMAN} ${MANDIR}/${ZICMAN}
#		${INSTALL} -c ${ZDUMAN} ${MANDIR}/${ZDUMAN}
#		${INSTALL} -c -s zic ${DESTDIR}/usr/sbin/zic

zdump.0:	$(ZDUMANSRC)
		${MANROFF} ${ZDUMANSRC} > ${ZDUMAN}

zic.0:		$(ZICMANSRC)
		${MANROFF} ${ZICMANSRC} > ${ZICMAN}

zdump:		$(TZDOBJS)
		${CC} ${CFLAGS} ${LFLAGS} ${TZDOBJS} -o $@ ${LIBS}

zic:		$(TZCOBJS)
		${CC} ${CFLAGS} ${LFLAGS} ${TZCOBJS} -o $@ ${LIBS}

lint:		$(TZCSRCS) $(TZDSRCS)
		lint $(LINTFLAGS) $(CFLAGS) $(TZCSRCS)
		lint $(LINTFLAGS) $(CFLAGS) $(TZDSRCS)

clean:
		rm -f *.o zdump zic $(ZICMAN) $(ZDUMAN)
