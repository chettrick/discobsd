TOPSRC!=cd ../..; pwd
include ${TOPSRC}/target.mk

BINDIR=		/usr/games
MANDIR=		/usr/share/man/cat6

PROG=		worms

OBJS=		${PROG}.o

MAN=		${PROG}.0
MANSRC=		${PROG}.6

LDADD=		-ltermcap

all:		${PROG} ${MAN}

${PROG}:	${OBJS}
		${CC} ${CFLAGS} ${LDFLAGS} -o $@.elf ${OBJS} ${LDADD} ${LIBS}
		${OBJDUMP} -S $@.elf > $@.dis
		${SIZE} $@.elf
		${ELF2AOUT} $@.elf $@ && rm $@.elf

${MAN}:		${MANSRC}
		${MANROFF} ${MANSRC} > $@

clean:
		rm -f *.o *.elf ${PROG} *.0 *.dis tags *~

install:	all
		install ${PROG} ${DESTDIR}${BINDIR}/${PROG}
		install -m 444 ${MAN} ${DESTDIR}${MANDIR}/${MAN}
