TOPSRC!=cd ../..; pwd
include ${TOPSRC}/share/mk/sys.mk

BINDIR=		/usr/bin

PROG=		env

OBJS=		${PROG}.o
MAN=		${PROG}.0
MANSRC=		${PROG}.1

CFLAGS+=	-Wall

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
		${INSTALL} ${PROG} ${DESTDIR}${BINDIR}/${PROG}
		cp ${MAN} ${DESTDIR}/usr/share/man/cat1/
