#
# Public Domain.  1996/11/16 - Steven Schultz
#
TOPSRC!=cd ../..; pwd
include ${TOPSRC}/share/mk/sys.mk

CFLAGS		+= -Wall

SRCS		= mkfs.c
OBJS		= mkfs.o
MAN		= mkfs.0
MANSRC		= mkfs.8

all:		mkfs mkfs.0

mkfs:		${OBJS}
		${CC} ${CFLAGS} ${LDFLAGS} -o mkfs.elf ${OBJS} ${LIBS}
		${OBJDUMP} -S mkfs.elf > mkfs.dis
		${SIZE} mkfs.elf
		${ELF2AOUT} mkfs.elf $@ && rm mkfs.elf

mkfs.0:		${MANSRC}
		${MANROFF} ${MANSRC} > ${MAN}

clean:
		rm -f *.o *.elf *.dis *~ ${MAN} mkfs tags

depend:		${SRCS}
		mkdep ${CFLAGS} ${SRCS}

install:	all
		cp ${MAN} ${DESTDIR}/usr/share/man/cat8/
		${INSTALL} mkfs ${DESTDIR}/sbin/mkfs

lint:		${SRCS}
		lint -hax ${SRCS}

tags:		${SRCS}
		ctags ${SRCS}

# DO NOT DELETE THIS LINE -- mkdep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.
