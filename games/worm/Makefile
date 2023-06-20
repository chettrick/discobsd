TOPSRC!=cd ../..; pwd
include ${TOPSRC}/target.mk

BINDIR=		/usr/games

PROG=		worm

OBJS=		${PROG}.o

LDADD=		-lcurses -ltermcap

all:		${PROG}

${PROG}:	${OBJS}
		${CC} ${CFLAGS} ${LDFLAGS} -o $@.elf ${OBJS} ${LDADD} ${LIBS}
		${OBJDUMP} -S $@.elf > $@.dis
		${SIZE} $@.elf
		${ELF2AOUT} $@.elf $@ && rm $@.elf

clean:
		rm -f *.o *.elf ${PROG} *.dis tags *~

install:	all
		install ${PROG} ${DESTDIR}${BINDIR}/${PROG}
