TOPSRC!=cd ../..; pwd
include ${TOPSRC}/target.mk

BINDIR=		/usr/bin

PROG=		whereis

OBJS=		${PROG}.o

MAN=		${PROG}.0
MANSRC=		${PROG}.1

all:		${PROG} ${MAN}

${PROG}:	${OBJS}
		${CC} ${CFLAGS} ${LDFLAGS} -o $@.elf ${OBJS} ${LIBS}
		${OBJDUMP} -S $@.elf > $@.dis
		${SIZE} $@.elf
		${ELF2AOUT} $@.elf $@ && rm $@.elf

${MAN}:		${MANSRC}
		${MANROFF} ${MANSRC} > $@

clean:
		rm -f *.o *.elf ${MAN} ${PROG} *.dis tags *~

install:	all
		install ${PROG} ${DESTDIR}${BINDIR}/${PROG}
		cp ${MAN} ${DESTDIR}/usr/share/man/cat1/
