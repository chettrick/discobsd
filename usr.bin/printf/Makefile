#
# Public Domain.  1995/05/02 - Steven Schultz
#
TOPSRC!=cd ../..; pwd
include ${TOPSRC}/share/mk/sys.mk

CFLAGS		+= -DNOFPU

SRCS		= printf.c
OBJS		= printf.o
MAN		= printf.0
MANSRC		= printf.1

all:            printf ${MAN}

printf:         ${OBJS}
		${CC} ${CFLAGS} ${LDFLAGS} -o printf.elf ${OBJS} ${LIBS}
		${OBJDUMP} -S printf.elf > printf.dis
		${SIZE} printf.elf
		${ELF2AOUT} printf.elf $@ && rm printf.elf

${MAN}:         ${MANSRC}
		${MANROFF} ${MANSRC} > ${MAN}

clean:
		rm -f *.o *.elf ${MAN} printf *.elf *.dis tags *~

depend:         ${SRCS}
		mkdep ${CFLAGS} ${SRCS}

install:        all
		cp ${MAN} ${DESTDIR}/usr/share/man/cat1/
		${INSTALL} printf ${DESTDIR}/usr/bin/printf

lint:           ${SRCS}
		lint -hax ${SRCS}

tags:           ${SRCS}
		ctags ${SRCS}

# DO NOT DELETE THIS LINE -- mkdep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.
