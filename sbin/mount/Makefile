#
# Public Domain.  1996/1/15 - Steven Schultz
#
TOPSRC!=cd ../..; pwd
include ${TOPSRC}/share/mk/sys.mk

SRCS            = mount.c getmntopts.c mount_ufs.c
OBJS            = mount.o getmntopts.o mount_ufs.o
MAN		= mount.0
MANSRC		= mount.8

all:            mount ${MAN}

mount:       	${OBJS}
		${CC} ${CFLAGS} ${LDFLAGS} -o mount.elf ${OBJS} ${LIBS}
		${OBJDUMP} -S mount.elf > mount.dis
		${SIZE} mount.elf
		${ELF2AOUT} mount.elf $@ && rm mount.elf

${MAN}:         ${MANSRC}
		${MANROFF} ${MANSRC} > ${MAN}

clean:
		rm -f *.o *.elf ${MAN} mount *.elf *.dis tags *~

depend:         ${SRCS}
		mkdep ${CFLAGS} ${SRCS}

install:        all
		cp ${MAN} ${DESTDIR}/usr/share/man/cat8/
		${INSTALL} mount ${DESTDIR}/sbin/mount

lint:           ${SRCS}
		lint -hax ${SRCS}

tags:           ${SRCS}
		ctags ${SRCS}

# DO NOT DELETE THIS LINE -- mkdep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.
