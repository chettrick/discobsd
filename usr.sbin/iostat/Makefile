TOPSRC!=cd ../..; pwd
include ${TOPSRC}/share/mk/sys.mk

# Must run set-group-id kmem.

BINDIR=		/usr/sbin
BINMODE=	2751

PROG=		iostat

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
		install -m ${BINMODE} ${PROG} ${DESTDIR}${BINDIR}/${PROG}
