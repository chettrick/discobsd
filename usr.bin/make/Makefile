# Description file for the Make command
# Makefile 4.5 94/1/3
TOPSRC!=cd ../..; pwd
include ${TOPSRC}/share/mk/sys.mk

CFLAGS		+= -Wall -I../ar
YFLAGS		=

OBJS            = main.o doname.o misc.o files.o dosys.o gram.o

all:            xmake

xmake:          ${OBJS}
		${CC} ${CFLAGS} ${LDFLAGS} -o make.elf ${OBJS} ${LIBS}
		${OBJDUMP} -S make.elf > make.dis
		${SIZE} make.elf
		${ELF2AOUT} make.elf $@ && rm make.elf

clean:
		rm -f *.o *.elf xmake *.elf *.dis tags *~ gram.c y.tab.[ch]

install:        all
		${INSTALL} xmake ${DESTDIR}/usr/bin/make

gram.c:         gram.y
		${YACC} ${YFLAGS} gram.y
		fgrep -v "static char yaccpar_sccsid" y.tab.c >gram.c
		rm -f y.tab.c
