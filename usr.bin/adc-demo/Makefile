#==========================================
#    Makefile: makefile for adc-demo
#	Copyright 2012 Majenko Technolohies
#		(matt@majenko.co.uk
#	Last Modified: 29/01/2012
#==========================================

TOPSRC!=cd ../..; pwd
include ${TOPSRC}/share/mk/sys.mk

OBJS            = adc-demo.o
SRCS            = adc-demo.c

all:            adc-demo

adc-demo:       ${OBJS}
		${CC} ${CFLAGS} ${LDFLAGS} -o adc-demo.elf ${OBJS} ${LIBS}
		${OBJDUMP} -S adc-demo.elf > adc-demo.dis
		${SIZE} adc-demo.elf
		${ELF2AOUT} adc-demo.elf $@

clean:
		-rm -f adc-demo ${OBJS} adc-demo.elf adc-demo.dis

install:        all
		${INSTALL} adc-demo ${DESTDIR}/usr/bin/
