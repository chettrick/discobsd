TOPSRC!=cd ../..; pwd
include ${TOPSRC}/share/mk/sys.mk

# Must run set-group-id kmem.

BINDIR=		/usr/sbin
BINMODE=	2751

PROG=		iostat

OBJS=		${PROG}.o

MAN=		${PROG}.0
MANSRC=		${PROG}.8

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
		${INSTALL} -m ${BINMODE} ${PROG} ${DESTDIR}${BINDIR}/${PROG}
		cp ${MAN} ${DESTDIR}/usr/share/man/cat8/
