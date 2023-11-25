TOPSRC!=cd ../..; pwd
include ${TOPSRC}/target.mk

BINDIR=		/usr/bin

PROG=		bc

OBJS=		${PROG}.o

YFLAGS=

all:		${PROG}

${PROG}:	${OBJS}
		${CC} ${CFLAGS} ${LDFLAGS} -o $@.elf ${OBJS} ${LIBS}
		${OBJDUMP} -S $@.elf > $@.dis
		${SIZE} $@.elf
		${ELF2AOUT} $@.elf $@ && rm $@.elf

clean:
		rm -f *.o *.elf ${PROG} *.dis tags *~ y.tab.[ch]

install:	all lib.b
		install -c -m 444 lib.b ${DESTDIR}/usr/share/misc
		install ${PROG} ${DESTDIR}${BINDIR}/${PROG}
