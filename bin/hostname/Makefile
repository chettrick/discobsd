#
# Public Domain.  1995/03/13 - Steven Schultz
#
TOPSRC!=cd ../..; pwd
include ${TOPSRC}/share/mk/sys.mk

CFLAGS		+= -Wall

SRCS            = hostname.c
OBJS            = hostname.o
MAN             = hostname.0

all:            hostname ${MAN}

hostname:       ${OBJS}
		${CC} ${CFLAGS} ${LDFLAGS} -o hostname.elf ${OBJS} ${LIBS}
		${OBJDUMP} -S hostname.elf > hostname.dis
		${SIZE} hostname.elf
		${ELF2AOUT} hostname.elf $@ && rm hostname.elf

.SUFFIXES:      .0 .1

.1.0:
		${MANROFF} $*.1 > $@

clean:
		rm -f *.o *.elf ${MAN} hostname *.elf *.dis tags *~

depend:         ${SRCS}
		mkdep ${CFLAGS} ${SRCS}

install:        all
		${INSTALL} hostname ${DESTDIR}/bin/
		cp hostname.0 ${DESTDIR}/usr/share/man/cat1/

lint:           ${SRCS}
		lint -hax ${SRCS}

tags:           ${SRCS}
		ctags ${SRCS}
