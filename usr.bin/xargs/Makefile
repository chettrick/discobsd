#
# Public Domain.  1995/03/13 - Steven Schultz
#
TOPSRC!=cd ../..; pwd
include ${TOPSRC}/share/mk/sys.mk

CFLAGS		+= -Wall

SRCS		= xargs.c
OBJS		= xargs.o
MAN		= xargs.0
MANSRC		= xargs.1

all:            xargs ${MAN}

xargs:          ${OBJS}
		${CC} ${CFLAGS} ${LDFLAGS} -o xargs.elf ${OBJS} ${LIBS}
		${OBJDUMP} -S xargs.elf > xargs.dis
		${SIZE} xargs.elf
		${ELF2AOUT} xargs.elf $@ && rm xargs.elf

${MAN}:        	${MANSRC}
		${MANROFF} ${MANSRC} > ${MAN}

clean:
		rm -f *.o *.elf ${MAN} xargs *.elf *.dis tags *~

depend:         ${SRCS}
		mkdep ${CFLAGS} ${SRCS}

install:        all
		${INSTALL} xargs ${DESTDIR}/usr/bin/
		cp xargs.0 ${DESTDIR}/usr/share/man/cat1/

lint:           ${SRCS}
		lint -hax ${SRCS}

tags:           ${SRCS}
		ctags ${SRCS}
