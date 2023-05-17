#==========================================
#    Makefile: makefile for yacc
#	Copyright 2012 Majenko Technolohies
#		(matt@majenko.co.uk
#	Last Modified: 29/01/2012
#==========================================

TOPSRC!=cd ../..; pwd
include $(TOPSRC)/target.mk

CFLAGS          = -ansi

OBJS            = y1.o y2.o y3.o y4.o
SRCS            = y1.c y2.c y3.c y4.c

all:            yacc

yacc:       ${OBJS}
		${CC} ${CFLAGS} ${LDFLAGS} -o yacc.elf ${OBJS} ${LIBS}
		${OBJDUMP} -S yacc.elf > yacc.dis
		${SIZE} yacc.elf
		${ELF2AOUT} yacc.elf $@

clean:
		-rm -f yacc ${OBJS} yacc.elf yacc.dis

install:        all yaccpar
		install yacc ${DESTDIR}/usr/bin/
		install yaccpar ${DESTDIR}/usr/share/misc

y1.o: y1.c dextern
y2.o: y2.c dextern
y3.o: y3.c dextern
y4.o: y4.c dextern
