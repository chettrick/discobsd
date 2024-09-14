#
# Public Domain.  1996/2/20 - Steven Schultz
#
#	@(#)Makefile	1.0 (2.11BSD GTE) 1996/2/20
#
CFLAGS=	 -O
SEPFLAG= -i
VPATH=	/usr/src/bin/ls
SRCS=	xinstall.c stat_flags.c
OBJS=	xinstall.o stat_flags.o
MAN=	install.0
MANSRC=	install.1

all: xinstall ${MAN}

xinstall: ${OBJS}
	${CC} ${CFLAGS} ${SEPFLAG} -o $@ ${OBJS}

install.0: ${MANSRC}
	/usr/man/manroff ${MANSRC} > ${MAN}

clean:
	rm -f ${OBJS} ${MAN} xinstall tags 

depend: ${SRCS}
	mkdep ${CFLAGS} ${SRCS}

install: all
	install -c -o bin -g bin -m 444 ${MAN} ${DESTDIR}/usr/man/cat1
	install -s -o root -g bin -m 755 xinstall ${DESTDIR}/usr/bin/install

lint: ${SRCS}
	lint -hax ${SRCS}

tags: ${SRCS}
	ctags ${SRCS}
# DO NOT DELETE THIS LINE -- mkdep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.
