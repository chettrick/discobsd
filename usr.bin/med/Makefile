#==========================================
#    Makefile: makefile for med
#	Copyright 2012 Majenko Technolohies
#		(matt@majenko.co.uk
#	Last Modified: 29/01/2012
#==========================================

TOPSRC!=cd ../..; pwd
include ${TOPSRC}/share/mk/sys.mk

OBJS            = med.o
SRCS            = med.c
LIBS            += -lcurses -ltermcap -lvmf -lc

all:            med

med: 		${OBJS}
		${CC} ${CFLAGS} ${LDFLAGS} -o med.elf ${OBJS} ${LIBS}
		${OBJDUMP} -S med.elf > med.dis
		${SIZE} med.elf
		${ELF2AOUT} med.elf $@ && rm med.elf

clean:
		-rm -f med ${OBJS} med.elf med.dis

install:        all
		${INSTALL} med ${DESTDIR}/usr/bin/
