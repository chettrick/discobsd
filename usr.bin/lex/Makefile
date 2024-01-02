TOPSRC!=cd ../../; pwd
include ${TOPSRC}/share/mk/sys.mk

PROG=		lex

CFLAGS+=	-O0
CFLAGS+=	-DSMALL
#CFLAGS+=	-ggdb
#CFLAGS+=	-DDEBUG

YFLAGS=

OBJS=		y.tab.o lmain.o sub1.o sub2.o header.o

all:		${PROG}

${PROG}:	${OBJS}
		${CC} ${CFLAGS} ${LDFLAGS} -o ${PROG}.elf ${OBJS} ${LIBS}
		${OBJDUMP} -S ${PROG}.elf > ${PROG}.dis
		${SIZE} ${PROG}.elf
		${ELF2AOUT} ${PROG}.elf $@ && rm ${PROG}.elf

y.tab.c:	parser.y
		${YACC} ${YFLAGS} parser.y
lmain.o:	lmain.c ldefs.h
		${CC} -c ${CFLAGS} lmain.c
sub1.o:		sub1.c ldefs.h
		${CC} -c ${CFLAGS} sub1.c
sub2.o:		sub2.c ldefs.h
		${CC} -c ${CFLAGS} sub2.c
header.o:	header.c ldefs.h
		${CC} -c ${CFLAGS} header.c

install:	all
		-mkdir -p ${DESTDIR}/usr/share/misc
		install -c -m 444 ncform ${DESTDIR}/usr/share/misc
		install -m 755 ${PROG} ${DESTDIR}/usr/bin

clean:
		rm -f *.o ${PROG} ${PROG}.dis ${PROG}.elf y.tab.[ch]
