TOPSRC!=cd ../..; pwd
include ${TOPSRC}/share/mk/sys.mk

BINDIR=		/usr/games
MANDIR=		/usr/share/man/cat6

PROG=		bcd

OBJS=		${PROG}.o

MAN=		${PROG}.0
MANSRC=		${PROG}.6

all:		${PROG} ${MAN}

${PROG}:	${OBJS}
		${CC} ${CFLAGS} ${LDFLAGS} -o $@.elf ${OBJS} ${LIBS}
		${OBJDUMP} -S $@.elf > $@.dis
		${SIZE} $@.elf
		${ELF2AOUT} $@.elf $@ && rm $@.elf

${MAN}:		${MANSRC}
		${MANROFF} ${MANSRC} > $@

clean:
		rm -f *.o *.elf ${PROG} *.0 *.dis tags *~

install:	all
		${INSTALL} ${PROG} ${DESTDIR}${BINDIR}/${PROG}
		${INSTALL} -m 444 ${MAN} ${DESTDIR}${MANDIR}/${MAN}
		ln -f ${DESTDIR}${MANDIR}/${MAN} ${DESTDIR}${MANDIR}/ppt.0
