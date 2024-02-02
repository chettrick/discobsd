#
# Public Domain.  1997/3/27 - Steven Schultz
#
TOPSRC!=cd ../..; pwd
include ${TOPSRC}/share/mk/sys.mk

CFLAGS		+= -O

SRCS		= stty.c
OBJS		= stty.o
MAN		= stty.0
MANSRC		= stty.1

all:		stty ${MAN}

stty:		${OBJS}
		${CC} ${CFLAGS} ${LDFLAGS} -o stty.elf ${OBJS} ${LIBS}
		${OBJDUMP} -S stty.elf > stty.dis
		${SIZE} stty.elf
		${ELF2AOUT} stty.elf $@ && rm stty.elf

${MAN}:		${MANSRC}
		${MANROFF} ${MANSRC} > ${MAN}

clean:
		rm -f *.o *.elf ${MAN} stty *.elf *.dis tags *~

depend:		${SRCS}
		mkdep ${CFLAGS} ${SRCS}

install:	all
		cp ${MAN} ${DESTDIR}/usr/share/man/cat1/
		${INSTALL} stty ${DESTDIR}/bin/stty

lint:		${SRCS}
		lint -hax ${SRCS}

tags:		${SRCS}
		ctags ${SRCS}

# DO NOT DELETE THIS LINE -- mkdep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.
