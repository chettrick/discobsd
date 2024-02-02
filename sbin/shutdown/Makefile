#
# Public Domain.  1996/11/16 - Steven Schultz
# Should be installed with group operator.
#
TOPSRC!=cd ../..; pwd
include ${TOPSRC}/share/mk/sys.mk

SRCS		= shutdown.c
OBJS		= shutdown.o
MAN		= shutdown.0
MANSRC		= shutdown.8

all:            shutdown ${MAN}

shutdown:       ${OBJS}
		${CC} ${CFLAGS} ${LDFLAGS} -o shutdown.elf ${OBJS} ${LIBS}
		${OBJDUMP} -S shutdown.elf > shutdown.dis
		${SIZE} shutdown.elf
		${ELF2AOUT} shutdown.elf $@ && rm shutdown.elf

${MAN}:         ${MANSRC}
		${MANROFF} ${MANSRC} > ${MAN}

clean:
		rm -f *.o *.elf ${MAN} shutdown *.elf *.dis tags *~

depend:         ${SRCS}
		mkdep ${CFLAGS} ${SRCS}

install:        all
		cp ${MAN} ${DESTDIR}/usr/share/man/cat8/
		${INSTALL} -m 4750 shutdown ${DESTDIR}/sbin/shutdown

lint:           ${SRCS}
		lint -hax ${SRCS}

tags:           ${SRCS}
		ctags ${SRCS}

# DO NOT DELETE THIS LINE -- mkdep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.
