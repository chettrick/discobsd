TOPSRC!=cd ../..; pwd
include ${TOPSRC}/target.mk

# Must run set-user-id root.

BINDIR=		/usr/bin
BINMODE=	4751

PROG=		mail

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
