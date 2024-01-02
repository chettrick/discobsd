#	@(#)Makefile	1.0 (2.11BSD GTE) 4/5/94
TOPSRC!=cd ../..; pwd
include ${TOPSRC}/share/mk/sys.mk

BINDIR=		/usr/bin

PROG=		m4

SRCS=		eval.c expr.c look.c main.c misc.c
OBJS=		eval.o expr.o look.o main.o misc.o

DEFS= 		-DEXTENDED -Dunix -I.
CFLAGS+=	-O ${DEFS}

all:		${PROG}

${PROG}:	${OBJS}
		${CC} ${CFLAGS} ${LDFLAGS} -o $@.elf ${OBJS} ${LIBS}
		${OBJDUMP} -S $@.elf > $@.dis
		${SIZE} $@.elf
		${ELF2AOUT} $@.elf $@ && rm $@.elf

install:	all
		install ${PROG} ${DESTDIR}${BINDIR}/${PROG}

clean:
		rm -f *.o *.elf ${PROG} *.dis tags *~
