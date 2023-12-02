TOPSRC!=cd ../..; pwd
include ${TOPSRC}/target.mk

BINDIR=		/usr/bin

PROG=		bc

OBJS=		${PROG}.o

MAN=		${PROG}.0
MANSRC=		${PROG}.1

YFLAGS=

all:		${PROG} ${MAN}

${PROG}:	${OBJS}
		${CC} ${CFLAGS} ${LDFLAGS} -o $@.elf ${OBJS} ${LIBS}
		${OBJDUMP} -S $@.elf > $@.dis
		${SIZE} $@.elf
		${ELF2AOUT} $@.elf $@ && rm $@.elf

${MAN}:		${MANSRC}
		${MANROFF} ${MANSRC} > $@

clean:
		rm -f *.o *.elf ${MAN} ${PROG} *.dis tags *~ y.tab.[ch]

install:	all lib.b
		install -c -m 444 lib.b ${DESTDIR}/usr/share/misc
		install ${PROG} ${DESTDIR}${BINDIR}/${PROG}
		cp ${MAN} ${DESTDIR}/usr/share/man/cat1/
