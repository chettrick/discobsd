TOPSRC!=cd ../..; pwd
include ${TOPSRC}/share/mk/sys.mk

BINDIR=		/usr/games
MANDIR=		/usr/share/man/cat6

PROG=		canfield

OBJS=		${PROG}.o

MAN=		${PROG}.0
MANSRC=		${PROG}.6

LDADD=		-lcurses -ltermcap

all:		${PROG} cfscores ${MAN}

${PROG}:	${OBJS}
		${CC} ${CFLAGS} ${LDFLAGS} -o $@.elf ${OBJS} ${LDADD} ${LIBS}
		${OBJDUMP} -S $@.elf > $@.dis
		${SIZE} $@.elf
		${ELF2AOUT} $@.elf $@ && rm $@.elf

cfscores:	cfscores.o
		${CC} ${CFLAGS} ${LDFLAGS} -o $@.elf cfscores.o ${LIBS}
		${OBJDUMP} -S $@.elf > $@.dis
		${SIZE} $@.elf
		${ELF2AOUT} $@.elf $@ && rm $@.elf

${MAN}:		${MANSRC}
		${MANROFF} ${MANSRC} > $@

clean:
		rm -f *.o *.elf ${PROG} cfscores *.0 *.dis tags *~

install:	all
		install ${PROG} ${DESTDIR}${BINDIR}/${PROG}
		install cfscores ${DESTDIR}${BINDIR}/cfscores
		install -m 644 ${MAN} ${DESTDIR}${MANDIR}/${MAN}
		ln -f ${DESTDIR}${MANDIR}/${MAN} ${DESTDIR}${MANDIR}/cfscores.0
		cat < /dev/null > ${DESTDIR}/usr/games/lib/cfscores
		chmod 666 ${DESTDIR}/usr/games/lib/cfscores
