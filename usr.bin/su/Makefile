TOPSRC!=cd ../..; pwd
include ${TOPSRC}/share/mk/sys.mk

# Must run set-user-id root.

BINDIR=		/usr/bin
BINMODE=	4751

PROG=		su

OBJS=		${PROG}.o

all:		${PROG}

${PROG}:	${OBJS}
		${CC} ${CFLAGS} ${LDFLAGS} -o $@.elf ${OBJS} ${LIBS}
		${OBJDUMP} -S $@.elf > $@.dis
		${SIZE} $@.elf
		${ELF2AOUT} $@.elf $@ && rm $@.elf

clean:
		rm -f *.o *.elf ${PROG} *.dis tags *~

install:	all
		${INSTALL} -m ${BINMODE} ${PROG} ${DESTDIR}${BINDIR}/${PROG}
